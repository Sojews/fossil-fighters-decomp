// DinoUiItemItemButton.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_ui_item_item_button_menu.h"
#include "game/dino_ui_item_item_button.h"
#include "game/dino_ui_item_menu_item.h"
#include "game/dino_sound.h"
#include "m/lcd.h"
#include "m/display.h"
#include "m/bright.h"

DinoUiItemItemButton::~DinoUiItemItemButton() {
}

DinoUiItemMenuItem::~DinoUiItemMenuItem() {
}

DinoUiItemMenu::~DinoUiItemMenu() {
}

DinoUiItemBackButton::~DinoUiItemBackButton() {
}

DinoUiItemOKButton::~DinoUiItemOKButton() {
}

DinoUiItemPageLButton::~DinoUiItemPageLButton() {
}

DinoUiItemPageRButton::~DinoUiItemPageRButton() {
}

MRect DinoUiItemMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiItemMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiItemMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

void DinoUiItemMenuItem::OnFocusChanged(BOOL focused) {
    mMotion->Play(1, 0);
}

const char *DinoUiItemMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUiItemOKButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

void DinoUiItemBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

void DinoUiItemBackButton::OnDecide() {
    mMotion->Play(4, 1);
    mOwner->OnBackDecided();
    DinoSoundPlaySe(0x90, 0);
}

void DinoUiItemPageRButton::OnDecide() {
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

void DinoUiItemPageLButton::OnDecide() {
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

void DinoUiItemOKButton::OnDecide() {
    mMotion->Play(4, 1);
    mOwner->OnOKDecided();
    DinoSoundPlaySe(0x9A, 0);
}
