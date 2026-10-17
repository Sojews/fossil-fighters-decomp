// DinoUiCleaningMenuItem.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

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
void DinoUiCleaningMenu::OnDecide(MMenuItem *item) {
    for (int i = 0; i < 8; i++) {
        if (item == mUnkC0[i]) {
            int index = i + mPage * 8;
            if (index < mUnkE8.size()) {
                if (IsDecidable()) {
                    Decide(i);
                } else {
                    DinoSoundPlaySe(0xA4, 0);
                }
            }
            return;
        }
    }
}
