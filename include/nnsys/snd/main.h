// NitroSystem sound: setting up and running the sound library.
#ifndef NNSYS_SND_MAIN_H
#define NNSYS_SND_MAIN_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

void NNS_SndInit(void);
void NNS_SndMain(void);

// Internal parts other files of the library call.
void NNSi_SndInitResourceMgr(void);
void NNSi_SndCaptureInit(void);
void NNSi_SndCaptureMain(void);
void NNSi_SndCaptureBeginSleep(void);
void NNSi_SndCaptureEndSleep(void);
void NNSi_SndPlayerInit(void);
void NNSi_SndPlayerMain(void);
void NNSi_SndArcStrmMain(void);

#ifdef __cplusplus
}
#endif

#endif
