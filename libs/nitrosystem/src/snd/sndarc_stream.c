// NitroSystem sound: sound archive streams. Each player owns a stream and a buffer of BLOCK_NUM blocks per channel;
// when the stream asks for a block, a command goes to the stream thread (or, while preparing, the prepare thread),
// which reads the STRM data from the ROM file or from memory, decoding ADPCM, and hands it over.

#include "nitro/fs.h"
#include "nitro/mi.h"
#include "nitro/snd.h"
#include "nnsys/snd/fader.h"
#include "nnsys/snd/sndarc.h"
#include "nnsys/snd/sndarc_stream.h"

#define BLOCK_SIZE 512
#define BLOCK_NUM 4
#define COMMAND_BUFFER_NUM (NNS_SND_STRM_PLAYER_NUM * (BLOCK_NUM - 2))
#define ADPCM_INDEX_NUM 89
#define STRM_CHANNEL_MAX 6

#define NNS_SND_ARC_STRM_FORCE_STEREO (1 << 0)

typedef struct {
    u32 fileId;
    u8 volume;
    u8 playerPrio;
    u8 playerNo;
    u8 flags;
} NNSSndArcStrmInfo;

typedef struct {
    u8 numChannels;
    u8 chNoList[2];
} NNSSndArcStrmPlayerInfo;

struct NNSSndStrmPlayer;
typedef BOOL (*OpenStreamFunc)(struct NNSSndStrmPlayer *player, u32 fileId);
typedef void (*CloseStreamFunc)(struct NNSSndStrmPlayer *player);
typedef s32 (*ReadStreamFunc)(struct NNSSndStrmPlayer *player, void *dest, u32 size, u32 offset);
typedef void (*CancelStreamFunc)(struct NNSSndStrmPlayer *player);

// The STRM file's header and HEAD block.
typedef struct {
    SNDBinaryFileHeader fileHeader;
    SNDBinaryBlockHeader blockHeader;
    u8 format;
    u8 loopFlag;
    u8 numChannels;
    u8 pad;
    u16 sampleRate;
    u16 timer;
    u32 loopStart;
    u32 loopEnd;
    u32 dataOffset;
    u32 numBlocks;
    u32 blockSize;
    u32 blockSamples;
    u32 lastBlockSize;
    u32 lastBlockSamples;
} NNSSndStrmData;

enum {
    STRM_FORMAT_PCM8,
    STRM_FORMAT_PCM16,
    STRM_FORMAT_ADPCM
};

typedef struct {
    s16 prevSample;
    u8 prevIndex;
    u8 padding;
} AdpcmState;

typedef struct NNSSndStrmPlayer {
    NNSSndStrm stream;
    FSFile file;
    u32 fileOffset;             // where the file starts in the archive (or in memory)
    NNSSndStrmData info;
    NNSSndFader fader;
    AdpcmState adpcmState[STRM_CHANNEL_MAX];
    BOOL activeFlag : 1;
    BOOL playFlag : 1;
    BOOL startFlag : 1;
    BOOL fadeOutFlag : 1;
    BOOL dirtyFlag : 1;         // starting mid-block in ADPCM: the block's start has to be decoded first
    BOOL finishFlag : 1;
    BOOL monoFlag : 1;
    volatile int finishCounter; // blocks left to play after the data ended
    volatile BOOL prepareFlag;
    volatile int commandCount;
    int allocChannelCount;
    u8 numChannels;
    u8 padding;
    u8 chNoList[STRM_CHANNEL_MAX];
    void *buffer;
    u32 bufSize;
    NNSSndStrmCallback strmCallback;
    void *strmCallbackArg;
    NNSSndArcStrmCallback sndArcStrmCallback;
    void *sndArcStrmCallbackArg;
    int strmNo;
    int playerNo;
    NNSSndStrmHandle *handle;
    int prio;
    int initVolume;
    int extVolume;
    int volume;
    u32 curSample;
    OpenStreamFunc openStreamFunc;
    CloseStreamFunc closeStreamFunc;
    ReadStreamFunc readStreamFunc;
    CancelStreamFunc cancelStreamFunc;
} NNSSndStrmPlayer;

// A block for the stream thread to fill.
typedef struct {
    NNSFndLink link;
    NNSSndStrmPlayer *player;
    NNSSndStrmCallbackStatus status;
    int numChannels;
    void *buffer[STRM_CHANNEL_MAX];
    u32 bufLen;
} LoadCommand;

static const s8 cAdpcmIndexTable[16] = {-1, -1, -1, -1, 2, 4, 6, 8, -1, -1, -1, -1, 2, 4, 6, 8};

static const s16 cAdpcmStepSizeTable[ADPCM_INDEX_NUM] = {
    7,     8,     9,     10,    11,    12,    13,    14,    16,    17,    19,    21,    23,    25,    28,
    31,    34,    37,    41,    45,    50,    55,    60,    66,    73,    80,    88,    97,    107,   118,
    130,   143,   157,   173,   190,   209,   230,   253,   279,   307,   337,   371,   408,   449,   494,
    544,   598,   658,   724,   796,   876,   963,   1060,  1166,  1282,  1411,  1552,  1707,  1878,  2066,
    2272,  2499,  2749,  3024,  3327,  3660,  4026,  4428,  4871,  5358,  5894,  6484,  7132,  7845,  8630,
    9493,  10442, 11487, 12635, 13899, 15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767,
};

static NNSSndStrmThread sStrmThread;
static NNSSndStrmThread *sPrepareThread = NULL;
static NNSFndList sFreeCommandList;
static LoadCommand sLoadCommandBuffer[COMMAND_BUFFER_NUM];
static NNSSndStrmPlayer sStrmPlayer[NNS_SND_STRM_PLAYER_NUM];
static u8 *sDecodeBuffer;
static u8 sDecodeBufferArea[BLOCK_SIZE] __attribute__((aligned(32)));
static OSMutex sDecodeBufferMutex;

static void StrmThread(void *arg);
static NNSSndStrmPlayer *AllocPlayer(NNSSndStrmHandle *handle, int playerNo, int prio);
static void FreePlayer(NNSSndStrmPlayer *player);
static BOOL PrepareStrm(NNSSndStrmHandle *handle, const NNSSndArcStrmInfo *strmInfo, int playerNo, int playerPrio,
                        int strmNo, u32 offset, NNSSndStrmCallback strmCallback, void *strmCallbackArg,
                        NNSSndArcStrmCallback sndArcStrmCallback, void *sndArcStrmCallbackArg);
static void StopStrm(NNSSndStrmPlayer *player, int fadeFrame);
static void ForceStopStrm(NNSSndStrmPlayer *player);
static void ShutdownPlayer(NNSSndStrmPlayer *player);
static BOOL AllocChannel(NNSSndStrmPlayer *player, int numChannels, const u8 chNoList[]);
static void FreeChannel(NNSSndStrmPlayer *player);
static LoadCommand *ReadCommandBuffer(NNSFndList *commandList);
static LoadCommand *AllocCommandBuffer(void);
static void FreeCommandBuffer(LoadCommand *command);
static void StrmCallback(NNSSndStrmCallbackStatus status, int numChannels, void *buffer[], u32 len,
                         NNSSndStrmFormat format, void *arg);
static void DisposeCallback(void *mem, u32 size, u32 data1, u32 data2);
static void CreateThread(NNSSndStrmThread *thread, u32 threadPrio);
static void RemoveCommandByPlayer(NNSFndList *commandList, const NNSSndStrmPlayer *player);
static void MakeWaveData(LoadCommand *command);
static void OnDataEnd(NNSSndStrmPlayer *player);
static void SetupStreamFunction(NNSSndStrmPlayer *player, u32 fileId);
static BOOL OpenFileStream(NNSSndStrmPlayer *player, u32 fileId);
static void CloseFileStream(NNSSndStrmPlayer *player);
static s32 ReadFileStream(NNSSndStrmPlayer *player, void *dest, u32 size, u32 offset);
static void CancelFileStream(NNSSndStrmPlayer *player);
static BOOL OpenMemoryStream(NNSSndStrmPlayer *player, u32 fileId);
static void CloseMemoryStream(NNSSndStrmPlayer *player);
static s32 ReadMemoryStream(NNSSndStrmPlayer *player, void *dest, u32 size, u32 offset);
static void CancelMemoryStream(NNSSndStrmPlayer *player);

static inline s16 DecodeAdpcm(int code, AdpcmState *state) {
    int step;
    int sample;
    int index;
    int d;

    sample = state->prevSample;
    index = state->prevIndex;
    step = cAdpcmStepSizeTable[index];
    d = step >> 3;
    if (code & 4) {
        d += step;
    }
    if (code & 2) {
        d += step >> 1;
    }
    if (code & 1) {
        d += step >> 2;
    }
    if (code & 8) {
        sample -= d;
        if (sample < -32768) {
            sample = -32768;
        }
    } else {
        sample += d;
        if (sample > 32767) {
            sample = 32767;
        }
    }
    index += cAdpcmIndexTable[code];
    if (index < 0) {
        index = 0;
    } else if (index > ADPCM_INDEX_NUM - 1) {
        index = ADPCM_INDEX_NUM - 1;
    }
    state->prevSample = (s16) sample;
    state->prevIndex = (u8) index;
    return (s16) sample;
}

void NNS_SndArcStrmInit(u32 threadPrio, NNSSndHeapHandle heap) {
    int playerNo;
    NNSSndStrmPlayer *player;
    int i;
    BOOL result;

    {
        static BOOL initialized = FALSE;
        if (initialized) {
            result = NNS_SndArcStrmSetupPlayer(heap);
            return;
        }
        initialized = TRUE;
    }
    NNS_FndInitList(&sFreeCommandList, 0);
    for (i = 0; i < COMMAND_BUFFER_NUM; i++) {
        NNS_FndAppendListObject(&sFreeCommandList, &sLoadCommandBuffer[i]);
    }
    OS_InitMutex(&sDecodeBufferMutex);
    sDecodeBuffer = sDecodeBufferArea;
    for (playerNo = 0; playerNo < NNS_SND_STRM_PLAYER_NUM; ++playerNo) {
        player = &sStrmPlayer[playerNo];
        player->activeFlag = FALSE;
        FS_InitFile(&player->file);
        NNS_SndStrmInit(&player->stream);
        player->playerNo = playerNo;
        player->numChannels = 0;
        player->buffer = NULL;
        player->bufSize = 0;
        player->allocChannelCount = 0;
    }
    result = NNS_SndArcStrmSetupPlayer(heap);
    CreateThread(&sStrmThread, threadPrio);
}

BOOL NNS_SndArcStrmSetupPlayer(NNSSndHeapHandle heap) {
    int playerNo;
    const NNSSndArcStrmPlayerInfo *playerInfo;
    NNSSndStrmPlayer *player;
    void *buffer;
    u32 bufSize;
    int i;

    for (playerNo = 0; playerNo < NNS_SND_STRM_PLAYER_NUM; ++playerNo) {
        player = &sStrmPlayer[playerNo];
        playerInfo = (const NNSSndArcStrmPlayerInfo *) NNS_SndArcGetStrmPlayerInfo(playerNo);
        if (playerInfo == NULL) {
            continue;
        }
        player->numChannels = playerInfo->numChannels;
        for (i = 0; i < playerInfo->numChannels; i++) {
            player->chNoList[i] = playerInfo->chNoList[i];
        }
        if (heap != NULL) {
            bufSize = (u32) (BLOCK_SIZE * BLOCK_NUM * player->numChannels);
            buffer = NNS_SndHeapAlloc(heap, bufSize, DisposeCallback, (u32) player, 0);
            if (buffer == NULL) {
                return FALSE;
            }
            ForceStopStrm(player);
            player->buffer = buffer;
            player->bufSize = bufSize;
        }
    }
    return TRUE;
}

void NNS_SndArcStrmCreatePrepareThread(NNSSndStrmThread *thread, u32 threadPrio) {
    if (sPrepareThread != NULL) {
        return;
    }
    CreateThread(thread, threadPrio);
    sPrepareThread = thread;
}

BOOL NNS_SndArcStrmPrepare(NNSSndStrmHandle *handle, int strmNo, u32 offset) {
    const NNSSndArcStrmInfo *strmInfo;

    strmInfo = (const NNSSndArcStrmInfo *) NNS_SndArcGetStrmInfo(strmNo);
    if (strmInfo == NULL) {
        return FALSE;
    }
    return PrepareStrm(handle, strmInfo, strmInfo->playerNo, strmInfo->playerPrio, strmNo, offset, NULL, NULL,
                       NULL, NULL);
}

void NNS_SndArcStrmStartPrepared(NNSSndStrmHandle *handle) {
    if (!NNS_SndStrmHandleIsValid(handle)) {
        return;
    }
    handle->player->startFlag = TRUE;
}

BOOL NNS_SndArcStrmStart(NNSSndStrmHandle *handle, int strmNo, u32 offset) {
    BOOL result;

    result = NNS_SndArcStrmPrepare(handle, strmNo, offset);
    if (!result) {
        return FALSE;
    }
    NNS_SndArcStrmStartPrepared(handle);
    return TRUE;
}

void NNS_SndArcStrmStop(NNSSndStrmHandle *handle, int fadeFrame) {
    if (!NNS_SndStrmHandleIsValid(handle)) {
        return;
    }
    StopStrm(handle->player, fadeFrame);
}

void NNS_SndArcStrmMoveVolume(NNSSndStrmHandle *handle, int volume, int frames) {
    if (!NNS_SndStrmHandleIsValid(handle)) {
        return;
    }
    if (handle->player->fadeOutFlag) {
        return;
    }
    NNSi_SndFaderSet(&handle->player->fader, volume << 8, frames);
}

void NNS_SndStrmHandleInit(NNSSndStrmHandle *handle) {
    handle->player = NULL;
}

void NNS_SndStrmHandleRelease(NNSSndStrmHandle *handle) {
    if (handle->player == NULL) {
        return;
    }
    handle->player->handle = NULL;
    handle->player = NULL;
}

// In milliseconds.
u32 NNS_SndArcStrmGetCurrentPlayingPos(NNSSndStrmHandle *handle) {
    NNSSndStrmPlayer *player;
    u64 pos;

    if (!NNS_SndStrmHandleIsValid(handle)) {
        return 0;
    }
    player = handle->player;
    pos = player->curSample;
    pos *= 1000;
    pos /= player->info.sampleRate;
    return (u32) pos;
}

// Called every frame: starts prepared streams, applies the volume and stops finished or faded-out streams.
void NNSi_SndArcStrmMain(void) {
    NNSSndStrmPlayer *player;
    int playerNo;
    int volume;

    for (playerNo = 0; playerNo < NNS_SND_STRM_PLAYER_NUM; ++playerNo) {
        player = &sStrmPlayer[playerNo];
        if (!player->activeFlag) {
            continue;
        }
        if (player->finishCounter == 0) {
            ForceStopStrm(player);
            continue;
        }
        if (player->startFlag) {
            if (player->prepareFlag) {
                NNS_SndStrmStart(&player->stream);
                player->playFlag = TRUE;
                player->startFlag = FALSE;
            }
        }
        if (!player->playFlag) {
            continue;
        }
        NNSi_SndFaderUpdate(&player->fader);
        volume = SND_CalcDecibel(NNSi_SndFaderGet(&player->fader) >> 8) + SND_CalcDecibel(player->initVolume) +
                 SND_CalcDecibel(player->extVolume);
        if (volume != player->volume) {
            NNS_SndStrmSetVolume(&player->stream, volume);
            player->volume = volume;
        }
        if (player->fadeOutFlag) {
            if (NNSi_SndFaderIsFinished(&player->fader)) {
                ForceStopStrm(player);
            }
        }
    }
}

static NNSSndStrmPlayer *AllocPlayer(NNSSndStrmHandle *handle, int playerNo, int prio) {
    NNSSndStrmPlayer *player;

    if (NNS_SndStrmHandleIsValid(handle)) {
        NNS_SndStrmHandleRelease(handle);
    }
    player = &sStrmPlayer[playerNo];
    if (player->buffer == NULL) {
        return NULL;
    }
    if (player->activeFlag) {
        if (prio < player->prio) {
            return NULL;
        }
        ForceStopStrm(player);
    }
    player->prio = prio;
    player->activeFlag = TRUE;
    player->handle = handle;
    handle->player = player;
    return player;
}

static void FreePlayer(NNSSndStrmPlayer *player) {
    if (player->handle != NULL) {
        player->handle->player = NULL;
        player->handle = NULL;
    }
    player->activeFlag = FALSE;
    player->startFlag = FALSE;
    player->playFlag = FALSE;
}

static BOOL PrepareStrm(NNSSndStrmHandle *handle, const NNSSndArcStrmInfo *strmInfo, int playerNo, int playerPrio,
                        int strmNo, u32 offset, NNSSndStrmCallback strmCallback, void *strmCallbackArg,
                        NNSSndArcStrmCallback sndArcStrmCallback, void *sndArcStrmCallbackArg) {
    NNSSndStrmPlayer *player;
    NNSSndStrmFormat format;
    int numChannels;
    u64 tmp;
    BOOL ret;

    player = AllocPlayer(handle, playerNo, playerPrio);
    if (player == NULL) {
        return FALSE;
    }
    SetupStreamFunction(player, strmInfo->fileId);
    if (!player->openStreamFunc(player, strmInfo->fileId)) {
        FreePlayer(player);
        return FALSE;
    }
    tmp = player->info.sampleRate;
    tmp *= offset;
    tmp /= 1000;
    player->curSample = (u32) tmp;
    if (player->curSample != 0 && player->info.format == STRM_FORMAT_ADPCM) {
        player->dirtyFlag = TRUE;
    } else {
        player->dirtyFlag = FALSE;
    }
    player->finishCounter = BLOCK_NUM;
    player->finishFlag = FALSE;
    player->playFlag = FALSE;
    player->prepareFlag = FALSE;
    player->startFlag = FALSE;
    player->fadeOutFlag = FALSE;
    player->commandCount = 0;
    player->strmCallback = strmCallback;
    player->strmCallbackArg = strmCallbackArg;
    player->sndArcStrmCallback = sndArcStrmCallback;
    player->sndArcStrmCallbackArg = sndArcStrmCallbackArg;
    player->strmNo = strmNo;
    player->volume = 0;
    player->initVolume = strmInfo->volume;
    player->extVolume = 127;
    NNSi_SndFaderInit(&player->fader);
    NNSi_SndFaderSet(&player->fader, 127 << 8, 1);
    switch (player->info.format) {
    case STRM_FORMAT_PCM8:
        format = NNS_SND_STRM_FORMAT_PCM8;
        break;
    case STRM_FORMAT_PCM16:
    case STRM_FORMAT_ADPCM:
        format = NNS_SND_STRM_FORMAT_PCM16;
        break;
    }
    numChannels = player->info.numChannels;
    if (strmInfo->flags & NNS_SND_ARC_STRM_FORCE_STEREO) {
        numChannels = 2;
    }
    if (numChannels > player->numChannels) {
        numChannels = player->numChannels;
    }
    player->monoFlag = numChannels == 1 ? TRUE : FALSE;
    ret = AllocChannel(player, numChannels, player->chNoList);
    if (!ret) {
        player->closeStreamFunc(player);
        FreePlayer(player);
        return FALSE;
    }
    ret = NNS_SndStrmSetup(&player->stream, format, player->buffer, player->bufSize * numChannels / player->numChannels,
                           player->info.timer, BLOCK_NUM, StrmCallback, player);
    if (!ret) {
        FreeChannel(player);
        player->closeStreamFunc(player);
        FreePlayer(player);
        return FALSE;
    }
    if (numChannels == 2) {
        NNS_SndStrmSetChannelPan(&player->stream, 0, 0);
        NNS_SndStrmSetChannelPan(&player->stream, 1, 127);
    }
    return TRUE;
}

static void StopStrm(NNSSndStrmPlayer *player, int fadeFrame) {
    if (!player->playFlag) {
        ForceStopStrm(player);
        return;
    }
    if (fadeFrame == 0) {
        ForceStopStrm(player);
        return;
    }
    NNSi_SndFaderSet(&player->fader, 0, fadeFrame);
    player->fadeOutFlag = TRUE;
    player->prio = 0;
}

static void ForceStopStrm(NNSSndStrmPlayer *player) {
    OS_LockMutex(&sStrmThread.mutex);
    if (sPrepareThread) {
        OS_LockMutex(&sPrepareThread->mutex);
    }
    if (player->playFlag) {
        NNS_SndStrmStop(&player->stream);
    }
    if (player->activeFlag) {
        player->cancelStreamFunc(player);
    }
    ShutdownPlayer(player);
    OS_UnlockMutex(&sStrmThread.mutex);
    if (sPrepareThread) {
        OS_UnlockMutex(&sPrepareThread->mutex);
    }
}

static void ShutdownPlayer(NNSSndStrmPlayer *player) {
    if (!player->activeFlag) {
        return;
    }
    FreeChannel(player);
    player->closeStreamFunc(player);
    RemoveCommandByPlayer(&sStrmThread.commandList, player);
    if (sPrepareThread) {
        RemoveCommandByPlayer(&sPrepareThread->commandList, player);
    }
    FreePlayer(player);
}

// Channels are counted: NNS_SndArcStrmAllocChannel can hold them between streams.
static BOOL AllocChannel(NNSSndStrmPlayer *player, int numChannels, const u8 chNoList[]) {
    if (player->allocChannelCount == 0) {
        if (!NNS_SndStrmAllocChannel(&player->stream, numChannels, chNoList)) {
            return FALSE;
        }
    }
    player->allocChannelCount++;
    return TRUE;
}

static void FreeChannel(NNSSndStrmPlayer *player) {
    if (player->allocChannelCount == 0) {
        return;
    }
    player->allocChannelCount--;
    if (player->allocChannelCount == 0) {
        NNS_SndStrmFreeChannel(&player->stream);
    }
}

static void CreateThread(NNSSndStrmThread *thread, u32 threadPrio) {
    OS_CreateThread(&thread->thread, StrmThread, thread, thread->stack + NNS_SND_STRM_THREAD_STACK_SIZE / sizeof(u64),
                    NNS_SND_STRM_THREAD_STACK_SIZE, threadPrio);
    NNS_FndInitList(&thread->commandList, 0);
    OS_InitMutex(&thread->mutex);
    OS_InitThreadQueue(&thread->threadQ);
    OS_WakeupThreadDirect(&thread->thread);
}

static void RemoveCommandByPlayer(NNSFndList *commandList, const NNSSndStrmPlayer *player) {
    OSIntrMode old;
    LoadCommand *command;
    LoadCommand *next;

    old = OS_DisableInterrupts();
    for (command = (LoadCommand *) NNS_FndGetNextListObject(commandList, NULL); command != NULL; command = next) {
        next = (LoadCommand *) NNS_FndGetNextListObject(commandList, command);
        if (command->player == player) {
            NNS_FndRemoveListObject(commandList, command);
            FreeCommandBuffer(command);
        }
    }
    OS_RestoreInterrupts(old);
}

static LoadCommand *ReadCommandBuffer(NNSFndList *commandList) {
    OSIntrMode old;
    LoadCommand *command;

    old = OS_DisableInterrupts();
    command = (LoadCommand *) NNS_FndGetNextListObject(commandList, NULL);
    if (command != NULL) {
        NNS_FndRemoveListObject(commandList, command);
        command->player->commandCount--;
    }
    OS_RestoreInterrupts(old);
    return command;
}

static LoadCommand *AllocCommandBuffer(void) {
    OSIntrMode old;
    LoadCommand *command;

    old = OS_DisableInterrupts();
    command = (LoadCommand *) NNS_FndGetNextListObject(&sFreeCommandList, NULL);
    if (command != NULL) {
        NNS_FndRemoveListObject(&sFreeCommandList, command);
    }
    OS_RestoreInterrupts(old);
    return command;
}

static void FreeCommandBuffer(LoadCommand *command) {
    OSIntrMode old;

    old = OS_DisableInterrupts();
    NNS_FndAppendListObject(&sFreeCommandList, command);
    OS_RestoreInterrupts(old);
}

// When the sound heap frees a player's buffer.
static void DisposeCallback(void *mem, u32 size, u32 data1, u32 data2) {
    NNSSndStrmPlayer *player = (NNSSndStrmPlayer *) data1;

    if (mem == player->buffer) {
        OS_LockMutex(&sStrmThread.mutex);
        if (sPrepareThread) {
            OS_LockMutex(&sPrepareThread->mutex);
        }
        ForceStopStrm(player);
        player->buffer = NULL;
        player->bufSize = 0;
        player->numChannels = 0;
        if (player->allocChannelCount > 0) {
            NNS_SndStrmFreeChannel(&player->stream);
            player->allocChannelCount = 0;
        }
        OS_UnlockMutex(&sStrmThread.mutex);
        if (sPrepareThread) {
            OS_UnlockMutex(&sPrepareThread->mutex);
        }
    }
}

// The stream wants a block: queue a command for it (if the thread is behind, the oldest waiting block is cleared
// and dropped instead).
static void StrmCallback(NNSSndStrmCallbackStatus status, int numChannels, void *buffer[], u32 len,
                         NNSSndStrmFormat format, void *arg) {
    NNSSndStrmPlayer *player = (NNSSndStrmPlayer *) arg;
    LoadCommand *command;
    NNSSndStrmThread *thread;
    int ch;

    if (player->commandCount >= BLOCK_NUM - 2) {
        command = NULL;
        while ((command = (LoadCommand *) NNS_FndGetNextListObject(&sStrmThread.commandList, command)) != NULL) {
            if (command->player == player) {
                break;
            }
        }
        for (ch = 0; ch < command->numChannels; ch++) {
            MI_CpuFill8(command->buffer[ch], 0, command->bufLen);
        }
        NNS_FndRemoveListObject(&sStrmThread.commandList, command);
        player->commandCount--;
        FreeCommandBuffer(command);
    }
    command = AllocCommandBuffer();
    command->player = player;
    command->status = status;
    command->numChannels = numChannels;
    for (ch = 0; ch < numChannels; ch++) {
        command->buffer[ch] = buffer[ch];
    }
    command->bufLen = len;
    thread = &sStrmThread;
    if (status == NNS_SND_STRM_CALLBACK_SETUP && sPrepareThread) {
        thread = sPrepareThread;
    }
    player->commandCount++;
    NNS_FndAppendListObject(&thread->commandList, command);
    OS_WakeupThread(&thread->threadQ);
}

// The data ran out: the callback can name a stream to carry on with (it has to have the same rate and kind).
static void OnDataEnd(NNSSndStrmPlayer *player) {
    NNSSndArcStrmCallbackInfo info;
    NNSSndArcStrmCallbackParam param;
    const NNSSndArcStrmInfo *strmInfo;
    u8 oldFormat;
    u16 oldSampleRate;
    u64 tmp;
    BOOL result;

    info.playerNo = player->playerNo;
    info.strmNo = player->strmNo;
    param.strmNo = player->strmNo;
    param.offset = 0;
    result = player->sndArcStrmCallback(NNS_SND_ARC_STRM_CALLBACK_DATA_END, &info, &param,
                                        player->sndArcStrmCallbackArg);
    if (!result) {
        return;
    }
    strmInfo = (const NNSSndArcStrmInfo *) NNS_SndArcGetStrmInfo(param.strmNo);
    if (strmInfo == NULL) {
        return;
    }
    oldFormat = player->info.format;
    oldSampleRate = player->info.sampleRate;
    player->closeStreamFunc(player);
    SetupStreamFunction(player, strmInfo->fileId);
    if (!player->openStreamFunc(player, strmInfo->fileId)) {
        return;
    }
    if (oldSampleRate != player->info.sampleRate) {
        return;
    }
    if (oldFormat == STRM_FORMAT_PCM8 && player->info.format != STRM_FORMAT_PCM8 ||
        oldFormat != STRM_FORMAT_PCM8 && player->info.format == STRM_FORMAT_PCM8) {
        return;
    }
    player->strmNo = param.strmNo;
    tmp = player->info.sampleRate;
    tmp *= param.offset;
    tmp /= 1000;
    player->curSample = (u32) tmp;
    if (player->curSample != 0 && player->info.format == STRM_FORMAT_ADPCM) {
        player->dirtyFlag = TRUE;
    } else {
        player->dirtyFlag = FALSE;
    }
    player->finishFlag = FALSE;
}

// Fills a command's buffers from the stream data, block by block.
static void MakeWaveData(LoadCommand *command) {
    NNSSndStrmPlayer *player = command->player;
    BOOL loopFlag;
    u32 destOffset;
    u32 restSize;
    u32 blockNo;
    u32 blockSize;
    u32 blockSamples;
    u32 blockOffsetSample;
    u32 blockOffset;
    u32 offset;
    u32 samples;
    u32 size;
    u32 readSize;
    int ch;

    if (player->finishFlag && player->finishCounter > 0) {
        player->finishCounter--;
    }
    destOffset = 0;
    restSize = command->bufLen;
    while (restSize > 0) {
        if (player->finishFlag) {
            for (ch = 0; ch < command->numChannels; ch++) {
                MI_CpuFill8((u8 *) command->buffer[ch] + destOffset, 0, restSize);
            }
            break;
        }
        blockNo = player->curSample / player->info.blockSamples;
        if (blockNo < player->info.numBlocks - 1) {
            blockSize = player->info.blockSize;
            blockSamples = player->info.blockSamples;
        } else {
            blockSize = player->info.lastBlockSize;
            blockSamples = player->info.lastBlockSamples;
        }
        blockOffsetSample = player->curSample;
        blockOffsetSample -= blockNo * player->info.blockSamples;
        samples = restSize;
        if (player->info.format != STRM_FORMAT_PCM8) {
            samples >>= 1;
        }
        if (player->dirtyFlag) {
            if (blockOffsetSample == 0) {
                player->dirtyFlag = FALSE;
            } else {
                samples = blockOffsetSample;
                blockOffsetSample = 0;
            }
        }
        loopFlag = FALSE;
        if (blockOffsetSample + samples >= blockSamples) {
            samples = blockSamples - blockOffsetSample;
            if (blockNo >= player->info.numBlocks - 1) {
                if (player->info.loopFlag) {
                    loopFlag = TRUE;
                } else {
                    player->finishFlag = TRUE;
                }
            }
        }
        blockOffset = blockOffsetSample;
        size = samples;
        switch (player->info.format) {
        case STRM_FORMAT_PCM8:
            readSize = size;
            break;
        case STRM_FORMAT_PCM16:
            blockOffset <<= 1;
            size <<= 1;
            readSize = size;
            break;
        case STRM_FORMAT_ADPCM: {
            u32 endSample = blockOffsetSample + samples;
            blockOffset >>= 1;
            endSample++;
            endSample >>= 1;
            readSize = endSample - blockOffset;
            if (blockOffsetSample == 0) {
                readSize += sizeof(AdpcmState);
            } else {
                blockOffset += sizeof(AdpcmState);
            }
            size <<= 1;
            break;
        }
        }
        offset = blockOffset;
        offset += blockNo * player->info.blockSize * player->info.numChannels;
        offset += player->info.dataOffset;
        for (ch = 0; ch < command->numChannels; ch++) {
            void *dest;
            void *readDest;
            dest = readDest = (u8 *) command->buffer[ch] + destOffset;
            if (ch < player->info.numChannels) {
                s32 resultSize;
                if (player->info.format == STRM_FORMAT_ADPCM) {
                    OS_LockMutex(&sDecodeBufferMutex);
                    readDest = sDecodeBuffer;
                }
                resultSize = player->readStreamFunc(player, readDest, readSize, offset + ch * blockSize);
                if (resultSize != readSize) {
                    size = 0;
                    samples = 0;
                    loopFlag = FALSE;
                    player->finishFlag = TRUE;
                    if (player->info.format == STRM_FORMAT_ADPCM) {
                        OS_UnlockMutex(&sDecodeBufferMutex);
                    }
                    break;
                }
                if (player->info.format == STRM_FORMAT_ADPCM) {
                    AdpcmState *state = &player->adpcmState[ch];
                    u8 *srcp = sDecodeBuffer;
                    s16 *destp = dest;
                    u32 i;
                    u32 end;
                    if (blockOffsetSample == 0) {
                        AdpcmState *header = (AdpcmState *) srcp;
                        srcp = (u8 *) (header + 1);
                        *state = *header;
                    }
                    end = blockOffsetSample + samples;
                    i = blockOffsetSample;
                    if (i & 1) {
                        *destp++ = DecodeAdpcm((*srcp >> 4) & 0xF, state);
                        i++;
                        srcp++;
                    }
                    while (i < (end & ~1)) {
                        *destp++ = DecodeAdpcm(*srcp & 0xF, state);
                        i++;
                        *destp++ = DecodeAdpcm((*srcp >> 4) & 0xF, state);
                        i++;
                        srcp++;
                    }
                    if (i < end) {
                        *destp++ = DecodeAdpcm(*srcp & 0xF, state);
                        i++;
                    }
                    OS_UnlockMutex(&sDecodeBufferMutex);
                }
            } else if (player->monoFlag) {
                MI_CpuFill8(dest, 0, size);
            } else {
                MI_CpuCopy8((u8 *) command->buffer[0] + destOffset, dest, size);
            }
        }
        if (player->dirtyFlag) {
            player->dirtyFlag = FALSE;
            continue;
        }
        if (loopFlag) {
            player->curSample = player->info.loopStart;
        } else {
            player->curSample += samples;
        }
        destOffset += size;
        restSize -= size;
        if (player->finishFlag && player->sndArcStrmCallback) {
            OnDataEnd(player);
        }
    }
    if (player->strmCallback != NULL) {
        player->strmCallback(command->status, command->numChannels, command->buffer, command->bufLen,
                             player->info.format == STRM_FORMAT_PCM8 ? NNS_SND_STRM_FORMAT_PCM8
                                                                     : NNS_SND_STRM_FORMAT_PCM16,
                             player->strmCallbackArg);
    }
    for (ch = 0; ch < command->numChannels; ch++) {
        DC_FlushRange(command->buffer[ch], command->bufLen);
    }
    if (command->status == NNS_SND_STRM_CALLBACK_SETUP) {
        player->prepareFlag = TRUE;
    }
}

// Streams whose file is loaded into memory are read from there; the others from the ROM file.
static void SetupStreamFunction(NNSSndStrmPlayer *player, u32 fileId) {
    if (NNS_SndArcGetFileAddress(fileId) == NULL) {
        player->openStreamFunc = OpenFileStream;
        player->closeStreamFunc = CloseFileStream;
        player->readStreamFunc = ReadFileStream;
        player->cancelStreamFunc = CancelFileStream;
    } else {
        player->openStreamFunc = OpenMemoryStream;
        player->closeStreamFunc = CloseMemoryStream;
        player->readStreamFunc = ReadMemoryStream;
        player->cancelStreamFunc = CancelMemoryStream;
    }
}

static BOOL OpenFileStream(NNSSndStrmPlayer *player, u32 fileId) {
    FSFileID id;

    if (NNS_SndArcReadFile(fileId, &player->info, sizeof(player->info), 0) != sizeof(player->info)) {
        return FALSE;
    }
    NNS_SndArcGetFileID(&id);
    if (!FS_OpenFileFast(&player->file, id)) {
        return FALSE;
    }
    player->fileOffset = NNS_SndArcGetFileOffset(fileId);
    return TRUE;
}

static void CloseFileStream(NNSSndStrmPlayer *player) {
    FS_CloseFile(&player->file);
}

static s32 ReadFileStream(NNSSndStrmPlayer *player, void *dest, u32 size, u32 offset) {
    BOOL result;

    result = FS_SeekFile(&player->file, (s32) (player->fileOffset + offset), FS_SEEK_SET);
    return FS_ReadFile(&player->file, dest, (s32) size);
}

static void CancelFileStream(NNSSndStrmPlayer *player) {
    FS_CancelFile(&player->file);
}

static BOOL OpenMemoryStream(NNSSndStrmPlayer *player, u32 fileId) {
    player->fileOffset = (u32) NNS_SndArcGetFileAddress(fileId);
    MI_CpuCopy8((const void *) player->fileOffset, &player->info, sizeof(player->info));
    return TRUE;
}

static void CloseMemoryStream(NNSSndStrmPlayer *player) {
}

static s32 ReadMemoryStream(NNSSndStrmPlayer *player, void *dest, u32 size, u32 offset) {
    const u8 *src = (const u8 *) player->fileOffset;
    MI_CpuCopy8(src + offset, dest, size);
    return (s32) size;
}

static void CancelMemoryStream(NNSSndStrmPlayer *player) {
}

static void StrmThread(void *arg) {
    NNSSndStrmThread *thread = (NNSSndStrmThread *) arg;

    while (1) {
        LoadCommand *command;
        OS_SleepThread(&thread->threadQ);
        while (1) {
            OS_LockMutex(&thread->mutex);
            command = ReadCommandBuffer(&thread->commandList);
            if (command == NULL) {
                OS_UnlockMutex(&thread->mutex);
                break;
            }
            MakeWaveData(command);
            FreeCommandBuffer(command);
            OS_UnlockMutex(&thread->mutex);
        }
    }
}
