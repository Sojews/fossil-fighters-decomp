// DinoUiContainerDropMenuPageIncTask.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_ui_container_drop_menu_page_inc_task_menu.h"
#include "game/dino_ui_container_drop_menu_page_inc_task.h"
#include "game/dino_ui_container_drop_menu_item.h"
#include "game/dino_sound.h"
#include "m/lcd.h"
#include "m/display.h"
#include "m/bright.h"

#pragma auto_inline off

BOOL DinoUiContainerDropMenu::PageLeft() {
    mPage = (mPage + mLastPage) % (mLastPage + 1);
    ShowPage();
    return TRUE;
}

BOOL DinoUiContainerDropMenu::PageRight() {
    mPage = (mPage + 1) % (mLastPage + 1);
    ShowPage();
    return TRUE;
}

#pragma auto_inline on

DinoUiContainerDropMenuPageIncTask::~DinoUiContainerDropMenuPageIncTask() {
}

DinoUiContainerDropMenuItem::~DinoUiContainerDropMenuItem() {
}

DinoUiContainerDropMenu::~DinoUiContainerDropMenu() {
}

DinoUiContainerDropBackButton::~DinoUiContainerDropBackButton() {
}

DinoUiContainerDropPageLButton::~DinoUiContainerDropPageLButton() {
}

DinoUiContainerDropPageRButton::~DinoUiContainerDropPageRButton() {
}

MRect DinoUiContainerDropMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiContainerDropMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiContainerDropMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

void DinoUiContainerDropMenuItem::OnFocusChanged(BOOL focused) {
    mMotion->Play(1, 0);
}

const char *DinoUiContainerDropMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUiContainerDropBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

void DinoUiContainerDropBackButton::OnDecide() {
    mMotion->Play(4, 1);
    mOwner->OnBackDecided();
    DinoSoundPlaySe(0x90, 0);
}

void DinoUiContainerDropPageRButton::OnDecide() {
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

void DinoUiContainerDropPageLButton::OnDecide() {
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

void DinoUiContainerDropMenuPageIncTask::Execute() {
    mOwner->PageRight();
    Exit();
}


void DinoUiContainerDropMenu::OnIdle() {
    mStarted = TRUE;
    if (mUnk128 == 1) {
        MTask &self = *this;
        MTask *exit = MTaskExitTransfer::Create(&self);
        MTask *fade = gLcd->mMain->mBright->CreateFadeOutToBlack(30, 0);
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(fade);
        serial->AddTail(exit);
    }
    if (mUnk128) {
        mUnk128 = 0;
    }
}
