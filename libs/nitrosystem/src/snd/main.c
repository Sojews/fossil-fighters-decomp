// NitroSystem sound: setting up and running the sound library.

#include "nitro/pm.h"
#include "nitro/snd.h"
#include "nnsys/snd/main.h"

static struct {
    s8 unk00;
    BOOL unk04;
    u32 unk08;
    BOOL initialized;
    PMSleepCallbackInfo preSleepCallback;
    PMSleepCallbackInfo postSleepCallback;
} sSnd;

static void BeginSleep(void *arg);
static void EndSleep(void *arg);

void NNS_SndInit(void) {
    if (sSnd.initialized) {
        return;
    }
    sSnd.initialized = TRUE;
    SND_Init();
    PM_SetSleepCallback(&sSnd.preSleepCallback, BeginSleep, NULL);
    PM_SetSleepCallback(&sSnd.postSleepCallback, EndSleep, NULL);
    PM_PrependPreSleepCallback(&sSnd.preSleepCallback);
    PM_AppendPostSleepCallback(&sSnd.postSleepCallback);
    NNSi_SndInitResourceMgr();
    NNSi_SndCaptureInit();
    NNSi_SndPlayerInit();
    sSnd.unk00 = -1;
    sSnd.unk04 = TRUE;
}

void NNS_SndMain(void) {
    while (SND_RecvCommandReply(SND_COMMAND_NOBLOCK) != NULL) {
    }
    NNSi_SndPlayerMain();
    NNSi_SndCaptureMain();
    NNSi_SndArcStrmMain();
    SND_FlushCommand(SND_COMMAND_NOBLOCK);
}

static void BeginSleep(void *arg) {
    u32 tag;
    NNSi_SndCaptureBeginSleep();
    SND_StopTimer(0, 0, 0, 0);
    tag = SND_GetCurrentCommandTag();
    SND_FlushCommand(SND_COMMAND_BLOCK);
    SND_WaitForCommandProc(tag);
}

static void EndSleep(void *arg) {
    NNSi_SndCaptureEndSleep();
}
