// DMA channel bookkeeping.

#include "m/dma.h"
#include "m/irq.h"
#include "nitro/os.h"

static bool sDmaChannelUsed[4];

void MReserveDmaChannel(u32 channel) {
    MIrqLock lock;
    sDmaChannelUsed[channel] = TRUE;
}

u32 MAllocDmaChannel() {
    MIrqLock lock;
    for (u32 i = 0; i < 4; i++) {
        if (!sDmaChannelUsed[i]) {
            sDmaChannelUsed[i] = TRUE;
            return i;
        }
    }
    OS_Terminate();
    return 0;
}

void MFreeDmaChannel(u32 channel) {
    MIrqLock lock;
    sDmaChannelUsed[channel] = FALSE;
}
