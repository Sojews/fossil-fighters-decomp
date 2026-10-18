// DinoBattleView3.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_battle_view3.h"
#include "types.h"

DinoBattleViewLPCountPanel::~DinoBattleViewLPCountPanel() {
}

DinoBattleViewDecideFirstAttackTeam::~DinoBattleViewDecideFirstAttackTeam() {
}

void DinoBattleViewLPCountPanel::Idle() {
}

void DinoBattleViewDecideFirstAttackTeam::Idle() {
    (this->*mState)();
}

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x34];
    void (DinoBattleViewDecideFirstAttackTeam::*Start_34)();
} data_ov000_02197300;

DinoBattleViewDecideFirstAttackTeam::DinoBattleViewDecideFirstAttackTeam(u32 unk88, u32 unk8C, u32 unk90, u32 unk94)
    : MProcess(NULL), mUnk88(unk88), mUnk8C(unk8C), mUnk90(unk90), mUnk94(unk94) {
    mState = data_ov000_02197300.Start_34;
}
