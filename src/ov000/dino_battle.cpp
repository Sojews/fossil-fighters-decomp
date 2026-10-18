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

void DinoBattle::SetUnk1DC(u8 value) {
    if (mUnk1DC) {
        *mUnk1DC = value;
    }
}

void DinoBattle::SetUnk1E0(u8 value) {
    if (mUnk1E0) {
        *mUnk1E0 = value;
    }
}

u32 DinoBattle::GetUnk1F4(int i) {
    return mUnk1F4[i];
}

u32 DinoBattle::GetUnk1FC() {
    return mUnk1FC;
}

MMessageObserver *DinoBattle::GetUnk200() {
    return mUnk200;
}

void DinoBattle::Idle() {
    (this->*mState)();
}

void DinoBattleDebugSelect::Idle() {
    (this->*mState)();
}
