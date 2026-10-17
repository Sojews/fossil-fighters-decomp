// NitroSDK CARD: noticing the card being pulled out. The ARM7 reports it; the game may get a say through a
// callback, then the system powers off or halts.

#include "card_common.h"

#define PXI_FIFO_TAG_CARD 14
#define CARD_PXI_COMMAND_MASK 0x3f
#define CARD_PXI_COMMAND_TERMINATE 0x01
#define CARD_PXI_COMMAND_PULLED_OUT 0x11

#define PM_SUCCESS 0
#define PM_BUSY 4

#define HW_CPU_CLOCK_ARM9 67027964
#define PAD_DETECT_FOLD_MASK 0x8000
#define PAD_DETECT_FOLD_SHIFT 15
#define HW_BUTTON_XY_BUF 0x027fffa8

// Where the boot code left the card's ID, depending on how the system booted.
#define HW_BOOT_CHECK_INFO_BUF 0x027ffc00
#define HW_BOOT_CHECK_FLAG 0x027ffc10
#define HW_RED_RESERVED 0x027ff800

typedef BOOL (*CARDPulledOutCallback)(void);

u32 PM_ForceToPowerOff(void);
void MI_StopDma(u32 dmaNo);
void WaitByLoop(s32 count);

static CARDPulledOutCallback CARD_UserCallback;
static BOOL CARDi_IsPulledOutFlag;

static void CARDi_PulledOutCallback(int tag, u32 data, BOOL err);
void CARD_TerminateForPulledOut(void);
static void CARDi_SendtoPxi(u32 data, u32 wait);

static inline BOOL PAD_DetectFold(void) {
    return (BOOL) ((*(vu16 *) HW_BUTTON_XY_BUF & PAD_DETECT_FOLD_MASK) >> PAD_DETECT_FOLD_SHIFT);
}

void CARD_InitPulledOutCallback(void) {
    PXI_Init();
    PXI_SetFifoRecvCallback(PXI_FIFO_TAG_CARD, CARDi_PulledOutCallback);
    CARD_UserCallback = NULL;
}

static void CARDi_PulledOutCallback(int tag, u32 data, BOOL err) {
#pragma unused(tag, err)
    u32 command = data & CARD_PXI_COMMAND_MASK;
    if (command == CARD_PXI_COMMAND_PULLED_OUT) {
        if (!CARDi_IsPulledOutFlag) {
            BOOL isTerminateImm = TRUE;
            CARDi_IsPulledOutFlag = TRUE;
            if (CARD_UserCallback) {
                isTerminateImm = CARD_UserCallback();
            }
            if (isTerminateImm) {
                CARD_TerminateForPulledOut();
            }
        }
    } else {
        OS_Terminate();
    }
}

BOOL CARD_IsPulledOut(void) {
    return CARDi_IsPulledOutFlag;
}

void CARD_TerminateForPulledOut(void) {
    BOOL should_be_halt = TRUE;

    MI_StopDma(0);
    MI_StopDma(1);
    MI_StopDma(2);
    MI_StopDma(3);
    if (PAD_DetectFold()) {
        u32 res;
        while ((res = PM_ForceToPowerOff()) == PM_BUSY) {
            OS_SpinWait(HW_CPU_CLOCK_ARM9 / 100);
        }
        if (res == PM_SUCCESS) {
            should_be_halt = FALSE;
        }
    }
    if (should_be_halt) {
        CARDi_SendtoPxi(CARD_PXI_COMMAND_TERMINATE, 1);
    }
    OS_Terminate();
}

void CARDi_CheckPulledOutCore(u32 id) {
    vu32 iplCardID = *(u32 *) (*(u16 *) HW_BOOT_CHECK_FLAG == 0 ? HW_RED_RESERVED : HW_BOOT_CHECK_INFO_BUF);
    if (id != (u32) iplCardID) {
        OSIntrMode bak_cpsr = OS_DisableInterrupts();
        CARDi_PulledOutCallback(PXI_FIFO_TAG_CARD, CARD_PXI_COMMAND_PULLED_OUT, FALSE);
        (void) OS_RestoreInterrupts(bak_cpsr);
    }
}

static void CARDi_SendtoPxi(u32 data, u32 wait) {
    while (PXI_SendWordByFifo(PXI_FIFO_TAG_CARD, data, FALSE) != 0) {
        WaitByLoop((s32) wait);
    }
}
