// DinoUiReviveStatusMenu.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_ui_revive_status_menu.h"
#include "types.h"
#include "game/dino_sound.h"

DinoUiReviveStatusMenu::~DinoUiReviveStatusMenu() {
}

DinoUiReviveStatusLoop::~DinoUiReviveStatusLoop() {
}

DinoUiReviveStatusLoopCleaning::~DinoUiReviveStatusLoopCleaning() {
}

DinoUiReviveStatusLoopBattle::~DinoUiReviveStatusLoopBattle() {
}

DinoUiReviveStatusMenuItem::~DinoUiReviveStatusMenuItem() {
}

DinoUiReviveStatusBackButton::~DinoUiReviveStatusBackButton() {
}

MRect DinoUiReviveStatusMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiReviveStatusMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiReviveStatusMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

void DinoUiReviveStatusBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

const char *DinoUiReviveStatusMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUiReviveStatusBackButton::OnDecide() {
    mMotion->Play(4, 1);
    mOwner->OnBackDecided();
    DinoSoundPlaySe(0x90, 0);
}
