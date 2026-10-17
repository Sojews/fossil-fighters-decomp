// DinoSenryuSample.
// Only the destructors are decompiled so far.

#include "game/dino_senryu_sample.h"

DinoSenryuSample::~DinoSenryuSample() {
}

void DinoSenryuSample::Idle() {
    (this->*mState)();
}
