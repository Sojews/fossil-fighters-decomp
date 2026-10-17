// NitroSDK SPI: power management. The power management chip sits on the ARM7's SPI bus; requests go there over
// the PXI FIFO. Also sleep mode, the LCD power switch and the callbacks run around sleep.

#include "nitro/os.h"
#include "nitro/pxi.h"
#include "nitro/pm.h"

#define PXI_FIFO_TAG_PM 8

#define SPI_PXI_START_BIT 0x02000000
#define SPI_PXI_END_BIT 0x01000000
#define SPI_PXI_INDEX_SHIFT 16
#define SPI_PXI_RESULT_COMMAND_MASK 0x7f00
#define SPI_PXI_RESULT_COMMAND_SHIFT 8
#define SPI_PXI_RESULT_DATA_MASK 0x00ff

#define SPI_PXI_COMMAND_PM_SYNC 0x60
#define SPI_PXI_COMMAND_PM_SLEEP_START 0x61
#define SPI_PXI_COMMAND_PM_SLEEP_END 0x62
#define SPI_PXI_COMMAND_PM_UTILITY 0x63
#define SPI_PXI_COMMAND_PM_REG_WRITE 0x64
#define SPI_PXI_COMMAND_PM_REG_READ 0x65
#define SPI_PXI_COMMAND_PM_SELF_BLINK 0x67
#define SPI_PXI_COMMAND_PM_REG_READ_RESULT 0x70

#define PM_REGISTER_NUM 5

#define PM_RESULT_SUCCESS 0
#define PM_RESULT_BUSY 1
#define PM_RESULT_ERROR 2
#define PM_INVALID_COMMAND 0xffff

#define PMIC_REG_CTL 0
#define PMIC_REG_AMP 2
#define PMIC_REG_GAIN 3
#define PMIC_CTL_BKLT_BOTTOM 0x04
#define PMIC_CTL_BKLT_TOP 0x08

#define PM_UTIL_LED_ON 1
#define PM_UTIL_LED_BLINK_LOW_SPEED 2
#define PM_UTIL_LED_BLINK_HIGH_SPEED 3
#define PM_UTIL_LCD_BOTTOM_BACKLIGHT_ON 4
#define PM_UTIL_LCD_BOTTOM_BACKLIGHT_OFF 5
#define PM_UTIL_LCD_TOP_BACKLIGHT_ON 6
#define PM_UTIL_LCD_TOP_BACKLIGHT_OFF 7
#define PM_UTIL_LCD_BACKLIGHT_ON 8
#define PM_UTIL_LCD_BACKLIGHT_OFF 9
#define PM_UTIL_FORCE_POWER_OFF 14

#define PM_LED_NONE 0
#define PM_LED_ON 1
#define PM_LED_BLINK_HIGH 2
#define PM_LED_BLINK_LOW 3

#define PM_LCD_TOP 0
#define PM_LCD_BOTTOM 1
#define PM_LCD_ALL 2
#define PM_BACKLIGHT_OFF 0
#define PM_BACKLIGHT_ON 1

#define PM_TRIGGER_CARTRIDGE 0x10
#define PM_BACKLIGHT_RECOVER_TOP_SHIFT 5
#define PM_BACKLIGHT_RECOVER_BOTTOM_SHIFT 6

// Turning the LCDs back on this soon (in frames) after turning them off doesn't take.
#define PMi_LCD_WAIT_SYS_CYCLES 7
#define PMi_POWER_OFF_WAIT_CYCLES 0x996a00
#define PMi_WAKE_UP_WAIT_CYCLES 0x28e900

#define OS_IE_FIFO_RECV (1 << 18)
#define OS_IE_CARD_IREQ (1 << 20)
#define OS_IE_ALL 0x3fffff
#define OS_INTRMODE_IRQ_DISABLE 0x80

#define reg_GX_DISPCNT (*(volatile u32 *) 0x04000000)
#define reg_GXS_DB_DISPCNT (*(volatile u32 *) 0x04001000)
#define reg_GX_POWCNT (*(volatile u16 *) 0x04000304)
#define reg_OS_IME (*(volatile u16 *) 0x04000208)
#define reg_OS_IF (*(volatile u32 *) 0x04000214)
#define REG_DISPCNT_MODE_MASK 0x00030000
#define REG_DB_DISPCNT_MODE_MASK 0x00010000
#define REG_POWCNT_LCD_MASK 0x0001

#define OS_VBLANK_COUNT (*(volatile u32 *) 0x027ffc3c)
static inline u32 OS_GetVBlankCount(void) {
    return OS_VBLANK_COUNT;
}
#define OS_CARD_REMOVED_STATE (*(u16 *) 0x027ffc40)

typedef void (*PMCallback)(u32 result, void *arg);

typedef struct {
    volatile u16 ready;     // 0x0
    u16 *buffer;            // 0x4
} PMiRegisterRequest;

void PXIi_HandlerRecvFifoNotEmpty(void);
u32 OS_GetCpsrIrq(void);
void OS_Halt(void);
BOOL CTRDG_IsExisting(void);

static PMiRegisterRequest PMi_RegisterRequest[PM_REGISTER_NUM];
static OSMutex PMi_Mutex;
static volatile BOOL PMi_SleepStartFlag;
static volatile BOOL PMi_LockFlag;
static PMSleepCallbackInfo *PMi_PreSleepCallbackList;
static u32 PMi_LCDOffCount;
static PMSleepCallbackInfo *PMi_PostSleepCallbackList;
static volatile BOOL PMi_SleepEndFlag;
static PMCallback PMi_Callback;
static void *PMi_CallbackArg;
static u32 *PMi_SelfBlinkResult;
static u16 PMi_Initialized;

u32 PM_SendUtilityCommandAsync(u32 number, PMCallback callback, void *arg);
BOOL PMi_SetLCDPower(u32 sw, u32 led, BOOL skip, BOOL isSync);
void PMi_SendPxiData(u32 data);
u32 PMi_SetLED(u32 status);
u32 PM_GetBackLight(u32 *top, u32 *bottom);
u32 PM_SetBackLight(u32 target, u32 sw);
u32 PM_ForceToPowerOff(void);
void PMi_ExecuteList(PMSleepCallbackInfo *info);
static void PMi_CommonCallback(int tag, u32 data, BOOL err);

static BOOL PMi_Lock(void) {
    OSIntrMode enabled = OS_DisableInterrupts();
    if (PMi_LockFlag) {
        OS_RestoreInterrupts(enabled);
        return FALSE;
    }
    PMi_LockFlag = TRUE;
    OS_RestoreInterrupts(enabled);
    return TRUE;
}

static void PMi_WaitBusy(void) {
    volatile BOOL *lock = &PMi_LockFlag;
    while (*lock) {
        if (OS_GetCpsrIrq() == OS_INTRMODE_IRQ_DISABLE) {
            PXIi_HandlerRecvFifoNotEmpty();
        }
    }
}

void PMi_DummyCallback(u32 result, void *arg) {
    *(u32 *) arg = result;
}

static void PMi_CallCallbackAndUnlock(u32 result) {
    PMCallback callback = PMi_Callback;
    void *arg = PMi_CallbackArg;
    if (PMi_LockFlag) {
        PMi_LockFlag = FALSE;
    }
    if (callback) {
        PMi_Callback = NULL;
        callback(result, arg);
    }
}

void PM_Init(void) {
    int i;

    if (PMi_Initialized) {
        return;
    }
    PMi_Initialized = 1;
    PMi_LockFlag = FALSE;
    PMi_Callback = NULL;
    PXI_Init();
    while (!PXI_IsCallbackReady(PXI_FIFO_TAG_PM, PXI_PROC_ARM7)) {
    }
    PXI_SetFifoRecvCallback(PXI_FIFO_TAG_PM, PMi_CommonCallback);
    for (i = 0; i < PM_REGISTER_NUM; i++) {
        PMi_RegisterRequest[i].ready = FALSE;
    }
    OS_InitMutex(&PMi_Mutex);
    PMi_LCDOffCount = OS_VBLANK_COUNT;
}

static void PMi_CommonCallback(int tag, u32 data, BOOL err) {
#pragma unused(tag)
    u16 command;
    u16 result;

    if (err) {
        PMi_CallCallbackAndUnlock(PM_RESULT_ERROR);
        return;
    }
    command = (u16) ((data & SPI_PXI_RESULT_COMMAND_MASK) >> SPI_PXI_RESULT_COMMAND_SHIFT);
    result = (u16) (data & SPI_PXI_RESULT_DATA_MASK);
    if (command >= SPI_PXI_COMMAND_PM_REG_READ_RESULT &&
        command <= SPI_PXI_COMMAND_PM_REG_READ_RESULT + PM_REGISTER_NUM - 1) {
        u16 value = (u16) (result & 0xff);
        int index = command - SPI_PXI_COMMAND_PM_REG_READ_RESULT;
        if (PMi_RegisterRequest[index].buffer) {
            *PMi_RegisterRequest[index].buffer = value;
        }
        PMi_RegisterRequest[index].ready = TRUE;
        result = PM_RESULT_SUCCESS;
    } else if (command == SPI_PXI_COMMAND_PM_SYNC) {
        PMi_SleepStartFlag = TRUE;
    } else if (command == SPI_PXI_COMMAND_PM_SLEEP_END) {
        PMi_SleepEndFlag = TRUE;
    } else if (command == SPI_PXI_COMMAND_PM_SELF_BLINK) {
        if (PMi_SelfBlinkResult) {
            *PMi_SelfBlinkResult = result;
        }
        result = PM_RESULT_SUCCESS;
    }
    PMi_CallCallbackAndUnlock(result);
}

u32 PMi_SendSleepStart(u32 trigger, u32 keyIntrData) {
    if (!PMi_Lock()) {
        return PM_RESULT_BUSY;
    }
    PMi_SleepStartFlag = FALSE;
    PMi_SendPxiData(SPI_PXI_START_BIT | SPI_PXI_END_BIT | (0 << SPI_PXI_INDEX_SHIFT) |
                    (SPI_PXI_COMMAND_PM_SYNC << 8));
    while (!PMi_SleepStartFlag) {
    }
    PMi_SleepStartFlag = FALSE;
    PMi_SleepEndFlag = FALSE;
    PMi_SetLCDPower(PM_LCD_POWER_OFF, PM_LED_BLINK_HIGH, FALSE, TRUE);
    PMi_SendPxiData(SPI_PXI_START_BIT | (0 << SPI_PXI_INDEX_SHIFT) | (SPI_PXI_COMMAND_PM_SLEEP_START << 8) |
                    (u8) trigger);
    PMi_SendPxiData(SPI_PXI_END_BIT | (1 << SPI_PXI_INDEX_SHIFT) | (u16) keyIntrData);
    return PM_RESULT_SUCCESS;
}

u32 PM_SendUtilityCommandAsync(u32 number, PMCallback callback, void *arg) {
    if (!PMi_Lock()) {
        return PM_RESULT_BUSY;
    }
    PMi_Callback = callback;
    PMi_CallbackArg = arg;
    PMi_SendPxiData(SPI_PXI_START_BIT | (0 << SPI_PXI_INDEX_SHIFT) | (SPI_PXI_COMMAND_PM_UTILITY << 8) |
                    (u8) (number >> 16));
    PMi_SendPxiData(SPI_PXI_END_BIT | (1 << SPI_PXI_INDEX_SHIFT) | (u16) number);
    return PM_RESULT_SUCCESS;
}

u32 PMi_ReadRegisterAsync(u16 reg, u16 *buffer, PMCallback callback, void *arg) {
    if (!PMi_Lock()) {
        return PM_RESULT_BUSY;
    }
    PMi_Callback = callback;
    PMi_CallbackArg = arg;
    PMi_RegisterRequest[reg].ready = FALSE;
    PMi_RegisterRequest[reg].buffer = buffer;
    PMi_SendPxiData(SPI_PXI_START_BIT | SPI_PXI_END_BIT | (0 << SPI_PXI_INDEX_SHIFT) |
                    (SPI_PXI_COMMAND_PM_REG_READ << 8) | (u8) reg);
    return PM_RESULT_SUCCESS;
}

u32 PMi_ReadRegister(u16 reg, u16 *buffer) {
    u32 commandResult;
    u32 result = PMi_ReadRegisterAsync(reg, buffer, PMi_DummyCallback, &commandResult);
    if (result == PM_RESULT_SUCCESS) {
        PMi_WaitBusy();
        return commandResult;
    }
    return result;
}

u32 PMi_WriteRegisterAsync(u16 reg, u32 data, PMCallback callback, void *arg) {
    if (!PMi_Lock()) {
        return PM_RESULT_BUSY;
    }
    PMi_Callback = callback;
    PMi_CallbackArg = arg;
    PMi_SendPxiData(SPI_PXI_START_BIT | (0 << SPI_PXI_INDEX_SHIFT) | (SPI_PXI_COMMAND_PM_REG_WRITE << 8) |
                    (u8) reg);
    PMi_SendPxiData(SPI_PXI_END_BIT | (1 << SPI_PXI_INDEX_SHIFT) | (u16) data);
    return PM_RESULT_SUCCESS;
}

u32 PMi_WriteRegister(u16 reg, u16 data) {
    u32 commandResult;
    u32 result = PMi_WriteRegisterAsync(reg, data, PMi_DummyCallback, &commandResult);
    if (result == PM_RESULT_SUCCESS) {
        PMi_WaitBusy();
        return commandResult;
    }
    return result;
}

u32 PMi_SetLEDAsync(u32 status, PMCallback callback, void *arg) {
    u32 command;
    switch (status) {
    case PM_LED_ON:
        command = PM_UTIL_LED_ON;
        break;
    case PM_LED_BLINK_LOW:
        command = PM_UTIL_LED_BLINK_LOW_SPEED;
        break;
    case PM_LED_BLINK_HIGH:
        command = PM_UTIL_LED_BLINK_HIGH_SPEED;
        break;
    default:
        command = 0;
    }
    return command ? PM_SendUtilityCommandAsync(command, callback, arg) : PM_INVALID_COMMAND;
}

u32 PMi_SetLED(u32 status) {
    u32 commandResult;
    u32 result = PMi_SetLEDAsync(status, PMi_DummyCallback, &commandResult);
    if (result == PM_RESULT_SUCCESS) {
        PMi_WaitBusy();
        return commandResult;
    }
    return result;
}

u32 PM_SetBackLightAsync(u32 target, u32 sw, PMCallback callback, void *arg) {
    u32 command = 0;
    if (target == PM_LCD_TOP) {
        if (sw == PM_BACKLIGHT_ON) {
            command = PM_UTIL_LCD_TOP_BACKLIGHT_ON;
        }
        if (sw == PM_BACKLIGHT_OFF) {
            command = PM_UTIL_LCD_TOP_BACKLIGHT_OFF;
        }
    } else if (target == PM_LCD_BOTTOM) {
        if (sw == PM_BACKLIGHT_ON) {
            command = PM_UTIL_LCD_BOTTOM_BACKLIGHT_ON;
        }
        if (sw == PM_BACKLIGHT_OFF) {
            command = PM_UTIL_LCD_BOTTOM_BACKLIGHT_OFF;
        }
    } else if (target == PM_LCD_ALL) {
        if (sw == PM_BACKLIGHT_ON) {
            command = PM_UTIL_LCD_BACKLIGHT_ON;
        }
        if (sw == PM_BACKLIGHT_OFF) {
            command = PM_UTIL_LCD_BACKLIGHT_OFF;
        }
    }
    return command ? PM_SendUtilityCommandAsync(command, callback, arg) : PM_INVALID_COMMAND;
}

u32 PM_SetBackLight(u32 target, u32 sw) {
    u32 commandResult;
    u32 result = PM_SetBackLightAsync(target, sw, PMi_DummyCallback, &commandResult);
    if (result == PM_RESULT_SUCCESS) {
        PMi_WaitBusy();
        return commandResult;
    }
    return result;
}

u32 PM_ForceToPowerOffAsync(PMCallback callback, void *arg) {
    OS_SpinWait(PMi_POWER_OFF_WAIT_CYCLES);
    if (PM_GetLCDPower() != PM_LCD_POWER_ON) {
        u32 top, bottom;
        PM_GetBackLight(&top, &bottom);
        if (top) {
            PM_SetBackLight(PM_LCD_TOP, PM_BACKLIGHT_OFF);
        }
        if (bottom) {
            PM_SetBackLight(PM_LCD_BOTTOM, PM_BACKLIGHT_OFF);
        }
        while (!PM_SetLCDPower(PM_LCD_POWER_ON)) {
            OS_SpinWait(PMi_POWER_OFF_WAIT_CYCLES);
        }
    }
    return PM_SendUtilityCommandAsync(PM_UTIL_FORCE_POWER_OFF, callback, arg);
}

u32 PM_ForceToPowerOff(void) {
    u32 commandResult;
    u32 result = PM_ForceToPowerOffAsync(PMi_DummyCallback, &commandResult);
    if (result == PM_RESULT_SUCCESS) {
        PMi_WaitBusy();
        return commandResult;
    }
    return result;
}

u32 PM_SetAmp(u32 status) {
    return PMi_WriteRegister(PMIC_REG_AMP, (u16) status);
}

u32 PM_SetAmpGain(u32 gain) {
    return PMi_WriteRegister(PMIC_REG_GAIN, (u16) gain);
}

u32 PM_GetBackLight(u32 *top, u32 *bottom) {
    u16 status;
    u32 result = PMi_ReadRegister(PMIC_REG_CTL, &status);
    if (result == PM_RESULT_SUCCESS) {
        if (top) {
            *top = (status & PMIC_CTL_BKLT_TOP) ? PM_BACKLIGHT_ON : PM_BACKLIGHT_OFF;
        }
        if (bottom) {
            *bottom = (status & PMIC_CTL_BKLT_BOTTOM) ? PM_BACKLIGHT_ON : PM_BACKLIGHT_OFF;
        }
    }
    return result;
}

void PMi_SendPxiData(u32 data) {
    while (PXI_SendWordByFifo(PXI_FIFO_TAG_PM, data, FALSE) != 0) {
    }
}

static inline u16 OS_DisableIrq(void) {
    u16 prep = reg_OS_IME;
    reg_OS_IME = 0;
    return prep;
}

static inline u16 OS_EnableIrq(void) {
    u16 prep = reg_OS_IME;
    reg_OS_IME = 1;
    return prep;
}

static inline u16 OS_RestoreIrq(u16 enable) {
    u16 prep = reg_OS_IME;
    reg_OS_IME = enable;
    return prep;
}

void PM_GoSleepMode(u32 trigger, u32 logic, u16 keyPattern) {
    u32 top;
    u32 bottom;
    volatile u32 vcount;
    OSIntrMode prepIntrMode;
    u32 prepIrqMask;
    u32 prepDispCnt;
    u32 prepDispCntSub;
    BOOL prepLCDPower;
    u16 prepIME;
    BOOL powerOffFlag = FALSE;
    u16 sendTrigger;
    u16 sendLogic;

    PMi_ExecuteList(PMi_PreSleepCallbackList);

    prepIME = OS_DisableIrq();
    prepIntrMode = OS_DisableInterrupts();
    prepIrqMask = OS_DisableIrqMask(OS_IE_ALL);
    OS_SetIrqMask(OS_IE_FIFO_RECV);
    OS_RestoreInterrupts(prepIntrMode);
    (void) OS_EnableIrq();

    if ((trigger & PM_TRIGGER_CARD) && OS_CARD_REMOVED_STATE == 2) {
        trigger &= ~PM_TRIGGER_CARD;
    }
    if ((trigger & PM_TRIGGER_CARTRIDGE) && !CTRDG_IsExisting()) {
        trigger &= ~PM_TRIGGER_CARTRIDGE;
    }

    prepDispCnt = reg_GX_DISPCNT;
    prepDispCntSub = reg_GXS_DB_DISPCNT;
    prepLCDPower = PM_GetLCDPower();
    PM_GetBackLight(&top, &bottom);
    PM_SetBackLight(PM_LCD_ALL, PM_BACKLIGHT_OFF);

    vcount = OS_GetVBlankCount();
    while (vcount == OS_GetVBlankCount()) {
    }
    vcount = OS_GetVBlankCount();
    reg_GX_DISPCNT = reg_GX_DISPCNT & ~REG_DISPCNT_MODE_MASK;
    reg_GXS_DB_DISPCNT = reg_GXS_DB_DISPCNT & ~REG_DB_DISPCNT_MODE_MASK;
    while (vcount == OS_GetVBlankCount()) {
    }
    vcount = OS_GetVBlankCount();
    while (vcount == OS_GetVBlankCount()) {
    }

    sendTrigger = (u16) (trigger | (top << PM_BACKLIGHT_RECOVER_TOP_SHIFT) |
                         (bottom << PM_BACKLIGHT_RECOVER_BOTTOM_SHIFT));
    sendLogic = (u16) (keyPattern | logic);
    while (PMi_SendSleepStart(sendTrigger, sendLogic) != PM_RESULT_SUCCESS) {
    }

    OS_Halt();
    OS_SpinWait(PMi_WAKE_UP_WAIT_CYCLES);

    if ((trigger & PM_TRIGGER_CARD) && (reg_OS_IF & OS_IE_CARD_IREQ)) {
        powerOffFlag = TRUE;
    }
    if (!powerOffFlag) {
        if (prepLCDPower == PM_LCD_POWER_ON) {
            PMi_SetLCDPower(PM_LCD_POWER_ON, PM_LED_ON, TRUE, TRUE);
        } else {
            PMi_SetLED(PM_LED_ON);
        }
        reg_GX_DISPCNT = prepDispCnt;
        reg_GXS_DB_DISPCNT = prepDispCntSub;
    }

    (void) OS_DisableInterrupts();
    OS_SetIrqMask(prepIrqMask);
    OS_RestoreInterrupts(prepIntrMode);
    (void) OS_RestoreIrq(prepIME);

    if (powerOffFlag) {
        PM_ForceToPowerOff();
    }
    PMi_ExecuteList(PMi_PostSleepCallbackList);
}

static inline void PMi_SetLEDWithSync(u32 led, BOOL isSync) {
    if (isSync) {
        PMi_SetLED(led);
    } else {
        PMi_SetLEDAsync(led, NULL, NULL);
    }
}

BOOL PMi_SetLCDPower(u32 sw, u32 led, BOOL skip, BOOL isSync) {
    switch (sw) {
    case PM_LCD_POWER_ON:
        if (!skip && OS_VBLANK_COUNT - PMi_LCDOffCount <= PMi_LCD_WAIT_SYS_CYCLES) {
            return FALSE;
        }
        if (led != PM_LED_NONE) {
            PMi_SetLEDWithSync(led, isSync);
        }
        reg_GX_POWCNT = (u16) (reg_GX_POWCNT | REG_POWCNT_LCD_MASK);
        break;
    case PM_LCD_POWER_OFF:
        reg_GX_POWCNT &= ~REG_POWCNT_LCD_MASK;
        PMi_LCDOffCount = OS_VBLANK_COUNT;
        if (led != PM_LED_NONE) {
            PMi_SetLEDWithSync(led, isSync);
        }
        break;
    }
    return TRUE;
}

BOOL PM_SetLCDPower(u32 sw) {
    if (sw != PM_LCD_POWER_ON) {
        sw = PM_LCD_POWER_OFF;
    }
    return PMi_SetLCDPower(sw, PM_LED_NONE, FALSE, TRUE);
}

u32 PM_GetLCDPower(void) {
    return (reg_GX_POWCNT & REG_POWCNT_LCD_MASK) ? PM_LCD_POWER_ON : PM_LCD_POWER_OFF;
}

void PMi_PrependList(PMSleepCallbackInfo **list, PMSleepCallbackInfo *info) {
    if (list == NULL) {
        return;
    }
    info->next = *list;
    *list = info;
}

void PMi_AppendList(PMSleepCallbackInfo **list, PMSleepCallbackInfo *info) {
    PMSleepCallbackInfo *p;
    if (list == NULL) {
        return;
    }
    p = *list;
    if (p == NULL) {
        info->next = NULL;
        *list = info;
        return;
    }
    while (p->next) {
        p = p->next;
    }
    info->next = p->next;
    p->next = info;
}

void PMi_DeleteList(PMSleepCallbackInfo **list, PMSleepCallbackInfo *info) {
    PMSleepCallbackInfo *p;
    PMSleepCallbackInfo *prev;
    if (list == NULL) {
        return;
    }
    prev = p = *list;
    while (p) {
        if (p == info) {
            if (p == prev) {
                *list = p->next;
            } else {
                prev->next = p->next;
            }
            return;
        }
        prev = p;
        p = p->next;
    }
}

void PMi_ExecuteList(PMSleepCallbackInfo *info) {
    while (info) {
        info->callback(info->arg);
        info = info->next;
    }
}

void PM_PrependPreSleepCallback(PMSleepCallbackInfo *info) {
    PMi_PrependList(&PMi_PreSleepCallbackList, info);
}

void PM_AppendPostSleepCallback(PMSleepCallbackInfo *info) {
    PMi_AppendList(&PMi_PostSleepCallbackList, info);
}

void PM_DeletePreSleepCallback(PMSleepCallbackInfo *info) {
    PMi_DeleteList(&PMi_PreSleepCallbackList, info);
}

void PM_DeletePostSleepCallback(PMSleepCallbackInfo *info) {
    PMi_DeleteList(&PMi_PostSleepCallbackList, info);
}
