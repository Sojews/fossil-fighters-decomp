// NitroSystem sound: capture (recording the mixer output). Only what the game still links is here: the
// parts that stop it and keep it running across sleep.

#include "nitro/mi.h"
#include "nitro/os.h"
#include "nitro/snd.h"
#include "nnsys/snd/fader.h"
#include "nnsys/snd/main.h"
#include "nnsys/snd/resource_mgr.h"

typedef enum {
    NNS_SND_CAPTURE_TYPE_REVERB,
    NNS_SND_CAPTURE_TYPE_EFFECT
} NNSSndCaptureType;

typedef struct {
    BOOL active;
    NNSSndCaptureType type;
    u32 unk08;
    void *buffer0;
    void *buffer1;
    u32 bufSize;
    u32 unk18;
    u32 unk1C;
    u32 lockedChannels;
    u32 chBitMask;
    u32 capBitMask;
    int alarmNo;
    u32 unk30[3];
    NNSSndFader fader;
    BOOL stopOnFadeEnd;
    int volume;
} CaptureInfo;

static struct {
    u32 unk00;
    u32 unk04;
    OSMessageQueue messageQueue;
    OSMessage messages[8];
    CaptureInfo info;
} sCapture;

void NNSi_SndCaptureStop(void);

void NNSi_SndCaptureInit(void) {
    sCapture.unk00 = 0;
    sCapture.info.active = FALSE;
}

void NNSi_SndCaptureMain(void) {
    CaptureInfo *info = &sCapture.info;
    NNSSndFader *fader;
    int volume;
    if (!sCapture.info.active) {
        return;
    }
    if (info->type != NNS_SND_CAPTURE_TYPE_REVERB) {
        return;
    }
    fader = &info->fader;
    NNSi_SndFaderUpdate(fader);
    if (info->stopOnFadeEnd && NNSi_SndFaderIsFinished(fader)) {
        NNSi_SndCaptureStop();
        return;
    }
    volume = NNSi_SndFaderGet(fader) >> 8;
    if (volume != info->volume) {
        SND_SetChannelVolume(info->chBitMask, volume, 0);
        info->volume = volume;
    }
}

void NNSi_SndCaptureStop(void) {
    CaptureInfo *info = &sCapture.info;
    BOOL useAlarm;
    if (!sCapture.info.active) {
        return;
    }
    useAlarm = info->alarmNo >= 0;
    SND_StopTimer(info->chBitMask, info->capBitMask, useAlarm ? 1 << info->alarmNo : 0, 0);
    if (useAlarm) {
        u32 tag = SND_GetCurrentCommandTag();
        SND_FlushCommand(SND_COMMAND_BLOCK);
        SND_WaitForCommandProc(tag);
        while (OS_ReceiveMessage(&sCapture.messageQueue, NULL, OS_MESSAGE_NOBLOCK)) {
        }
    }
    if (info->capBitMask != 0) {
        NNS_SndUnlockCapture(info->capBitMask);
    }
    if (info->lockedChannels != 0) {
        NNS_SndUnlockChannel(info->lockedChannels);
    }
    if (useAlarm) {
        NNS_SndFreeAlarm(info->alarmNo);
    }
    if (info->type == NNS_SND_CAPTURE_TYPE_EFFECT) {
        SND_SetOutputSelector(0, 0, 0, 0);
    }
    info->active = FALSE;
}

void NNSi_SndCaptureBeginSleep(void) {
    CaptureInfo *info = &sCapture.info;
    u32 tag;
    if (!sCapture.info.active) {
        return;
    }
    SND_StopTimer(info->chBitMask, info->capBitMask, info->alarmNo >= 0 ? 1 << info->alarmNo : 0, 0);
    tag = SND_GetCurrentCommandTag();
    SND_FlushCommand(SND_COMMAND_BLOCK);
    SND_WaitForCommandProc(tag);
}

void NNSi_SndCaptureEndSleep(void) {
    CaptureInfo *info = &sCapture.info;
    if (!sCapture.info.active) {
        return;
    }
    info->unk1C = 0;
    MIi_CpuClear32(0, info->buffer0, info->bufSize);
    MIi_CpuClear32(0, info->buffer1, info->bufSize);
    DC_FlushRange(info->buffer0, info->bufSize);
    DC_FlushRange(info->buffer1, info->bufSize);
    SND_StartTimer(info->chBitMask, info->capBitMask, info->alarmNo >= 0 ? 1 << info->alarmNo : 0, 0);
}
