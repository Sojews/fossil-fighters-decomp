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
#include "game/dino_backup.h"
#include "game/dino_sound.h"
#include "m/display.h"
#include "m/lcd.h"

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
        mSerial.AddTail(new DinoUiCleaningMenu(mResult));
        break;
    case 1:
        MUnloadOverlay(0, M_OVERLAY_ID(ov005));
        Exit();
        break;
    }
}

// The TU's pooled pointer-to-member constants (see menu.cpp).
// (One block; dsd splits it at data_020bd6a8)
extern "C" struct {
    u8 unk00[0x4];
    void (DinoEventRevive::*End_04)();
    u8 unk0C[0x14 - 0xC];
    void (DinoEventRevive::*End_14)();
    void (DinoEventRevive::*End_1C)();
    void (DinoEventRevive::*StateLevelMessage_24)();
    void (DinoEventRevive::*StateCleaning_2C)();
    void (DinoEventRevive::*StateCleaned_34)();
    void (DinoEventRevive::*StateEffectEnd_3C)();
    void (DinoEventRevive::*StateEffect_44)();
    void (DinoEventRevive::*End_4C)();
    void (DinoEventRevive::*StateContainerSelect_54)();
    void (DinoEventRevive::*StateContainerSelect_5C)();
    u8 unk64[0x7C - 0x64];
    void (DinoEventRevive::*StateStart_7C)();
    u8 unk84[0x94 - 0x84];
    void (DinoEventRevive::*End_94)();
    void (DinoEventRevive::*End_9C)();
    u8 unkA4[0xBC - 0xA4];
    void (DinoEventRevive::*StateSelected_BC)();
    u8 unkC4[0xCC - 0xC4];
    void (DinoEventRevive::*Unk02038abc_CC)();
    u8 unkD4[0xDC - 0xD4];
    void (DinoEventRevive::*StateContainerSelect_DC)();
    void (DinoEventRevive::*StateNoneMessage_E4)();
} data_020bd628;

// What the event was started for: 0 revives, 1 and 2 other messages, 3 the excavation level.
void DinoEventRevive::StateMode() {
    switch (mUnkD8) {
    default: {
        std::string extra("");
        MTask *exit = MTaskExitTransfer::Create(this);
        MTask *message = CreateMessage(0x17, extra);
        MTaskSet &serial = mSerial;
        serial.AddTail(message);
        serial.AddTail(exit);
        break;
    }
    case 3:
        mState = data_020bd628.StateLevelMessage_24;
        break;
    case 0:
        mState = data_020bd628.StateStart_7C;
        break;
    case 2:
        mState = data_020bd628.StateNoneMessage_E4;
        break;
    case 1:
        mState = data_020bd628.Unk02038abc_CC;
        break;
    }
}

// Shows a message (no extra text).
inline void DinoEventRevive::AddMessage(u32 message) {
    mSerial.AddTail(CreateMessage(message, ""));
}

// NONMATCHING: the game's messages share one stack slot for their temporary string.
void DinoEventRevive::StateLevelMessage() {
    u32 level = gDinoBackup->GetExcavateLevelUnk0();
    if (gDinoBackup->GetSystemVar(0x19) == 0) {
        AddMessage(0x22);
    } else if (level == 0) {
        AddMessage(0x1D);
    } else if (level == gDinoBackup->GetMaxExcavateLevelUnk0()) {
        AddMessage(0x1A);
    } else {
        AddMessage(0x15);
    }
    mState = data_020bd628.End_4C;
}

void DinoEventRevive::StateNoneMessage() {
    mSerial.AddTail(CreateMessage(0x16, ""));
    mState = data_020bd628.End_9C;
}

void DinoEventRevive::StateStart() {
    if (!gDinoBackup->HasInactiveUnk98()) {
        mSerial.AddTail(CreateMessage(3, ""));
        mState = data_020bd628.End_04;
        return;
    }
    mSerial.AddTail(new DinoEventReviveFieldSuspendTask(mEvent));
    gDinoBackup->SetSystemVar(0x2D, 0);
    mState = data_020bd628.StateContainerSelect_5C;
}

void DinoEventRevive::StateContainerSelect() {
    gLcd->SetBottomDisplay(gLcd->mSub);
    gCurrentDisplay = gLcd->GetBottomDisplay();
    DinoSoundLoadScene(mScene);
    DinoSoundPlayBgm(mBgm, 0);
    mSerial.AddTail(new DinoEventReviveContainerSelect(&mRock));
    mState = data_020bd628.StateSelected_BC;
}

void DinoEventRevive::StateSelected() {
    if (mRock == -1) {
        mSerial.AddTail(new DinoEventReviveFieldResumeTask(mEvent));
        mState = data_020bd628.End_94;
    } else {
        mState = data_020bd628.StateCleaning_2C;
    }
}

void DinoEventRevive::StateCleaning() {
    mSerial.AddTail(new DinoEventReviveCleaning(mRock, &mResult));
    mState = data_020bd628.StateCleaned_34;
}

void DinoEventRevive::StateCleaned() {
    switch (mResult.mOutcome) {
    case 3:
        mSerial.AddTail(new DinoEventReviveFieldResumeTask(mEvent));
        mState = data_020bd628.End_1C;
        break;
    case 4:
        mState = data_020bd628.StateContainerSelect_DC;
        break;
    case 1:
    case 2:
        mState = data_020bd628.StateEffect_44;
        break;
    }
}

void DinoEventRevive::StateEffect() {
    mSerial.AddTail(new DinoEventReviveEffect(mCreature, mCreature2, &mEffectDone));
    mState = data_020bd628.StateEffectEnd_3C;
}

void DinoEventRevive::StateEffectEnd() {
    if (mEffectDone == 0) {
        mState = data_020bd628.StateContainerSelect_54;
        return;
    }
    mSerial.AddTail(new DinoEventReviveFieldResumeTask(mEvent));
    mState = data_020bd628.End_14;
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
