// BattleSelect.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/battle_select.h"
#include "types.h"
#include "game/dino_battle_view2.h"

#pragma auto_inline off

BOOL DinoBattleViewAbilityIcon::IsAnimating() {
    for (int i = 0; i < 4; i++) {
        if ((mMask & (1 << i)) && mValue[i] != mTarget[i]) {
            return TRUE;
        }
    }
    return FALSE;
}

// NONMATCHING: the game keeps the position on the stack below the offset point; ours swaps the two slots.
MRect DinoBattleViewAbilityIcon::GetRect() {
    MPoint pos = mMotion->GetPosition();
    MPoint p(pos.x - mOam->mOffset.x, pos.y - mOam->mOffset.y);
    MRect r(-20, -20, 18, 18);
    r.Offset(p);
    return r;
}

BOOL DinoBattleViewAbilityIcon::IsHidden() {
    return mMotion->IsHidden();
}

#pragma auto_inline on

DebugAutoBattle::~DebugAutoBattle() {
    delete mRandom;
}

void DebugAutoBattle::Idle() {
    Exit();
}

BattleSelect::~BattleSelect() {
}

UseCreatureSelect::~UseCreatureSelect() {
}

PlayerLevelSelect::~PlayerLevelSelect() {
}

IncValTask::~IncValTask() {
}

TimeCountTask::~TimeCountTask() {
}

void TimeCountTask::Idle() {
}

void UseCreatureSelect::Idle() {
    (this->*mState)();
}

void PlayerLevelSelect::Idle() {
    (this->*mState)();
}

void IncValTask::Execute() {
    (*mValue)++;
}
