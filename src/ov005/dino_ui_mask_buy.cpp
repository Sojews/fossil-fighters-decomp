// DinoUiMaskBuy, DinoUiMaskBuyMaskButton.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_ui_mask_buy_menu.h"
#include "game/dino_ui_mask_buy.h"
#include "game/dino_ui_mask_buy_mask_button.h"
#include "game/dino_ui_mask_buy_menu_item.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "game/dino_sound.h"

DinoUiMaskBuy::~DinoUiMaskBuy() {
}

DinoUiMaskBuyMaskButton::~DinoUiMaskBuyMaskButton() {
}

DinoUiMaskBuyMenuItem::~DinoUiMaskBuyMenuItem() {
}

DinoUiMaskBuyMenu::~DinoUiMaskBuyMenu() {
}

DinoUiMaskBuyBackButton::~DinoUiMaskBuyBackButton() {
}

DinoUiMaskBuyPageLButton::~DinoUiMaskBuyPageLButton() {
}

DinoUiMaskBuyPageRButton::~DinoUiMaskBuyPageRButton() {
}

DinoUiMaskBuyBuyButton::~DinoUiMaskBuyBuyButton() {
}

void DinoUiMaskBuy::Idle() {
}

MRect DinoUiMaskBuyMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiMaskBuyMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiMaskBuyMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

void DinoUiMaskBuyMenuItem::OnFocusChanged(BOOL focused) {
    mMotion->Play(1, 0);
}

const char *DinoUiMaskBuyMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUiMaskBuyBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

void DinoUiMaskBuyBackButton::OnDecide() {
    mMotion->Play(4, 1);
    mOwner->OnBackDecided();
    DinoSoundPlaySe(0x90, 0);
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask0<DinoUiMaskBuyMenu, DinoUiMaskBuyMenu>;
template MTask *MCreateCallTask(DinoUiMaskBuyMenu *obj, void (DinoUiMaskBuyMenu::*func)());
