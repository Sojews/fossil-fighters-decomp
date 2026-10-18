// DinoBattle, DinoBattleAutoDemo, DinoBattleDebugSelect.
// Only the destructors are decompiled so far.

#include "game/battle_select.h"
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

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x28];
    void (DinoBattleDebugSelect::*Unk0215a05c_28)();
} data_ov000_021911ac;
extern "C" struct {
    u8 unk00[0x10];
    void (DinoBattleAutoDemo::*Start_10)();
} data_ov000_0219122c;

void DinoBattleDebugSelect::StateLevelSelect() {
    if (mCancelled) {
        Exit();
        return;
    }
    BOOL unk = mParams.unk00 != 2;
    mSerial.AddTail(new PlayerLevelSelect(&mParams.unk00, unk, 2));
    mState = data_ov000_021911ac.Unk0215a05c_28;
}

DinoBattleAutoDemo::DinoBattleAutoDemo(MTask *unk7C, u8 *result) : MProcess(NULL), mUnk7C(unk7C), mResult(result) {
    mState = data_ov000_0219122c.Start_10;
}
