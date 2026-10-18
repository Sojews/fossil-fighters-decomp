// DinoUiShopSell.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_common_window_yes_no_dialog.h"

#include "game/dino_ui3_parts_rock_name.h"

#include "game/dino_ui3_parts_cursor.h"

#include "m/input.h"

#include "game/dino_ui3_parts_kaseki_button.h"

#include "game/dino_backup.h"

#include "game/dino_ui3_parts_money.h"

#include "m/obj.h"
#include "game/dino_ui_shop_sell_menu.h"
#include "game/dino_ui_shop_sell.h"
#include "game/dino_ui_shop_sell_menu_item.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "game/dino_sound.h"
#include "m/lcd.h"
#include "m/display.h"
#include "m/bright.h"

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

void DinoUiShopSellPageRButton::OnDecide() {
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

void DinoUiShopSellPageLButton::OnDecide() {
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

void DinoUiShopSellMenu::OnBackDecided() {
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
struct DinoUiShopSellStatics {
    u8 unk00[0x8];
    MPoint mTopLeft;     // 0x08
    u8 unk10[0x48];
    MPoint mBottomRight; // 0x58
};
extern "C" DinoUiShopSellStatics data_ov005_02165ff8;

MRect DinoUiShopSellSellButton::GetFrame() {
    return MRect(data_ov005_02165ff8.mTopLeft, data_ov005_02165ff8.mBottomRight);
}

void DinoUiShopSellSellButton::OnDecide() {
    if (mOwner->Sell()) {
        DinoSoundPlaySe(0x98, 0);
        mMotion->Play(3, 1);
        MTask *play = mMotion->CreatePlayTransfer(2, 0);
        MTask *wait = mMotion->CreateWaitStopTask();
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(wait);
        serial->AddTail(play);
    }
}

void DinoUiShopSellSellButton::OnFocusChanged(BOOL focused) {
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
template class MCallTask0<DinoUiShopSellMenu, DinoUiShopSellMenu>;

BOOL DinoUiShopSellMenu::PageRight() {
    mPage = (mPage + 1) % (mLastPage + 1);
    ClearItems();
    mFraction->SetNumerator(mPage + 1);
    RefreshPage2();
    if (mFocused != mLinkFrom) {
        FocusNearestItem(mCursor->GetPosition());
    }
    return TRUE;
}

BOOL DinoUiShopSellMenu::PageLeft() {
    mPage = (mPage + mLastPage) % (mLastPage + 1);
    ClearItems();
    mFraction->SetNumerator(mPage + 1);
    RefreshPage2();
    if (mFocused != mLinkFrom) {
        FocusNearestItem(mCursor->GetPosition());
    }
    return TRUE;
}

void DinoUiShopSellMenu::ClearItems() {
    RemoveLink(mLinkFrom, 4);
    for (int i = 0; i < mItems.size(); i++) {
        RemoveItem((MMenuItem *)mItems[i]);
        delete (MMenuItem *)mItems[i];
    }
}

DinoUiShopSell::DinoUiShopSell() : MProcess(NULL) {
    gLcd->mMain->ResetHooks();
    gLcd->mMain->Setup(5, 3, 4, 1, 0, 0);
    gLcd->mMain->SetBackdrop(0x8000);
    gLcd->mMain->mObj->Show();
    gLcd->VSyncNow();
    mSerial.AddTail(new DinoUiShopSellMenu());
    mSerial.AddTail(MTaskExitTransfer::Create(this));
}

#pragma auto_inline off

template MTask *MCreateCallTask(DinoUiShopSellMenu *obj, void (DinoUiShopSellMenu::*func)());

u8 DinoUiShopSellMenu::GetFlag(int index) {
    return mFlags[index];
}

extern const u32 data_0208a1f4;

// What the fossil sells for: cleaned ones by their score (100 is full price), uncleaned ones by their own price.
u32 DinoUiShopSellMenu::GetPrice(int index) {
    DinoBackupEntry entry;
    gDinoBackup->GetUnk98(index, &entry);
    u32 price = GetKasekiDef(entry.id)->sellPrice;
    if (!entry.active) {
        if (entry.unk10_0 == 1) {
            price = data_0208a1f4;
        } else if (entry.type == 1) {
            price = GetKasekiDef(entry.id)->unk20;
        }
    } else if (entry.type == 1) {
        u32 score = entry.score;
        if (score < 100) {
            price = price * 75 / 100;
            price = (price * score / 99 + 99) / 100 * 100;
        }
    }
    return price;
}

// Chooses or unchooses the fossil.
void DinoUiShopSellMenu::SetFlag(int index, u8 flag) {
    if (flag != mFlags[index]) {
        mFlags[index] = flag;
        if (flag) {
            mCount++;
            DinoSoundPlaySe(0x9C, 0);
        } else {
            mCount--;
            DinoSoundPlaySe(0x9D, 0);
        }
    }
}

void DinoUiShopSellMenu::RefreshTotal() {
    int total = 0;
    for (int i = 0; i < 0x40; i++) {
        if (mFlags[i] == 1) {
            total += GetPrice(i);
        }
    }
    mMoney->SetChange(total);
}

// Moves the cursor to the item, and shows the fossil's name and price.
void DinoUiShopSellMenu::OnFocus(MMenuItem *item) {
    if (item == NULL) {
        mCursor->Hide();
        return;
    }
    mFocused = item;
    SetSelectedItem(dynamic_cast<DinoHelpItem *>(item));
    if (mCursor->IsHidden()) {
        mCursor->Show();
        mCursor->SetFrame(item->GetFrame());
    } else {
        mCursorTasks->Clear();
        mCursorTasks->AddTail(mCursor->CreateMoveTask(mCursor->GetFrame(), item->GetFrame(), 10, -5));
    }
    for (int i = 0; i < 8; i++) {
        if (item == mItems[i]) {
            u32 index = mPage * 8 + i;
            if (index >= mFossils.size()) {
                continue;
            }
            DinoBackupEntry entry;
            gDinoBackup->GetUnk98(index, &entry);
            mRockName->SetEntry(&entry, FALSE);
            mMoney->SetPrice(GetPrice(mPage * 8 + i));
            mSelected = mPage * 8 + i;
            return;
        }
    }
    mRockName->Clear();
    mMoney->SetPrice(0);
    mSelected = 0xFF;
}

extern const u32 data_0208a9e0, data_0208a9e4, data_0208a9e8, data_0208aa04, data_0208aa08;

// Asks whether to sell the chosen fossils.
BOOL DinoUiShopSellMenu::Sell() {
    if (mCount == 0) {
        DinoSoundPlaySe(0xA4, 0);
        return FALSE;
    }
    mCursor->Hide();
    mConfirmed = FALSE;
    DinoCommonWindowYesNoDialog *dialog =
        new DinoCommonWindowYesNoDialog(gDinoResource->GetText(data_0208a9e8), &mConfirmed);
    dialog->SetLabels(gDinoResource->GetText(data_0208aa08), gDinoResource->GetText(data_0208aa04));
    dialog->SetLabels2(gDinoResource->GetText(data_0208a9e4), gDinoResource->GetText(data_0208a9e0));
    MTask *finish = MCreateCallTask(this, &DinoUiShopSellMenu::FinishSell);
    MTask *show = mCursor->CreateShowTransfer();
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(dialog);
    serial->AddTail(show);
    serial->AddTail(finish);
    return TRUE;
}

// A fossil button: chooses or unchooses it.
void DinoUiShopSellMenu::OnDecide(MMenuItem *item) {
    for (int i = 0; i < 8; i++) {
        if (item == mItems[i]) {
            int index = mPage * 8 + i;
            if (index < mFossils.size()) {
                u8 flag = !GetFlag(index);
                SetFlag(mPage * 8 + i, flag);
                ((DinoUi3PartsKasekiButton *)mItems[i])->SetMarked(flag);
                RefreshTotal();
            }
            return;
        }
    }
}

// X jumps to the sell button; B unchooses the selected fossil, or goes to (and then presses) the back button.
void DinoUiShopSellMenu::OnIdle() {
    if (GetInput()->GetPressed() & 8) {
        FocusItem(mLinkFrom);
    }
    if (GetInput()->GetPressed() & 2) {
        if (mSelected != 0xFF && GetFlag(mSelected)) {
            SetFlag(mSelected, FALSE);
            ((DinoUi3PartsKasekiButton *)mItems[mSelected % 8])->SetMarked(FALSE);
            RefreshTotal();
            return;
        }
        if (mBack->IsFocused()) {
            mBack->Decide();
        } else {
            FocusItem(mBack);
        }
    }
}
