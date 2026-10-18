// NitroSDK CTRDG: startup (reading the cartridge header for the ARM7) and the messages from the ARM7.

#include "ctrdg_internal.h"

static void CTRDGi_CallbackForInitModuleInfo(int tag, u32 data, BOOL err);
static void CTRDGi_PulledOutCallback(int tag, u32 data, BOOL err);
static void CTRDGi_CallbackForSetPhi(int tag, u32 data, BOOL err);

void MIi_SetPhiClock(u32 clock);

static int CTRDGi_Lock = FALSE;
CTRDGPulledOutCallback CTRDG_UserCallback = NULL;
static BOOL isCartridgePullOut = FALSE;
static BOOL skipCheck = FALSE;
static BOOL ctrdg_already_pullout = FALSE;

void CTRDG_Init(void) {
    static BOOL isInitialized;

    if (isInitialized) {
        return;
    }
    isInitialized = TRUE;
    CTRDGi_InitCommon();
    ctrdg_already_pullout = FALSE;
    PXI_Init();
    while (!PXI_IsCallbackReady(PXI_FIFO_TAG_CTRDG, PXI_PROC_ARM7)) {
    }
    PXI_SetFifoRecvCallback(PXI_FIFO_TAG_CTRDG, CTRDGi_CallbackForInitModuleInfo);
    CTRDGi_InitModuleInfo();
    PXI_SetFifoRecvCallback(PXI_FIFO_TAG_CTRDG, NULL);
    PXI_SetFifoRecvCallback(PXI_FIFO_TAG_CTRDG, CTRDGi_PulledOutCallback);
    CTRDG_UserCallback = NULL;
    {
        static CTRDGiTaskWork CTRDGTaskList;
        CTRDGi_InitTaskThread(&CTRDGTaskList);
    }
    PXI_SetFifoRecvCallback(PXI_FIFO_TAG_CTRDG_PHI, CTRDGi_CallbackForSetPhi);
    CTRDG_Enable(FALSE);
}

static CTRDGHeader headerBuf __attribute__((aligned(32)));

// Reads the header into main memory and waits for the ARM7 to take it.
void CTRDGi_InitModuleInfo(void) {
    static BOOL isInitialized;
    CTRDGLockByProc lockInfo;
    u32 lastIE;
    BOOL lastIME;

    if (isInitialized) {
        return;
    }
    isInitialized = TRUE;
    if (!(reg_OS_PAUSE & 1)) {
        return;
    }
    lastIE = OS_SetIrqMask(1 << 18);
    lastIME = OS_EnableIrq();
    CTRDGi_LockByProcessor(CTRDGi_Work.lockID, &lockInfo);
    {
        u32 proc = MI_GetMainMemoryPriority();
        CTRDGRomCycle rc;
        CTRDGi_ChangeLatestAccessCycle(&rc);
        MI_SetMainMemoryPriority(0);
        DC_InvalidateRange(&((u8 *) &headerBuf)[0x80], sizeof(headerBuf) - 0x80);
        MI_DmaCopy16(1, (void *) (HW_CTRDG_ROM + 0x80), &((u8 *) &headerBuf)[0x80], sizeof(headerBuf) - 0x80);
        MI_SetMainMemoryPriority(proc);
        CTRDGi_RestoreAccessCycle(&rc);
    }
    CTRDGi_UnlockByProcessor(CTRDGi_Work.lockID, &lockInfo);
    if (*(u8 *) HW_IS_CTRDG_EXIST || !*(u8 *) HW_SET_CTRDG_MODULE_INFO_ONCE) {
        int i;
        CTRDGHeader *chb = &headerBuf;
        CTRDGModuleInfo *cip = CTRDGi_GetModuleInfoAddr();
        cip->moduleID.raw = chb->moduleID;
        for (i = 0; i < 3; i++) {
            cip->exLsiID[i] = chb->exLsiID[i];
        }
        cip->makerCode = chb->makerCode;
        cip->gameCode = chb->gameCode;
        *(u8 *) HW_IS_CTRDG_EXIST = (u8) (CTRDG_IsExisting() ? 1 : 0);
        *(u8 *) HW_SET_CTRDG_MODULE_INFO_ONCE = TRUE;
    }
    MIi_CpuCopy32((void *) CTRDG_SYSROM9_NINLOGO_ADR, &headerBuf.nintendoLogo, sizeof(headerBuf.nintendoLogo));
    DC_FlushAll();
    CTRDGi_SendtoPxi(CTRDG_PXI_COMMAND_INIT_MODULE_INFO |
                     (((u32) &headerBuf - HW_MAIN_MEM) >> 5) << CTRDG_PXI_COMMAND_PARAM_SHIFT);
    while (CTRDGi_Work.subpInitialized != TRUE) {
        WaitByLoop(1);
    }
    OS_RestoreIrq(lastIME);
    OS_SetIrqMask(lastIE);
}

static void CTRDGi_CallbackForInitModuleInfo(int tag, u32 data, BOOL err) {
    if ((data & CTRDG_PXI_COMMAND_MASK) == CTRDG_PXI_COMMAND_INIT_MODULE_INFO) {
        CTRDGi_Work.subpInitialized = TRUE;
    } else {
        OS_Terminate();
    }
}

static void CTRDGi_PulledOutCallback(int tag, u32 data, BOOL err) {
    if ((data & CTRDG_PXI_COMMAND_MASK) == CTRDG_PXI_COMMAND_PULLED_OUT) {
        if (ctrdg_already_pullout == FALSE) {
            BOOL isTerminateImm = FALSE;
            if (CTRDG_UserCallback) {
                isTerminateImm = CTRDG_UserCallback();
            }
            if (isTerminateImm) {
                CTRDG_TerminateForPulledOut();
            }
            ctrdg_already_pullout = TRUE;
        }
    } else {
        OS_Terminate();
    }
}

void CTRDG_SetPulledOutCallback(CTRDGPulledOutCallback callback) {
    CTRDG_UserCallback = callback;
}

void CTRDG_TerminateForPulledOut(void) {
    CTRDGi_SendtoPxi(CTRDG_PXI_COMMAND_TERMINATE);
    OS_Terminate();
}

void CTRDG_SendToARM7(void *arg) {
    PXI_SendWordByFifo(PXI_FIFO_TAG_CTRDG_Ex, (u32) arg, FALSE);
}

void CTRDG_CheckPulledOut(void) {
    if (skipCheck || isCartridgePullOut) {
        return;
    }
    isCartridgePullOut = CTRDG_IsPulledOut();
    if (!CTRDG_IsExisting()) {
        if (!isCartridgePullOut) {
            skipCheck = TRUE;
            return;
        }
    }
    if (isCartridgePullOut) {
        CTRDGi_PulledOutCallback(PXI_FIFO_TAG_CTRDG, CTRDG_PXI_COMMAND_PULLED_OUT, FALSE);
    }
}

void CTRDG_SetPhiClock(u32 clock) {
    u32 data = (clock << CTRDG_PXI_COMMAND_PARAM_SHIFT) | CTRDG_PXI_COMMAND_SET_PHI;
    MIi_SetPhiClock(clock);
    CTRDGi_Lock = TRUE;
    while (PXI_SendWordByFifo(PXI_FIFO_TAG_CTRDG_PHI, data, FALSE) != PXI_FIFO_SUCCESS) {
        WaitByLoop(1);
    }
    while (CTRDGi_Lock) {
        WaitByLoop(1);
    }
}

static void CTRDGi_CallbackForSetPhi(int tag, u32 data, BOOL err) {
    CTRDGi_Lock = FALSE;
}
