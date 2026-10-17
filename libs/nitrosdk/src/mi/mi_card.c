// NitroSDK MI: DMA from the card data register.

#include "nitro/mi.h"

#define REG_DMA0SAD_ADDR 0x040000b0
#define MI_DMA_SRC_FIX 0x01000000
#define MI_CNT_CARDRECV32 0xaf000001

void MIi_DmaSetParams(u32 dmaNo, u32 src, u32 dest, u32 ctrl);
void MIi_CheckAnotherAutoDMA(u32 dmaNo, u32 dmaType);
void MIi_CheckDma0SourceAddress(u32 dmaNo, u32 src, u32 size, u32 dir);

void MIi_CardDmaCopy32(u32 dmaNo, const void *src, void *dest, u32 size) {
    vu32 *dmaCntp;
    MIi_CheckAnotherAutoDMA(dmaNo, (u32) -1);
    MIi_CheckDma0SourceAddress(dmaNo, (u32) src, size, MI_DMA_SRC_FIX);
    if (size == 0) {
        return;
    }
    dmaCntp = &((vu32 *) REG_DMA0SAD_ADDR)[dmaNo * 3 + 2];
    while (*dmaCntp & 0x80000000) {
    }
    MIi_DmaSetParams(dmaNo, (u32) src, (u32) dest, MI_CNT_CARDRECV32);
}
