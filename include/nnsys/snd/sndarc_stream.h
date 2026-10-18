// NitroSystem sound: playing the sound archive's streams (STRM files), read from the ROM or from memory on a
// thread of their own and decoded into a stream's ring buffer.
#ifndef NNSYS_SND_SNDARC_STREAM_H
#define NNSYS_SND_SNDARC_STREAM_H

#include "nitro/os.h"
#include "nnsys/fnd/list.h"
#include "nnsys/snd/heap.h"
#include "nnsys/snd/stream.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NNS_SND_STRM_PLAYER_NUM 4
#define NNS_SND_STRM_THREAD_STACK_SIZE 1024

struct NNSSndStrmPlayer;

typedef enum {
    NNS_SND_ARC_STRM_CALLBACK_DATA_END
} NNSSndArcStrmCallbackStatus;

typedef struct {
    int playerNo;
    int strmNo;
} NNSSndArcStrmCallbackInfo;

// What to play when the data ends: another stream (or the same), from an offset in milliseconds.
typedef struct {
    int strmNo;
    u32 offset;
} NNSSndArcStrmCallbackParam;

typedef BOOL (*NNSSndArcStrmCallback)(NNSSndArcStrmCallbackStatus status, const NNSSndArcStrmCallbackInfo *info,
                                      NNSSndArcStrmCallbackParam *param, void *arg);

typedef struct NNSSndStrmHandle {
    struct NNSSndStrmPlayer *player;
} NNSSndStrmHandle;

// A thread that loads the stream data, with the commands waiting for it.
typedef struct {
    OSThread thread;
    u64 stack[NNS_SND_STRM_THREAD_STACK_SIZE / sizeof(u64)];
    OSThreadQueue threadQ;
    OSMutex mutex;
    NNSFndList commandList;
} NNSSndStrmThread;

static inline BOOL NNS_SndStrmHandleIsValid(const NNSSndStrmHandle *handle) {
    return handle->player != NULL;
}

void NNS_SndArcStrmInit(u32 threadPrio, NNSSndHeapHandle heap);
BOOL NNS_SndArcStrmSetupPlayer(NNSSndHeapHandle heap);
void NNS_SndArcStrmCreatePrepareThread(NNSSndStrmThread *thread, u32 threadPrio);
BOOL NNS_SndArcStrmPrepare(NNSSndStrmHandle *handle, int strmNo, u32 offset);
void NNS_SndArcStrmStartPrepared(NNSSndStrmHandle *handle);
BOOL NNS_SndArcStrmStart(NNSSndStrmHandle *handle, int strmNo, u32 offset);
void NNS_SndArcStrmStop(NNSSndStrmHandle *handle, int fadeFrame);
void NNS_SndArcStrmMoveVolume(NNSSndStrmHandle *handle, int volume, int frames);
void NNS_SndStrmHandleInit(NNSSndStrmHandle *handle);
void NNS_SndStrmHandleRelease(NNSSndStrmHandle *handle);
u32 NNS_SndArcStrmGetCurrentPlayingPos(NNSSndStrmHandle *handle);

#ifdef __cplusplus
}
#endif

#endif
