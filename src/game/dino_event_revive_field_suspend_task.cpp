// DinoEventReviveFieldSuspendTask, DinoEventReviveFieldResumeTask, DinoEventReviveContainerSelect, DinoEventReviveDepositSelect, DinoEventReviveCleaning, DinoEventReviveEffect.
// Only the destructors are decompiled so far.

#include "game/dino_event_revive_field_suspend_task.h"
#include "game/dino_event_revive_field_resume_task.h"
#include "game/dino_event_revive_container_select.h"
#include "game/dino_event_revive_deposit_select.h"
#include "game/dino_event_revive_cleaning.h"
#include "game/dino_event_revive_effect.h"
#include "game/dino_event_revive.h"
#include "game/dino_event_capture_fade_ui_task.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"

DinoEventReviveFieldSuspendTask::~DinoEventReviveFieldSuspendTask() {
}

DinoEventRevive::~DinoEventRevive() {
}

DinoEventReviveFieldResumeTask::~DinoEventReviveFieldResumeTask() {
}

DinoEventReviveContainerSelect::~DinoEventReviveContainerSelect() {
}

DinoEventReviveDepositSelect::~DinoEventReviveDepositSelect() {
}

DinoEventReviveCleaning::~DinoEventReviveCleaning() {
}

DinoEventReviveEffect::~DinoEventReviveEffect() {
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MBgCallTask0<DinoEventReviveEffect>;
template MTask *MCreateBgCallTask(DinoEventReviveEffect *obj, void (DinoEventReviveEffect::*func)(), int thread);
template class MBgCallTask0<DinoEventCaptureFadeUiTask>;
template MTask *MCreateBgCallTask(DinoEventCaptureFadeUiTask *obj, void (DinoEventCaptureFadeUiTask::*func)(), int thread);
