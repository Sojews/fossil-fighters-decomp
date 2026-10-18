// DinoUiCleaningMenuItem.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_ui3_parts_rock_name.h"

#include "game/dino_ui3_parts_cursor.h"

#include "game/dino_backup.h"

#include "game/dino_ui_cleaning_menu_item_menu.h"
#include "game/dino_ui_cleaning_menu_item.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "game/dino_sound.h"
#include "m/lcd.h"
#include "m/display.h"
#include "m/bright.h"

DinoUiCleaningMenuItem::~DinoUiCleaningMenuItem() {
}

DinoUiCleaningMenu::~DinoUiCleaningMenu() {
}

DinoUiCleaningBackButton::~DinoUiCleaningBackButton() {
}

DinoUiCleaningPageLButton::~DinoUiCleaningPageLButton() {
}

DinoUiCleaningPageRButton::~DinoUiCleaningPageRButton() {
}

void DinoUiCleaningMenu::OnIdle() {
}

MRect DinoUiCleaningMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiCleaningMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiCleaningMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

void DinoUiCleaningMenuItem::OnFocusChanged(BOOL focused) {
    mMotion->Play(1, 0);
}

const char *DinoUiCleaningMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUiCleaningBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

void DinoUiCleaningBackButton::OnDecide() {
    mMotion->Play(4, 1);
    mOwner->OnBackDecided();
    DinoSoundPlaySe(0x90, 0);
}

void DinoUiCleaningPageRButton::OnDecide() {
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

void DinoUiCleaningPageLButton::OnDecide() {
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

DinoUiCleaningTest::~DinoUiCleaningTest() {
}

void DinoUiCleaningTest::Idle() {
    (this->*mState)();
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask0<DinoUiCleaningMenu, DinoUiCleaningMenu>;
template MTask *MCreateCallTask(DinoUiCleaningMenu *obj, void (DinoUiCleaningMenu::*func)());

// NONMATCHING: the game computes the index before loading the item count; ours loads the count first.
// Moves the cursor to the item, and shows the fossil's name (marked if it can't be cleaned).
void DinoUiCleaningMenu::OnFocus(MMenuItem *item) {
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
            if (index >= mUnkE8.size()) {
                continue;
            }
            DinoBackupEntry entry;
            gDinoBackup->GetUnk98(index, &entry);
            mRockName->SetEntry(&entry, !IsDecidable(mPage * 8 + i));
            return;
        }
    }
    mRockName->Clear();
}

void DinoUiCleaningMenu::OnDecide(MMenuItem *item) {
    for (int i = 0; i < 8; i++) {
        if (item == mItems[i]) {
            u32 index = mPage * 8 + i;
            if (index >= mUnkE8.size()) {
                return;
            }
            if (IsDecidable(index)) {
                Decide(i);
            } else {
                DinoSoundPlaySe(0xA4, 0);
            }
            return;
        }
    }
}

// Fossils already cleaned, or that can't be, can't be chosen.
BOOL DinoUiCleaningMenu::IsDecidable(u32 index) {
    DinoBackupEntry entry;
    gDinoBackup->GetUnk98(index, &entry);
    if (entry.active) {
        return FALSE;
    }
    if (GetKasekiDef(entry.id)->unk04 == 0) {
        return TRUE;
    }
    return FALSE;
}

BOOL DinoUiCleaningMenu::PageRight() {
    if (mPage == mLastPage) {
        mPage = 0;
    } else {
        mPage++;
    }
    ClearItems();
    mFraction->SetNumerator(mPage + 1);
    RefreshPage2();
    FocusNearestItem(GetCursor());
    return TRUE;
}

BOOL DinoUiCleaningMenu::PageLeft() {
    if (mPage == 0) {
        mPage = mLastPage;
    } else {
        mPage--;
    }
    ClearItems();
    mFraction->SetNumerator(mPage + 1);
    RefreshPage2();
    FocusNearestItem(GetCursor());
    return TRUE;
}

void DinoUiCleaningMenu::ClearItems() {
    RemoveLink(mLinkFrom, 2);
    for (int i = 0; i < mItems.size(); i++) {
        RemoveItem((MMenuItem *)mItems[i]);
        delete (MMenuItem *)mItems[i];
    }
}

DinoUiCleaningMenu::DinoUiCleaningMenu(DinoEventLoop *event) : MMenu(NULL, 0), mEvent(event) {
    mUnk104[0] = 0;
    mUnk104[1] = 0;
    mPage = 0;
    mLastPage = 0;
}

#pragma auto_inline off

void DinoUiCleaningTest::Finish() {
    mSerial.AddTail(MTaskExitTransfer::Create(this));
}
