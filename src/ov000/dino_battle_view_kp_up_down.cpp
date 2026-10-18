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

// Pooled file statics.
struct DinoBattleViewKPUpDownStatics {
    u32 unk0;
    int mPopNumberCount;    // 0x4
};
extern "C" DinoBattleViewKPUpDownStatics data_ov000_021978e0;

#pragma auto_inline off

int DinoBattleViewPopNumber::GetCount() {
    return data_ov000_021978e0.mPopNumberCount;
}

MTask *DinoBattleViewPopNumber::CreateWaitTask() {
    return new WaitPopNumberTask();
}

#pragma auto_inline on

void WaitPopNumberTask::Execute() {
    if (DinoBattleViewPopNumber::GetCount() == 0) {
        Exit();
    }
}
