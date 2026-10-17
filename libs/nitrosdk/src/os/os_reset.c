// NitroSDK OS: software reset. The ARM7 is told first; the rest happens in OSi_DoResetSystem (in ITCM).

#include "nitro/os.h"
#include "nitro/pxi.h"

#define PXI_FIFO_TAG_OS 12
#define OS_PXI_COMMAND_MASK 0x7f00
#define OS_PXI_COMMAND_SHIFT 8
#define OS_PXI_COMMAND_RESET 0x10

#define OS_BOOTTYPE_DOWNLOAD_MB 2
#define OS_IE_FIFO_RECV (1 << 18)
#define HW_RESET_PARAMETER_BUF 0x027ffc20

s32 OS_GetLockID(void);
void CARD_LockRom(u16 lock_id);
void MI_StopDma(u32 dmaNo);
void OSi_DoResetSystem(void);

static u16 OSi_IsInitReset;
u16 OSi_IsResetOccurred;

static void OSi_CommonCallback(int tag, u32 data, BOOL err);
static void OSi_SendToPxi(u16 data);

#pragma thumb off
void OS_InitReset(void) {
    if (OSi_IsInitReset) {
        return;
    }
    OSi_IsInitReset = TRUE;
    PXI_Init();
    while (!PXI_IsCallbackReady(PXI_FIFO_TAG_OS, PXI_PROC_ARM7)) {
    }
    PXI_SetFifoRecvCallback(PXI_FIFO_TAG_OS, OSi_CommonCallback);
}

static void OSi_CommonCallback(int tag, u32 data, BOOL err) {
#pragma unused(tag, err)
    u16 command = (u16) ((data & OS_PXI_COMMAND_MASK) >> OS_PXI_COMMAND_SHIFT);
    if (command == OS_PXI_COMMAND_RESET) {
        OSi_IsResetOccurred = TRUE;
    } else {
        OS_Terminate();
    }
}

static void OSi_SendToPxi(u16 data) {
    u32 pxi_send_data = ((u32) data) << 8;
    while (PXI_SendWordByFifo(PXI_FIFO_TAG_OS, pxi_send_data, FALSE) != 0) {
    }
}

void OS_ResetSystem(u32 parameter) {
    if (*(u16 *) 0x027ffc40 == OS_BOOTTYPE_DOWNLOAD_MB) {
        OS_Terminate();
    }
    CARD_LockRom((u16) OS_GetLockID());
    MI_StopDma(0);
    MI_StopDma(1);
    MI_StopDma(2);
    MI_StopDma(3);
    (void) OS_SetIrqMask(OS_IE_FIFO_RECV);
    (void) OS_ResetRequestIrqMask((u32) ~0);
    *(u32 *) HW_RESET_PARAMETER_BUF = parameter;
    OSi_SendToPxi(OS_PXI_COMMAND_RESET);
    OSi_DoResetSystem();
}
#pragma thumb reset
