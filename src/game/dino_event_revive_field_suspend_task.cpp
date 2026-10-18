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
#include "game/dino_event_ui_task.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "m/overlay.h"
#include "game/dino_ui_cleaning_menu_item_menu.h"

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

void DinoEventRevive::Idle() {
    (this->*mState)();
}

void DinoEventReviveFieldResumeTask::Idle() {
    switch (mStep++) {
    case 0:
        mEvent->OpenField(0);
        break;
    case 1:
        mEvent->Fade(0, 30);
        MTask *exit = MTaskExitTransfer::Create(this);
        MTask *wait = mEvent->CreateFadeWaitTask();
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(wait);
        serial->AddTail(exit);
        break;
    }
}

M_EXTERN_OVERLAY(ov005);

void DinoEventReviveContainerSelect::Idle() {
    switch (mStep++) {
    case 0:
        MLoadOverlay(0, M_OVERLAY_ID(ov005));
        mSerial.AddTail(new DinoUiCleaningMenu(mEvent));
        break;
    case 1:
        MUnloadOverlay(0, M_OVERLAY_ID(ov005));
        Exit();
        break;
    }
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MBgCallTask0<DinoEventReviveEffect>;
template MTask *MCreateBgCallTask(DinoEventReviveEffect *obj, void (DinoEventReviveEffect::*func)(), int thread);
template class MBgCallTask0<DinoEventCaptureFadeUiTask>;
template MTask *MCreateBgCallTask(DinoEventCaptureFadeUiTask *obj, void (DinoEventCaptureFadeUiTask::*func)(), int thread);

#pragma auto_inline off

DinoEventUiTask::DinoEventUiTask(DinoEventLoop *event, u32 arg, u8 flag) : MProcess(NULL) {
    mEvent = event;
    mArg = arg;
    mUi = NULL;
    mFlag = flag;
}

DinoEventCaptureFadeUiTask::DinoEventCaptureFadeUiTask(DinoEventLoop *event, u32 arg) : MProcess(NULL) {
    mEvent = event;
    mArg = arg;
    mUi = NULL;
}
