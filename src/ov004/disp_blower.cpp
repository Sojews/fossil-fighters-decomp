// DispBlower, OptionKeyCheckTask.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/disp_blower_menu.h"
#include "game/disp_blower.h"
#include "game/option_key_check_task.h"
#include "game/dino_ui_option_menu_item.h"
#include "game/help.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"

DispBlower::~DispBlower() {
}

OptionKeyCheckTask::~OptionKeyCheckTask() {
}

DinoUiOptionMenuItem::~DinoUiOptionMenuItem() {
}

DinoUiOptionSpeedMenuItem::~DinoUiOptionSpeedMenuItem() {
}

BGMVolumeButton::~BGMVolumeButton() {
}

SEVolumeButton::~SEVolumeButton() {
}

MicTestButton::~MicTestButton() {
}

DinoUiOptionBackButton::~DinoUiOptionBackButton() {
}

DinoUiOptionOkButton::~DinoUiOptionOkButton() {
}

DinoUiOptionPower::~DinoUiOptionPower() {
    MSleepHook::Unregister();
}

DinoUiOptionMenu::~DinoUiOptionMenu() {
}

void DinoUiOptionMenu::OnIdle() {
}

void DinoUiOptionPower::OnWake() {
}

void DinoUiOptionPower::Execute() {
}

MRect DinoUiOptionMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiOptionMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiOptionMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask0<DinoUiOptionMenu, DinoHelpBase>;
template MTask *MCreateCallTask(DinoUiOptionMenu *obj, void (DinoHelpBase::*func)());
