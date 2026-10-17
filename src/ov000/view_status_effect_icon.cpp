// ViewStatusEffectIcon.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/view_status_effect_icon.h"
#include "types.h"

DinoBattleViewStatusEffectIcon::~DinoBattleViewStatusEffectIcon() {
}

ViewStatusEffectIcon::~ViewStatusEffectIcon() {
}

ViewStatusEffectIconWaitStop::~ViewStatusEffectIconWaitStop() {
}

ViewStatusEffectIconLoadMotionTask::~ViewStatusEffectIconLoadMotionTask() {
}

ViewStatusEffectIconShowTask::~ViewStatusEffectIconShowTask() {
}

void ViewStatusEffectIcon::Idle() {
}

ViewStatusEffectIcon::PlayEraseMotionTask::~PlayEraseMotionTask() {
}

ViewStatusEffectIcon::SetPriorityTask::~SetPriorityTask() {
}

void ViewStatusEffectIconWaitStop::Execute() {
    if (!mIcon->IsPlaying()) {
        Exit();
    }
}

void ViewStatusEffectIconShowTask::Execute() {
    if (mShow) {
        mIcon->ShowMotion();
    } else {
        mIcon->HideMotion();
    }
    Exit();
}
