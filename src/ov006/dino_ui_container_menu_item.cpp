// DinoUiContainerMenuItem.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_backup.h"
#include "game/dino_resource.h"
#include "game/dino_common_window_message_box.h"
#include "game/dino_ui_container_menu_item_menu.h"
#include "game/dino_ui_container_menu_item.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "game/dino_sound.h"
#include "m/lcd.h"
#include "m/display.h"
#include "m/bright.h"

DinoUiContainerMenuItem::~DinoUiContainerMenuItem() {
}

DinoUiContainerMenu::~DinoUiContainerMenu() {
}

DinoUiContainerBackButton::~DinoUiContainerBackButton() {
}

DinoUiContainerOKButton::~DinoUiContainerOKButton() {
}

DinoUiContainerPageLButton::~DinoUiContainerPageLButton() {
}

DinoUiContainerPageRButton::~DinoUiContainerPageRButton() {
}

void DinoUiContainerMenu::OnIdle() {
}

MRect DinoUiContainerMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiContainerMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiContainerMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

void DinoUiContainerMenuItem::OnFocusChanged(BOOL focused) {
    mMotion->Play(1, 0);
}

const char *DinoUiContainerMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUiContainerOKButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

void DinoUiContainerBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

void DinoUiContainerBackButton::OnDecide() {
    mMotion->Play(4, 1);
    mOwner->OnBackDecided();
    DinoSoundPlaySe(0x90, 0);
}

void DinoUiContainerPageRButton::OnDecide() {
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

void DinoUiContainerPageLButton::OnDecide() {
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

void DinoUiContainerOKButton::OnDecide() {
    mMotion->Play(4, 1);
    mOwner->OnOKDecided();
    DinoSoundPlaySe(0x9A, 0);
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask0<DinoUiContainerMenu, DinoUiContainerMenu>;
template MTask *MCreateCallTask(DinoUiContainerMenu *obj, void (DinoUiContainerMenu::*func)());

BOOL DinoUiContainerMenu::PageRight() {
    mPage = (mPage + 1) % (mLastPage + 1);
    ClearItems();
    mFraction->SetNumerator(mPage + 1);
    RefreshPage2();
    FocusNearestItem(mCursor->GetPosition());
    return TRUE;
}

BOOL DinoUiContainerMenu::PageLeft() {
    mPage = (mPage + mLastPage) % (mLastPage + 1);
    ClearItems();
    mFraction->SetNumerator(mPage + 1);
    RefreshPage2();
    FocusNearestItem(mCursor->GetPosition());
    return TRUE;
}

void DinoUiContainerMenu::ClearItems() {
    for (int i = 0; i < mItems.size(); i++) {
        RemoveItem((MMenuItem *)mItems[i]);
        delete (MMenuItem *)mItems[i];
    }
}

// NONMATCHING: register allocation only.
void DinoUiContainerMenu::OnDecide(MMenuItem *item) {
    for (int i = 0; i < 3; i++) {
        if (item == mTabItems[i]) {
            SelectTab(i);
        }
    }
    for (int i = 0; i < (mPage != mLastPage ? 8 : mUnk100.size() - mPage * 8); i++) {
        if (item == mItems[i]) {
            Decide();
            return;
        }
    }
}

extern const int data_02089a20; // fade frames
extern const int data_02089a88;

void DinoUiContainerMenu::SelectTab(int tab) {
    if (tab < 0 || tab > 2) {
        return;
    }
    switch (tab) {
    case 0: {
        *mResult = 1;
        MTask &self = *this;
        MTask *exit = MTaskExitTransfer::Create(&self);
        MTask *fade = gLcd->mMain->mBright->CreateFadeOutToBlack(data_02089a20, data_02089a88);
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(fade);
        serial->AddTail(exit);
        DinoSoundPlaySe(0x94, 0);
        break;
    }
    case 1:
        break;
    case 2: {
        *mResult = 3;
        MTask &self = *this;
        MTask *exit = MTaskExitTransfer::Create(&self);
        MTask *fade = gLcd->mMain->mBright->CreateFadeOutToBlack(data_02089a20, data_02089a88);
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(fade);
        serial->AddTail(exit);
        DinoSoundPlaySe(0x94, 0);
        break;
    }
    }
}

extern const int data_02089a34;
extern const u32 data_0208aa3c; // "the team was changed" text
extern const int data_02089a30;
extern const int data_02089a2c;

void DinoUiContainerMenu::OnBackDecided() {
    *mResult = 9;
    mSerial.AddTail(new MTimer(data_02089a34));
    if (DinoTeamDiffers(mOldTeam, mTeam)) {
        mSerial.AddTail(new DinoCommonWindowMessageBox(MPoint(128, 96), gDinoResource->GetText(data_0208aa3c), 4));
    }
    MTask &self = *this;
    MTask *exit = MTaskExitTransfer::Create(&self);
    MTask *fade = gLcd->mMain->mBright->CreateFadeOutToBlack(data_02089a30, data_02089a2c);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(fade);
    serial->AddTail(exit);
}

extern const u32 data_0208aae0; // "the team will be saved" text

// NONMATCHING: the else branch swaps the exit/fade registers.
void DinoUiContainerMenu::OnOKDecided() {
    MTask *fade;
    MTask *exit;
    *mResult = 0;
    if (mOldTeam->slots[0]) {
        if (DinoTeamDiffers(mOldTeam, mTeam)) {
            mSerial.AddTail(new DinoCommonWindowMessageBox(MPoint(128, 96), gDinoResource->GetText(data_0208aae0), 4));
        } else {
            mSerial.AddTail(new MTimer(30));
        }
        MTask &self = *this;
        exit = MTaskExitTransfer::Create(&self);
        fade = gLcd->mMain->mBright->CreateFadeOutToBlack(30, 0);
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(fade);
        serial->AddTail(exit);
    } else {
        MTask &self = *this;
        exit = MTaskExitTransfer::Create(&self);
        fade = gLcd->mMain->mBright->CreateFadeOutToBlack(30, 0);
        MTask *timer = new MTimer(30);
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(timer);
        serial->AddTail(fade);
        serial->AddTail(exit);
    }
}

// Where the cursor sits on each tab.
extern MPoint data_020b5858[3];
extern "C" void func_ov003_02159a8c(void *panel, DinoBackupEntry *entry, int arg);
extern "C" void func_ov003_02159b28(void *panel);

// NONMATCHING: register allocation only (i and mPage swap in the last loop, as in OnDecide).
void DinoUiContainerMenu::OnFocus(MMenuItem *item) {
    if (item == NULL) {
        mCursor->Hide();
        return;
    }
    SetSelectedItem(dynamic_cast<DinoHelpItem *>(item));
    MRect frame = item->GetFrame();
    mCursor->SetMode(1);
    for (int i = 0; i < 3; i++) {
        if (item == mTabItems[i]) {
            frame.MoveTo(data_020b5858[i]);
            mCursor->SetMode(2);
        }
    }
    for (int i = 0; i < mItems.size(); i++) {
        if (item == mItems[i]) {
            frame.top += 16;
            frame.bottom += 16;
        }
    }
    if (mCursor->IsHidden()) {
        mCursor->SetFrame(frame);
    } else {
        mCursorTasks->Clear();
        mCursorTasks->AddTail(mCursor->CreateMoveTask(mCursor->GetFrame(), frame, 10, -5));
    }
    mCursor->Show();
    for (int i = 0; i < (mPage != mLastPage ? 8 : mUnk100.size() - mPage * 8); i++) {
        if (item == mItems[i]) {
            DinoBackupEntry entry;
            gDinoBackup->GetUnk98(mPage * 8 + i, &entry);
            func_ov003_02159a8c(mPanel, &entry, 0);
            return;
        }
    }
    func_ov003_02159b28(mPanel);
}
