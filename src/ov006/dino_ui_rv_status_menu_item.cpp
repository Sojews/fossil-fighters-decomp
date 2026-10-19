// DinoUiRvStatusMenuItem.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_ui_rv_status_menu_item_menu.h"
#include "game/dino_ui_rv_status_menu_item.h"
#include "game/help.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "game/dino_sound.h"
#include "m/lcd.h"
#include "m/display.h"
#include "m/bright.h"

#pragma auto_inline off

DinoUiRvStatusMenuItem::DinoUiRvStatusMenuItem(DinoUiRvStatusMenu *owner, const std::string &path, MPoint pos, u32 textId) {
    mOwner = owner;
    mTextId = textId;
    mMotion = new MMotion(std::string(path), mOwner->GetOam(), 2, 1);
    mMotion->Play(1, 0);
    mMotion->SetPosition(pos);
    mMotion->Show();
    mParallel.AddTail(mMotion);
}

void DinoUiRvStatusMenu::PageLeft() {
    mPage = (mPage + mLastPage) % (mLastPage + 1);
    ShowPage();
}

void DinoUiRvStatusMenu::PageRight() {
    mPage = (mPage + 1) % (mLastPage + 1);
    ShowPage();
}

#pragma auto_inline on

DinoUiRvStatusMenuItem::~DinoUiRvStatusMenuItem() {
}

DinoUiRvStatusMenu::~DinoUiRvStatusMenu() {
}

DinoUiRvStatusBackButton::~DinoUiRvStatusBackButton() {
}

DinoUiRvStatusPageLButton::~DinoUiRvStatusPageLButton() {
}

DinoUiRvStatusPageRButton::~DinoUiRvStatusPageRButton() {
}

MRect DinoUiRvStatusMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiRvStatusMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiRvStatusMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

const char *DinoUiRvStatusMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUiRvStatusMenuItem::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(2, 0);
    } else {
        mMotion->Play(1, 0);
    }
}

void DinoUiRvStatusBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

void DinoUiRvStatusBackButton::OnDecide() {
    mMotion->Play(4, 1);
    mOwner->OnBackDecided();
    DinoSoundPlaySe(0x90, 0);
}

void DinoUiRvStatusPageRButton::OnDecide() {
    mOwner->PageRight();
    mMotion->Play(2, 1);
    mSerial.Clear();
    MTask *play = mMotion->CreatePlayTransfer(1, 0);
    MTask *wait = mMotion->CreateWaitStopTask();
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(wait);
    serial->AddTail(play);
    DinoSoundPlaySe(0x93, 0);
}

void DinoUiRvStatusPageLButton::OnDecide() {
    mOwner->PageLeft();
    mMotion->Play(2, 1);
    mSerial.Clear();
    MTask *play = mMotion->CreatePlayTransfer(1, 0);
    MTask *wait = mMotion->CreateWaitStopTask();
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(wait);
    serial->AddTail(play);
    DinoSoundPlaySe(0x93, 0);
}

void DinoUiRvStatusMenu::OnDecide(MMenuItem *item) {
    for (int i = 0; i < 4; i++) {
        if (item == mTabItems[i]) {
            SelectTab(i);
        }
    }
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask0<DinoUiRvStatusMenu, DinoHelpBase>;
template MTask *MCreateCallTask(DinoUiRvStatusMenu *obj, void (DinoHelpBase::*func)());
template class MCallTask1<DinoUiRvStatusMenu, DinoUiRvStatusMenu, u8>;
template MTask *MCreateCallTask(DinoUiRvStatusMenu *obj, void (DinoUiRvStatusMenu::*func)(u8), u8 a0);

// Where the cursor sits on each tab.
extern MPoint data_020b5890[4];

void DinoUiRvStatusMenu::OnFocus(MMenuItem *item) {
    if (item == NULL) {
        mCursor->Hide();
        return;
    }
    SetSelectedItem(dynamic_cast<DinoHelpItem *>(item));
    MRect frame = item->GetFrame();
    mCursor->SetMode(1);
    for (int i = 0; i < 4; i++) {
        if (item == mTabItems[i]) {
            frame.MoveTo(data_020b5890[i]);
            mCursor->SetMode(2);
        }
    }
    if (mCursor->IsHidden()) {
        mCursor->SetFrame(frame);
    } else {
        mCursorTasks->Clear();
        mCursorTasks->AddTail(mCursor->CreateMoveTask(mCursor->GetFrame(), frame, 10, -5));
    }
    mCursor->Show();
}
