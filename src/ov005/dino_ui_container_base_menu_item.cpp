// DinoUiContainerBaseMenuItem.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_ui_container_base_menu_item_menu.h"
#include "game/dino_ui_container_base_menu_item.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"

DinoUiContainerBaseMenuItem::~DinoUiContainerBaseMenuItem() {
}

DinoUiContainerBaseBackButton::~DinoUiContainerBaseBackButton() {
}

DinoUiContainerBasePageLButton::~DinoUiContainerBasePageLButton() {
}

DinoUiContainerBasePageRButton::~DinoUiContainerBasePageRButton() {
}

DinoUiContainerBaseDecideButton::~DinoUiContainerBaseDecideButton() {
}

DinoUiContainerBaseMenu::~DinoUiContainerBaseMenu() {
}

MRect DinoUiContainerBaseMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiContainerBaseMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiContainerBaseMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

void DinoUiContainerBaseMenuItem::OnFocusChanged(BOOL focused) {
    mMotion->Play(1, 0);
}

const char *DinoUiContainerBaseMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUiContainerBaseBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
class DinoUiCanonPutinMenu {};
template class MCallTask0<DinoUiCanonPutinMenu, DinoUiCanonPutinMenu>;
template MTask *MCreateCallTask(DinoUiCanonPutinMenu *obj, void (DinoUiCanonPutinMenu::*func)());
