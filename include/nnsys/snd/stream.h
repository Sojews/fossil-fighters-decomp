// NitroSystem sound: streams, which play PCM from a ring buffer the caller refills from a callback.
#ifndef NNSYS_SND_STREAM_H
#define NNSYS_SND_STREAM_H

#include "nitro/pm.h"
#include "nnsys/fnd/list.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NNS_SND_STRM_CHANNEL_MAX 16

typedef enum {
    NNS_SND_STRM_FORMAT_PCM8,
    NNS_SND_STRM_FORMAT_PCM16
} NNSSndStrmFormat;

typedef enum {
    NNS_SND_STRM_CALLBACK_SETUP,
    NNS_SND_STRM_CALLBACK_INTERVAL
} NNSSndStrmCallbackStatus;

typedef void (*NNSSndStrmCallback)(NNSSndStrmCallbackStatus status, int numChannels, void *buffer[], u32 len,
                                   NNSSndStrmFormat format, void *arg);

typedef struct {
    NNSFndLink link;
    PMSleepCallbackInfo preSleepInfo;
    PMSleepCallbackInfo postSleepInfo;
    NNSSndStrmFormat format;
    BOOL active : 1;
    BOOL startFlag : 1;         // Started, with sleep callbacks registered
    u32 bufSize;                // Per channel
    int bufferPages;
    NNSSndStrmCallback callback;
    void *callbackArg;
    int curBuffer;
    int volume;
    int alarmNo;
    u32 chBitMask;
    int numChannels;
    u8 channelNo[NNS_SND_STRM_CHANNEL_MAX];
} NNSSndStrm;

void NNS_SndStrmInit(NNSSndStrm *stream);
BOOL NNS_SndStrmAllocChannel(NNSSndStrm *stream, int numChannels, const u8 chNoList[]);
void NNS_SndStrmFreeChannel(NNSSndStrm *stream);
BOOL NNS_SndStrmSetup(NNSSndStrm *stream, NNSSndStrmFormat format, void *buffer, u32 bufSize, int timer,
                      int interval, NNSSndStrmCallback callback, void *arg);
void NNS_SndStrmStart(NNSSndStrm *stream);
void NNS_SndStrmStop(NNSSndStrm *stream);
void NNS_SndStrmSetVolume(NNSSndStrm *stream, int volume);
void NNS_SndStrmSetChannelPan(NNSSndStrm *stream, int chIdx, int pan);

#ifdef __cplusplus
}
#endif

#endif
