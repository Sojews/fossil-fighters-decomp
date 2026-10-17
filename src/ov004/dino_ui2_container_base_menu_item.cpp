// DinoUi2ContainerBaseMenuItem.
// Only the destructors are decompiled so far.

// The buttons' destructors have the base destructor body inlined; auto-inlining the one defined above does that.
#pragma auto_inline on

#include "game/dino_ui2_container_base_menu_item_menu.h"
#include "game/dino_ui2_container_base_menu_item.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"

DinoUi2ContainerBaseMenuItem::~DinoUi2ContainerBaseMenuItem() {
}

DinoUi2ContainerBaseBackButton::~DinoUi2ContainerBaseBackButton() {
}

DinoUi2ContainerBasePageLButton::~DinoUi2ContainerBasePageLButton() {
}

DinoUi2ContainerBasePageRButton::~DinoUi2ContainerBasePageRButton() {
}

DinoUi2ContainerBaseDecideButton::~DinoUi2ContainerBaseDecideButton() {
}

#pragma auto_inline off

DinoUi2ContainerBaseMenu::~DinoUi2ContainerBaseMenu() {
    GetDisplay()->mBlend->Reset();
}

DinoUi2CannonShootMenu::~DinoUi2CannonShootMenu() {
}

#pragma auto_inline on

MRect DinoUi2ContainerBaseMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUi2ContainerBaseMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUi2ContainerBaseMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

void DinoUi2ContainerBaseMenuItem::OnFocusChanged(BOOL focused) {
    mMotion->Play(1, 0);
}

const char *DinoUi2ContainerBaseMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUi2ContainerBaseBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask0<DinoUi2CannonShootMenu, DinoUi2CannonShootMenu>;
template MTask *MCreateCallTask(DinoUi2CannonShootMenu *obj, void (DinoUi2CannonShootMenu::*func)());
