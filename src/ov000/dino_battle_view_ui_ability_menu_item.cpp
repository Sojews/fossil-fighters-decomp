// DinoBattleViewUiAbilityMenuItem.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_battle_view_ability_text_window.h"
#include "game/dino_battle_view_ability_menu_bg.h"
#include "game/dino_battle_view_ui_ability_menu_item.h"
#include "types.h"

DinoBattleViewUiAbilityMenuItem::~DinoBattleViewUiAbilityMenuItem() {
}

DinoBattleViewUiAbilityMenu::~DinoBattleViewUiAbilityMenu() {
}

AttackMenuBarMotion::~AttackMenuBarMotion() {
}

MenuEnableTask::~MenuEnableTask() {
}

DinAttackItemHelp::~DinAttackItemHelp() {
}

void AttackMenuBarMotion::Idle() {
}

AttackMenuBarMotion::ShowTask::~ShowTask() {
}

AttackMenuBarMotion::SetPriorityTask::~SetPriorityTask() {
}

DinoBattleViewAbilityMenuBG::SetPriorityTask::~SetPriorityTask() {
}

DinoBattleViewAbilityTextWindow::ShowTask::~ShowTask() {
}

void DinoBattleViewAbilityTextWindow::Execute() {
    MTask *task = mTask;
    if (task != NULL && task->mState == MTask::STATE_RUN) {
        task->Execute();
    }
}

void MenuEnableTask::Execute() {
    if (mEnable) {
        mMenu->Activate();
    } else {
        mMenu->Deactivate();
    }
    Exit();
}
