// NitroSDK CTRDG (the Game Boy Advance slot): the module information the ARM7 reads at startup, the cartridge
// lock shared with the ARM7, and the task thread for backup memory access.
#ifndef CTRDG_INTERNAL_H
#define CTRDG_INTERNAL_H

#include "nitro/mi.h"
#include "nitro/os.h"
#include "nitro/pxi.h"

#define HW_CTRDG_ROM 0x08000000
#define HW_CTRDG_MODULE_INFO_BUF 0x027FFC30
#define HW_SET_CTRDG_MODULE_INFO_ONCE 0x027FFF9A
#define HW_IS_CTRDG_EXIST 0x027FFF9B
#define HW_CTRDG_LOCK_BUF 0x027FFFE8
#define HW_MAIN_MEM 0x02000000
#define CTRDG_SYSROM9_NINLOGO_ADR 0xFFFF0020

#define reg_MI_EXMEMCNT (*(vu16 *) 0x04000204)
#define reg_OS_IME (*(vu16 *) 0x04000208)
#define reg_OS_PAUSE (*(vu16 *) 0x04000300)

#define PXI_FIFO_TAG_CTRDG 13
#define PXI_FIFO_TAG_CTRDG_Ex 16
#define PXI_FIFO_TAG_CTRDG_PHI 17
#define PXI_FIFO_SUCCESS 0

#define CTRDG_PXI_COMMAND_MASK 0x3F
#define CTRDG_PXI_COMMAND_PARAM_SHIFT 6
#define CTRDG_PXI_COMMAND_INIT_MODULE_INFO 0x01
#define CTRDG_PXI_COMMAND_TERMINATE 0x02
#define CTRDG_PXI_COMMAND_SET_PHI 0x03
#define CTRDG_PXI_COMMAND_PULLED_OUT 0x11

#define CTRDG_LOCKED_BY_MYPROC_FLAG 0x40
#define CTRDG_IS_ROM_CODE 0x96
#define OS_LOCK_SUCCESS 0

// The cartridge ROM header.
typedef struct {
    u32 startAddress;
    u8 nintendoLogo[0x9C];
    char titleName[12];
    u32 gameCode;
    u16 makerCode;
    u8 isRomCode;
    u8 machineCode;
    u8 deviceType;
    u8 exLsiID[3];
    u8 reserved_A[4];
    u8 softVersion;
    u8 complement;
    u16 moduleID;
} CTRDGHeader;

typedef struct {
    union {
        struct {
            u8 bitID;
            u8 numberID : 5;
            u8 : 2;
            u8 disableExLsiID : 1;
        };
        u16 raw;
    };
} CTRDGModuleID;

// What was in the slot at startup.
typedef struct {
    CTRDGModuleID moduleID;
    u8 exLsiID[3];
    u8 isAgbCartridge : 1;
    u8 detectPullOut : 1;
    u8 : 0;
    u16 makerCode;
    u32 gameCode;
} CTRDGModuleInfo;

typedef struct {
    vu16 subpInitialized;
    u16 lockID;
} CTRDGWork;

typedef enum {
    MI_CTRDG_ROMCYCLE1_10,
    MI_CTRDG_ROMCYCLE1_8,
    MI_CTRDG_ROMCYCLE1_6,
    MI_CTRDG_ROMCYCLE1_18
} MICartridgeRomCycle1st;

typedef enum {
    MI_CTRDG_ROMCYCLE2_6,
    MI_CTRDG_ROMCYCLE2_4
} MICartridgeRomCycle2nd;

typedef struct {
    MICartridgeRomCycle1st c1;
    MICartridgeRomCycle2nd c2;
} CTRDGRomCycle;

typedef struct {
    BOOL locked;
    OSIntrMode irq;
} CTRDGLockByProc;

typedef BOOL (*CTRDGPulledOutCallback)(void);

#define CTRDG_TASK_PRIORITY_DEFAULT 20
#define CTRDG_TASK_STACK_SIZE 1024

struct CTRDGTaskInfo;
typedef u32 (*CTRDG_TASK_FUNC)(struct CTRDGTaskInfo *);

typedef struct CTRDGTaskInfo {
    CTRDG_TASK_FUNC task;
    CTRDG_TASK_FUNC callback;
    u32 result;
    u8 *data;
    u8 *adr;
    u32 offset;
    u32 size;
    u8 *dst;
    u16 sec_num;
    u8 busy;
    u8 param[1];
} CTRDGTaskInfo;

typedef struct {
    OSThread th[1];
    CTRDGTaskInfo *volatile list;
    CTRDGTaskInfo end_task;
} CTRDGiTaskWork;

#define CTRDGi_GetHeaderAddr() ((CTRDGHeader *) HW_CTRDG_ROM)
#define CTRDGi_GetModuleIDImageAddr() ((u16 *) (HW_CTRDG_ROM + 0x1FFFE))
#define CTRDGi_GetModuleInfoAddr() ((CTRDGModuleInfo *) HW_CTRDG_MODULE_INFO_BUF)

// The ROM access cycles in EXMEMCNT.
static inline MICartridgeRomCycle1st MI_GetCartridgeRomCycle1st(void) {
    return (MICartridgeRomCycle1st) ((reg_MI_EXMEMCNT & 0xC) >> 2);
}
static inline MICartridgeRomCycle2nd MI_GetCartridgeRomCycle2nd(void) {
    return (MICartridgeRomCycle2nd) ((reg_MI_EXMEMCNT & 0x10) >> 4);
}
static inline void MI_SetCartridgeRomCycle1st(MICartridgeRomCycle1st c) {
    reg_MI_EXMEMCNT = (u16) ((reg_MI_EXMEMCNT & ~0xC) | (c << 2));
}
static inline void MI_SetCartridgeRomCycle2nd(MICartridgeRomCycle2nd c) {
    reg_MI_EXMEMCNT = (u16) ((reg_MI_EXMEMCNT & ~0x10) | (c << 4));
}
static inline u32 MI_GetMainMemoryPriority(void) {
    return (reg_MI_EXMEMCNT & 0x8000) >> 15;
}
static inline void MI_SetMainMemoryPriority(u32 proc) {
    reg_MI_EXMEMCNT = (u16) ((reg_MI_EXMEMCNT & ~0x8000) | (proc << 15));
}

static inline BOOL OS_EnableIrq(void) {
    u16 prep = reg_OS_IME;
    reg_OS_IME = 1;
    return (BOOL) prep;
}
static inline BOOL OS_RestoreIrq(BOOL enable) {
    u16 prep = reg_OS_IME;
    reg_OS_IME = (u16) enable;
    return (BOOL) prep;
}

void CpuSet(const void *src, void *dest, u32 ctrl);
void WaitByLoop(s32 count);
s32 OS_GetLockID(void);
s32 OS_TryLockCartridge(u16 lockID);
s32 OS_UnLockCartridge(u16 lockID);
u16 OS_ReadOwnerOfLockWord(void *lock);
u32 OS_SetDPermissionsForProtectionRegion(u32 setMask, u32 flags);
void OS_ExitThread(void);
void DC_InvalidateRange(void *startAddr, u32 nBytes);

extern CTRDGWork CTRDGi_Work;

void CTRDGi_InitCommon(void);
void CTRDGi_InitModuleInfo(void);
void CTRDGi_SendtoPxi(u32 data);
void CTRDGi_LockByProcessor(u16 lockID, CTRDGLockByProc *info);
void CTRDGi_UnlockByProcessor(u16 lockID, CTRDGLockByProc *info);
void CTRDGi_ChangeLatestAccessCycle(CTRDGRomCycle *r);
void CTRDGi_RestoreAccessCycle(CTRDGRomCycle *r);
BOOL CTRDGi_IsAgbCartridgeAtInit(void);
BOOL CTRDG_IsExisting(void);
BOOL CTRDG_IsOptionCartridge(void);
BOOL CTRDG_IsPulledOut(void);
void CTRDG_Enable(BOOL enable);
void CTRDG_TerminateForPulledOut(void);
void CTRDGi_InitTaskThread(void *p_work);
void CTRDGi_InitTaskInfo(CTRDGTaskInfo *pt);

#endif
