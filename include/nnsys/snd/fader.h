// NitroSystem sound: a value that moves to a target over a number of frames.
#ifndef NNSYS_SND_FADER_H
#define NNSYS_SND_FADER_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int origin;
    int target;
    int counter;
    int frame;
} NNSSndFader;

void NNSi_SndFaderInit(NNSSndFader *fader);
void NNSi_SndFaderSet(NNSSndFader *fader, int target, int frames);
int NNSi_SndFaderGet(const NNSSndFader *fader);
void NNSi_SndFaderUpdate(NNSSndFader *fader);
BOOL NNSi_SndFaderIsFinished(const NNSSndFader *fader);

#ifdef __cplusplus
}
#endif

#endif
