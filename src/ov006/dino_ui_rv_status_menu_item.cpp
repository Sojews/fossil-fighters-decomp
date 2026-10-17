// DinoUiRvStatusMenuItem.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_ui_rv_status_menu_item_menu.h"
#include "game/dino_ui_rv_status_menu_item.h"
#include "game/help.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "game/dino_sound.h"
#include "m/lcd.h"
#include "m/display.h"
#include "m/bright.h"

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

MRect DinoUiRvStatusMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

const char *DinoUiRvStatusMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUiRvStatusMenuItem::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(2, 0);
    } else {
        mMotion->Play(1, 0);
    }
}

void DinoUiRvStatusBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

void DinoUiRvStatusBackButton::OnDecide() {
    mMotion->Play(4, 1);
    mOwner->OnBackDecided();
    DinoSoundPlaySe(0x90, 0);
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask0<DinoUiRvStatusMenu, DinoHelpBase>;
template MTask *MCreateCallTask(DinoUiRvStatusMenu *obj, void (DinoHelpBase::*func)());
template class MCallTask1<DinoUiRvStatusMenu, DinoUiRvStatusMenu, u8>;
template MTask *MCreateCallTask(DinoUiRvStatusMenu *obj, void (DinoUiRvStatusMenu::*func)(u8), u8 a0);
