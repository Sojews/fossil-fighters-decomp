// The parts of the NitroSDK PXI (ARM9/ARM7 FIFO) library the SDK uses (declarations only).
#ifndef NITRO_PXI_H
#define NITRO_PXI_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PXI_FIFO_TAG_SOUND 7
#define PXI_PROC_ARM7 1

typedef void (*PXIFifoCallback)(int tag, u32 data, BOOL err);

void PXI_Init(void);
void PXI_SetFifoRecvCallback(int fifotag, PXIFifoCallback callback);
BOOL PXI_IsCallbackReady(int fifotag, int proc);
int PXI_SendWordByFifo(int fifotag, u32 data, BOOL err);

#ifdef __cplusplus
}
#endif

#endif
