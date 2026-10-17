// DinoBattleViewEffectTelop.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_battle_view_effect_telop.h"
#include "types.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"

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

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MBgCallTask0<DinoBattleViewEffectTelop>;
template MTask *MCreateBgCallTask(DinoBattleViewEffectTelop *obj, void (DinoBattleViewEffectTelop::*func)(), int thread);

void SetBGModeTask::Execute() {
    mBg->SetMode(mMode);
    Exit();
}
