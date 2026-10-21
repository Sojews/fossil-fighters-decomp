// DinoEventReviveFieldSuspendTask, DinoEventReviveFieldResumeTask, DinoEventReviveContainerSelect, DinoEventReviveDepositSelect, DinoEventReviveCleaning, DinoEventReviveEffect.
// Only the destructors are decompiled so far.

#define MSL_STRING_BASICS
#include "game/dino_static_init.h"
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
#include "game/dino_event_talk_window.h"
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
    u8 unk84[0x8C - 0x84];
    void (DinoEventRevive::*State02038544_8C)();
    void (DinoEventRevive::*End_94)();
    void (DinoEventRevive::*End_9C)();
    void (DinoEventRevive::*State02037dbc_A4)();
    u8 unkAC[0xB4 - 0xAC];
    void (DinoEventRevive::*StateCleaned_B4)();
    void (DinoEventRevive::*StateSelected_BC)();
    u8 unkC4[0xCC - 0xC4];
    void (DinoEventRevive::*StateCheckRocks_CC)();
    u8 unkD4[0xDC - 0xD4];
    void (DinoEventRevive::*StateContainerSelect_DC)();
    void (DinoEventRevive::*StateNoneMessage_E4)();
    void (DinoEventRevive::*StateDeposit_EC)();
} data_020bd628;

// The texts of the revival menu's choices.
extern const u32 data_0208a9a8;
extern const u32 data_0208a9ac;
extern const u32 data_0208a9b0;
extern const u32 data_0208a9b4;
extern const u32 data_0208a9b8;

// The message's text.
static inline const char *DinoReviveMessageText(MResource<DinoMessageData> &messages, u32 i) {
    const u8 *data = (const u8 *)messages.Get();
    const u8 *entry = data + ((const u32 *)(data + ((const u32 *)data)[2]))[i];
    return (const char *)entry + ((const u32 *)entry)[1];
}

// NONMATCHING: the game allocates each push_back's two flag bytes together (ours puts the fast-path ones first).
// Asks what to do: revive, the stored rocks (when there are some and the level allows), the creature parts (when there
// are some), another choice, or leave (-2).
void DinoEventRevive::State02038544() {
    DinoEventTalkWindow *window =
        new DinoEventTalkWindow(gCurrentDisplay, std::string(DinoReviveMessageText(mMessages, 2)), 0, 0);
    std::vector<DinoWindowChoice> choices;
    choices.push_back(DinoWindowChoice(0, std::string(gDinoResource->GetText(data_0208a9b8))));
    if (gDinoBackup->GetExcavateLevelUnk0() && gDinoBackup->IsUnk9CEmpty()) {
        choices.push_back(DinoWindowChoice(1, std::string(gDinoResource->GetText(data_0208a9b4))));
    }
    if (gDinoBackup->CountCreatureParts()) {
        choices.push_back(DinoWindowChoice(2, std::string(gDinoResource->GetText(data_0208a9b0))));
    }
    choices.push_back(DinoWindowChoice(3, std::string(gDinoResource->GetText(data_0208a9ac))));
    choices.push_back(DinoWindowChoice(-2, std::string(gDinoResource->GetText(data_0208a9a8))));
    window->SetChoices((u32)&mUnkD8, MPoint(0xFE, 0x8E), choices, 3);
    mSerial.AddTail(window);
    mState = data_020bd628.StateMode_6C;
}

// Nothing to revive (the flag is set, or no rock is left), or choose the rocks to put in.
void DinoEventRevive::StateCheckRocks() {
    if (gDinoBackup->GetSystemFlag(12)) {
        mSerial.AddTail(CreateMessage(0x14));
        mState = data_020bd628.StateCleaned_B4;
    } else if (!gDinoBackup->HasUnrevealedUnk98()) {
        mSerial.AddTail(CreateMessage(3));
        mState = data_020bd628.State02038544_64;
    } else {
        mUnkE0.clear();
        mSerial.AddTail(new DinoEventReviveDepositSelect(mEvent, &mUnkE0));
        mState = data_020bd628.StateDeposit_EC;
    }
}

// Moves the rocks chosen to deposit from the bag into storage. Says so the first time storage gets any.
void DinoEventRevive::StateDeposit() {
    if (mUnkE0.size() != 0) {
        for (u32 i = 0; i < mUnkE0.size(); i++) {
            DinoBackupEntry entry;
            gDinoBackup->GetUnk98((u32)mUnkE0[i], &entry);
            gDinoBackup->SetUnk9C(gDinoBackup->FindFreeUnk9C(), &entry);
            MI_CpuFill8(&entry, 0, sizeof(entry));
            gDinoBackup->SetUnk98((u32)mUnkE0[i], &entry);
        }
        gDinoBackup->CompactUnk98();
    }
    if (!gDinoBackup->IsUnk9CEmpty()) {
        mSerial.AddTail(CreateMessage(0x13));
        gDinoBackup->MarkUnk168();
    }
    mState = data_020bd628.State02038544_8C;
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

void DinoEventRevive::StateLevelMessage() {
    u32 level = gDinoBackup->GetExcavateLevelUnk0();
    if (gDinoBackup->GetSystemVar(0x19) == 0) {
        mSerial.AddTail(CreateMessage(0x22));
    } else if (level == 0) {
        mSerial.AddTail(CreateMessage(0x1D));
    } else if (level == gDinoBackup->GetMaxExcavateLevelUnk0()) {
        mSerial.AddTail(CreateMessage(0x1A));
    } else {
        mSerial.AddTail(CreateMessage(0x15));
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

DinoEventRevive::DinoEventRevive(DinoEventLoop *event) : MProcess(gLcd->GetBottomDisplay()), mEvent(event) {
    mMessages = MResource<DinoMessageData>("/msg/msg_cleaning");
    mState = data_020bd628.State02037dbc_A4;
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

// (Emitted ahead of Idle, so it isn't the next function after its caller.)
template MTask *MCreateBgCallTask(DinoEventReviveEffect *obj, void (DinoEventReviveEffect::*func)(), int thread);

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
template class MBgCallTask0<DinoEventCaptureFadeUiTask>;
template MTask *MCreateBgCallTask(DinoEventCaptureFadeUiTask *obj, void (DinoEventCaptureFadeUiTask::*func)(), int thread);

#pragma auto_inline off

DinoEventUiTask::DinoEventUiTask(DinoEventLoop *event, u32 arg, u8 flag) : MProcess(NULL) {
    mEvent = event;
    mArg = arg;
    mStep = 0;
    mFlag = flag;
}


DinoEventCaptureFadeUiTask::DinoEventCaptureFadeUiTask(DinoEventLoop *event, u32 arg) : MProcess(NULL) {
    mEvent = event;
    mArg = arg;
    mStep = 0;
}

M_EXTERN_OVERLAY(ov004);
M_EXTERN_OVERLAY(ov006);

void DinoEventUiTask::Idle() {
    switch (mStep++) {
    case 0:
        mEvent->Fade(mFlag ? 1 : 3, 30);
        mSerial.AddTail(mEvent->CreateFadeWaitTask());
        break;
    case 1:
        mEvent->CloseField(FALSE);
        break;
    case 2:
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
        mSerial.AddTail(CreateUi());
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
        mEvent->OpenField(FALSE);
        mEvent->Fade(mFlag ? 0 : 2, 30);
        mSerial.AddTail(mEvent->CreateFadeWaitTask());
        break;
    case 4:
        Exit();
        break;
    }
}

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

// A talk window with the message, its %s filled in with extra.
MTask *DinoEventRevive::CreateMessage(u32 message, const std::string &extra) {
    const u8 *data = (const u8 *)mMessages.Get();
    const u8 *entry = data + ((const u32 *)(data + ((const u32 *)data)[2]))[message];
    const char *format = (const char *)entry + ((const u32 *)entry)[1];
    char *text = new char[strlen(format) + extra.size() + 1];
    OS_SPrintf(text, format, extra.c_str());
    MTask *window = new DinoEventTalkWindow(GetDisplay(), std::string(text), 0, 0);
    delete[] text;
    return window;
}

// The file's static initialization (see dino_static_init.h): the common layout header.
// NONMATCHING: the game schedules the stores differently.
extern "C" u8 data_020bd6a8[];
extern "C" u8 data_020bd728[];
extern "C" u8 data_020bd7a8[];
#pragma always_inline on

static struct DinoEventReviveStaticInit {
    DinoEventReviveStaticInit() {
        DINO_POOL_NV(MPoint, data_020bd6a8, 0x2c) = MPoint(256, 192);
        DINO_POOL_NV(MRect, data_020bd728, 0x78) = DinoSizeRect(DINO_POOL_NV(MPoint, data_020bd6a8, 0x2c));
        DINO_POOL_NV(MPoint, data_020bd6a8, 0x54) = MPoint(2, 2);
        DINO_POOL_NV(MPoint, data_020bd6a8, 0x44) = MPoint(36, 28);
        DINO_POOL_NV(MRect, data_020bd7a8, 0x8) = DinoSizeRect(DINO_POOL_NV(MPoint, data_020bd6a8, 0x44));
        DINO_POOL_NV(MPoint, data_020bd7a8, 0x20) = MPoint(256, 192);
        DINO_POOL_NV(MPoint, data_020bd7a8, 0x18) = MPoint(0, 24);
        DINO_POOL_NV(MRect, data_020bd7a8, 0x28) = MRect(0, 0, 256, 24);
    }
} sDinoEventReviveStaticInit;
