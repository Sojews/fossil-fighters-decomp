// DinoUiContainerBaseMenuItem.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_ui3_parts_rock_name.h"

#include "game/dino_ui3_parts_cursor.h"

#include "game/dino_backup.h"

#include "m/input.h"

#include "game/dino_ui3_parts_kaseki_button.h"

#include "game/dino_ui_container_base_menu_item_menu.h"
#include "game/dino_ui_container_base_menu_item.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "game/dino_sound.h"
#include "m/lcd.h"
#include "m/display.h"
#include "m/bright.h"

DinoUiContainerBaseMenuItem::~DinoUiContainerBaseMenuItem() {
}

DinoUiContainerBaseBackButton::~DinoUiContainerBaseBackButton() {
}

DinoUiContainerBasePageLButton::~DinoUiContainerBasePageLButton() {
}

DinoUiContainerBasePageRButton::~DinoUiContainerBasePageRButton() {
}

DinoUiContainerBaseDecideButton::~DinoUiContainerBaseDecideButton() {
}

DinoUiContainerBaseMenu::DinoUiContainerBaseMenu()
    : MMenu(NULL, 0), mPage(0), mLastPage(0), mCount(0), mUnk104(0), mSelected(0xFF), mIdled(FALSE) {
    for (int i = 0; i < 0x40; i++) {
        mFlags[i] = 0;
    }
    mItems.clear();
}

DinoUiContainerBaseMenu::~DinoUiContainerBaseMenu() {
}

MRect DinoUiContainerBaseMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiContainerBaseMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiContainerBaseMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

void DinoUiContainerBaseMenuItem::OnFocusChanged(BOOL focused) {
    mMotion->Play(1, 0);
}

const char *DinoUiContainerBaseMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUiContainerBaseBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

void DinoUiContainerBaseBackButton::OnDecide() {
    mMotion->Play(4, 1);
    mOwner->OnBackDecided();
    DinoSoundPlaySe(0x90, 0);
}

void DinoUiContainerBasePageRButton::OnDecide() {
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

void DinoUiContainerBasePageLButton::OnDecide() {
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

void DinoUiContainerBaseMenu::OnBackDecided() {
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

void DinoUiContainerBaseDecideButton::OnFocusChanged(BOOL focused) {
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

// Pooled file statics: the button corners.
struct DinoUiContainerBaseStatics {
    u8 unk00[0x50];
    MPoint mTopLeft;     // 0x50
    u8 unk58[0x20];
    MPoint mBottomRight; // 0x78
};
extern "C" DinoUiContainerBaseStatics data_ov005_0216c2e8;

MRect DinoUiContainerBaseDecideButton::GetFrame() {
    return MRect(data_ov005_0216c2e8.mTopLeft, data_ov005_0216c2e8.mBottomRight);
}

void DinoUiContainerBaseDecideButton::OnDecide() {
    if (mOwner->TryDecide()) {
        DinoSoundPlaySe(0x98, 0);
        mMotion->Play(3, 1);
        MTask *play = mMotion->CreatePlayTransfer(2, 0);
        MTask *wait = mMotion->CreateWaitStopTask();
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(wait);
        serial->AddTail(play);
    }
}

BOOL DinoUiContainerBaseMenu::PageRight() {
    mPage = (mPage + 1) % (mLastPage + 1);
    ClearItems();
    mFraction->SetNumerator(mPage + 1);
    RefreshPage2();
    FocusNearestItem(GetCursor());
    return TRUE;
}

BOOL DinoUiContainerBaseMenu::PageLeft() {
    mPage = (mPage + mLastPage) % (mLastPage + 1);
    ClearItems();
    mFraction->SetNumerator(mPage + 1);
    RefreshPage2();
    FocusNearestItem(GetCursor());
    return TRUE;
}

// NONMATCHING: the game reloads the item count for clear(); ours reuses the loop's last load.
void DinoUiContainerBaseMenu::ClearItems() {
    RemoveLink(mLinkFrom, 4);
    for (int i = 0; i < mItems.size(); i++) {
        RemoveItem((MMenuItem *)mItems[i]);
        delete (MMenuItem *)mItems[i];
    }
    mItems.clear();
}

#pragma auto_inline off

u8 DinoUiContainerBaseMenu::GetFlag(int index) {
    return mFlags[index];
}

// The help for the decide and back buttons.
void DinoUiContainerBaseMenu::AddHelpEntries(u32 backText) {
    AddEntry((u32)(DinoHelpItem *)mLinkFrom, 1, 0);
    AddEntry((u32)(DinoHelpItem *)mBack, 4, backText);
}

void DinoUiContainerBaseMenu::ResetDecideButton() {
    DinoUiContainerBaseMenuItem *decide = mLinkFrom;
    decide->mSerial.Clear();
    decide->mMotion->Play(1, 0);
}

void DinoUiContainerBaseMenu::HidePageButtons() {
    mPageL->MMenuItem::Disable();
    mPageL->mMotion->Hide();
    mPageR->MMenuItem::Disable();
    mPageR->mMotion->Hide();
}

// Entries that can't be marked now (and aren't) are greyed out.
void DinoUiContainerBaseMenu::RefreshMarks() {
    for (int i = 0; i < mItems.size(); i++) {
        DinoBackupEntry entry;
        GetEntry(mPage * 8 + i, &entry);
        if (!GetFlag(mPage * 8 + i) && (!IsMarkable(&entry) || !CanMark())) {
            ((DinoUi3PartsKasekiButton *)mItems[i])->SetActive(FALSE);
        } else {
            ((DinoUi3PartsKasekiButton *)mItems[i])->SetActive(TRUE);
        }
    }
}

void DinoUiContainerBaseMenu::SetFlag(int index, u8 flag) {
    if (flag != mFlags[index]) {
        mFlags[index] = flag;
        if (flag) {
            mCount++;
            DinoSoundPlaySe(0x9C, 0);
        } else {
            mCount--;
            DinoSoundPlaySe(0x9D, 0);
        }
        RefreshMarks();
    }
}

// X jumps to the decide button; B unmarks the selected entry, or goes to (and then presses) the back button.
void DinoUiContainerBaseMenu::OnIdle() {
    mIdled = TRUE;
    if (GetInput()->GetPressed() & 8) {
        FocusItem(mLinkFrom);
    }
    if (GetInput()->GetPressed() & 2) {
        if (mSelected != 0xFF && GetFlag(mSelected)) {
            SetFlag(mSelected, FALSE);
            ((DinoUi3PartsKasekiButton *)mItems[mSelected % 8])->SetMarked(FALSE);
            return;
        }
        if (mBack->IsFocused()) {
            mBack->Decide();
        } else {
            FocusItem(mBack);
        }
    }
}

// Moves the cursor to the item, and shows the entry's name (marked if it can't be marked now).
void DinoUiContainerBaseMenu::OnFocus(MMenuItem *item) {
    mIdled = FALSE;
    if (item == NULL) {
        mCursor->Hide();
        return;
    }
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
            if (index >= mUnk148.size()) {
                continue;
            }
            DinoBackupEntry entry;
            GetEntry(index, &entry);
            if (!GetFlag(mPage * 8 + i) && (!IsMarkable(&entry) || !CanMark())) {
                mRockName->SetEntry(&entry, TRUE);
            } else {
                mRockName->SetEntry(&entry, FALSE);
            }
            mSelected = mPage * 8 + i;
            return;
        }
    }
    mRockName->Clear();
    mSelected = 0xFF;
}

// An entry button: marks it (if it and the container allow) or unmarks it.
void DinoUiContainerBaseMenu::OnDecide(MMenuItem *item) {
    for (int i = 0; i < 8; i++) {
        if (item == mItems[i]) {
            if (!mIdled) {
                return;
            }
            u32 index = mPage * 8 + i;
            if (index >= mUnk148.size()) {
                return;
            }
            u8 flag = !GetFlag(index);
            if (flag) {
                DinoBackupEntry entry;
                GetEntry(mPage * 8 + i, &entry);
                if (!IsMarkable(&entry)) {
                    DinoSoundPlaySe(0xA4, 0);
                    return;
                }
                if (!CanMark()) {
                    DinoSoundPlaySe(0xA4, 0);
                    mSerial.AddTail(CreateUnk2C());
                    return;
                }
            }
            SetFlag(mPage * 8 + i, flag);
            ((DinoUi3PartsKasekiButton *)mItems[i])->SetMarked(flag);
            return;
        }
    }
}
