// NitroSystem sound: sequence players, grouped into players that limit how many play at once.
#ifndef NNSYS_SND_PLAYER_H
#define NNSYS_SND_PLAYER_H

#include "nnsys/fnd/list.h"
#include "nnsys/snd/fader.h"
#include "nnsys/snd/heap.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NNS_SND_PLAYER_NUM 32
#define NNS_SND_SEQ_PLAYER_NUM 16

typedef struct NNSSndSeqPlayer NNSSndSeqPlayer;

typedef struct {
    NNSSndSeqPlayer *player;
} NNSSndHandle;

typedef struct {
    NNSFndList playList;        // The sequence players playing, by priority
    NNSFndList heapList;        // Free heaps for them
    u32 playableSeqCount;
    u32 allocChBitFlag;
    u8 volume;
} NNSSndPlayer;

typedef struct {
    NNSFndLink link;
    NNSSndHeapHandle handle;
    NNSSndSeqPlayer *player;    // Using it, or NULL
    int playerNo;
} NNSSndPlayerHeap;

enum {
    NNS_SND_SEQ_TYPE_NONE,
    NNS_SND_SEQ_TYPE_SEQ,
    NNS_SND_SEQ_TYPE_SEQARC
};

enum {
    NNS_SND_SEQ_PLAYER_STATUS_FREE,
    NNS_SND_SEQ_PLAYER_STATUS_PLAY,
    NNS_SND_SEQ_PLAYER_STATUS_FADEOUT
};

struct NNSSndSeqPlayer {
    NNSSndHandle *handle;
    NNSSndPlayer *player;
    NNSSndPlayerHeap *heap;
    NNSFndLink playerLink;      // In the player's playList
    NNSFndLink prioLink;        // In the priority list, or the free list
    NNSSndFader fader;
    u8 status;
    u8 prepared;                // The SND library has the sequence ready
    u8 pause;
    u8 startRequest;
    u32 commandTag;
    u16 seqType;
    u16 unk36;
    u16 seqNo;
    u16 seqArcIndex;
    u8 playerNo;
    u8 prio;
    s16 volume;
    u8 initVolume;
    u8 extVolume;
};

void NNS_SndPlayerSetPlayerVolume(int playerNo, int volume);
void NNS_SndPlayerSetPlayableSeqCount(int playerNo, int seqCount);
void NNS_SndPlayerSetAllocatableChannel(int playerNo, u32 chBitFlag);
BOOL NNS_SndPlayerCreateHeap(int playerNo, NNSSndHeapHandle heap, u32 size);
void NNS_SndPlayerStopSeqByPlayerNo(int playerNo, int fadeFrame);
void NNS_SndPlayerStopSeqBySeqNo(int seqNo, int fadeFrame);
void NNS_SndPlayerStopSeqBySeqArcNo(int seqArcNo, int fadeFrame);
void NNS_SndPlayerStopSeqBySeqArcIdx(int seqArcNo, int index, int fadeFrame);
void NNS_SndPlayerPauseByPlayerNo(int playerNo, BOOL flag);
static inline BOOL NNS_SndHandleIsValid(const NNSSndHandle *handle) {
    return handle->player != NULL;
}

void NNS_SndHandleInit(NNSSndHandle *handle);
void NNS_SndHandleReleaseSeq(NNSSndHandle *handle);
int NNS_SndPlayerCountPlayingSeqBySeqNo(int seqNo);
int NNS_SndPlayerCountPlayingSeqBySeqArcIdx(int seqArcNo, int index);
void NNS_SndPlayerSetVolume(NNSSndHandle *handle, int volume);
void NNS_SndPlayerSetInitialVolume(NNSSndHandle *handle, int volume);
void NNS_SndPlayerMoveVolume(NNSSndHandle *handle, int targetVolume, int frames);
void NNS_SndPlayerSetChannelPriority(NNSSndHandle *handle, int prio);
void NNS_SndPlayerSetTrackMute(NNSSndHandle *handle, u32 trackBitMask, BOOL flag);
void NNS_SndPlayerSetTrackVolume(NNSSndHandle *handle, u32 trackBitMask, int volume);
void NNS_SndPlayerSetTrackPitch(NNSSndHandle *handle, u32 trackBitMask, int pitch);
void NNS_SndPlayerSetTrackPan(NNSSndHandle *handle, u32 trackBitMask, int pan);
void NNS_SndPlayerSetTempoRatio(NNSSndHandle *handle, int ratio);
void NNS_SndPlayerSetSeqNo(NNSSndHandle *handle, int seqNo);
void NNS_SndPlayerSetSeqArcNo(NNSSndHandle *handle, int seqArcNo, int index);
BOOL NNS_SndPlayerWriteGlobalVariable(int varNo, s16 var);

NNSSndSeqPlayer *NNSi_SndPlayerAllocSeqPlayer(NNSSndHandle *handle, int playerNo, int prio);
void NNSi_SndPlayerFreeSeqPlayer(NNSSndSeqPlayer *seqPlayer);
void NNSi_SndPlayerStartSeq(NNSSndSeqPlayer *seqPlayer, const void *seqBase, u32 seqOffset, const void *bank);
void NNSi_SndPlayerStopSeq(NNSSndSeqPlayer *seqPlayer, int fadeFrame);
void NNSi_SndPlayerPause(NNSSndSeqPlayer *seqPlayer, BOOL flag);
NNSSndHeapHandle NNSi_SndPlayerAllocHeap(int playerNo, NNSSndSeqPlayer *seqPlayer);

#ifdef __cplusplus
}
#endif

#endif
