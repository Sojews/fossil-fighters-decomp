// DinoBattleViewUi.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_battle_view_ui_change_menu.h"
#include "types.h"
#include "game/dino_battle_view_ui_attacker_menu.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "game/dino_battle_view_ui_defender_menu.h"
#include "game/dino_battle_view_ui_select_all.h"
#include "game/dino_battle_view.h"

DinoBattleViewUiChangeMenu::~DinoBattleViewUiChangeMenu() {
}

DinoBattleViewUiAttackerMenu::~DinoBattleViewUiAttackerMenu() {
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
class DinoBattleViewCreatureInfo {};
template class MCallTask0<DinoBattleViewCreatureInfo, DinoBattleViewCreatureInfo>;
template MTask *MCreateCallTask(DinoBattleViewCreatureInfo *obj, void (DinoBattleViewCreatureInfo::*func)());

DinoBattleViewUiDefenderMenu::~DinoBattleViewUiDefenderMenu() {
}

DinoBattleViewUiSelectAllMenu::~DinoBattleViewUiSelectAllMenu() {
}

DinoBattleViewUiRootMenuItem::~DinoBattleViewUiRootMenuItem() {
}

DinoBattleViewUiSelectAreaCommon::~DinoBattleViewUiSelectAreaCommon() {
}

DinoBattleViewUiCreatureInfoMenu::~DinoBattleViewUiCreatureInfoMenu() {
}

DinoBattleViewInfoLRItem::~DinoBattleViewInfoLRItem() {
}

DinoBattleViewCancelMenuItem::~DinoBattleViewCancelMenuItem() {
}

DinoBattleHelpCreatureInfo::~DinoBattleHelpCreatureInfo() {
}

DinoFrameItem::~DinoFrameItem() {
}

void DinoBattleHelpBase::Idle() {
}

void DinoBattleViewCancelMenuItem::OnDecide() {
}

void DinoBattleViewInfoLRItem::OnDecide() {
}

void DinoBattleViewUiRootMenuItem::OnDecide() {
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
