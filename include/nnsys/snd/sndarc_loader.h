// NitroSystem sound: loading sequences, banks and wave archives from the sound archive into a sound heap.
#ifndef NNSYS_SND_SNDARC_LOADER_H
#define NNSYS_SND_SNDARC_LOADER_H

#include "nnsys/snd/heap.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NNS_SND_ARC_LOAD_SEQ (1 << 0)
#define NNS_SND_ARC_LOAD_BANK (1 << 1)
#define NNS_SND_ARC_LOAD_WAVE (1 << 2)
#define NNS_SND_ARC_LOAD_SEQARC (1 << 3)

enum {
    NNS_SND_ARC_LOAD_SUCCESS,
    NNS_SND_ARC_LOAD_ERROR_INVALID_GROUP_NO,
    NNS_SND_ARC_LOAD_ERROR_INVALID_SEQ_NO,
    NNS_SND_ARC_LOAD_ERROR_INVALID_SEQARC_NO,
    NNS_SND_ARC_LOAD_ERROR_INVALID_BANK_NO,
    NNS_SND_ARC_LOAD_ERROR_INVALID_WAVEARC_NO,
    NNS_SND_ARC_LOAD_ERROR_FAILED_LOAD_SEQ,
    NNS_SND_ARC_LOAD_ERROR_FAILED_LOAD_SEQARC,
    NNS_SND_ARC_LOAD_ERROR_FAILED_LOAD_BANK,
    NNS_SND_ARC_LOAD_ERROR_FAILED_LOAD_WAVE
};

BOOL NNS_SndArcLoadGroup(int groupNo, NNSSndHeapHandle heap);
int NNSi_SndArcLoadGroup(int groupNo, NNSSndHeapHandle heap);
int NNSi_SndArcLoadSeq(int seqNo, u32 loadFlag, NNSSndHeapHandle heap, BOOL bSetAddr, void **pData);
int NNSi_SndArcLoadSeqArc(int seqArcNo, u32 loadFlag, NNSSndHeapHandle heap, BOOL bSetAddr, void **pData);
int NNSi_SndArcLoadBank(int bankNo, u32 loadFlag, NNSSndHeapHandle heap, BOOL bSetAddr, void **pData);
int NNSi_SndArcLoadWaveArc(int waveArcNo, u32 loadFlag, NNSSndHeapHandle heap, BOOL bSetAddr, void **pData);
void *NNSi_SndArcLoadFile(u32 fileId, NNSSndHeapDisposeCallback callback, u32 data1, u32 data2,
                          NNSSndHeapHandle heap);

#ifdef __cplusplus
}
#endif

#endif
