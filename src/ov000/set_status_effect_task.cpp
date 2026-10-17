// SetStatusEffectTask.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/set_status_effect_task.h"
#include "types.h"
#include "game/create_player_update_ability_icon_task.h"

extern "C" void func_ov000_0216d3c0(void *, u8, u32);

SetStatusEffectTask::~SetStatusEffectTask() {
}

DinoCreatureUpdateTask::~DinoCreatureUpdateTask() {
}

DamagedUpdate::~DamagedUpdate() {
}

void DinoBattleModelCreature::Idle() {
    Dispatch();
}

void DinoCreatureUpdateTask::Execute() {
    func_ov000_0216d3c0((void *)mUnk10, mUnk14, mUnk18);
    Exit();
}
