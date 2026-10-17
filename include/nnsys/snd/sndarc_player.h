// NitroSystem sound: playing sequences straight from the sound archive.
#ifndef NNSYS_SND_SNDARC_PLAYER_H
#define NNSYS_SND_SNDARC_PLAYER_H

#include "nnsys/snd/player.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    u8 seqMax;
    u8 padding;
    u16 allocChBitFlag;
    u32 heapSize;
} NNSSndArcPlayerInfo;

// A sequence archive's entry for one of its sequences.
typedef struct {
    u32 offset;
    u16 bankNo;
    u8 volume;
    u8 channelPrio;
    u8 playerPrio;
    u8 playerNo;
    u16 reserved;
} NNSSndSeqArcSeqInfo;

typedef struct {
    u8 fileHeader[0x10];
    u32 blockKind;
    u32 blockSize;
    u32 baseOffset;
} NNSSndSeqData;

BOOL NNS_SndArcPlayerSetup(NNSSndHeapHandle heap);
BOOL NNS_SndArcPlayerStartSeq(NNSSndHandle *handle, int seqNo);
BOOL NNS_SndArcPlayerStartSeqArc(NNSSndHandle *handle, int seqArcNo, int index);
const NNSSndSeqArcSeqInfo *NNSi_SndSeqArcGetSeqInfo(const void *seqArc, int index);

#ifdef __cplusplus
}
#endif

#endif
