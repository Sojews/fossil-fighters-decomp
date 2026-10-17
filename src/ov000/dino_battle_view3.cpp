// DinoBattleView3.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_battle_view3.h"
#include "types.h"

DinoBattleViewLPCountPanel::~DinoBattleViewLPCountPanel() {
}

DinoBattleViewDecideFirstAttackTeam::~DinoBattleViewDecideFirstAttackTeam() {
}

void DinoBattleViewLPCountPanel::Idle() {
}

void DinoBattleViewDecideFirstAttackTeam::Idle() {
    (this->*mState)();
}
