// ViewStatusEffectIcon.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/view_status_effect_icon.h"
#include "types.h"
#include "m/smart_oam.h"

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

#pragma auto_inline off

int StatusEffectEntry::GetAnime() {
    int offset = mFlag ? 12 : 0;
    int anime = mCount <= 1 ? 2 : 3;
    anime += offset;
    return anime;
}

void DinoBattleViewStatusEffectIcon::HideIcons() {
    for (u32 i = 0; i < 2; i++) {
        mSlots[i].mIcon->HideMotion();
    }
}

void DinoBattleViewStatusEffectIcon::ResetIconModes() {
    for (int i = 0; i < 2; i++) {
        if (mSlots[i].mIcon->mMotion != NULL) {
            mSlots[i].mIcon->mMotion->GetUnit()->SetMode(0);
        }
    }
}

void ViewStatusEffectIcon::Idle() {
}

void ViewStatusEffectIcon::SetPriorityTask::Execute() {
    mIcon->SetMotionPriority(mPriority);
    Exit();
}

#pragma auto_inline on

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
