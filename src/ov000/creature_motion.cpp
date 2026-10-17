// CreatureMotion.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/creature_motion.h"
#include "types.h"

CreatureMotion::~CreatureMotion() {
}

NopTask::~NopTask() {
}

CreateCreatureMotionTask::~CreateCreatureMotionTask() {
}

EffectMotionTask::~EffectMotionTask() {
}

SetCretureIconTask::~SetCretureIconTask() {
}

SetCretureIconPriorityTask::~SetCretureIconPriorityTask() {
}

UpdateLifeBarTask::~UpdateLifeBarTask() {
}

UpdateLifePointTask::~UpdateLifePointTask() {
}

SetColorMaskTask::~SetColorMaskTask() {
}

ShuffleMoveTask::~ShuffleMoveTask() {
}

void EffectMotionTask::Idle() {
}

void NopTask::Execute() {
}
