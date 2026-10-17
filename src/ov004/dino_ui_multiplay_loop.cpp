// DinoUiMultiplayLoop.
// Only the destructors are decompiled so far.

#include "game/dino_ui_multiplay_loop.h"
#include "game/help.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "game/dino_ui_multiplay_menu_item2.h"
#include "game/dino_ui_multiplay_menu_item.h"
#include "game/dino_ui_multiplay_menu_item_menu.h"
#include "game/dino_sound.h"

DinoUiMultiplayLoop::~DinoUiMultiplayLoop() {
}

MRect DinoUiMultiplayMenuItem2::GetRect() {
    return GetFrame();
}

MRect DinoUiMultiplayMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiMultiplayMenuItem2::GetDisplay() {
    return MProcess::GetDisplay();
}

MDisplay *DinoUiMultiplayMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiMultiplayMenuItem2::GetFrame() {
    return mMotion->GetFrame();
}

MRect DinoUiMultiplayMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

MRect DinoUiMultiplayMenuBackButton::GetFrame() {
    return mMotion->GetFrame();
}

MRect DinoUiMultiplayWirelessButton::GetFrame() {
    return mMotion->GetFrame();
}

const char *DinoUiMultiplayMenuItem2::GetText() {
    return gDinoResource->GetText(mTextId);
}

const char *DinoUiMultiplayMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUiMultiplayLoop::Idle() {
    (this->*mState)();
}

void DinoUiMultiplayMenuItem::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(2, 0);
    } else {
        mMotion->Play(1, 0);
    }
}

void DinoUiMultiplayMenuBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

void DinoUiMultiplayMenuBackButton::OnDecide() {
    DinoSoundPlaySe(0x38F, 0);
    mMotion->Play(4, 1);
    mOwner->Decide(0);
}

void DinoUiMultiplayWirelessButton::OnDecide() {
    DinoSoundPlaySe(0x390, 0);
    mMotion->Play(3, 1);
    mOwner->Decide(1);
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask0<DinoUiMultiplayMenu, DinoHelpBase>;
template MTask *MCreateCallTask(DinoUiMultiplayMenu *obj, void (DinoHelpBase::*func)());

void DinoUiMultiplayMenuItem2::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion2->Play(2, 0);
        mMotion->Play(2, 0);
    } else {
        mMotion2->Play(1, 0);
        mMotion->Play(1, 0);
    }
}

void DinoUiMultiplayMenu::OnFocus(MMenuItem *item) {
    SetSelectedItem(dynamic_cast<DinoHelpItem *>(item));
    if (!mSilentFocus) {
        DinoSoundPlaySe(0x38E, 0);
    }
    mSilentFocus = FALSE;
}
