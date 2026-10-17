// DinoBattleViewEffectTelop.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_battle_view_effect_telop.h"
#include "types.h"

DinoBattleViewEffectTelop::~DinoBattleViewEffectTelop() {
}

SetBGModeTask::~SetBGModeTask() {
}

SkipTelopTask::~SkipTelopTask() {
}

SkipWinTelopTask::~SkipWinTelopTask() {
}

SkipLoseTelopTask::~SkipLoseTelopTask() {
}

NopWaitTask::~NopWaitTask() {
}

void NopWaitTask::Execute() {
}

void DinoBattleViewEffectTelop::Idle() {
    (this->*mState)();
}
