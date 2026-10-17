// NitroSystem sound: the sound heap, which frees in reverse order of allocation by saved levels.
#ifndef NNSYS_SND_HEAP_H
#define NNSYS_SND_HEAP_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NNSSndHeap *NNSSndHeapHandle;
typedef void (*NNSSndHeapDisposeCallback)(void *mem, u32 size, u32 data1, u32 data2);

NNSSndHeapHandle NNS_SndHeapCreate(void *startAddress, u32 size);
void NNS_SndHeapDestroy(NNSSndHeapHandle heap);
void NNS_SndHeapClear(NNSSndHeapHandle heap);
void *NNS_SndHeapAlloc(NNSSndHeapHandle heap, u32 size, NNSSndHeapDisposeCallback callback, u32 data1, u32 data2);
int NNS_SndHeapSaveState(NNSSndHeapHandle heap);
void NNS_SndHeapLoadState(NNSSndHeapHandle heap, int level);
u32 NNS_SndHeapGetFreeSize(NNSSndHeapHandle heap);

#ifdef __cplusplus
}
#endif

#endif
