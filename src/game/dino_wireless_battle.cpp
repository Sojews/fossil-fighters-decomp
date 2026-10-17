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
