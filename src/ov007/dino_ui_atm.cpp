// DinoUiATM, DinoUiATMTeamButton.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_ui_atm_menu.h"
#include "game/dino_ui_atm.h"
#include "game/dino_ui_atm_team_button.h"
#include "game/dino_ui_atm_back_button.h"
#include "game/dino_backup.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"

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
