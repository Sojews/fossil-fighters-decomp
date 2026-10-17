// DinoUiCleaningMenuItem.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_ui_cleaning_menu_item_menu.h"
#include "game/dino_ui_cleaning_menu_item.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "game/dino_sound.h"

DinoUiCleaningMenuItem::~DinoUiCleaningMenuItem() {
}

DinoUiCleaningMenu::~DinoUiCleaningMenu() {
}

DinoUiCleaningBackButton::~DinoUiCleaningBackButton() {
}

DinoUiCleaningPageLButton::~DinoUiCleaningPageLButton() {
}

DinoUiCleaningPageRButton::~DinoUiCleaningPageRButton() {
}

void DinoUiCleaningMenu::OnIdle() {
}

MRect DinoUiCleaningMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiCleaningMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiCleaningMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

void DinoUiCleaningMenuItem::OnFocusChanged(BOOL focused) {
    mMotion->Play(1, 0);
}

const char *DinoUiCleaningMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUiCleaningBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

void DinoUiCleaningBackButton::OnDecide() {
    mMotion->Play(4, 1);
    mOwner->OnBackDecided();
    DinoSoundPlaySe(0x90, 0);
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask0<DinoUiCleaningMenu, DinoUiCleaningMenu>;
template MTask *MCreateCallTask(DinoUiCleaningMenu *obj, void (DinoUiCleaningMenu::*func)());
