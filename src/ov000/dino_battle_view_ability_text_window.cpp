// DinoBattleViewAbilityTextWindow.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_battle_view_ability_text_window.h"
#include "types.h"
#include "game/dino_battle_view_ability_menu_bg.h"

DinoBattleViewAbilityTextWindow::~DinoBattleViewAbilityTextWindow() {
    delete mTask;
}

void DinoBattleViewAbilityMenuBG::Idle() {
    (this->*mState)();
}
