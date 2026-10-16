// The parts of the NitroSDK MI (memory) library the game code uses (declarations only).
#ifndef NITRO_MI_H
#define NITRO_MI_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

void MIi_CpuCopy16(const void *src, void *dest, u32 size);
void MIi_CpuCopy32(const void *src, void *dest, u32 size);
void MIi_CpuCopyFast(const void *src, void *dest, u32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void MIi_CpuClearFast(u32 data, void *dest, u32 size);
void MI_DmaCopy16(u32 dmaNo, const void *src, void *dest, u32 size);
void MI_DmaCopy32(u32 dmaNo, const void *src, void *dest, u32 size);
void MI_DmaFill16(u32 dmaNo, void *dest, u16 data, u32 size);
void MI_DmaFill32(u32 dmaNo, void *dest, u32 data, u32 size);

void DC_FlushRange(const void *startAddr, u32 nBytes);

#ifdef __cplusplus
}
#endif

#endif
