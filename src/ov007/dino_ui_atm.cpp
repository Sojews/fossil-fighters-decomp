// DinoUiATM, DinoUiATMTeamButton.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_sound.h"
#include "game/dino_ui_atm_menu.h"
#include "game/dino_ui_atm.h"
#include "game/dino_ui_atm_team_button.h"
#include "game/dino_ui_atm_back_button.h"
#include "game/dino_backup.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"

DinoUiATMMenu::DinoUiATMMenu(u32 *result, u8 arg1, u32 arg2, u8 arg3) : MMenu(NULL, 0) {
    mUnk108 = arg2;
    mResult = result;
    mUnk114 = 10;
    mUnk118 = 0;
    mUnk11C = 0;
    mUnk120 = -2;
    mUnk124 = arg1;
    mUnk125 = arg3;
}

DinoUiATM::~DinoUiATM() {
}

DinoUiATMTeamButton::~DinoUiATMTeamButton() {
}

DinoUiATMBackButton::~DinoUiATMBackButton() {
}

DinoUiATMMenu::~DinoUiATMMenu() {
}

void DinoUiATMMenu::OnIdle() {
}

void DinoUiATMTeamButton::OnDecide() {
}

MRect DinoUiATMBackButton::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiATMBackButton::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiATMTeamButton::GetFrame() {
    return mMotion->GetFrame();
}

MRect DinoUiATMBackButton::GetFrame() {
    return mMotion->GetFrame();
}

const char *DinoUiATMBackButton::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoUiATM::Idle() {
    (this->*mState)();
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MBgCallTask0<DinoBackup>;
template MTask *MCreateBgCallTask(DinoBackup *obj, void (DinoBackup::*func)(), int thread);
template class MCallTask0<DinoUiATMMenu, DinoUiATMMenu>;
template MTask *MCreateCallTask(DinoUiATMMenu *obj, void (DinoUiATMMenu::*func)());

void DinoUiATMBackButton::OnFocusChanged(BOOL focused) {
    mSerial.Clear();
    if (focused) {
        mMotion->Play(3, 0);
        mMotion2->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
        mMotion2->Play(2, 0);
    }
}

void DinoUiATMTeamButton::OnFocusChanged(BOOL focused) {
    mSerial.Clear();
    if (focused) {
        MTask *show = mMotion->CreateShowTransfer();
        MTask *timer = new MTimer(10);
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(timer);
        serial->AddTail(show);
    } else {
        mMotion->Hide();
    }
}

void DinoUiATMBackButton::OnDecide() {
    mMotion->Play(4, 1);
    mMotion2->Play(4, 1);
    MTask *play2 = mMotion2->CreatePlayTransfer(3, 0);
    MTask *play = mMotion->CreatePlayTransfer(3, 0);
    MTask *wait = mMotion->CreateWaitStopTask();
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(wait);
    serial->AddTail(play);
    serial->AddTail(play2);
    mOwner->OnBackDecided();
    DinoSoundPlaySe(0x90, 0);
}
