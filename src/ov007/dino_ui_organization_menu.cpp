// DinoUiOrganizationMenu (team organization).
// Only the destructor is decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_sound.h"
#include "game/dino_ui_organization_menu.h"
#include "game/dino_ui_field_menu_fraction.h"
#include "m/display.h"
#include "m/blend.h"
#include "m/call_task.h"
#include "m/bg_new_task.h"

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


void DinoUiOrganizationMenu::OnIdle() {
    mUnk23E = TRUE;
    if (GetInput()->GetPressed() & 0x800) {
        TabLeft();
        return;
    }
    if (GetInput()->GetPressed() & 0x400) {
        TabRight();
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

extern "C" const int data_02089a0c;

#pragma auto_inline off

void DinoUiOrganizationMenu::ClearSlots() {
    RemoveSlotLinks();
    for (int i = 0; i < 15; i++) {
        RemoveItem(mSlots[i]);
        delete mSlots[i];
    }
}

void DinoUiOrganizationMenu::FinishPageRight() {
    ClearSlots();
    mFraction->SetNumerator(mPage + 1);
    CreateSlots();
    Enable();
    FocusNearestItem(GetCursor());
}

void DinoUiOrganizationMenu::FinishPageLeft() {
    ClearSlots();
    mFraction->SetNumerator(mPage + 1);
    CreateSlots();
    Enable();
    FocusNearestItem(GetCursor());
}

void DinoUiOrganizationMenu::TabLeft() {
    SetTab((mTab + 3) % 4);
    if (mFocusTabs) {
        FocusItem(mTabs[mTab]);
    }
}

void DinoUiOrganizationMenu::TabRight() {
    SetTab((mTab + 1) % 4);
    if (mFocusTabs) {
        FocusItem(mTabs[mTab]);
    }
}

BOOL DinoUiOrganizationMenu::HasUnk222(u8 id) {
    for (int i = 0; i < 5; i++) {
        if (id == mUnk222[i]) {
            return TRUE;
        }
    }
    return FALSE;
}

void DinoUiOrganizationMenu::CreateSlots() {
    for (int i = 0; i < 15; i++) {
        CreateSlot(i);
    }
}

DinoUiOrganizationSlotCreateTask::DinoUiOrganizationSlotCreateTask(DinoUiOrganizationMenu *menu) : MProcess(NULL) {
    mMenu = menu;
    mIndex = 0;
}

#pragma auto_inline on

void DinoUiOrganizationSlotCreateTask::Idle() {
    if (mIndex < 15) {
        mMenu->CreateSlot(mIndex);
        if (mIndex < mMenu->mSlotCount - 1) {
            mSerial.AddTail(new MTimer(data_02089a0c));
        }
    } else {
        Exit();
    }
    mIndex++;
}

// DinoUiOrganizationMenu::OnFocus (not decompiled yet) re-enables the help through a call task.
template MTask *MCreateCallTask(DinoUiOrganizationMenu *obj, void (DinoHelpBase::*func)());
template MTask *MCreateCallTask(DinoUiOrganizationMenu *obj, void (DinoUiOrganizationMenu::*func)());
template MTask *MCreateBgCallTask(DinoUiOrganizationMenu *obj, void (DinoUiOrganizationMenu::*func)(), int thread);
template MTask *MCreateCallTask(DinoUiOrganizationMenu *obj, void (DinoUiOrganizationMenu::*func)(bool), bool a1);
