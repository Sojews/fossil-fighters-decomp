// Bookkeeping of the four DMA channels the game hands out (for raster effects).
#ifndef M_DMA_H
#define M_DMA_H

#include "types.h"

// Marks a particular channel as used.
void MReserveDmaChannel(u32 channel);
// Returns a free channel and marks it used; stops the game if all four are used.
u32 MAllocDmaChannel();
void MFreeDmaChannel(u32 channel);

#endif
