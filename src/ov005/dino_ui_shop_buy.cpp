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
#include "game/dino_ui3_parts_money.h"
#include "game/dino_ui3_parts_kaseki_button.h"
#include "game/dino_backup.h"
#include "m/input.h"

#pragma auto_inline off

// Clears the page's items.
void DinoUiShopBuyMenu::ClearItems() {
    RemoveLink(mLinkFrom, 4);
    for (int i = 0; i < mItems.size(); i++) {
        RemoveItem((MMenuItem *)mItems[i]);
        delete (MMenuItem *)mItems[i];
    }
}

BOOL DinoUiShopBuyMenu::PageLeft() {
    mPage = (mPage + mLastPage) % (mLastPage + 1);
    ClearItems();
    RefreshPage2();
    RefreshPage3();
    return TRUE;
}

#pragma auto_inline on

#pragma auto_inline off

BOOL DinoUiShopBuyMenu::PageRight() {
    mPage = (mPage + 1) % (mLastPage + 1);
    ClearItems();
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
        if (mOwner->mCount) {
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

#pragma auto_inline off

u8 DinoUiShopBuyMenu::GetFlag(int index) {
    return mFlags[index];
}

u32 DinoUiShopBuyMenu::GetPrice(int index) {
    DinoBackupEntry entry;
    gDinoBackup->GetUnk148(index, &entry);
    u32 id = entry.id;
    const DinoKasekiDefs *defs = (const DinoKasekiDefs *)mFile.GetData();
    return ((const DinoKasekiDef *)((const u8 *)defs + ((const u32 *)((const u8 *)defs + defs->tableOffset))[id]))->price;
}

// Chooses or unchooses the item, keeping the total.
void DinoUiShopBuyMenu::SetFlag(int index, u8 flag) {
    if (flag != mFlags[index]) {
        mFlags[index] = flag;
        if (flag) {
            mTotal += GetPrice(index);
            mCount++;
            DinoSoundPlaySe(0x9C, 0);
        } else {
            mTotal -= GetPrice(index);
            mCount--;
            DinoSoundPlaySe(0x9D, 0);
        }
    }
}

// The items whose price would go over the money (or that there's no room for) can't be chosen.
void DinoUiShopBuyMenu::RefreshPage3() {
    mMoney->SetChange(mTotal);
    for (int i = 0; i < mItems.size(); i++) {
        if (!GetFlag(mPage * 8 + i) &&
            (mTotal + GetPrice(mPage * 8 + i) > mFunds || mCount >= gDinoBackup->CountFreeUnk98())) {
            ((DinoUi3PartsKasekiButton *)mItems[i])->SetActive(FALSE);
        } else {
            ((DinoUi3PartsKasekiButton *)mItems[i])->SetActive(TRUE);
        }
    }
}

// X jumps to the buy button; B unchooses the selected item, or goes to (and then presses) the back button.
void DinoUiShopBuyMenu::OnIdle() {
    if (GetInput()->GetPressed() & 8) {
        FocusItem(mLinkFrom);
    }
    if (GetInput()->GetPressed() & 2) {
        if (mSelected != 0xFF && GetFlag(mSelected)) {
            SetFlag(mSelected, FALSE);
            ((DinoUi3PartsKasekiButton *)mItems[mSelected % 8])->SetMarked(FALSE);
            RefreshPage3();
            return;
        }
        if (mUnkC8->IsFocused()) {
            mUnkC8->Decide();
        } else {
            FocusItem(mUnkC8);
        }
    }
}
