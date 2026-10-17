// DinoUiShopBuy.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_ui_shop_buy_menu.h"
#include "game/dino_ui_shop_buy.h"
#include "game/dino_ui_shop_buy_menu_item.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "game/dino_sound.h"
#include "m/lcd.h"
#include "m/display.h"
#include "m/bright.h"

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

void DinoUiShopBuyBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

void DinoUiShopBuyBackButton::OnDecide() {
    mMotion->Play(4, 1);
    mOwner->OnBackDecided();
    DinoSoundPlaySe(0x90, 0);
}

void DinoUiShopBuyPageRButton::OnDecide() {
    if (mOwner->PageRight()) {
        mMotion->Play(2, 1);
        mSerial.Clear();
        MTask *play = mMotion->CreatePlayTransfer(1, 0);
        MTask *wait = mMotion->CreateWaitStopTask();
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(wait);
        serial->AddTail(play);
        DinoSoundPlaySe(0x93, 0);
    }
}

void DinoUiShopBuyPageLButton::OnDecide() {
    if (mOwner->PageLeft()) {
        mMotion->Play(2, 1);
        mSerial.Clear();
        MTask *play = mMotion->CreatePlayTransfer(1, 0);
        MTask *wait = mMotion->CreateWaitStopTask();
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(wait);
        serial->AddTail(play);
        DinoSoundPlaySe(0x93, 0);
    }
}

void DinoUiShopBuyMenu::OnBackDecided() {
    MTask *fade;
    MTask *exit;
    MTask &self = *this;
    exit = MTaskExitTransfer::Create(&self);
    fade = gLcd->mMain->mBright->CreateFadeOutToBlack(30, 0);
    MTask *timer = new MTimer(30);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(timer);
    serial->AddTail(fade);
    serial->AddTail(exit);
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask0<DinoUiShopBuyMenu, DinoUiShopBuyMenu>;
template MTask *MCreateCallTask(DinoUiShopBuyMenu *obj, void (DinoUiShopBuyMenu::*func)());
