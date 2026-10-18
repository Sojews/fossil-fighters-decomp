// BattleSelect.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/battle_select.h"
#include "types.h"

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
