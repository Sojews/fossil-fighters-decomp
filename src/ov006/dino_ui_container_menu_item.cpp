// DinoUiContainerMenuItem.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_ui_container_menu_item_menu.h"
#include "game/dino_ui_container_menu_item.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "game/dino_sound.h"

DinoUiContainerMenuItem::~DinoUiContainerMenuItem() {
}

DinoUiContainerMenu::~DinoUiContainerMenu() {
}

DinoUiContainerBackButton::~DinoUiContainerBackButton() {
}

DinoUiContainerOKButton::~DinoUiContainerOKButton() {
}

DinoUiContainerPageLButton::~DinoUiContainerPageLButton() {
}

DinoUiContainerPageRButton::~DinoUiContainerPageRButton() {
}

void DinoUiContainerMenu::OnIdle() {
}

MRect DinoUiContainerMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiContainerMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiContainerMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

void DinoUiContainerMenuItem::OnFocusChanged(BOOL focused) {
    mMotion->Play(1, 0);
}

const char *DinoUiContainerMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUiContainerOKButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

void DinoUiContainerBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

void DinoUiContainerBackButton::OnDecide() {
    mMotion->Play(4, 1);
    mOwner->OnBackDecided();
    DinoSoundPlaySe(0x90, 0);
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask0<DinoUiContainerMenu, DinoUiContainerMenu>;
template MTask *MCreateCallTask(DinoUiContainerMenu *obj, void (DinoUiContainerMenu::*func)());
