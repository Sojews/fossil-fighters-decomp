// DinoUiShopBuy.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "m/obj.h"
#include "game/dino_ui_shop_buy_menu.h"
#include "game/dino_ui_shop_buy.h"
#include "game/dino_ui_shop_buy_menu_item.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "game/dino_sound.h"
#include "m/lcd.h"
#include "m/display.h"
#include "m/bright.h"

#pragma auto_inline off

BOOL DinoUiShopBuyMenu::PageLeft() {
    mPage = (mPage + mLastPage) % (mLastPage + 1);
    RefreshPage1();
    RefreshPage2();
    RefreshPage3();
    return TRUE;
}

#pragma auto_inline on

#pragma auto_inline off

BOOL DinoUiShopBuyMenu::PageRight() {
    mPage = (mPage + 1) % (mLastPage + 1);
    RefreshPage1();
    RefreshPage2();
    RefreshPage3();
    return TRUE;
}

#pragma auto_inline on

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

// Pooled file statics: the button corners.
struct DinoUiShopBuyStatics {
    u8 unk00[0x14];
    MPoint mBottomRight; // 0x14
    u8 unk1C[0x18];
    MPoint mTopLeft;     // 0x34
};
extern "C" DinoUiShopBuyStatics data_ov005_02166ac8;

MRect DinoUiShopBuyBuyButton::GetFrame() {
    return MRect(data_ov005_02166ac8.mTopLeft, data_ov005_02166ac8.mBottomRight);
}

void DinoUiShopBuyBuyButton::OnDecide() {
    if (mOwner->Buy()) {
        DinoSoundPlaySe(0x98, 0);
        mMotion->Play(3, 1);
        MTask *play = mMotion->CreatePlayTransfer(2, 0);
        MTask *wait = mMotion->CreateWaitStopTask();
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(wait);
        serial->AddTail(play);
    }
}

void DinoUiShopBuyBuyButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        if (mOwner->mUnk11C) {
            mMotion->Play(2, 0);
        } else {
            mMotion->Play(1, 0);
        }
    } else {
        mMotion->Play(1, 0);
        mSerial.Clear();
    }
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask0<DinoUiShopBuyMenu, DinoUiShopBuyMenu>;
template MTask *MCreateCallTask(DinoUiShopBuyMenu *obj, void (DinoUiShopBuyMenu::*func)());

DinoUiShopBuy::DinoUiShopBuy() : MProcess(NULL) {
    gLcd->mMain->ResetHooks();
    gLcd->mMain->Setup(5, 3, 4, 1, 0, 0);
    gLcd->mMain->SetBackdrop(0x8000);
    gLcd->mMain->mObj->Show();
    gLcd->SetBottomDisplay(gLcd->mMain);
    gCurrentDisplay = gLcd->GetBottomDisplay();
    gLcd->VSyncNow();
    mSerial.AddTail(new DinoUiShopBuyMenu());
    mSerial.AddTail(MTaskExitTransfer::Create(this));
}
