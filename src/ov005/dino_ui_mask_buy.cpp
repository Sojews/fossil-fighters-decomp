// DinoUiMaskBuy, DinoUiMaskBuyMaskButton.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_backup.h"

#include "game/dino_common_window_yes_no_dialog.h"

#include "game/dino_ui3_parts_mask_name.h"

#include "game/dino_ui3_parts_cursor.h"

#include "m/input.h"

#include "m/smart_oam.h"

#include "game/dino_resource.h"

#include "game/dino_ui3_parts_money.h"

#include "m/obj.h"
#include "game/dino_ui_mask_buy_menu.h"
#include "game/dino_ui_mask_buy.h"
#include "game/dino_ui_mask_buy_mask_button.h"
#include "game/dino_ui2_parts_kaseki_button.h"
#include "game/dino_ui_mask_buy_menu_item.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "game/dino_sound.h"
#include "m/lcd.h"
#include "m/display.h"
#include "m/bright.h"
#include "game/dino_static_init.h"

#pragma auto_inline off

DinoUiMaskBuyMenu::DinoUiMaskBuyMenu() : MMenu(NULL, 0), mPage(0), mLastPage(0), mTotal(0), mCount(0), mSelected(0xFF) {
    for (int i = 0; i < 5; i++) {
        mFlags[i] = 0;
    }
}

DinoUiMaskBuyMenuItem::DinoUiMaskBuyMenuItem(DinoUiMaskBuyMenu *owner, const std::string &path, MPoint pos, u32 textId) {
    mOwner = owner;
    mTextId = textId;
    mMotion = new MMotion(std::string(path), mOwner->GetOam(), 2, 1);
    mMotion->Play(1, 0);
    mMotion->SetPosition(pos);
    mMotion->SetPriority(1);
    mMotion->Show();
    mMotion->GetUnit()->SetMode(1);
    mParallel.AddTail(mMotion);
}

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
    u8 unk00[0x8];
    MPoint mFirst;       // 0x08: the page's first mask button
    MPoint mRow;         // 0x10: to the next row
    u8 unk18[0x28 - 0x18];
    MPoint mTopLeft;     // 0x28
    u8 unk30[0x8];
    MPoint mBottomRight; // 0x38
    u8 unk40[0x58 - 0x40];
    MPoint mColumn;      // 0x58: from a mask button to the next in its row
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

template MTask *MCreateCallTask(DinoUiMaskBuyMenu *obj, void (DinoUiMaskBuyMenu::*func)());

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

// Moves the cursor to the item, and shows the mask's name (greyed if it can't be bought now) and price.
void DinoUiMaskBuyMenu::OnFocus(MMenuItem *item) {
    if (item == NULL) {
        mCursor->Hide();
        return;
    }
    if (item == mBack || item == mBuy) {
        SetSelectedItem(dynamic_cast<DinoHelpItem *>(item));
    }
    if (mCursor->IsHidden()) {
        mCursor->Show();
        mCursor->SetFrame(item->GetFrame());
    } else {
        mCursorTasks->Clear();
        mCursorTasks->AddTail(mCursor->CreateMoveTask(mCursor->GetFrame(), item->GetFrame(), 10, -5));
    }
    for (int i = 0; i < 8; i++) {
        if (item == mUnkC4[i]) {
            u32 index = mPage * 8 + i;
            if (index >= mMasks.size()) {
                continue;
            }
            if (!GetFlag(index) && mTotal + GetPrice(mPage * 8 + i) > mFunds) {
                mMaskName->SetMask(mMasks[mPage * 8 + i], TRUE);
            } else {
                mMaskName->SetMask(mMasks[mPage * 8 + i], FALSE);
            }
            mMoney->SetPrice(GetPrice(mPage * 8 + i));
            mSelected = mPage * 8 + i;
            return;
        }
    }
    mMaskName->Clear();
    mMoney->SetPrice(0);
    mSelected = 0xFF;
}

extern const u32 data_0208a9ec, data_0208a9f0, data_0208a9f4, data_0208aa04, data_0208aa08;

// Asks whether to buy the chosen masks.
BOOL DinoUiMaskBuyMenu::Buy() {
    if (mCount == 0) {
        DinoSoundPlaySe(0xA4, 0);
        return FALSE;
    }
    mCursor->Hide();
    mConfirmed = FALSE;
    DinoCommonWindowYesNoDialog *dialog =
        new DinoCommonWindowYesNoDialog(gDinoResource->GetText(data_0208a9f4), &mConfirmed);
    dialog->SetLabels(gDinoResource->GetText(data_0208aa08), gDinoResource->GetText(data_0208aa04));
    dialog->SetLabels2(gDinoResource->GetText(data_0208a9f0), gDinoResource->GetText(data_0208a9ec));
    MTask *finish = MCreateCallTask(this, &DinoUiMaskBuyMenu::FinishBuy);
    MTask *show = mCursor->CreateShowTransfer();
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(dialog);
    serial->AddTail(show);
    serial->AddTail(finish);
    return TRUE;
}

// After the yes/no: buys the chosen masks it can pay for, and starts the page over.
void DinoUiMaskBuyMenu::FinishBuy() {
    if (mConfirmed == TRUE) {
        DinoSoundPlaySe(0x107, 0);
        for (int i = 0; i < 5; i++) {
            if (mFlags[i]) {
                mFlags[i] = FALSE;
                u32 price = GetPrice(i);
                if (price <= mFunds) {
                    mFunds -= price;
                    gDinoBackup->SetSystemVar(3, mFunds);
                    gDinoBackup->SetHeadMask(mMasks[i], TRUE);
                }
            }
        }
        mCount = 0;
        RefreshPage1();
        LoadMasks();
        RefreshPage2();
        mTotal = 0;
        mMoney->RefreshMoney();
        RefreshPage3();
        DinoUiMaskBuyBuyButton *buy = mBuy;
        buy->mSerial.Clear();
        buy->mMotion->Play(1, 0);
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
void DinoUiMaskBuyMenu::RefreshPage1() {
    RemoveLink(mBuy, 4);
    RemoveLink(mBack, 2);
    for (int i = 0; i < mUnkC4.size(); i++) {
        RemoveItem((MMenuItem *)mUnkC4[i]);
        delete (MMenuItem *)mUnkC4[i];
    }
    mUnkC4.clear();
}

// Makes the page's mask buttons (empty past the last mask) and links them in a grid, below the buy button and above
// the back button.
// NONMATCHING: the empty slot's path (the masks' table, filled by the TU's static initializer) is read through the
// TU's data pool in the game (one more literal).
void DinoUiMaskBuyMenu::RefreshPage2() {
    mUnkC4.clear();
    for (int i = 0; i < 8; i++) {
        DinoUiMaskBuyMaskButton *button;
        if (i + mPage * 8 < mMasks.size()) {
            button = new DinoUiMaskBuyMaskButton(
                mOam, MPoint(data_ov005_0216b200.mFirst.x + data_ov005_0216b200.mColumn.x * (i % 4) + data_ov005_0216b200.mRow.x * (i / 4),
                       data_ov005_0216b200.mFirst.y + data_ov005_0216b200.mColumn.y * (i % 4) + data_ov005_0216b200.mRow.y * (i / 4)),
                mMasks[i + mPage * 8], GetFlag(i + mPage * 8));
        } else {
            button = new DinoUiMaskBuyMaskButton(
                mOam, MPoint(data_ov005_0216b200.mFirst.x + data_ov005_0216b200.mColumn.x * (i % 4) + data_ov005_0216b200.mRow.x * (i / 4),
                       data_ov005_0216b200.mFirst.y + data_ov005_0216b200.mColumn.y * (i % 4) + data_ov005_0216b200.mRow.y * (i / 4)),
                0, 0);
        }
        mUnkC4.push_back(button);
        AddItem(button);
    }
    DinoUi2LinkKasekiGrid(this, (std::vector<MMenuItem *> &)mUnkC4, mBuy);
    AddLink(mBuy, 4, (MMenuItem *)mUnkC4[4]);
    AddLink((MMenuItem *)mUnkC4[0], 4, mBack);
    AddLink((MMenuItem *)mUnkC4[1], 4, mBack);
    AddLink((MMenuItem *)mUnkC4[2], 4, mBack);
    AddLink((MMenuItem *)mUnkC4[3], 4, mBack);
    AddLink(mBack, 2, (MMenuItem *)mUnkC4[0]);
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

// The file's static initialization (see dino_static_init.h).
extern "C" u32 data_ov005_0216b300[];
static struct DinoUiMaskBuyStaticInit {
    DinoUiMaskBuyStaticInit() {
        DINO_POOL_WORD(data_ov005_0216b300, 0x20) = DINO_POOL_WORD(data_ov005_0216b180, 0x1c);
        DINO_POOL_WORD(data_ov005_0216b300, 0x24) = DINO_POOL_WORD(data_ov005_0216b180, 0x38);
        DINO_POOL_WORD(data_ov005_0216b300, 0x28) = DINO_POOL_WORD(data_ov005_0216b180, 0x14);
        DINO_POOL_WORD(data_ov005_0216b300, 0x2c) = DINO_POOL_WORD(data_ov005_0216b180, 0x60);
        DINO_POOL_WORD(data_ov005_0216b300, 0x30) = DINO_POOL_WORD(data_ov005_0216b180, 0x10);
        DINO_POOL_WORD(data_ov005_0216b300, 0x34) = DINO_POOL_WORD(data_ov005_0216b180, 0x8);
        DINO_POOL_WORD(data_ov005_0216b300, 0x38) = DINO_POOL_WORD(data_ov005_0216b180, 0x4c);
        DINO_POOL_WORD(data_ov005_0216b300, 0x3c) = DINO_POOL_WORD(data_ov005_0216b180, 0x3c);
        DINO_POOL_WORD(data_ov005_0216b300, 0x40) = DINO_POOL_WORD(data_ov005_0216b180, 0x40);
        DINO_POOL_WORD(data_ov005_0216b300, 0x44) = DINO_POOL_WORD(data_ov005_0216b180, 0x20);
        DINO_POOL_WORD(data_ov005_0216b300, 0x48) = DINO_POOL_WORD(data_ov005_0216b180, 0x2c);
        DINO_POOL_WORD(data_ov005_0216b300, 0x4c) = DINO_POOL_WORD(data_ov005_0216b180, 0x18);
        DINO_POOL_WORD(data_ov005_0216b300, 0x50) = DINO_POOL_WORD(data_ov005_0216b180, 0x48);
        DINO_POOL_WORD(data_ov005_0216b300, 0x54) = DINO_POOL_WORD(data_ov005_0216b180, 0x24);
        DINO_POOL_WORD(data_ov005_0216b300, 0x58) = DINO_POOL_WORD(data_ov005_0216b180, 0x54);
        DINO_POOL_WORD(data_ov005_0216b300, 0x5c) = DINO_POOL_WORD(data_ov005_0216b180, 0x64);
        DINO_POOL_WORD(data_ov005_0216b300, 0x60) = DINO_POOL_WORD(data_ov005_0216b180, 0x34);
        DINO_POOL_WORD(data_ov005_0216b300, 0x64) = DINO_POOL_WORD(data_ov005_0216b180, 0xc);
        DINO_POOL_WORD(data_ov005_0216b300, 0x68) = DINO_POOL_WORD(data_ov005_0216b180, 0x5c);
        DINO_POOL_WORD(data_ov005_0216b300, 0x6c) = DINO_POOL_WORD(data_ov005_0216b180, 0x28);
        DINO_POOL_WORD(data_ov005_0216b200, 0x48) = 128;
        DINO_POOL_WORD(data_ov005_0216b200, 0x4c) = 96;
        DINO_POOL_WORD(data_ov005_0216b180, 0x78) = 128;
        DINO_POOL_WORD(data_ov005_0216b180, 0x7c) = 96;
        DINO_POOL_WORD(data_ov005_0216b200, 0x0) = 128;
        DINO_POOL_WORD(data_ov005_0216b200, 0x4) = 96;
        DINO_POOL_WORD(data_ov005_0216b200, 0x8) = 53;
        DINO_POOL_WORD(data_ov005_0216b200, 0xc) = 35;
        DINO_POOL_WORD(data_ov005_0216b200, 0x58) = 50;
        DINO_POOL_WORD(data_ov005_0216b200, 0x5c) = 0;
        DINO_POOL_WORD(data_ov005_0216b200, 0x10) = 0;
        DINO_POOL_WORD(data_ov005_0216b200, 0x14) = 50;
        DINO_POOL_WORD(data_ov005_0216b200, 0x20) = -32;
        DINO_POOL_WORD(data_ov005_0216b200, 0x24) = 4;
        DINO_POOL_WORD(data_ov005_0216b200, 0x18) = 241;
        DINO_POOL_WORD(data_ov005_0216b200, 0x1c) = 128;
        DINO_POOL_WORD(data_ov005_0216b180, 0x68) = 128;
        DINO_POOL_WORD(data_ov005_0216b180, 0x6c) = 96;
        DINO_POOL_WORD(data_ov005_0216b200, 0x50) = 24;
        DINO_POOL_WORD(data_ov005_0216b200, 0x54) = 144;
        DINO_POOL_WORD(data_ov005_0216b200, 0x28) = 54;
        DINO_POOL_WORD(data_ov005_0216b200, 0x2c) = 115;
        DINO_POOL_WORD(data_ov005_0216b200, 0x38) = 202;
        DINO_POOL_WORD(data_ov005_0216b200, 0x3c) = 139;
    }
} sDinoUiMaskBuyStaticInit;
