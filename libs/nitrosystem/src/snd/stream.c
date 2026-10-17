// NitroSystem sound: streams, which play PCM from a ring buffer the caller refills from a callback.

#include "nitro/os.h"
#include "nitro/snd.h"
#include "nnsys/snd/resource_mgr.h"
#include "nnsys/snd/stream.h"

typedef struct {
    void *buffer;
    int volume;
} ChannelInfo;

static ChannelInfo sChannelInfo[NNS_SND_STRM_CHANNEL_MAX];
static void *sBufferAddrs[NNS_SND_STRM_CHANNEL_MAX];
static NNSFndList sStrmList;
static BOOL sInitialized;

static void ForceStopStrm(NNSSndStrm *stream);
static void ShutdownStrm(NNSSndStrm *stream);
static void AlarmCallback(void *arg);
static void StrmCallback(NNSSndStrm *stream, BOOL interval);
static void BeginSleep(void *arg);
static void EndSleep(void *arg);

void NNS_SndStrmInit(NNSSndStrm *stream) {
    if (!sInitialized) {
        NNS_FndInitList(&sStrmList, 0);
        sInitialized = TRUE;
    }
    PM_SetSleepCallback(&stream->preSleepInfo, BeginSleep, stream);
    PM_SetSleepCallback(&stream->postSleepInfo, EndSleep, stream);
    stream->chBitMask = 0;
    stream->numChannels = 0;
    stream->active = FALSE;
    stream->startFlag = FALSE;
}

BOOL NNS_SndStrmAllocChannel(NNSSndStrm *stream, int numChannels, const u8 chNoList[]) {
    u32 chBitMask = 0;
    int i;
    for (i = 0; i < numChannels; i++) {
        stream->channelNo[i] = chNoList[i];
        chBitMask |= 1 << chNoList[i];
    }
    if (!NNS_SndLockChannel(chBitMask)) {
        return FALSE;
    }
    stream->numChannels = numChannels;
    stream->chBitMask = chBitMask;
    return TRUE;
}

void NNS_SndStrmFreeChannel(NNSSndStrm *stream) {
    if (stream->chBitMask == 0) {
        return;
    }
    NNS_SndUnlockChannel(stream->chBitMask);
    stream->chBitMask = 0;
    stream->numChannels = 0;
}

BOOL NNS_SndStrmSetup(NNSSndStrm *stream, NNSSndStrmFormat format, void *buffer, u32 bufSize, int timer,
                      int interval, NNSSndStrmCallback callback, void *arg) {
    u32 samples;
    u32 alarmPeriod;
    int i;
    OSIntrMode enabled;

    if (stream->active) {
        NNS_SndStrmStop(stream);
    }
    stream->bufSize = interval * (bufSize / (interval * 32 * stream->numChannels)) * 32;
    samples = stream->bufSize;
    if (format == NNS_SND_STRM_FORMAT_PCM16) {
        samples >>= 1;
    }
    alarmPeriod = timer * samples / interval;
    stream->alarmNo = NNS_SndAllocAlarm();
    if (stream->alarmNo < 0) {
        return FALSE;
    }
    for (i = 0; i < stream->numChannels; i++) {
        int chNo = stream->channelNo[i];
        sChannelInfo[chNo].buffer = (u8 *) buffer + stream->bufSize * i;
        sChannelInfo[chNo].volume = 0;
        SND_SetupChannelPcm(chNo, format, sChannelInfo[chNo].buffer, 1, 0, stream->bufSize >> 2, 127, 0, timer << 5,
                            64);
    }
    SND_SetupAlarm(stream->alarmNo, alarmPeriod, alarmPeriod, AlarmCallback, stream);
    NNS_FndAppendListObject(&sStrmList, stream);
    stream->format = format;
    stream->bufferPages = interval;
    stream->callback = callback;
    stream->callbackArg = arg;
    stream->curBuffer = 0;
    stream->volume = 0;
    stream->active = TRUE;

    enabled = OS_DisableInterrupts();
    stream->bufferPages = 1;
    StrmCallback(stream, FALSE);
    stream->bufferPages = interval;
    OS_RestoreInterrupts(enabled);
    return TRUE;
}

void NNS_SndStrmStart(NNSSndStrm *stream) {
    SND_StartTimer(stream->chBitMask, 0, 1 << stream->alarmNo, 0);
    if (!stream->startFlag) {
        PM_PrependPreSleepCallback(&stream->preSleepInfo);
        PM_AppendPostSleepCallback(&stream->postSleepInfo);
        stream->startFlag = TRUE;
    }
}

void NNS_SndStrmStop(NNSSndStrm *stream) {
    if (stream->active) {
        ForceStopStrm(stream);
    }
}

void NNS_SndStrmSetVolume(NNSSndStrm *stream, int volume) {
    int i;
    stream->volume = volume;
    for (i = 0; i < stream->numChannels; i++) {
        int chNo = stream->channelNo[i];
        int v = SND_CalcChannelVolume(stream->volume + sChannelInfo[chNo].volume);
        SND_SetChannelVolume(1 << chNo, (u8) v, v >> 8);
    }
}

void NNS_SndStrmSetChannelPan(NNSSndStrm *stream, int chIdx, int pan) {
    if (chIdx > stream->numChannels - 1) {
        return;
    }
    SND_SetChannelPan(1 << stream->channelNo[chIdx], pan);
}

static void ForceStopStrm(NNSSndStrm *stream) {
    if (stream->startFlag) {
        u32 tag;
        SND_StopTimer(stream->chBitMask, 0, 1 << stream->alarmNo, 0);
        PM_DeletePreSleepCallback(&stream->preSleepInfo);
        PM_DeletePostSleepCallback(&stream->postSleepInfo);
        stream->startFlag = FALSE;
        tag = SND_GetCurrentCommandTag();
        SND_FlushCommand(SND_COMMAND_BLOCK);
        SND_WaitForCommandProc(tag);
    }
    ShutdownStrm(stream);
}

static void ShutdownStrm(NNSSndStrm *stream) {
    NNS_SndFreeAlarm(stream->alarmNo);
    NNS_FndRemoveListObject(&sStrmList, stream);
    stream->active = FALSE;
}

static void AlarmCallback(void *arg) {
    StrmCallback(arg, TRUE);
}

static void StrmCallback(NNSSndStrm *stream, BOOL interval) {
    u32 len = stream->bufSize / stream->bufferPages;
    u32 offset = len * stream->curBuffer;
    int i;
    for (i = 0; i < stream->numChannels; i++) {
        sBufferAddrs[i] = (u8 *) sChannelInfo[stream->channelNo[i]].buffer + offset;
    }
    stream->callback((NNSSndStrmCallbackStatus) interval, stream->numChannels, sBufferAddrs, len, stream->format,
                     stream->callbackArg);
    stream->curBuffer++;
    if (stream->curBuffer >= stream->bufferPages) {
        stream->curBuffer = 0;
    }
}

static void BeginSleep(void *arg) {
    NNSSndStrm *stream = arg;
    if (stream->startFlag) {
        u32 tag;
        SND_StopTimer(stream->chBitMask, 0, 1 << stream->alarmNo, 0);
        tag = SND_GetCurrentCommandTag();
        SND_FlushCommand(SND_COMMAND_BLOCK);
        SND_WaitForCommandProc(tag);
    }
}

static void EndSleep(void *arg) {
    NNSSndStrm *stream = arg;
    if (stream->startFlag) {
        while (stream->curBuffer != 0) {
            OSIntrMode enabled = OS_DisableInterrupts();
            StrmCallback(stream, TRUE);
            OS_RestoreInterrupts(enabled);
        }
        SND_StartTimer(stream->chBitMask, 0, 1 << stream->alarmNo, 0);
    }
}
