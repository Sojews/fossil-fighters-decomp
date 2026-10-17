// DinoUiPartsTabIconMenuItem, DinoUiStatusMenuItem, DinoUiStatusChangeModeHelpTask.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_ui_parts_tab_icon_menu_item_menu.h"
#include "game/dino_ui_parts_tab_icon_menu_item.h"
#include "game/dino_ui_status_menu_item.h"
#include "game/dino_ui_status_change_mode_help_task.h"
#include "game/help.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"

DinoUiPartsTabIconMenuItem::~DinoUiPartsTabIconMenuItem() {
}

DinoUiStatusMenuItem::~DinoUiStatusMenuItem() {
}

DinoUiStatusChangeModeHelpTask::~DinoUiStatusChangeModeHelpTask() {
}

DinoUiStatusMenu::~DinoUiStatusMenu() {
}

DinoUiStatusBackButton::~DinoUiStatusBackButton() {
}

DinoUiStatusOKButton::~DinoUiStatusOKButton() {
}

void DinoUiStatusChangeModeHelpTask::Idle() {
}

void DinoUiPartsTabIconMenuItem::OnSelectChanged(BOOL value) {
}

void DinoUiPartsTabIconMenuItem::OnDecide() {
}

void DinoUiPartsTabIconMenuItem::OnFocusChanged(BOOL value) {
}

MRect DinoUiStatusMenuItem::GetRect() {
    return GetFrame();
}

MRect DinoUiPartsTabIconMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiStatusMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MDisplay *DinoUiPartsTabIconMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiStatusMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

const char *DinoUiStatusMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

const char *DinoUiPartsTabIconMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUiStatusMenuItem::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask0<DinoUiStatusMenu, DinoUiStatusMenu>;
template MTask *MCreateCallTask(DinoUiStatusMenu *obj, void (DinoUiStatusMenu::*func)());
template class MCallTask0<DinoUiStatusChangeModeHelpTask, DinoHelpBase>;
template MTask *MCreateCallTask(DinoUiStatusChangeModeHelpTask *obj, void (DinoHelpBase::*func)());
