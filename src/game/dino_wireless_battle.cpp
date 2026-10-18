// DinoWirelessBattle.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_wireless_battle.h"
#include "types.h"

DinoWirelessBattle::~DinoWirelessBattle() {
}

void DinoWirelessBattle::Idle() {
    (this->*mState)();
}

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x40];
    void (DinoWirelessBattle::*Start_40)();
} data_020c0480;

DinoWirelessBattle::DinoWirelessBattle() : MProcess(NULL) {
    mState = data_020c0480.Start_40;
}
