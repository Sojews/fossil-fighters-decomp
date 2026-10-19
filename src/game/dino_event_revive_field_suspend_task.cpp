// DinoEventReviveFieldSuspendTask, DinoEventReviveFieldResumeTask, DinoEventReviveContainerSelect, DinoEventReviveDepositSelect, DinoEventReviveCleaning, DinoEventReviveEffect.
// Only the destructors are decompiled so far.

#define MSL_STRING_BASICS
#include "game/dino_cleaning.h"
#include "m/bright.h"
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
#include "m/screen.h"
#include "game/dino_ui_cleaning_leave.h"
#include "game/dino_ui_revive_effect.h"
#include "game/dino_ui_revive_status_menu.h"

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

MTask *DinoEventReviveDepositSelect::CreateUi() {
    return new DinoUiCleaningLeave(mList);
}

DinoEventReviveCleaning::~DinoEventReviveCleaning() {
}

M_EXTERN_OVERLAY(ov002);

// Cleans the rock (in overlay 2), then unloads it.
void DinoEventReviveCleaning::Idle() {
    switch (mStep++) {
    case 0: {
        MLoadOverlay(0, M_OVERLAY_ID(ov002));
        DinoBackupEntry entry;
        gDinoBackup->GetUnk98(mRock, &entry);
        mSerial.AddTail(new DinoCleaning(entry.id, mResult, entry.unkC_12, entry.unk10_0, 1, mRock));
        break;
    }
    case 1:
        MUnloadOverlay(0, M_OVERLAY_ID(ov002));
        Exit();
        break;
    }
}

DinoEventReviveEffect::~DinoEventReviveEffect() {
}

void DinoEventRevive::Idle() {
    (this->*mState)();
}

void DinoEventReviveFieldSuspendTask::Idle() {
    switch (mStep++) {
    case 0: {
        MTask *fadeMain;
        MTask *fadeSub;
        fadeSub = gLcd->mSub->mBright->CreateFadeOutToBlack(30, 0);
        fadeMain = gLcd->mMain->mBright->CreateFadeOutToBlack(30, 0);
        MParallelTaskSet *fades = new MParallelTaskSet(TRUE);
        fades->AddTail(fadeMain);
        fades->AddTail(fadeSub);
        mSerial.AddTail(fades);
        break;
    }
    case 1:
        mEvent->CloseField(0);
        Exit();
        break;
    }
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
    void (DinoEventReviveEffect::*LoadStatus_0C)();
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
    void (DinoEventRevive::*State02038544_64)();
    void (DinoEventRevive::*StateMode_6C)();
    u8 unk74[0x7C - 0x74];
    void (DinoEventRevive::*StateStart_7C)();
    u8 unk84[0x94 - 0x84];
    void (DinoEventRevive::*End_94)();
    void (DinoEventRevive::*End_9C)();
    u8 unkA4[0xBC - 0xA4];
    void (DinoEventRevive::*StateSelected_BC)();
    u8 unkC4[0xCC - 0xC4];
    void (DinoEventRevive::*StateCheckRocks_CC)();
    u8 unkD4[0xDC - 0xD4];
    void (DinoEventRevive::*StateContainerSelect_DC)();
    void (DinoEventRevive::*StateNoneMessage_E4)();
} data_020bd628;

inline void DinoEventRevive::AddMessage(u32 message) {
    mSerial.AddTail(CreateMessage(message, ""));
}

// Nothing to revive (the flag is set, or no rock is left), or choose the rocks to put in.
// NONMATCHING: the game's messages share one stack slot for their temporary string (see StateLevelMessage).
void DinoEventRevive::StateCheckRocks() {
    if (gDinoBackup->GetSystemFlag(12)) {
        AddMessage(0x14);
        mState = data_020bd628.StateCleaned_34;
    } else if (!gDinoBackup->HasUnrevealedUnk98()) {
        AddMessage(3);
        mState = data_020bd628.State02038544_64;
    } else {
        mUnkE0.clear();
        mSerial.AddTail(new DinoEventReviveDepositSelect(mEvent, &mUnkE0));
        mState = data_020bd628.StateMode_6C;
    }
}

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
        mState = data_020bd628.StateCheckRocks_CC;
        break;
    }
}

// Shows a message (no extra text).

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

M_EXTERN_OVERLAY(ov003);
M_EXTERN_OVERLAY(ov007);

// Plays the revive effect while the creature's status loads, then shows the status.
void DinoEventReviveEffect::Idle() {
    switch (mStep++) {
    case 0:
        MLoadOverlay(0, M_OVERLAY_ID(ov003));
        MLoadOverlay(0, M_OVERLAY_ID(ov007));
        mSerial.AddTail(new DinoUiReviveEffect(mCreature, mCreature2, mDone, 1));
        break;
    case 1:
        mSerial.AddTail(MCreateBgCallTask(this, data_020bd628.LoadStatus_0C, 1));
        break;
    case 2:
        mSerial.AddTail(mUi);
        break;
    case 3:
        MUnloadOverlay(0, M_OVERLAY_ID(ov003));
        MUnloadOverlay(0, M_OVERLAY_ID(ov007));
        Exit();
        break;
    }
}

void DinoEventReviveEffect::LoadStatus() {
    mUi = new DinoUiReviveStatusLoopCleaning(&mCreature, &mCreature2, mDone, 0);
    if (mUi->mState == MTask::STATE_RUN) {
        mUi->Execute();
    }
    if (mUi->mState == MTask::STATE_RUN) {
        mUi->Execute();
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
    mStep = 0;
}

M_EXTERN_OVERLAY(ov004);
M_EXTERN_OVERLAY(ov006);

void DinoEventCaptureFadeUiTask::LoadUi() {
    mEvent->CloseField(FALSE);
    switch (mArg) {
    case 1:
        MLoadOverlay(0, M_OVERLAY_ID(ov003));
        break;
    case 2:
        MLoadOverlay(0, M_OVERLAY_ID(ov004));
        break;
    case 3:
        MLoadOverlay(0, M_OVERLAY_ID(ov005));
        break;
    case 4:
        MLoadOverlay(0, M_OVERLAY_ID(ov003));
        MLoadOverlay(0, M_OVERLAY_ID(ov006));
        break;
    case 5:
        MLoadOverlay(0, M_OVERLAY_ID(ov003));
        MLoadOverlay(0, M_OVERLAY_ID(ov007));
        break;
    }
    mUi = CreateUi();
    if (mUi->mState == MTask::STATE_RUN) {
        mUi->Execute();
    }
    if (mUi->mState == MTask::STATE_RUN) {
        mUi->Execute();
    }
}

// The field's capture fades (in the field overlay).
extern "C" MProcess *func_ov001_02159cd0(DinoField *field);
extern "C" MProcess *func_ov001_02159d3c(DinoField *field);

// The TU's pooled pointer-to-member constants.
extern "C" struct {
    void (DinoEventCaptureFadeUiTask::*LoadUi_00)();
} data_020bda10;

// Fades the field out, slides the screen down while the UI loads, runs it, then brings the field back.
void DinoEventCaptureFadeUiTask::Idle() {
    switch (mStep++) {
    case 0:
        mSerial.AddTail(func_ov001_02159cd0(mEvent->GetField()));
        break;
    case 1:
        gLcd->GetBottomDisplay()->mBright->SetMode(2);
        gLcd->GetBottomDisplay()->mBright->SetFactor(16);
        mParallel.AddTail(mEvent->mScreen->CreateOffsetAnime(MPoint(0, -92), MPoint(0, 0), 29, -2));
        mSerial.AddTail(MCreateBgCallTask(this, data_020bda10.LoadUi_00, 1));
        break;
    case 2:
        mSerial.AddTail(mUi);
        break;
    case 3:
        switch (mArg) {
        case 1:
            MUnloadOverlay(0, M_OVERLAY_ID(ov003));
            break;
        case 2:
            MUnloadOverlay(0, M_OVERLAY_ID(ov004));
            break;
        case 3:
            MUnloadOverlay(0, M_OVERLAY_ID(ov005));
            break;
        case 4:
            MUnloadOverlay(0, M_OVERLAY_ID(ov003));
            MUnloadOverlay(0, M_OVERLAY_ID(ov006));
            break;
        case 5:
            MUnloadOverlay(0, M_OVERLAY_ID(ov003));
            MUnloadOverlay(0, M_OVERLAY_ID(ov007));
            break;
        }
        mEvent->CreateField();
        mSerial.AddTail(func_ov001_02159d3c(mEvent->GetField()));
        break;
    case 4:
        Exit();
        break;
    }
}

// String members the game emits in this file.
template std::string &std::string::operator+=(const std::string &str);
