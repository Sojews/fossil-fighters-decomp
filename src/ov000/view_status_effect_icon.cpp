// ViewStatusEffectIcon.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/view_status_effect_icon.h"
#include "types.h"
#include "m/smart_oam.h"
#include "game/dino_resource.h"

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

// NONMATCHING: spill slots of dx/dy/-dx and the loop counters are permuted
DinoBattleViewStatusEffectIcon::DinoBattleViewStatusEffectIcon(MSmartOam *oam, u32 side, u32 unk74)
    : MProcess(NULL), mUnk74(unk74), mOam(oam) {
    mSide = side;
    mTimer = 0;
    const DinoBattleAdjustDefs *defs = gDinoResource->GetBattleAdjustDefs();
    int dx = defs->statusIconX;
    int dy = defs->statusIconY;
    for (int i = 0; i < 2; i++) {
        mSlots[i].mUnk10 = -1;
    }
    mIcons = new MParallelTaskSet();
    for (int i = 0; i < 2; i++) {
        mSlots[i].mIcon = new ViewStatusEffectIcon(mOam);
        mOffsets[i].x = i != mSide ? -11 : -10;
        mOffsets[i].y = 0;
        if (mSide == 1) {
            mOffsets[i].x += (int)-dx;
            mOffsets[i].y += dy;
        } else {
            mOffsets[i].x += dx;
            mOffsets[i].y += dy;
        }
        mIcons->AddTail(mSlots[i].mIcon);
    }
    mParallel.AddTail(mIcons);
    mQueue = new MSerialTaskSet();
    mParallel.AddTail(mQueue);
}

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

// NONMATCHING: the game keeps the iterator in a stack slot and walks the slots with two pointers
void DinoBattleViewStatusEffectIcon::RemoveEntry(u32 id) {
    for (int i = 0; i < 2; i++) {
        for (std::vector<StatusEffectEntry>::iterator it = mSlots[i].mEntries.begin(); it != mSlots[i].mEntries.end(); it++) {
            if (id == (*it).mId) {
                mTimer = 60;
                mSlots[i].mEntries.erase(it);
                break;
            }
        }
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

#pragma auto_inline off
template void std::__vec_deleter<StatusEffectEntry, std::allocator<StatusEffectEntry> >::sub_push_back(const StatusEffectEntry &x, Metrowerks::int2type<true>);
