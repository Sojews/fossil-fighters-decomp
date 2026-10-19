// DinoUiCleaningMenuItem.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_ui_cleaning_leave.h"
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
#include "game/dino_common_window_menu_item_flush_task_menu.h"

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

DinoUiCleaningMenu::DinoUiCleaningMenu(s32 *result) : MMenu(NULL, 0), mResult(result) {
    mChoice = 0;
    mAnswer = 0;
    mPage = 0;
    mLastPage = 0;
}

#pragma auto_inline off

DinoUiCleaningMenuItem::DinoUiCleaningMenuItem(DinoUiCleaningMenu *owner, const std::string &path, MPoint pos, u32 textId) {
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

void DinoUiCleaningTest::Finish() {
    mSerial.AddTail(MTaskExitTransfer::Create(this));
}

// The test's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0xC];
    void (DinoUiCleaningTest::*Setup_0C)();
    void (DinoUiCleaningTest::*Unk_14)();
    void (DinoUiCleaningTest::*Finish_1C)();
} data_ov005_02167204;

DinoUiCleaningTest::DinoUiCleaningTest() : MProcess(NULL) {
    mState = data_ov005_02167204.Setup_0C;
}

void DinoUiCleaningTest::Leave() {
    mSerial.AddTail(new DinoUiCleaningLeave(&mUnk7C));
    mState = data_ov005_02167204.Finish_1C;
}

// Back: no rock; fades both screens out and leaves.
void DinoUiCleaningMenu::OnBackDecided() {
    *mResult = -1;
    MTask *fadeMain;
    MTask *fadeSub;
    MTask *exit;
    exit = CreateExitTransfer();
    fadeSub = gLcd->mSub->mBright->CreateFadeOutToBlack(30, 0);
    fadeMain = gLcd->mMain->mBright->CreateFadeOutToBlack(30, 0);
    MParallelTaskSet *fades = new MParallelTaskSet(TRUE);
    fades->AddTail(fadeMain);
    fades->AddTail(fadeSub);
    MTask *wait = new MTimer(30);
    MTaskSet &serial = mSerial;
    serial.AddTail(wait);
    serial.AddTail(fades);
    serial.AddTail(exit);
}

// Where the confirmation window opens: by the decided item's place in the 4-wide grid.
extern "C" struct {
    u8 unk00[0x4C];
    MPoint mWindowBase;     // 0x4C
    MPoint mWindowColStep;  // 0x54
    MPoint mWindowRowStep;  // 0x5C
    MPoint mWindowOffset;   // 0x64
} data_ov005_02167184;
extern "C" struct {
    void (DinoUiCleaningMenu::*FinishDecide_00)();
} data_ov005_02167200;
extern const u32 data_0208aa24, data_0208aa28, data_0208a638, data_0208a63c;

// Asks to confirm the rock.
// NONMATCHING: the game works the window's place out after allocating it, the column twice
void DinoUiCleaningMenu::Decide(int index) {
    mCursor->Hide();
    mChoice = index + mPage * 8;
    mAnswer = 0;
    int row = index / 4;
    int col = index % 4;
    MPoint pos(data_ov005_02167184.mWindowBase.x + (col - (col == 3)) * data_ov005_02167184.mWindowColStep.x +
                   row * data_ov005_02167184.mWindowRowStep.x + data_ov005_02167184.mWindowOffset.x,
               data_ov005_02167184.mWindowBase.y + (col - (col == 3)) * data_ov005_02167184.mWindowColStep.y +
                   row * data_ov005_02167184.mWindowRowStep.y + data_ov005_02167184.mWindowOffset.y);
    DinoCommonWindowMenu *window = new DinoCommonWindowMenu(gLcd->GetBottomDisplay(), &mAnswer, pos, NULL, 0);
    window->AddChoice(1, gDinoResource->GetText(data_0208aa28), gDinoResource->GetText(data_0208a63c));
    window->AddChoice(-2, gDinoResource->GetText(data_0208aa24), gDinoResource->GetText(data_0208a638));
    MTask *show = mCursor->CreateShowTransfer();
    MTask *finish = MCreateCallTask(this, data_ov005_02167200.FinishDecide_00);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(window);
    serial->AddTail(finish);
    serial->AddTail(show);
}

// Yes: saves the page and fades both screens out to white, then leaves.
void DinoUiCleaningMenu::FinishDecide() {
    if (mAnswer == 1) {
        gDinoBackup->SetSystemVar(0x2D, mPage);
        *mResult = mChoice;
        MTask *fadeMain;
        MTask *fadeSub;
        MTask *exit;
        exit = CreateExitTransfer();
        fadeSub = gLcd->mSub->mBright->CreateFadeOutToWhite(30, 0);
        fadeMain = gLcd->mMain->mBright->CreateFadeOutToWhite(30, 0);
        MParallelTaskSet *fades = new MParallelTaskSet(TRUE);
        fades->AddTail(fadeMain);
        fades->AddTail(fadeSub);
        MTask *stopBgm = DinoSoundCreateStopBgmTask(-1, 30);
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(stopBgm);
        serial->AddTail(fades);
        serial->AddTail(exit);
    }
}
