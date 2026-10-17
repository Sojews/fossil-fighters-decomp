// DinoUiMultiplayMenuItem, DinoUiMultiplayMenuItem2.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_ui_multiplay_menu_item_menu.h"
#include "game/dino_ui_multiplay_menu_item.h"
#include "game/dino_ui_multiplay_menu_item2.h"
#include "game/help.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"

DinoUiMultiplayMenuItem::~DinoUiMultiplayMenuItem() {
}

DinoUiMultiplayMenuItem2::~DinoUiMultiplayMenuItem2() {
}

DinoUiMultiplayMenu::~DinoUiMultiplayMenu() {
}

DinoUiMultiplayWirelessButton::~DinoUiMultiplayWirelessButton() {
}

DinoUiMultiplayMenuBackButton::~DinoUiMultiplayMenuBackButton() {
}

DinoUiMultiplayOrganizationButton::~DinoUiMultiplayOrganizationButton() {
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask0<DinoUiMultiplayMenu, DinoHelpBase>;
template MTask *MCreateCallTask(DinoUiMultiplayMenu *obj, void (DinoHelpBase::*func)());
