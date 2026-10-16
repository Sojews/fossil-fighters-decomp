// Bookkeeping of the four DMA channels the game hands out (for raster effects).
#ifndef M_DMA_H
#define M_DMA_H

#include "types.h"

// Channel 3 does the game's own copies and fills (MCopy, MFill) while it's free and DMA is enabled.
#define M_DMA_NO 3

extern u8 gDmaEnabled;
extern u8 gDmaChannelUsed[4];

// Marks a particular channel as used.
void MReserveDmaChannel(u32 channel);
// Returns a free channel and marks it used; stops the game if all four are used.
u32 MAllocDmaChannel();
void MFreeDmaChannel(u32 channel);

#endif
