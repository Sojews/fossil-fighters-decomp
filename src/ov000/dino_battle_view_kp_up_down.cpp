// DinoBattleViewKpUpDown.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_battle_view_kp_up_down.h"
#include "types.h"
#include "game/info_panel_window.h"

DinoBattleViewKPUpDown::~DinoBattleViewKPUpDown() {
}

WaitPopNumberTask::~WaitPopNumberTask() {
}

KPUpDownMotion::~KPUpDownMotion() {
}

void DinoBattleViewPopNumber::Idle() {
}

void KPUpDownMotion::Idle() {
}

void DinoBattleViewKPUpDown::Idle() {
}

extern "C" int func_ov000_0217de0c(void);

void WaitPopNumberTask::Execute() {
    if (func_ov000_0217de0c() == 0) {
        Exit();
    }
}
