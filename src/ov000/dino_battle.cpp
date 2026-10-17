// DinoBattle, DinoBattleAutoDemo, DinoBattleDebugSelect.
// Only the destructors are decompiled so far.

#include "game/dino_battle.h"

DinoBattle::~DinoBattle() {
    delete mUnk208;
}

DinoBattleAutoDemo::~DinoBattleAutoDemo() {
    delete mUnk7C;
}

DinoBattleDebugSelect::~DinoBattleDebugSelect() {
}

void DinoBattleAutoDemo::Idle() {
    (this->*mState)();
}
