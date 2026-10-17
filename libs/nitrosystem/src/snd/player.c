// NitroSystem sound: sequence players, grouped into players that limit how many play at once.

#include "nitro/snd.h"
#include "nnsys/snd/main.h"
#include "nnsys/snd/player.h"

#define VOLUME_MIN -0x8000
#define VOLUME_MAX 0x7fff
#define SILENT_VOLUME -723

static NNSSndPlayer sPlayer[NNS_SND_PLAYER_NUM];
static NNSSndSeqPlayer sSeqPlayer[NNS_SND_SEQ_PLAYER_NUM];
static NNSFndList sPrioList;    // Sequence players in use, by priority
static NNSFndList sFreeList;    // Sequence players not in use

static void InitPlayer(NNSSndSeqPlayer *seqPlayer);
static void InsertPlayerList(NNSSndPlayer *player, NNSSndSeqPlayer *seqPlayer);
static void InsertPrioList(NNSSndSeqPlayer *seqPlayer);
static void ForceStopSeq(NNSSndSeqPlayer *seqPlayer);
static NNSSndSeqPlayer *AllocSeqPlayer(int prio);
static void ShutdownPlayer(NNSSndSeqPlayer *seqPlayer);
static void PlayerHeapDisposeCallback(void *mem, u32 size, u32 data1, u32 data2);
static void SetPlayerPriority(NNSSndSeqPlayer *seqPlayer, int prio);

void NNS_SndPlayerSetPlayerVolume(int playerNo, int volume) {
    sPlayer[playerNo].volume = volume;
}

void NNS_SndPlayerSetPlayableSeqCount(int playerNo, int seqCount) {
    sPlayer[playerNo].playableSeqCount = (u16) seqCount;
}

void NNS_SndPlayerSetAllocatableChannel(int playerNo, u32 chBitFlag) {
    sPlayer[playerNo].allocChBitFlag = chBitFlag;
}

BOOL NNS_SndPlayerCreateHeap(int playerNo, NNSSndHeapHandle heap, u32 size) {
    NNSSndPlayerHeap *playerHeap;
    NNSSndHeapHandle handle;

    playerHeap = NNS_SndHeapAlloc(heap, sizeof(NNSSndPlayerHeap) + size, PlayerHeapDisposeCallback, 0, 0);
    if (playerHeap == NULL) {
        return FALSE;
    }
    playerHeap->player = NULL;
    playerHeap->playerNo = playerNo;
    playerHeap->handle = NULL;
    handle = NNS_SndHeapCreate(playerHeap + 1, size);
    if (handle == NULL) {
        return FALSE;
    }
    playerHeap->handle = handle;
    NNS_FndAppendListObject(&sPlayer[playerNo].heapList, playerHeap);
    return TRUE;
}

void NNS_SndPlayerStopSeqByPlayerNo(int playerNo, int fadeFrame) {
    int i;
    for (i = 0; i < NNS_SND_SEQ_PLAYER_NUM; i++) {
        NNSSndSeqPlayer *seqPlayer = &sSeqPlayer[i];
        if (seqPlayer->status != NNS_SND_SEQ_PLAYER_STATUS_FREE && seqPlayer->player == &sPlayer[playerNo]) {
            NNSi_SndPlayerStopSeq(seqPlayer, fadeFrame);
        }
    }
}

void NNS_SndPlayerStopSeqBySeqNo(int seqNo, int fadeFrame) {
    int i;
    for (i = 0; i < NNS_SND_SEQ_PLAYER_NUM; i++) {
        NNSSndSeqPlayer *seqPlayer = &sSeqPlayer[i];
        if (seqPlayer->status != NNS_SND_SEQ_PLAYER_STATUS_FREE && seqPlayer->seqType == NNS_SND_SEQ_TYPE_SEQ &&
            seqPlayer->seqNo == seqNo) {
            NNSi_SndPlayerStopSeq(seqPlayer, fadeFrame);
        }
    }
}

void NNS_SndPlayerStopSeqBySeqArcNo(int seqArcNo, int fadeFrame) {
    int i;
    for (i = 0; i < NNS_SND_SEQ_PLAYER_NUM; i++) {
        NNSSndSeqPlayer *seqPlayer = &sSeqPlayer[i];
        if (seqPlayer->status != NNS_SND_SEQ_PLAYER_STATUS_FREE && seqPlayer->seqType == NNS_SND_SEQ_TYPE_SEQARC &&
            seqPlayer->seqNo == seqArcNo) {
            NNSi_SndPlayerStopSeq(seqPlayer, fadeFrame);
        }
    }
}

void NNS_SndPlayerStopSeqBySeqArcIdx(int seqArcNo, int index, int fadeFrame) {
    int i;
    for (i = 0; i < NNS_SND_SEQ_PLAYER_NUM; i++) {
        NNSSndSeqPlayer *seqPlayer = &sSeqPlayer[i];
        if (seqPlayer->status != NNS_SND_SEQ_PLAYER_STATUS_FREE && seqPlayer->seqType == NNS_SND_SEQ_TYPE_SEQARC &&
            seqPlayer->seqNo == seqArcNo && seqPlayer->seqArcIndex == index) {
            NNSi_SndPlayerStopSeq(seqPlayer, fadeFrame);
        }
    }
}

void NNS_SndPlayerPauseByPlayerNo(int playerNo, BOOL flag) {
    NNSSndPlayer *player = &sPlayer[playerNo];
    NNSSndSeqPlayer *seqPlayer;
    NNSSndSeqPlayer *next;
    for (seqPlayer = NNS_FndGetNextListObject(&player->playList, NULL); seqPlayer != NULL; seqPlayer = next) {
        next = NNS_FndGetNextListObject(&player->playList, seqPlayer);
        NNSi_SndPlayerPause(seqPlayer, flag);
    }
}

void NNS_SndHandleInit(NNSSndHandle *handle) {
    handle->player = NULL;
}

void NNS_SndHandleReleaseSeq(NNSSndHandle *handle) {
    if (NNS_SndHandleIsValid(handle)) {
        handle->player->handle = NULL;
        handle->player = NULL;
    }
}

int NNS_SndPlayerCountPlayingSeqBySeqNo(int seqNo) {
    int count = 0;
    NNSSndSeqPlayer *seqPlayer;
    for (seqPlayer = NNS_FndGetNextListObject(&sPrioList, NULL); seqPlayer != NULL;
         seqPlayer = NNS_FndGetNextListObject(&sPrioList, seqPlayer)) {
        if (seqPlayer->seqType == NNS_SND_SEQ_TYPE_SEQ && seqPlayer->seqNo == seqNo) {
            count++;
        }
    }
    return count;
}

int NNS_SndPlayerCountPlayingSeqBySeqArcIdx(int seqArcNo, int index) {
    int count = 0;
    NNSSndSeqPlayer *seqPlayer;
    for (seqPlayer = NNS_FndGetNextListObject(&sPrioList, NULL); seqPlayer != NULL;
         seqPlayer = NNS_FndGetNextListObject(&sPrioList, seqPlayer)) {
        if (seqPlayer->seqType == NNS_SND_SEQ_TYPE_SEQARC && seqPlayer->seqNo == seqArcNo &&
            seqPlayer->seqArcIndex == index) {
            count++;
        }
    }
    return count;
}

void NNS_SndPlayerSetVolume(NNSSndHandle *handle, int volume) {
    if (NNS_SndHandleIsValid(handle)) {
        handle->player->extVolume = volume;
    }
}

void NNS_SndPlayerSetInitialVolume(NNSSndHandle *handle, int volume) {
    if (NNS_SndHandleIsValid(handle)) {
        handle->player->initVolume = volume;
    }
}

void NNS_SndPlayerMoveVolume(NNSSndHandle *handle, int targetVolume, int frames) {
    if (NNS_SndHandleIsValid(handle) && handle->player->status != NNS_SND_SEQ_PLAYER_STATUS_FADEOUT) {
        NNSi_SndFaderSet(&handle->player->fader, targetVolume << 8, frames);
    }
}

void NNS_SndPlayerSetChannelPriority(NNSSndHandle *handle, int prio) {
    if (NNS_SndHandleIsValid(handle)) {
        SND_SetPlayerChannelPriority(handle->player->playerNo, prio);
    }
}

void NNS_SndPlayerSetTrackMute(NNSSndHandle *handle, u32 trackBitMask, BOOL flag) {
    if (NNS_SndHandleIsValid(handle)) {
        SND_SetTrackMute(handle->player->playerNo, trackBitMask, flag);
    }
}

void NNS_SndPlayerSetTrackVolume(NNSSndHandle *handle, u32 trackBitMask, int volume) {
    if (NNS_SndHandleIsValid(handle)) {
        SND_SetTrackVolume(handle->player->playerNo, trackBitMask, SNDi_DecibelTable[volume]);
    }
}

void NNS_SndPlayerSetTrackPitch(NNSSndHandle *handle, u32 trackBitMask, int pitch) {
    if (NNS_SndHandleIsValid(handle)) {
        SND_SetTrackPitch(handle->player->playerNo, trackBitMask, pitch);
    }
}

void NNS_SndPlayerSetTrackPan(NNSSndHandle *handle, u32 trackBitMask, int pan) {
    if (NNS_SndHandleIsValid(handle)) {
        SND_SetTrackPan(handle->player->playerNo, trackBitMask, pan);
    }
}

void NNS_SndPlayerSetTempoRatio(NNSSndHandle *handle, int ratio) {
    if (NNS_SndHandleIsValid(handle)) {
        SND_SetPlayerTempoRatio(handle->player->playerNo, ratio);
    }
}

void NNS_SndPlayerSetSeqNo(NNSSndHandle *handle, int seqNo) {
    if (NNS_SndHandleIsValid(handle)) {
        handle->player->seqType = NNS_SND_SEQ_TYPE_SEQ;
        handle->player->seqNo = seqNo;
    }
}

void NNS_SndPlayerSetSeqArcNo(NNSSndHandle *handle, int seqArcNo, int index) {
    if (NNS_SndHandleIsValid(handle)) {
        handle->player->seqType = NNS_SND_SEQ_TYPE_SEQARC;
        handle->player->seqNo = seqArcNo;
        handle->player->seqArcIndex = index;
    }
}

BOOL NNS_SndPlayerWriteGlobalVariable(int varNo, s16 var) {
    SND_SetPlayerGlobalVariable(varNo, var);
    return TRUE;
}

void NNSi_SndPlayerInit(void) {
    int i;
    NNS_FndInitList(&sPrioList, 0x14);
    NNS_FndInitList(&sFreeList, 0x14);
    for (i = 0; i < NNS_SND_SEQ_PLAYER_NUM; i++) {
        NNSSndSeqPlayer *seqPlayer = &sSeqPlayer[i];
        seqPlayer->status = NNS_SND_SEQ_PLAYER_STATUS_FREE;
        seqPlayer->playerNo = i;
        NNS_FndAppendListObject(&sFreeList, seqPlayer);
    }
    for (i = 0; i < NNS_SND_PLAYER_NUM; i++) {
        NNSSndPlayer *player = &sPlayer[i];
        NNS_FndInitList(&player->playList, 0xc);
        NNS_FndInitList(&player->heapList, 0);
        player->volume = 127;
        player->playableSeqCount = 1;
        player->allocChBitFlag = 0;
    }
}

void NNSi_SndPlayerMain(void) {
    NNSSndSeqPlayer *seqPlayer;
    NNSSndSeqPlayer *next;
    u32 status = SND_GetPlayerStatus();
    for (seqPlayer = NNS_FndGetNextListObject(&sPrioList, NULL); seqPlayer != NULL; seqPlayer = next) {
        s32 volume;
        s32 extVolume;
        s32 initVolume;
        s32 playerVolume;
        s32 faderVolume;
        next = NNS_FndGetNextListObject(&sPrioList, seqPlayer);
        if (!seqPlayer->prepared && SND_IsFinishedCommandTag(seqPlayer->commandTag)) {
            seqPlayer->prepared = TRUE;
        }
        if (seqPlayer->prepared && !(status & (1 << seqPlayer->playerNo))) {
            ShutdownPlayer(seqPlayer);
            continue;
        }
        NNSi_SndFaderUpdate(&seqPlayer->fader);
        extVolume = SNDi_DecibelTable[seqPlayer->extVolume];
        initVolume = SNDi_DecibelTable[seqPlayer->initVolume];
        playerVolume = SNDi_DecibelTable[seqPlayer->player->volume];
        faderVolume = SNDi_DecibelTable[NNSi_SndFaderGet(&seqPlayer->fader) >> 8];
        volume = initVolume + extVolume + playerVolume + faderVolume;
        if (volume < VOLUME_MIN) {
            volume = VOLUME_MIN;
        } else if (volume > VOLUME_MAX) {
            volume = VOLUME_MAX;
        }
        if (volume != seqPlayer->volume) {
            SND_SetPlayerVolume(seqPlayer->playerNo, volume);
            seqPlayer->volume = volume;
        }
        if (seqPlayer->status == NNS_SND_SEQ_PLAYER_STATUS_FADEOUT && NNSi_SndFaderIsFinished(&seqPlayer->fader)) {
            ForceStopSeq(seqPlayer);
        }
        if (seqPlayer->startRequest) {
            SND_StartPreparedSeq(seqPlayer->playerNo);
            seqPlayer->startRequest = FALSE;
        }
    }
}

NNSSndSeqPlayer *NNSi_SndPlayerAllocSeqPlayer(NNSSndHandle *handle, int playerNo, int prio) {
    NNSSndPlayer *player = &sPlayer[playerNo];
    NNSSndSeqPlayer *seqPlayer;
    if (NNS_SndHandleIsValid(handle)) {
        NNS_SndHandleReleaseSeq(handle);
    }
    if (player->playList.numObjects >= player->playableSeqCount) {
        NNSSndSeqPlayer *lowest = NNS_FndGetNextListObject(&player->playList, NULL);
        if (lowest == NULL) {
            return NULL;
        }
        if (prio < lowest->prio) {
            return NULL;
        }
        ForceStopSeq(lowest);
    }
    seqPlayer = AllocSeqPlayer(prio);
    if (seqPlayer == NULL) {
        return NULL;
    }
    InsertPlayerList(player, seqPlayer);
    seqPlayer->handle = handle;
    handle->player = seqPlayer;
    return seqPlayer;
}

void NNSi_SndPlayerFreeSeqPlayer(NNSSndSeqPlayer *seqPlayer) {
    ShutdownPlayer(seqPlayer);
}

void NNSi_SndPlayerStartSeq(NNSSndSeqPlayer *seqPlayer, const void *seqBase, u32 seqOffset, const void *bank) {
    NNSSndPlayer *player = seqPlayer->player;
    SND_PrepareSeq(seqPlayer->playerNo, seqBase, seqOffset, bank);
    if (player->allocChBitFlag != 0) {
        SND_SetTrackAllocatableChannel(seqPlayer->playerNo, 0xffff, player->allocChBitFlag);
    }
    InitPlayer(seqPlayer);
    seqPlayer->commandTag = SND_GetCurrentCommandTag();
    seqPlayer->startRequest = TRUE;
    seqPlayer->status = NNS_SND_SEQ_PLAYER_STATUS_PLAY;
}

void NNSi_SndPlayerStopSeq(NNSSndSeqPlayer *seqPlayer, int fadeFrame) {
    if (seqPlayer == NULL) {
        return;
    }
    if (seqPlayer->status == NNS_SND_SEQ_PLAYER_STATUS_FREE) {
        return;
    }
    if (fadeFrame == 0) {
        ForceStopSeq(seqPlayer);
        return;
    }
    NNSi_SndFaderSet(&seqPlayer->fader, 0, fadeFrame);
    SetPlayerPriority(seqPlayer, 0);
    seqPlayer->status = NNS_SND_SEQ_PLAYER_STATUS_FADEOUT;
}

void NNSi_SndPlayerPause(NNSSndSeqPlayer *seqPlayer, BOOL flag) {
    if (seqPlayer == NULL) {
        return;
    }
    if (flag != seqPlayer->pause) {
        SND_PauseSeq(seqPlayer->playerNo, flag);
        seqPlayer->pause = flag;
    }
}

NNSSndHeapHandle NNSi_SndPlayerAllocHeap(int playerNo, NNSSndSeqPlayer *seqPlayer) {
    NNSSndPlayer *player = &sPlayer[playerNo];
    NNSSndPlayerHeap *heap = NNS_FndGetNextListObject(&player->heapList, NULL);
    if (heap == NULL) {
        return NULL;
    }
    NNS_FndRemoveListObject(&player->heapList, heap);
    heap->player = seqPlayer;
    seqPlayer->heap = heap;
    NNS_SndHeapClear(heap->handle);
    return heap->handle;
}

static void InitPlayer(NNSSndSeqPlayer *seqPlayer) {
    seqPlayer->pause = FALSE;
    seqPlayer->prepared = FALSE;
    seqPlayer->startRequest = FALSE;
    seqPlayer->seqType = NNS_SND_SEQ_TYPE_NONE;
    seqPlayer->volume = 0;
    seqPlayer->initVolume = 127;
    seqPlayer->extVolume = 127;
    NNSi_SndFaderInit(&seqPlayer->fader);
    NNSi_SndFaderSet(&seqPlayer->fader, 127 << 8, 1);
}

static void InsertPlayerList(NNSSndPlayer *player, NNSSndSeqPlayer *seqPlayer) {
    NNSSndSeqPlayer *next;
    for (next = NNS_FndGetNextListObject(&player->playList, NULL); next != NULL;
         next = NNS_FndGetNextListObject(&player->playList, next)) {
        if (seqPlayer->prio < next->prio) {
            break;
        }
    }
    NNS_FndInsertListObject(&player->playList, next, seqPlayer);
    seqPlayer->player = player;
}

static void InsertPrioList(NNSSndSeqPlayer *seqPlayer) {
    NNSSndSeqPlayer *next;
    for (next = NNS_FndGetNextListObject(&sPrioList, NULL); next != NULL;
         next = NNS_FndGetNextListObject(&sPrioList, next)) {
        if (seqPlayer->prio < next->prio) {
            break;
        }
    }
    NNS_FndInsertListObject(&sPrioList, next, seqPlayer);
}

static void ForceStopSeq(NNSSndSeqPlayer *seqPlayer) {
    if (seqPlayer->status == NNS_SND_SEQ_PLAYER_STATUS_FADEOUT) {
        SND_SetPlayerVolume(seqPlayer->playerNo, SILENT_VOLUME);
    }
    SND_StopSeq(seqPlayer->playerNo);
    ShutdownPlayer(seqPlayer);
}

static NNSSndSeqPlayer *AllocSeqPlayer(int prio) {
    NNSSndSeqPlayer *seqPlayer = NNS_FndGetNextListObject(&sFreeList, NULL);
    if (seqPlayer == NULL) {
        seqPlayer = NNS_FndGetNextListObject(&sPrioList, NULL);
        if (prio < seqPlayer->prio) {
            return NULL;
        }
        ForceStopSeq(seqPlayer);
    }
    NNS_FndRemoveListObject(&sFreeList, seqPlayer);
    seqPlayer->prio = prio;
    InsertPrioList(seqPlayer);
    return seqPlayer;
}

static void ShutdownPlayer(NNSSndSeqPlayer *seqPlayer) {
    NNSSndPlayer *player;
    if (seqPlayer->handle != NULL) {
        seqPlayer->handle->player = NULL;
        seqPlayer->handle = NULL;
    }
    player = seqPlayer->player;
    NNS_FndRemoveListObject(&player->playList, seqPlayer);
    seqPlayer->player = NULL;
    if (seqPlayer->heap != NULL) {
        NNS_FndAppendListObject(&player->heapList, seqPlayer->heap);
        seqPlayer->heap->player = NULL;
        seqPlayer->heap = NULL;
    }
    NNS_FndRemoveListObject(&sPrioList, seqPlayer);
    NNS_FndAppendListObject(&sFreeList, seqPlayer);
    seqPlayer->status = NNS_SND_SEQ_PLAYER_STATUS_FREE;
}

static void PlayerHeapDisposeCallback(void *mem, u32 size, u32 data1, u32 data2) {
    NNSSndPlayerHeap *heap = mem;
    if (heap->handle == NULL) {
        return;
    }
    NNS_SndHeapDestroy(heap->handle);
    if (heap->player != NULL) {
        heap->player->heap = NULL;
    } else {
        NNS_FndRemoveListObject(&sPlayer[heap->playerNo].heapList, heap);
    }
}

static void SetPlayerPriority(NNSSndSeqPlayer *seqPlayer, int prio) {
    NNSSndPlayer *player = seqPlayer->player;
    if (player != NULL) {
        NNS_FndRemoveListObject(&player->playList, seqPlayer);
        seqPlayer->player = NULL;
    }
    NNS_FndRemoveListObject(&sPrioList, seqPlayer);
    seqPlayer->prio = prio;
    if (player != NULL) {
        InsertPlayerList(player, seqPlayer);
    }
    InsertPrioList(seqPlayer);
}
