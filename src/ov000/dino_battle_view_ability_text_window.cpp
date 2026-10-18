// DinoBattleViewAbilityTextWindow.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_battle_view_ability_text_window.h"
#include "types.h"
#include "m/text.h"
#include "game/dino_battle_view_ability_menu_bg.h"

DinoBattleViewAbilityTextWindow::~DinoBattleViewAbilityTextWindow() {
    delete mTask;
}

#pragma auto_inline off

void DinoBattleViewAbilityMenuBG::SetPriority(int priority) {
    if (mUnk10D4) {
        mText->mBgCntBits.priority = priority;
    }
}

#pragma auto_inline on

void DinoBattleViewAbilityMenuBG::Idle() {
    (this->*mState)();
}

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x38];
    void (DinoBattleViewAbilityMenuBG::*Unk0217bee0_38)();
} data_ov000_021947ac;

void DinoBattleViewAbilityMenuBG::StateShow() {
    mText->Show();
    mState = data_ov000_021947ac.Unk0217bee0_38;
}
