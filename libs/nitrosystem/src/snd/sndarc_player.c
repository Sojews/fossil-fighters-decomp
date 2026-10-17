// NitroSystem sound: playing sequences straight from the sound archive.

#include "nnsys/snd/sndarc.h"
#include "nnsys/snd/sndarc_loader.h"
#include "nnsys/snd/sndarc_player.h"

static BOOL StartSeq(NNSSndHandle *handle, int playerNo, int bankNo, int playerPrio, const NNSSndArcSeqInfo *info,
                     int seqNo);
static BOOL StartSeqArc(NNSSndHandle *handle, int playerNo, int bankNo, int playerPrio,
                        const NNSSndSeqArcSeqInfo *info, const NNSSndSeqData *seqArc, int seqArcNo, int index);

BOOL NNS_SndArcPlayerSetup(NNSSndHeapHandle heap) {
    int playerNo;
    NNS_SndArcGetCurrent();     // (What's left of an assertion that there's a current archive)
    for (playerNo = 0; playerNo < NNS_SND_PLAYER_NUM; playerNo++) {
        const NNSSndArcPlayerInfo *info = NNS_SndArcGetPlayerInfo(playerNo);
        if (info == NULL) {
            continue;
        }
        NNS_SndPlayerSetPlayableSeqCount(playerNo, info->seqMax);
        NNS_SndPlayerSetAllocatableChannel(playerNo, info->allocChBitFlag);
        if (info->heapSize != 0 && heap != NULL) {
            int i;
            for (i = 0; i < info->seqMax; i++) {
                if (!NNS_SndPlayerCreateHeap(playerNo, heap, info->heapSize)) {
                    return FALSE;
                }
            }
        }
    }
    return TRUE;
}

BOOL NNS_SndArcPlayerStartSeq(NNSSndHandle *handle, int seqNo) {
    const NNSSndArcSeqInfo *info = NNS_SndArcGetSeqInfo(seqNo);
    if (info == NULL) {
        return FALSE;
    }
    return StartSeq(handle, info->playerNo, info->bankNo, info->playerPrio, info, seqNo);
}

BOOL NNS_SndArcPlayerStartSeqArc(NNSSndHandle *handle, int seqArcNo, int index) {
    const NNSSndArcSeqArcInfo *info = NNS_SndArcGetSeqArcInfo(seqArcNo);
    const NNSSndSeqData *seqArc;
    const NNSSndSeqArcSeqInfo *seqInfo;
    if (info == NULL) {
        return FALSE;
    }
    seqArc = NNS_SndArcGetFileAddress(info->fileId);
    if (seqArc == NULL) {
        return FALSE;
    }
    seqInfo = NNSi_SndSeqArcGetSeqInfo(seqArc, index);
    if (seqInfo == NULL) {
        return FALSE;
    }
    return StartSeqArc(handle, seqInfo->playerNo, seqInfo->bankNo, seqInfo->playerPrio, seqInfo, seqArc, seqArcNo,
                       index);
}

static BOOL StartSeq(NNSSndHandle *handle, int playerNo, int bankNo, int playerPrio, const NNSSndArcSeqInfo *info,
                     int seqNo) {
    NNSSndSeqPlayer *seqPlayer;
    NNSSndHeapHandle heap;
    const NNSSndSeqData *seq;
    void *bank;

    seqPlayer = NNSi_SndPlayerAllocSeqPlayer(handle, playerNo, playerPrio);
    if (seqPlayer == NULL) {
        return FALSE;
    }
    heap = NNSi_SndPlayerAllocHeap(playerNo, seqPlayer);
    if (NNSi_SndArcLoadBank(bankNo, NNS_SND_ARC_LOAD_BANK | NNS_SND_ARC_LOAD_WAVE, heap, FALSE, &bank) !=
        NNS_SND_ARC_LOAD_SUCCESS) {
        NNSi_SndPlayerFreeSeqPlayer(seqPlayer);
        return FALSE;
    }
    if (NNSi_SndArcLoadSeq(seqNo, NNS_SND_ARC_LOAD_SEQ, heap, FALSE, (void **) &seq) != NNS_SND_ARC_LOAD_SUCCESS) {
        NNSi_SndPlayerFreeSeqPlayer(seqPlayer);
        return FALSE;
    }
    NNSi_SndPlayerStartSeq(seqPlayer, (const u8 *) seq + seq->baseOffset, 0, bank);
    NNS_SndPlayerSetInitialVolume(handle, info->volume);
    NNS_SndPlayerSetChannelPriority(handle, info->channelPrio);
    NNS_SndPlayerSetSeqNo(handle, seqNo);
    return TRUE;
}

static BOOL StartSeqArc(NNSSndHandle *handle, int playerNo, int bankNo, int playerPrio,
                        const NNSSndSeqArcSeqInfo *info, const NNSSndSeqData *seqArc, int seqArcNo, int index) {
    NNSSndSeqPlayer *seqPlayer;
    void *bank;

    seqPlayer = NNSi_SndPlayerAllocSeqPlayer(handle, playerNo, playerPrio);
    if (seqPlayer == NULL) {
        return FALSE;
    }
    if (NNSi_SndArcLoadBank(bankNo, NNS_SND_ARC_LOAD_BANK | NNS_SND_ARC_LOAD_WAVE,
                            NNSi_SndPlayerAllocHeap(playerNo, seqPlayer), FALSE, &bank) != NNS_SND_ARC_LOAD_SUCCESS) {
        NNSi_SndPlayerFreeSeqPlayer(seqPlayer);
        return FALSE;
    }
    NNSi_SndPlayerStartSeq(seqPlayer, (const u8 *) seqArc + seqArc->baseOffset, info->offset, bank);
    NNS_SndPlayerSetInitialVolume(handle, info->volume);
    NNS_SndPlayerSetChannelPriority(handle, info->channelPrio);
    NNS_SndPlayerSetSeqArcNo(handle, seqArcNo, index);
    return TRUE;
}
