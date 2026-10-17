// NitroSystem sound: a value that moves to a target over a number of frames. (Not fader.c: the linker
// script names objects by file name, and src/sound/fader.cpp has that one.)

#include "nnsys/snd/fader.h"

void NNSi_SndFaderInit(NNSSndFader *fader) {
    fader->target = 0;
    fader->origin = 0;
    fader->frame = 0;
    fader->counter = 0;
}

void NNSi_SndFaderSet(NNSSndFader *fader, int target, int frames) {
    fader->origin = NNSi_SndFaderGet(fader);
    fader->target = target;
    fader->frame = frames;
    fader->counter = 0;
}

int NNSi_SndFaderGet(const NNSSndFader *fader) {
    if (fader->counter >= fader->frame) {
        return fader->target;
    }
    return fader->origin + (fader->target - fader->origin) * fader->counter / fader->frame;
}

void NNSi_SndFaderUpdate(NNSSndFader *fader) {
    if (fader->counter < fader->frame) {
        fader->counter++;
    }
}

BOOL NNSi_SndFaderIsFinished(const NNSSndFader *fader) {
    return fader->counter >= fader->frame;
}
