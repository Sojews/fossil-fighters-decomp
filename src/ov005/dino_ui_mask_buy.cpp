// DinoUiMaskBuy, DinoUiMaskBuyMaskButton.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "m/input.h"

#include "m/smart_oam.h"

#include "game/dino_resource.h"

#include "game/dino_ui3_parts_money.h"

#include "m/obj.h"
#include "game/dino_ui_mask_buy_menu.h"
#include "game/dino_ui_mask_buy.h"
#include "game/dino_ui_mask_buy_mask_button.h"
#include "game/dino_ui_mask_buy_menu_item.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "game/dino_sound.h"
#include "m/lcd.h"
#include "m/display.h"
#include "m/bright.h"

#pragma auto_inline off

BOOL DinoUiMaskBuyMenu::PageRight() {
    mPage = (mPage + 1) % (mLastPage + 1);
    RefreshPage1();
    RefreshPage2();
    RefreshPage3();
    return TRUE;
}

#pragma auto_inline on

#pragma auto_inline off

BOOL DinoUiMaskBuyMenu::PageLeft() {
    mPage = (mPage + mLastPage) % (mLastPage + 1);
    RefreshPage1();
    RefreshPage2();
    RefreshPage3();
    return TRUE;
}

#pragma auto_inline on

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

void DinoUiMaskBuyPageRButton::OnDecide() {
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

void DinoUiMaskBuyPageLButton::OnDecide() {
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

void DinoUiMaskBuyMenu::OnBackDecided() {
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
struct DinoUiMaskBuyStatics {
    u8 unk00[0x28];
    MPoint mTopLeft;     // 0x28
    u8 unk30[0x8];
    MPoint mBottomRight; // 0x38
};
extern "C" DinoUiMaskBuyStatics data_ov005_0216b200;

MRect DinoUiMaskBuyBuyButton::GetFrame() {
    return MRect(data_ov005_0216b200.mTopLeft, data_ov005_0216b200.mBottomRight);
}

void DinoUiMaskBuyBuyButton::OnDecide() {
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

void DinoUiMaskBuyBuyButton::OnFocusChanged(BOOL focused) {
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

void DinoUiMaskBuyMaskButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mView->Show();
    } else {
        mView->Hide();
    }
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask0<DinoUiMaskBuyMenu, DinoUiMaskBuyMenu>;
template MTask *MCreateCallTask(DinoUiMaskBuyMenu *obj, void (DinoUiMaskBuyMenu::*func)());

MRect DinoUiMaskBuyMaskButton::GetFrame() {
    return MRect(mPos.Offset(-24, -24), mPos.Offset(24, 24));
}

DinoUiMaskBuy::DinoUiMaskBuy() : MProcess(NULL) {
    gLcd->mMain->ResetHooks();
    gLcd->mMain->Setup(5, 3, 4, 1, 0, 0);
    gLcd->mMain->SetBackdrop(0x8000);
    gLcd->mMain->mObj->Show();
    gLcd->VSyncNow();
    mSerial.AddTail(new DinoUiMaskBuyMenu());
    mSerial.AddTail(MTaskExitTransfer::Create(this));
}

#pragma auto_inline off

u8 DinoUiMaskBuyMenu::GetFlag(int index) {
    return mFlags[index];
}

u32 DinoUiMaskBuyMenu::GetPrice(int index) {
    MResource<DinoHeadMaskDefs> defs = gDinoResource->LoadHeadMaskDefs();
    u32 id = mMasks[index];
    const DinoHeadMaskDefs *data = defs.Get();
    return ((const DinoHeadMaskDef *)((const u8 *)data + ((const u32 *)((const u8 *)data + data->tableOffset))[id]))
        ->price;
}

// Chooses or unchooses the mask, keeping the total.
void DinoUiMaskBuyMenu::SetFlag(int index, u8 flag) {
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

// The masks whose price would go over the money are greyed out.
void DinoUiMaskBuyMenu::RefreshPage3() {
    mMoney->SetChange(mTotal);
    for (int i = 0; i < 8; i++) {
        int index = mPage * 8 + i;
        if (index < mMasks.size()) {
            if (!GetFlag(index) && mTotal + GetPrice(mPage * 8 + i) > mFunds) {
                ((DinoUiMaskBuyMaskButton *)mUnkC4[i])->mMotion->GetUnit()->SetMode(0);
            } else {
                ((DinoUiMaskBuyMaskButton *)mUnkC4[i])->mMotion->GetUnit()->SetMode(1);
            }
        }
    }
}

// A mask button: chooses it (if the money allows) or unchooses it.
void DinoUiMaskBuyMenu::OnDecide(MMenuItem *item) {
    for (int i = 0; i < 8; i++) {
        if (item == mUnkC4[i]) {
            int index = mPage * 8 + i;
            if (index >= mMasks.size()) {
                continue;
            }
            u8 flag = !GetFlag(index);
            if (flag && mTotal + GetPrice(mPage * 8 + i) > mFunds) {
                DinoSoundPlaySe(0xA4, 0);
                return;
            }
            SetFlag(mPage * 8 + i, flag);
            DinoUiMaskBuyMaskButton *button = (DinoUiMaskBuyMaskButton *)mUnkC4[i];
            if (flag) {
                button->mMark->Show();
            } else {
                button->mMark->Hide();
            }
            RefreshPage3();
            return;
        }
    }
}

// Removes the page's mask buttons.
// NONMATCHING: the game reloads the size for clear() (as in the containers' ClearItems).
void DinoUiMaskBuyMenu::RefreshPage1() {
    RemoveLink(mBuy, 4);
    RemoveLink(mBack, 2);
    for (int i = 0; i < mUnkC4.size(); i++) {
        RemoveItem((MMenuItem *)mUnkC4[i]);
        delete (MMenuItem *)mUnkC4[i];
    }
    mUnkC4.clear();
}

// X jumps to the buy button; B unchooses the selected mask, or goes to (and then presses) the back button.
void DinoUiMaskBuyMenu::OnIdle() {
    if (GetInput()->GetPressed() & 8) {
        FocusItem(mBuy);
    }
    if (GetInput()->GetPressed() & 2) {
        if (mSelected != 0xFF && GetFlag(mSelected)) {
            SetFlag(mSelected, FALSE);
            ((DinoUiMaskBuyMaskButton *)mUnkC4[mSelected % 8])->mMark->Hide();
            RefreshPage3();
            return;
        }
        if (mBack->IsFocused()) {
            mBack->Decide();
        } else {
            FocusItem(mBack);
        }
    }
}
