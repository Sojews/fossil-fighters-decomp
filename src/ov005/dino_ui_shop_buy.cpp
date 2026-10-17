// DinoUiShopBuy.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_ui_shop_buy_menu.h"
#include "game/dino_ui_shop_buy.h"
#include "game/dino_ui_shop_buy_menu_item.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"

DinoUiShopBuy::~DinoUiShopBuy() {
}

DinoUiShopBuyMenuItem::~DinoUiShopBuyMenuItem() {
}

DinoUiShopBuyMenu::~DinoUiShopBuyMenu() {
}

DinoUiShopBuyBackButton::~DinoUiShopBuyBackButton() {
}

DinoUiShopBuyPageLButton::~DinoUiShopBuyPageLButton() {
}

DinoUiShopBuyPageRButton::~DinoUiShopBuyPageRButton() {
}

DinoUiShopBuyBuyButton::~DinoUiShopBuyBuyButton() {
}

void DinoUiShopBuy::Idle() {
}

MRect DinoUiShopBuyMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiShopBuyMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiShopBuyMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

void DinoUiShopBuyMenuItem::OnFocusChanged(BOOL focused) {
    mMotion->Play(1, 0);
}

const char *DinoUiShopBuyMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask0<DinoUiShopBuyMenu, DinoUiShopBuyMenu>;
template MTask *MCreateCallTask(DinoUiShopBuyMenu *obj, void (DinoUiShopBuyMenu::*func)());
