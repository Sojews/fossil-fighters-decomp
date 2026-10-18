// DinoUiMuseum.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_ui_museum_information_menu.h"
#include "types.h"
#include "game/dino_ui_museum.h"
#include "game/dino_ui_field_menu_fraction.h"
#include "m/lcd.h"
#include "m/bright.h"
#include "m/display.h"
#include "game/dino_sound.h"
#include "game/dino_ui_museum_3d_model.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"

DinoUiMuseumInformationMenu::~DinoUiMuseumInformationMenu() {
}

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

DinoUiMuseum::DinoUiMuseum() : MProcess(gLcd->mSub) {
}

DinoUiMuseumReviverButton::~DinoUiMuseumReviverButton() {
}

MRect DinoUiMuseumReviverButton::GetFrame() {
    return mMotion->GetFrame();
}

extern "C" void func_ov006_0218935c(void *owner);

void DinoUiMuseumBackButton::OnDecide() {
    mMotion->Play(4, 1);
    func_ov006_0218935c(mOwner);
    DinoSoundPlaySe(0x90, 0);
}

void DinoUiMuseumReviverButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(mBaseAnime + 3, 0);
    } else {
        mMotion->Play(mBaseAnime + 1, 0);
    }
    mSerial.Clear();
}

extern "C" void func_ov006_0218a538(DinoUiMuseumInformationMenu *menu);

void DinoUiMuseumInformationVoiceButton::OnDecide() {
    mMotion->Play(2, 1);
    MTask *play = mMotion->CreatePlayTransfer(1, 0);
    MTask *wait = mMotion->CreateWaitStopTask();
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(wait);
    serial->AddTail(play);
    func_ov006_0218a538(mOwner);
}

extern "C" void func_ov006_02189660(void *owner);
extern "C" void func_ov006_02189530(void *owner);

void DinoUiMuseumPageLButton::OnDecide() {
    func_ov006_02189660(mOwner);
    mMotion->Play(2, 1);
    mSerial.Clear();
    MTask *play = mMotion->CreatePlayTransfer(1, 0);
    MTask *wait = mMotion->CreateWaitStopTask();
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(wait);
    serial->AddTail(play);
    DinoSoundPlaySe(0x93, 0);
}

void DinoUiMuseumPageRButton::OnDecide() {
    func_ov006_02189530(mOwner);
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

void DinoUiMuseumMenu::ClearSlots() {
    for (int i = 0; i < 15; i++) {
        RemoveItem(mSlots[i]);
        delete mSlots[i];
    }
    RemoveLink(mLinkFrom, 2);
}

void DinoUiMuseumMenu::FinishPageRight() {
    ClearSlots();
    mFraction->SetNumerator(mPage + 1);
    CreateSlots();
    Enable();
    FocusNearestItem(mCursor->GetPosition());
}

void DinoUiMuseumMenu::FinishPageLeft() {
    ClearSlots();
    mFraction->SetNumerator(mPage + 1);
    CreateSlots();
    Enable();
    FocusNearestItem(mCursor->GetPosition());
}

void DinoUiMuseumMenu::CreateSlots() {
    for (int i = 0; i < 15; i++) {
        CreateSlot(i);
    }
}

DinoUiMuseumSlotCreateTask::DinoUiMuseumSlotCreateTask(DinoUiMuseumMenu *menu) : MProcess(NULL) {
    mMenu = menu;
    mIndex = 0;
}

#pragma auto_inline on

void DinoUiMuseumSlotCreateTask::Idle() {
    if (mIndex <= 14) {
        mMenu->CreateSlot(mIndex);
        if (mIndex < mMenu->mSlotCount - 1) {
            mSerial.AddTail(new MTimer(data_02089a0c));
        }
    } else {
        Exit();
    }
    mIndex++;
}

extern "C" void func_ov006_0218a570(DinoUiMuseumInformationMenu *menu);

void DinoUiMuseumInformationMenu::OnBackDecided() {
    MTask *fade;
    MTask *exit;
    MTask &self = *this;
    exit = MTaskExitTransfer::Create(&self);
    fade = gLcd->mSub->mBright->CreateFadeOutToBlack(5, 0);
    MTask *timer = new MTimer(30);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(timer);
    serial->AddTail(fade);
    serial->AddTail(exit);
}

void DinoUiMuseumInformationMenu::PageRight() {
    *mPage = (*mPage + 1) % mInfo->mPageCount;
    func_ov006_0218a570(this);
}

void DinoUiMuseumInformationMenu::PageLeft() {
    *mPage = (*mPage + mInfo->mPageCount - 1) % mInfo->mPageCount;
    func_ov006_0218a570(this);
}

void DinoUiMuseumMenu::OnIdle() {
    mUnk1AC = TRUE;
}

// DinoUiMuseum3DModel::Idle and DinoUiMuseum's page turns (not decompiled yet) start background loads.
template MTask *MCreateBgCallTask(DinoUiMuseum3DModel *obj, void (DinoUiMuseum3DModel::*func)(), int thread);
template MTask *MCreateBgCallTask(DinoUiMuseum *obj, void (DinoUiMuseum::*func)(), int thread);
// DinoUiMuseumMenu::OnFocus (not decompiled yet) re-enables the help through a call task.
template MTask *MCreateCallTask(DinoUiMuseumMenu *obj, void (DinoHelpBase::*func)());
template MTask *MCreateCallTask(DinoUiMuseum *obj, void (DinoUiMuseum::*func)());
