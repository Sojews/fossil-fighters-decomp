// DinoUiMedalMenu.
// Only the destructor is decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_ui_medal_menu.h"
#include "m/display.h"
#include "m/blend.h"
#include "game/dino_sound.h"

DinoUiMedalMenu::~DinoUiMedalMenu() {
    gCurrentDisplay->mBlend->Reset();
}

DinoUiSortMenuItem::~DinoUiSortMenuItem() {
}

DinoUiMedalList::~DinoUiMedalList() {
}

DinoUiMedalMenuItem::~DinoUiMedalMenuItem() {
}

DinoUiMedalBackButton::~DinoUiMedalBackButton() {
}

DinoUiMedalPageLButton::~DinoUiMedalPageLButton() {
}

DinoUiMedalPageRButton::~DinoUiMedalPageRButton() {
}

DinoUiMedalSlotCreateTask::~DinoUiMedalSlotCreateTask() {
}

void DinoUiSortMenuItem::OnSelectChanged(BOOL value) {
}

void DinoUiSortMenuItem::OnDecide() {
}

void DinoUiSortMenuItem::OnFocusChanged(BOOL value) {
}

MRect DinoUiMedalMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiMedalMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MDisplay *DinoUiSortMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiMedalMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

void DinoUiMedalBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

const char *DinoUiMedalMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

const char *DinoUiSortMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUiMedalList::Idle() {
    (this->*mState)();
}

void DinoUiMedalBackButton::OnDecide() {
    mMotion->Play(4, 1);
    mOwner->OnBackDecided(FALSE);
    DinoSoundPlaySe(0x90, 0);
}

void DinoUiMedalPageRButton::OnDecide() {
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

void DinoUiMedalPageLButton::OnDecide() {
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

void DinoUiMedalMenu::OnIdle() {
    mUnk204 = TRUE;
}

// NONMATCHING: the game loads mPos.x once for both corners
MRect DinoUiSortMenuItem::GetRect() {
    return MRect(mPos.Offset(0, -2), mPos.Offset(0x28, 0x10));
}
