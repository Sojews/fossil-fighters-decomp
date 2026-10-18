// NitroSDK CTRDG: whether the cartridge seen at startup is still there, and locking the slot against the ARM7.

#include "ctrdg_internal.h"

CTRDGWork CTRDGi_Work;
static BOOL CTRDGi_EnableFlag = FALSE;

void CTRDGi_InitCommon(void) {
    vu32 zero = 0;
    CpuSet((void *) &zero, &CTRDGi_Work, ((sizeof(CTRDGi_Work) / 4) & 0x1FFFFF) | (1 << 26) | (1 << 24));
    CTRDGi_Work.lockID = (u16) OS_GetLockID();
}

BOOL CTRDG_IsOptionCartridge(void) {
    return CTRDG_IsExisting() && !CTRDGi_IsAgbCartridgeAtInit();
}

BOOL CTRDGi_IsAgbCartridgeAtInit(void) {
    CTRDGModuleInfo *cip = CTRDGi_GetModuleInfoAddr();
    return cip->isAgbCartridge;
}

// Compares the cartridge with what was there at startup (and remembers if it was pulled out).
BOOL CTRDG_IsExisting(void) {
    BOOL retval = TRUE;
    CTRDGLockByProc lockInfo;
    CTRDGHeader *chp = CTRDGi_GetHeaderAddr();
    CTRDGModuleInfo *cip = CTRDGi_GetModuleInfoAddr();

    if (cip->moduleID.raw == 0xFFFF) {
        return FALSE;
    }
    if (cip->detectPullOut == TRUE) {
        return FALSE;
    }
    CTRDGi_LockByProcessor(CTRDGi_Work.lockID, &lockInfo);
    {
        CTRDGRomCycle rc;
        u8 isRomCode;
        CTRDGi_ChangeLatestAccessCycle(&rc);
        isRomCode = chp->isRomCode;
        if ((isRomCode == CTRDG_IS_ROM_CODE && cip->moduleID.raw != chp->moduleID) ||
            (isRomCode != CTRDG_IS_ROM_CODE && cip->moduleID.raw != *CTRDGi_GetModuleIDImageAddr()) ||
            (cip->gameCode != chp->gameCode && cip->isAgbCartridge)) {
            cip->detectPullOut = TRUE;
            retval = FALSE;
        }
        CTRDGi_RestoreAccessCycle(&rc);
    }
    CTRDGi_UnlockByProcessor(CTRDGi_Work.lockID, &lockInfo);
    return retval;
}

// The slowest cycles, which any cartridge can do.
void CTRDGi_ChangeLatestAccessCycle(CTRDGRomCycle *r) {
    r->c1 = MI_GetCartridgeRomCycle1st();
    r->c2 = MI_GetCartridgeRomCycle2nd();
    MI_SetCartridgeRomCycle1st(MI_CTRDG_ROMCYCLE1_18);
    MI_SetCartridgeRomCycle2nd(MI_CTRDG_ROMCYCLE2_6);
}

void CTRDGi_RestoreAccessCycle(CTRDGRomCycle *r) {
    MI_SetCartridgeRomCycle1st(r->c1);
    MI_SetCartridgeRomCycle2nd(r->c2);
}

// With interrupts off (unless this processor holds the lock already).
void CTRDGi_LockByProcessor(u16 lockID, CTRDGLockByProc *info) {
    while (1) {
        info->irq = OS_DisableInterrupts();
        if ((info->locked = OS_ReadOwnerOfLockWord((void *) HW_CTRDG_LOCK_BUF) & CTRDG_LOCKED_BY_MYPROC_FLAG) != 0 ||
            OS_TryLockCartridge(lockID) == OS_LOCK_SUCCESS) {
            break;
        }
        OS_RestoreInterrupts(info->irq);
        WaitByLoop(1);
    }
}

void CTRDGi_UnlockByProcessor(u16 lockID, CTRDGLockByProc *info) {
    if (!info->locked) {
        OS_UnLockCartridge(lockID);
    }
    OS_RestoreInterrupts(info->irq);
}

void CTRDGi_SendtoPxi(u32 data) {
    while (PXI_SendWordByFifo(PXI_FIFO_TAG_CTRDG, data, FALSE) != PXI_FIFO_SUCCESS) {
        WaitByLoop(1);
    }
}

// Lets the game write to the slot (protection region 3), unless it's an option cartridge.
void CTRDG_Enable(BOOL enable) {
    OSIntrMode bak_cpsr = OS_DisableInterrupts();
    CTRDGi_EnableFlag = enable;
    if (!CTRDG_IsOptionCartridge()) {
        u32 acc = (u32) (enable ? 0x1000 : 0x5000);
        OS_SetDPermissionsForProtectionRegion(0xF000, acc);
    }
    OS_RestoreInterrupts(bak_cpsr);
}
