// DinoUiShopSell.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_ui_shop_sell_menu.h"
#include "game/dino_ui_shop_sell.h"
#include "game/dino_ui_shop_sell_menu_item.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "game/dino_sound.h"

DinoUiShopSell::~DinoUiShopSell() {
}

DinoUiShopSellMenuItem::~DinoUiShopSellMenuItem() {
}

DinoUiShopSellMenu::~DinoUiShopSellMenu() {
}

DinoUiShopSellBackButton::~DinoUiShopSellBackButton() {
}

DinoUiShopSellPageLButton::~DinoUiShopSellPageLButton() {
}

DinoUiShopSellPageRButton::~DinoUiShopSellPageRButton() {
}

DinoUiShopSellSellButton::~DinoUiShopSellSellButton() {
}

void DinoUiShopSell::Idle() {
}

MRect DinoUiShopSellMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiShopSellMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiShopSellMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

void DinoUiShopSellMenuItem::OnFocusChanged(BOOL focused) {
    mMotion->Play(1, 0);
}

const char *DinoUiShopSellMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUiShopSellBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

void DinoUiShopSellBackButton::OnDecide() {
    mMotion->Play(4, 1);
    mOwner->OnBackDecided();
    DinoSoundPlaySe(0x90, 0);
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask0<DinoUiShopSellMenu, DinoUiShopSellMenu>;
template MTask *MCreateCallTask(DinoUiShopSellMenu *obj, void (DinoUiShopSellMenu::*func)());
