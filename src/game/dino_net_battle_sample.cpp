// DinoNetBattleSample.
// Only the destructors are decompiled so far.

#include "game/dino_net_battle_sample.h"

DinoNetBattleSample::~DinoNetBattleSample() {
}

void DinoNetBattleSample::Idle() {
    (this->*mState)();
}

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x14];
    void (DinoNetBattleSample::*Start_14)();
} data_020bd0d8;

DinoNetBattleSample::DinoNetBattleSample() : MProcess(NULL) {
    mState = data_020bd0d8.Start_14;
}
