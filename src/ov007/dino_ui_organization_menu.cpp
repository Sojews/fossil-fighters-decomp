// DinoUiOrganizationMenu (team organization).
// Only the destructor is decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_sound.h"
#include "game/dino_ui_organization_menu.h"
#include "m/display.h"
#include "m/blend.h"

DinoUiOrganizationMenu::~DinoUiOrganizationMenu() {
    gCurrentDisplay->mBlend->Reset();
}

DinoUiOrganizationMenuItem::~DinoUiOrganizationMenuItem() {
}

DinoUiOrganizationOKButton::~DinoUiOrganizationOKButton() {
}

DinoUiOrganizationBackButton::~DinoUiOrganizationBackButton() {
}

DinoUiOrganizationPageLButton::~DinoUiOrganizationPageLButton() {
}

DinoUiOrganizationPageRButton::~DinoUiOrganizationPageRButton() {
}

DinoUiOrganizationSlotCreateTask::~DinoUiOrganizationSlotCreateTask() {
}

DinoUiOrganizationMenuBlendResetTask::~DinoUiOrganizationMenuBlendResetTask() {
}

MRect DinoUiOrganizationMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiOrganizationMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiOrganizationMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

const char *DinoUiOrganizationMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUiOrganizationOKButton::OnFocusChanged(BOOL focused) {
    mSerial.Clear();
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

void DinoUiOrganizationBackButton::OnFocusChanged(BOOL focused) {
    mSerial.Clear();
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

void DinoUiOrganizationMenuBlendResetTask::Execute() {
    MBlend *blend = gCurrentDisplay->mBlend;
    blend->Reset();
    blend->SetMode(3);
    blend->SetBrightness(8);
    blend->SetAlpha(16, 0);
    blend->SetTarget1(0x10);
    blend->SetTarget2(0x3F);
    Exit();
}

extern "C" void func_ov007_0217dc6c(DinoUiOrganizationMenu *menu);
extern "C" void func_ov007_0217dcb0(DinoUiOrganizationMenu *menu);

void DinoUiOrganizationMenu::OnIdle() {
    mUnk23E = TRUE;
    if (GetInput()->GetPressed() & 0x800) {
        func_ov007_0217dc6c(this);
        return;
    }
    if (GetInput()->GetPressed() & 0x400) {
        func_ov007_0217dcb0(this);
    }
}

extern "C" void func_ov007_0217d548(void *owner);
extern "C" void func_ov007_0217cf88(void *owner);
extern "C" void func_ov007_0217d9d8(void *owner);
extern "C" void func_ov007_0217d908(void *owner);

void DinoUiOrganizationOKButton::OnDecide() {
    mMotion->Play(4, 1);
    MTask *play = mMotion->CreatePlayTransfer(3, 0);
    MTask *wait = mMotion->CreateWaitStopTask();
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(wait);
    serial->AddTail(play);
    func_ov007_0217d548(mOwner);
    DinoSoundPlaySe(0x9A, 0);
}

void DinoUiOrganizationBackButton::OnDecide() {
    mMotion->Play(4, 1);
    MTask *play = mMotion->CreatePlayTransfer(3, 0);
    MTask *wait = mMotion->CreateWaitStopTask();
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(wait);
    serial->AddTail(play);
    func_ov007_0217cf88(mOwner);
    DinoSoundPlaySe(0x90, 0);
}

void DinoUiOrganizationPageLButton::OnDecide() {
    func_ov007_0217d9d8(mOwner);
    mMotion->Play(2, 1);
    mSerial.Clear();
    MTask *play = mMotion->CreatePlayTransfer(1, 0);
    MTask *wait = mMotion->CreateWaitStopTask();
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(wait);
    serial->AddTail(play);
    DinoSoundPlaySe(0x93, 0);
}

void DinoUiOrganizationPageRButton::OnDecide() {
    func_ov007_0217d908(mOwner);
    mMotion->Play(2, 1);
    mSerial.Clear();
    MTask *play = mMotion->CreatePlayTransfer(1, 0);
    MTask *wait = mMotion->CreateWaitStopTask();
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(wait);
    serial->AddTail(play);
    DinoSoundPlaySe(0x93, 0);
}
