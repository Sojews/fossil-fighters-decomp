// DinoUiMuseum.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_ui_museum.h"
#include "types.h"
#include "game/dino_sound.h"
#include "game/dino_ui_museum_information_menu.h"

DinoUiMuseum::~DinoUiMuseum() {
}

DinoUiMuseumEffectLight::~DinoUiMuseumEffectLight() {
}

DinoUiMuseumMenuItem::~DinoUiMuseumMenuItem() {
}

DinoUiMuseumBackButton::~DinoUiMuseumBackButton() {
}

DinoUiMuseumPageLButton::~DinoUiMuseumPageLButton() {
}

DinoUiMuseumPageRButton::~DinoUiMuseumPageRButton() {
}

DinoUiMuseumSlotCreateTask::~DinoUiMuseumSlotCreateTask() {
}

DinoUiMuseumInformationMenuItem::~DinoUiMuseumInformationMenuItem() {
}

DinoUiMuseumInformationBackButton::~DinoUiMuseumInformationBackButton() {
}

DinoUiMuseumInformationPageLButton::~DinoUiMuseumInformationPageLButton() {
}

DinoUiMuseumInformationPageRButton::~DinoUiMuseumInformationPageRButton() {
}

DinoUiMuseumInformationVoiceButton::~DinoUiMuseumInformationVoiceButton() {
}

void DinoUiMuseumInformationMenu::OnIdle() {
}

void DinoUiMuseumInformationMenuItem::OnSelectChanged(BOOL value) {
}

void DinoUiMuseumInformationMenuItem::OnFocusChanged(BOOL value) {
}

void DinoUiMuseumMenuItem::OnFocusChanged(BOOL value) {
}

MRect DinoUiMuseumInformationMenuItem::GetRect() {
    return GetFrame();
}

MRect DinoUiMuseumMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiMuseumInformationMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MDisplay *DinoUiMuseumMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiMuseumInformationMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

MRect DinoUiMuseumMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

void DinoUiMuseumInformationBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

void DinoUiMuseumBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

const char *DinoUiMuseumInformationMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

const char *DinoUiMuseumMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUiMuseumInformationBackButton::OnDecide() {
    mMotion->Play(4, 1);
    mOwner->OnBackDecided();
    DinoSoundPlaySe(0x90, 0);
}

void DinoUiMuseumInformationPageRButton::OnDecide() {
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

void DinoUiMuseumInformationPageLButton::OnDecide() {
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
