// NitroSDK OS: lock words shared with the ARM7 (swapped atomically), and the card and cartridge bus locks.

#include "nitro/mi.h"
#include "nitro/os.h"

typedef struct {
    u32 lockFlag;
    u16 ownerID;
    u16 extension;
} OSLockWord;

#define REG_MI_EXMEMCNT (*(volatile u16 *) 0x04000204)
#define HW_INIT_LOCK_BUF ((OSLockWord *) 0x027ffff0)
#define HW_CTRDG_LOCK_BUF ((OSLockWord *) 0x027fffe8)
#define HW_CARD_LOCK_BUF ((OSLockWord *) 0x027fffe0)
#define HW_SHARED_LOCK_BUF ((void *) 0x027fffc0)
#define HW_LOCK_ID_FLAG_MAIN ((u32 *) 0x027fffb0)

#define OS_UNLOCK_SUCCESS 0
#define OS_UNLOCK_ERROR -2

void WaitByLoop(s32 count);
u32 MI_SwapWord(u32 setData, volatile u32 *destp);
OSIntrMode OS_DisableInterrupts_IrqAndFiq(void);
OSIntrMode OS_RestoreInterrupts_IrqAndFiq(OSIntrMode state);
s32 OS_LockByWord(u16 lockID, OSLockWord *lockp, void (*ctrlFuncp)(void));
s32 OS_UnlockByWord(u16 lockID, OSLockWord *lockp, void (*ctrlFuncp)(void));
static s32 OSi_DoTryLockByWord(u16 lockID, OSLockWord *lockp, void (*ctrlFuncp)(void), BOOL disableFiq);
void OSi_AllocateCartridgeBus(void);
void OSi_FreeCartridgeBus(void);
void OSi_AllocateCardBus(void);
void OSi_FreeCardBus(void);

void OS_InitLock(void) {
    static BOOL isInitialized;
    OSLockWord *lockp = HW_INIT_LOCK_BUF;
    if (isInitialized) {
        return;
    }
    isInitialized = TRUE;
    lockp->lockFlag = 0;
    OS_LockByWord(0x7e, lockp, NULL);
    while (lockp->extension != 0) {
        WaitByLoop(0x400);
    }
    HW_LOCK_ID_FLAG_MAIN[0] = 0xffffffff;
    HW_LOCK_ID_FLAG_MAIN[1] = 0xffff0000;
    MIi_CpuClear32(0, HW_SHARED_LOCK_BUF, 0x28);
    REG_MI_EXMEMCNT |= 0x800;
    REG_MI_EXMEMCNT |= 0x80;
    OS_UnlockByWord(0x7e, lockp, NULL);
    OS_LockByWord(0x7f, lockp, NULL);
}

static s32 OSi_DoLockByWord(u16 lockID, OSLockWord *lockp, void (*ctrlFuncp)(void), BOOL disableFiq) {
    s32 lastLockFlag;
    while ((lastLockFlag = OSi_DoTryLockByWord(lockID, lockp, ctrlFuncp, disableFiq)) > 0) {
        WaitByLoop(0x400);
    }
    return lastLockFlag;
}

s32 OS_LockByWord(u16 lockID, OSLockWord *lockp, void (*ctrlFuncp)(void)) {
    return OSi_DoLockByWord(lockID, lockp, ctrlFuncp, FALSE);
}

static s32 OSi_DoUnlockByWord(u16 lockID, OSLockWord *lockp, void (*ctrlFuncp)(void), BOOL disableFiq) {
    OSIntrMode enabled;
    if (lockID != lockp->ownerID) {
        return OS_UNLOCK_ERROR;
    }
    if (disableFiq) {
        enabled = OS_DisableInterrupts_IrqAndFiq();
    } else {
        enabled = OS_DisableInterrupts();
    }
    lockp->ownerID = 0;
    if (ctrlFuncp != NULL) {
        ctrlFuncp();
    }
    lockp->lockFlag = 0;
    if (disableFiq) {
        OS_RestoreInterrupts_IrqAndFiq(enabled);
    } else {
        OS_RestoreInterrupts(enabled);
    }
    return OS_UNLOCK_SUCCESS;
}

s32 OS_UnlockByWord(u16 lockID, OSLockWord *lockp, void (*ctrlFuncp)(void)) {
    return OSi_DoUnlockByWord(lockID, lockp, ctrlFuncp, FALSE);
}

static s32 OSi_DoTryLockByWord(u16 lockID, OSLockWord *lockp, void (*ctrlFuncp)(void), BOOL disableFiq) {
    s32 lastLockFlag;
    OSIntrMode enabled;
    if (disableFiq) {
        enabled = OS_DisableInterrupts_IrqAndFiq();
    } else {
        enabled = OS_DisableInterrupts();
    }
    lastLockFlag = (s32) MI_SwapWord(lockID, &lockp->lockFlag);
    if (lastLockFlag == 0) {
        if (ctrlFuncp != NULL) {
            ctrlFuncp();
        }
        lockp->ownerID = lockID;
    }
    if (disableFiq) {
        OS_RestoreInterrupts_IrqAndFiq(enabled);
    } else {
        OS_RestoreInterrupts(enabled);
    }
    return lastLockFlag;
}

s32 OS_UnlockCartridge(u16 lockID) {
    return OSi_DoUnlockByWord(lockID, HW_CTRDG_LOCK_BUF, OSi_FreeCartridgeBus, TRUE);
}

s32 OS_UnLockCartridge(u16 lockID) {
    return OS_UnlockCartridge(lockID);
}

s32 OS_TryLockCartridge(u16 lockID) {
    return OSi_DoTryLockByWord(lockID, HW_CTRDG_LOCK_BUF, OSi_AllocateCartridgeBus, TRUE);
}

void OSi_AllocateCartridgeBus(void) {
    REG_MI_EXMEMCNT = REG_MI_EXMEMCNT & ~0x80;
}

void OSi_FreeCartridgeBus(void) {
    REG_MI_EXMEMCNT |= 0x80;
}

s32 OS_LockCard(u16 lockID) {
    return OS_LockByWord(lockID, HW_CARD_LOCK_BUF, OSi_AllocateCardBus);
}

s32 OS_UnlockCard(u16 lockID) {
    return OS_UnlockByWord(lockID, HW_CARD_LOCK_BUF, OSi_FreeCardBus);
}

void OSi_AllocateCardBus(void) {
    REG_MI_EXMEMCNT &= ~0x800;
}

void OSi_FreeCardBus(void) {
    REG_MI_EXMEMCNT |= 0x800;
}

u16 OS_ReadOwnerOfLockWord(OSLockWord *lockp) {
    return lockp->ownerID;
}

#pragma thumb off
asm s32 OS_GetLockID(void) {
    ldr r3, =0x027fffb0
    ldr r1, [r3, #0]
    clz r2, r1
    cmp r2, #32
    movne r0, #0x40
    bne _found
    add r3, r3, #4
    ldr r1, [r3, #0]
    clz r2, r1
    cmp r2, #32
    ldr r0, =-3
    bxeq lr
    mov r0, #0x60
_found:
    add r0, r0, r2
    mov r1, #0x80000000
    mov r1, r1, lsr r2
    ldr r2, [r3, #0]
    bic r2, r2, r1
    str r2, [r3, #0]
    bx lr
}

asm void OS_ReleaseLockID(u16 lockID) {
    ldr r3, =0x027fffb0
    cmp r0, #0x60
    addpl r3, r3, #4
    subpl r0, r0, #0x60
    submi r0, r0, #0x40
    mov r1, #0x80000000
    mov r1, r1, lsr r0
    ldr r2, [r3, #0]
    orr r2, r2, r1
    str r2, [r3, #0]
    bx lr
}
#pragma thumb reset
