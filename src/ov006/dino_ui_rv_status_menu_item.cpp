// DinoUiRvStatusMenuItem.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_ui_rv_status_menu_item_menu.h"
#include "game/dino_ui_rv_status_menu_item.h"
#include "game/help.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"

DinoUiRvStatusMenuItem::~DinoUiRvStatusMenuItem() {
}

DinoUiRvStatusMenu::~DinoUiRvStatusMenu() {
}

DinoUiRvStatusBackButton::~DinoUiRvStatusBackButton() {
}

DinoUiRvStatusPageLButton::~DinoUiRvStatusPageLButton() {
}

DinoUiRvStatusPageRButton::~DinoUiRvStatusPageRButton() {
}

MRect DinoUiRvStatusMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiRvStatusMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask0<DinoUiRvStatusMenu, DinoHelpBase>;
template MTask *MCreateCallTask(DinoUiRvStatusMenu *obj, void (DinoHelpBase::*func)());
template class MCallTask1<DinoUiRvStatusMenu, DinoUiRvStatusMenu, u8>;
template MTask *MCreateCallTask(DinoUiRvStatusMenu *obj, void (DinoUiRvStatusMenu::*func)(u8), u8 a0);
