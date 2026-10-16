// DMA channel bookkeeping.

#include "m/dma.h"
#include "m/irq.h"
#include "nitro/os.h"

u8 gDmaEnabled = TRUE;
u8 gDmaChannelUsed[4];

void MReserveDmaChannel(u32 channel) {
    MIrqLock lock;
    gDmaChannelUsed[channel] = TRUE;
}

u32 MAllocDmaChannel() {
    MIrqLock lock;
    for (u32 i = 0; i < 4; i++) {
        if (!gDmaChannelUsed[i]) {
            gDmaChannelUsed[i] = TRUE;
            return i;
        }
    }
    OS_Terminate();
    return 0;
}

void MFreeDmaChannel(u32 channel) {
    MIrqLock lock;
    gDmaChannelUsed[channel] = FALSE;
}
