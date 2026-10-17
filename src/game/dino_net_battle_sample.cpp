// DinoNetBattleSample.
// Only the destructors are decompiled so far.

#include "game/dino_net_battle_sample.h"

DinoNetBattleSample::~DinoNetBattleSample() {
}

void DinoNetBattleSample::Idle() {
    (this->*mState)();
}
