// DinoUiCleaningMenuItem.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_ui_cleaning_leave.h"
#include "game/dino_ui3_parts_rock_name.h"

#include "game/dino_ui3_parts_cursor.h"

#include "game/dino_backup.h"
#include "game/dino_ui3_parts_kaseki_button.h"
#include "game/dino_ui2_parts_kaseki_button.h"

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

// (Not auto-inlined: RefreshPage2 calls it.)
#pragma auto_inline off
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
#pragma auto_inline reset

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

// The page's button grid (its first button, and the steps to the next in a row and to the next row); the confirmation
// window opens by the decided button.
extern "C" struct {
    u8 unk00[0x4C];
    MPoint mWindowBase;     // 0x4C: the first button
    MPoint mWindowColStep;  // 0x54
    MPoint mWindowRowStep;  // 0x5C
    MPoint mWindowOffset;   // 0x64
} data_ov005_02167184;

void DinoUiCleaningMenu::ClearItems() {
    RemoveLink(mLinkFrom, 2);
    for (int i = 0; i < mItems.size(); i++) {
        RemoveItem((MMenuItem *)mItems[i]);
        delete (MMenuItem *)mItems[i];
    }
}

// Makes the page's buttons (blank past the last rock; greyed out if it can't be cleaned) and links them in a grid.
void DinoUiCleaningMenu::RefreshPage2() {
    mItems.clear();
    for (int i = 0; i < 8; i++) {
        DinoUi3PartsKasekiButton *button;
        if (mUnkE8.size() > i + mPage * 8) {
            button = new DinoUi3PartsKasekiButton(
                mOam,
                MPoint(data_ov005_02167184.mWindowBase.x + data_ov005_02167184.mWindowColStep.x * (i % 4) + data_ov005_02167184.mWindowRowStep.x * (i / 4),
                       data_ov005_02167184.mWindowBase.y + data_ov005_02167184.mWindowColStep.y * (i % 4) + data_ov005_02167184.mWindowRowStep.y * (i / 4)),
                (const DinoUiKasekiIcon &)mUnkE8[i + mPage * 8], 0);
            button->SetActive(IsDecidable(i + mPage * 8));
        } else {
            button = new DinoUi3PartsKasekiButton(
                mOam,
                MPoint(data_ov005_02167184.mWindowBase.x + data_ov005_02167184.mWindowColStep.x * (i % 4) + data_ov005_02167184.mWindowRowStep.x * (i / 4),
                       data_ov005_02167184.mWindowBase.y + data_ov005_02167184.mWindowColStep.y * (i % 4) + data_ov005_02167184.mWindowRowStep.y * (i / 4)),
                (const DinoUiKasekiIcon &)DinoUi2GetEmptyKasekiInfo(), 0);
            button->HideMotion();
        }
        mItems.push_back(button);
        AddItem(button);
    }
    DinoUi2LinkKasekiGrid(this, (std::vector<MMenuItem *> &)mItems, mLinkFrom);
    AddLink((MMenuItem *)mItems[0], 4, mLinkFrom);
    AddLink((MMenuItem *)mItems[1], 4, mLinkFrom);
    AddLink((MMenuItem *)mItems[2], 4, mLinkFrom);
    AddLink((MMenuItem *)mItems[3], 4, mLinkFrom);
    AddLink(mLinkFrom, 2, (MMenuItem *)mItems[0]);
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
    void (DinoUiCleaningTest::*Leave_14)();
    void (DinoUiCleaningTest::*Finish_1C)();
} data_ov005_02167204;

DinoUiCleaningTest::DinoUiCleaningTest() : MProcess(NULL) {
    mState = data_ov005_02167204.Setup_0C;
}

// Shows the lower screen, and gives the player sixteen fossils and ten kaseki rocks to clean.
void DinoUiCleaningTest::Setup() {
    gLcd->mSub->ResetHooks();
    gLcd->mSub->Setup(5, 2, 4, 1, 0, 0);
    gLcd->VSyncNow();
    gLcd->mSub->SetBackdrop(0x8000);
    gLcd->mSub->mObj->Show();
    gLcd->SetBottomDisplay(gLcd->mSub);
    gCurrentDisplay = gLcd->GetBottomDisplay();
    gDinoBackup->SetSystemVar(2, 3);
    DinoBackupEntry entry;
    MI_CpuFill8(&entry, 0, sizeof(entry));
    for (int i = 0; i < 16; i++) {
        entry.id = i + 1;
        gDinoBackup->SetUnk98(i, &entry);
    }
    u32 rock;
    for (int i = 0; i < 10; i++) {
        gDinoBackup->GetKasekiRock(i + 1, &rock);
        rock |= 0x400;
        gDinoBackup->SetKasekiRock(i + 1, &rock);
    }
    mState = data_ov005_02167204.Leave_14;
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
