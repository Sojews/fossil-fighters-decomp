// DinoUiWirelessCancelItem.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "m/display.h"
#include "game/dino_ui2_container_base_menu_item_menu.h"
#include "m/multi_message.h"
#include "game/dino_sound.h"
#include "game/dino_resource.h"
#include "m/soft_reset.h"
#include "m/blend.h"
#include "game/dino_ui_wireless_cancel_item_menu.h"
#include "game/dino_ui_wireless_cancel_item.h"
#include "game/dino_ui_child_scanner_cancel_item.h"
#include "game/dino_ui_child_scanner_start_battle_parent_item.h"
#include "game/dino_ui_child_scanner_start_cannon_parent_item.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "m/observer.h"
#include "m/multi_play.h"
#include "m/text_codec.h"
#include "game/dino_common_window_message_box.h"
#include "game/dino_common_window_yes_no_dialog.h"
#include <stdio.h>

DinoUiWirelessCancelItem::~DinoUiWirelessCancelItem() {
}

DinoUiChildScannerCancelItem::~DinoUiChildScannerCancelItem() {
}

DinoUiChildScannerStartBattleParentItem::~DinoUiChildScannerStartBattleParentItem() {
}

DinoUiChildScannerStartCannonParentItem::~DinoUiChildScannerStartCannonParentItem() {
}

DinoUiWirelessCancel::~DinoUiWirelessCancel() {
}

DinoUiChildScannerParentItem::~DinoUiChildScannerParentItem() {
}

DinoUiChildScanner::~DinoUiChildScanner() {
}

DinoUiWirelessMatch::~DinoUiWirelessMatch() {
    delete mHelpDisabler;
}

DinoUiBattleParentScanner::~DinoUiBattleParentScanner() {
    GetDisplay()->mBlend->Reset();
}

DinoUiCannonParentScanner::~DinoUiCannonParentScanner() {
    GetDisplay()->mBlend->Reset();
}

DinoUiCannonChild::~DinoUiCannonChild() {
    GetDisplay()->mBlend->Reset();
}

DinoUiChildScannerBattleConnect::~DinoUiChildScannerBattleConnect() {
    GetDisplay()->mBlend->Reset();
}

DinoUiChildScannerCannonConnect::~DinoUiChildScannerCannonConnect() {
}

void DinoUiWirelessCancelItem::OnDecide() {
}

MRect DinoUiChildScannerStartCannonParentItem::GetRect() {
    return GetFrame();
}

MRect DinoUiChildScannerStartBattleParentItem::GetRect() {
    return GetFrame();
}

MRect DinoUiChildScannerCancelItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiChildScannerStartCannonParentItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MDisplay *DinoUiChildScannerStartBattleParentItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MDisplay *DinoUiChildScannerCancelItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiChildScannerStartCannonParentItem::GetFrame() {
    return mMotion->GetFrame();
}

MRect DinoUiChildScannerStartBattleParentItem::GetFrame() {
    return mMotion->GetFrame();
}

MRect DinoUiChildScannerCancelItem::GetFrame() {
    return mMotion->GetFrame();
}

MRect DinoUiWirelessCancelItem::GetFrame() {
    return mMotion->GetFrame();
}

void DinoUiChildScannerBattleConnect::OnMessage(MMessage *msg) {
    (this->*mOnMessage)(msg);
}

void DinoUiChildScannerCannonConnect::OnMessage(MMessage *msg) {
    (this->*mOnMessage)(msg);
}

void DinoUiBattleParentScanner::Idle() {
    (this->*mState)();
}

void DinoUiCannonParentScanner::OnMessage(MMessage *msg) {
    (this->*mOnMessage)(msg);
}

void DinoUiCannonChild::Idle() {
    (this->*mState)();
}

void DinoUiChildScannerStartCannonParentItem::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(2, 0);
    } else {
        mMotion->Play(1, 0);
    }
}

void DinoUiChildScannerStartBattleParentItem::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(2, 0);
    } else {
        mMotion->Play(1, 0);
    }
}

void DinoUiChildScannerCancelItem::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

void DinoUiWirelessCancelItem::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

void DinoUiChildScannerBattleConnect::Idle() {
    (this->*mState)();
}

void DinoUiChildScannerCannonConnect::Idle() {
    (this->*mState)();
}

void DinoUiCannonParentScanner::Idle() {
    (this->*mState)();
}

void DinoUiWirelessMatch::Idle() {
    (this->*mState)();
}

MRect DinoUiChildScannerParentItem::GetFrame() {
    return mMotion->GetFrame();
}

void DinoUiChildScannerStartCannonParentItem::OnDecide() {
    mMotion->Play(3, 0);
}

void DinoUiChildScannerStartBattleParentItem::OnDecide() {
    mMotion->Play(3, 0);
}

void DinoUiChildScannerCancelItem::OnDecide() {
    mMotion->Play(4, 0);
}

void DinoUiChildScannerParentItem::OnDecide() {
    if (mUnkA0) {
        mMotion->Play(3, 1);
    }
}

void DinoUiWirelessCancel::OnDecide(MMenuItem *item) {
    Cancel();
}

#pragma auto_inline off

std::string DinoUiChildScannerParentItem::GetName() {
    return mText;
}

DinoUiWirelessCancel::DinoUiWirelessCancel(u8 *cancelled) : MMenu(0) {
    mCancelled = cancelled;
}

#pragma auto_inline on

void DinoUiWirelessCancel::Cancel() {
    *mCancelled = TRUE;
}

extern const u32 data_0208ad7c; // a text number
const char *DinoUiChildScannerStartCannonParentItem::GetText() {
    return gDinoResource->GetText(data_0208ad7c);
}

extern const u32 data_0208ad80; // a text number
const char *DinoUiChildScannerStartBattleParentItem::GetText() {
    return gDinoResource->GetText(data_0208ad80);
}

extern const u32 data_0208ad78; // a text number
const char *DinoUiChildScannerCancelItem::GetText() {
    return gDinoResource->GetText(data_0208ad78);
}

void DinoUiCannonChild::OnMessage(MMessage *msg) {
    if (msg->mId == 0x100A7) {
        mStarted = TRUE;
    }
}

#pragma auto_inline off

void DinoUiCannonParentScanner::ShowCount() {
    mOnes->Play(mCount % 10 + 1, 0);
    mTens->Play(mCount / 10 + 1, 0);
}

// The file's pooled pointer-to-member constants: the game (built ipa) addresses them from anchors every 0x80 bytes,
// so until every state of the file is decompiled they are reached through these (named after the state each
// points to).
typedef void (DinoUiWirelessMatch::*DinoUiWirelessMatchState)();
extern "C" struct {
    u8 unk00[0x34];
    DinoUiWirelessMatchState state_02167e3c; // 0x34
    u8 unk3C[0x44 - 0x3C];
    DinoUiWirelessMatchState StateInit; // 0x44
    DinoUiWirelessMatchState state_021676f8; // 0x4c
    u8 unk54[0x5C - 0x54];
    void (DinoUiBattleParentScanner::*StateStart_5C)(); // 0x5c
    u8 unk64[0x74 - 0x64];
    void (DinoUiCannonParentScanner::*Parent0216623c_74)(); // 0x74
} data_ov004_021740d0;
extern "C" struct {
    u8 unk00[0x4];
    void (DinoUiBattleParentScanner::*StateShowCancel_04)(); // 0x04
    u8 unk0C[0x34 - 0xC];
    void (DinoUiCannonChild::*Child02166dc0_34)(); // 0x34
    u8 unk3C[0x4C - 0x3C];
    DinoUiWirelessMatchState StateRescan; // 0x4c
    void (DinoUiBattleParentScanner::*StateAsk_54)(); // 0x54
    void (MMessageObserver::*ClearQueue_5C)(); // 0x5c
} data_ov004_02174150;
extern "C" struct {
    u8 unk00[0x4];
    void (DinoUiChildScannerBattleConnect::*Unk02163cdc_04)(); // 0x04
    u8 unk0C[0x24 - 0xC];
    void (DinoUiBattleParentScanner::*State02165818_24)(); // 0x24
    u8 unk2C[0x34 - 0x2C];
    void (DinoUiBattleParentScanner::*State021655a0_34)(); // 0x34
    DinoUiWirelessMatchState StateChildScan; // 0x3c
    u8 unk40[0x50 - 0x40];
    DinoUiWirelessMatchState state_02167b38; // 0x50
    u8 unk54[0x5C - 0x54];
    void (DinoUiCannonParentScanner::*StateCheckFailed_64)(); // 0x5c
    u8 unk64[0x74 - 0x64];
    void (DinoUiCannonParentScanner::*ParentStart_7C)(); // 0x74
} data_ov004_021741d0;
extern "C" struct {
    u8 unk00[0xc];
    DinoUiWirelessMatchState StateStart; // 0x0c
    u8 unk14[0x1C - 0x14];
    DinoUiWirelessMatchState StateChildScan; // 0x1c
    void (DinoUiCannonChild::*StateShot_24)(); // 0x24
    u8 unk2C[0x5C - 0x2C];
    void (DinoUiCannonParentScanner::*Failed_5C)(); // 0x5c
    u8 unk64[0x6C - 0x64];
    void (DinoUiCannonParentScanner::*Failed_6C)(); // 0x6c
    void (DinoUiCannonParentScanner::*StateSync_74)(); // 0x74
} data_ov004_02174250;
extern "C" struct {
    u8 unk00[0xC];
    void (DinoUiBattleParentScanner::*StateShowCancel_0C)(); // 0x0c
    u8 unk14[0x54 - 0x14];
    DinoUiWirelessMatchState StateChildScan;    // 0x54
} data_ov004_021742d0;
extern "C" struct {
    u8 unk00[0x2c];
    DinoUiWirelessMatchState StateKeep; // 0x2c
    u8 unk34[0x5C - 0x34];
    DinoUiWirelessMatchState StateParentScanned; // 0x5c
    void (DinoUiCannonChild::*Child021674f8_64)(); // 0x64
} data_ov004_02174350;
extern "C" struct {
    u8 unk00[0x4];
    DinoUiWirelessMatchState StateChildScan;    // 0x4
} data_ov004_021743d0;

void DinoUiWirelessMatch::StateStart() {
    if (MGetResetParameter() == 1) {
        mState = data_ov004_021740d0.state_02167e3c;
    } else if (!mUnk98) {
        mState = data_ov004_021740d0.StateInit;
    } else {
        mState = data_ov004_021740d0.state_021676f8;
    }
}

// Takes over the connection kept in DinoResource.
void DinoUiWirelessMatch::StateInit() {
    mHelpDisabler = new DinoHelpDisabler;
    gDinoResource->GetUnk90((u32 *)&mMultiPlay, (u32 *)&mUnk80);
    MTask *b = mUnk80;
    MParallelTaskSet *parallel = &mParallel;
    parallel->AddTail(mMultiPlay);
    parallel->AddTail(b);
    mChosen = 2;
    mState = data_ov004_021743d0.StateChildScan;
}

void DinoUiWirelessMatch::StateChildScan() {
    mSerial.AddTail(new DinoUiChildScanner(mMultiPlay, &mChosen, &mChoice));
    mState = data_ov004_021741d0.state_02167b38;
}

void DinoUiWirelessMatch::StateCannonChild() {
    mSerial.AddTail(new DinoUiCannonChild(mMultiPlay, mChoice));
    mChosen = 2;
    mState = data_ov004_021741d0.StateChildScan;
}

void DinoUiWirelessMatch::StateBattleParentScan() {
    mSerial.AddTail(new DinoUiBattleParentScanner(mMultiPlay, &mScanResult));
    mState = data_ov004_02174350.StateParentScanned;
}

void DinoUiWirelessMatch::StateParentScanned() {
    switch (mScanResult) {
    case 0:
        mState = data_ov004_02174250.StateChildScan;
        break;
    case 1:
        mState = data_ov004_02174350.StateKeep;
        break;
    }
}

void DinoUiWirelessMatch::StateCannonParentScan() {
    mSerial.AddTail(new DinoUiCannonParentScanner(mMultiPlay, &mScanResult));
    mState = data_ov004_02174150.StateRescan;
}

void DinoUiWirelessMatch::StateRescan() {
    mState = data_ov004_021742d0.StateChildScan;
}

// Hands the connection's tasks over to DinoResource and ends.
void DinoUiWirelessMatch::StateKeep() {
    MTask *b = mUnk80;
    MParallelTaskSet *parallel = &mParallel;
    parallel->Remove(mMultiPlay);
    parallel->Remove(b);
    gDinoResource->SetUnk90((u32)mMultiPlay, (u32)mUnk80);
    *mResult = 1;
    Exit();
}

void DinoUiChildScanner::Cancel() {
    *mChosen = FALSE;
    mSerial.AddTail(MTaskExitTransfer::Create(this));
}

void DinoUiChildScanner::Choose(u32 choice) {
    *mChosen = TRUE;
    *mChoice = choice;
    mSerial.AddTail(MTaskExitTransfer::Create(this));
}

void DinoUiChildScanner::ExitWithReset() {
    MTask *exit = MTaskExitTransfer::Create(this);
    MTask *reset = mMultiPlay->CreateResetTask();
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(reset);
    serial->AddTail(exit);
}

void DinoUiChildScanner::OnIdle() {
    if (mMultiPlay->IsFailed()) {
        ExitWithReset();
    } else {
        MMessageObserver::Dispatch();
    }
}

#pragma auto_inline on

void DinoUiChildScanner::OnFocus(MMenuItem *item) {
    if (mSilentFocus) {
        mSilentFocus = FALSE;
    } else {
        DinoSoundPlaySe(0x38E, 0);
    }
    if (dynamic_cast<DinoUiChildScannerParentItem *>(item)) {
        SetSelectedItem(mParentHelp);
    } else {
        SetSelectedItem(dynamic_cast<DinoHelpItem *>(item));
    }
}

DinoUiWirelessMatch::DinoUiWirelessMatch(u32 *result, u8 arg)
    : MProcess(NULL), mResult(result), mUnk98(arg), mHelpDisabler(NULL) {
    mState = data_ov004_02174250.StateStart;
}

DinoUiBattleParentScanner::DinoUiBattleParentScanner(MMultiPlay *multiPlay, u32 *result)
    : MProcess(NULL), mMultiPlay(multiPlay), mResult(result) {
    mState = data_ov004_021740d0.StateStart_5C;
}

extern const u32 data_0208ab74, data_0208ab78;

// Shows "waiting for a partner" with a cancel button.
void DinoUiBattleParentScanner::StateShowCancel() {
    mTasks->AddTail(new DinoCommonWindowMessageWait2(std::string(gDinoResource->GetText(data_0208ab78)), MPoint(0x80, 0x60), 4, 0),
                    mCancel = new DinoUiWirelessCancel(&mCancelled));
    mState = data_ov004_021741d0.State021655a0_34;
}

void DinoUiBattleParentScanner::OnMessage(MMessage *msg) {
    switch (msg->mId) {
    case 5: {
        MMultiPlayChildConnectedMessage *connected = (MMultiPlayChildConnectedMessage *)msg;
        mAid = connected->mUnk0C;
        MI_CpuCopy8(connected->mUnk0E, mSsid, sizeof(mSsid));
        if (mSsid[0] != 1) {
            mSerial.AddTail(mMultiPlay->CreateDisconnectTask(mAid));
        } else {
            mState = data_ov004_02174150.StateAsk_54;
        }
        break;
    }
    case 6:
        mDisconnected = TRUE;
        break;
    }
}

// Asks whether to play with the child that connected.
void DinoUiBattleParentScanner::StateAsk() {
    mTasks->Clear();
    mAnswer = TRUE;
    std::string name = MEncodeUtf8(std::wstring((const wchar_t *)&mSsid[1]));
    char text[0x100];
    sprintf(text, gDinoResource->GetText(data_0208ab74), name.c_str());
    MTask *dialog = new DinoCommonWindowYesNoDialog(std::string(text), &mAnswer);
    mSerial.AddTail(dialog);
    mState = data_ov004_021741d0.State02165818_24;
}

extern const u32 data_0208ab5c;

static inline void AddSerial(MProcess *process, MTask *a, MTask *b, MTask *c) {
    MSerialTaskSet *serial = &process->mSerial;
    serial->AddTail(a);
    serial->AddTail(b);
    serial->AddTail(c);
}

static inline void AddSerial(MProcess *process, MTask *a, MTask *b, MTask *c, MTask *d) {
    MSerialTaskSet *serial = &process->mSerial;
    serial->AddTail(a);
    serial->AddTail(b);
    serial->AddTail(c);
    serial->AddTail(d);
}

// Waits for a child; cancelling fades out and leaves with no partner.
void DinoUiBattleParentScanner::State021655a0() {
    Dispatch();
    if (mCancelled) {
        *mResult = 0;
        MTask *exit = CreateExitTransfer();
        MTask *fade = GetDisplay()->mBlend->CreateFadeRatioAnime(8, 0, 10, 0);
        MTask *reset = mMultiPlay->CreateResetTask();
        MSerialTaskSet *resets = new MSerialTaskSet(TRUE);
        resets->AddTail(reset);
        MParallelTaskSet *closing = new MParallelTaskSet(TRUE);
        closing->AddTail(resets);
        closing->AddTail(fade);
        MTask *empty = MTaskSetMakeEmptyTask::Create(mTasks);
        MTask *close = mCancel->CreateCloseTask();
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(close);
        serial->AddTail(empty);
        serial->AddTail(closing);
        serial->AddTail(exit);
    }
}

// The answer: yes leaves with the child, no turns it away. If it left meanwhile, says so and waits again.
void DinoUiBattleParentScanner::State02165818() {
    mDisconnected = FALSE;
    Dispatch();
    if (mDisconnected) {
        AddSerial(this, mMultiPlay->CreateResetTask(),
                  new DinoCommonWindowMessageBox(MPoint(0x80, 0x60), gDinoResource->GetText(data_0208ab5c), 4),
                  MCreateCallTask(this, data_ov004_02174150.ClearQueue_5C),
                  mMultiPlay->CreateStartParentTask(mStartData, sizeof(mStartData), 0));
        mState = data_ov004_021742d0.StateShowCancel_0C;
        return;
    }
    if (mAnswer == TRUE) {
        *mResult = 1;
        mTasks->Clear();
        MTask *exit = CreateExitTransfer();
        MTask *fade = GetDisplay()->mBlend->CreateFadeRatioAnime(8, 0, 10, 0);
        AddSerial(this, mMultiPlay->CreateSynchronizeDelayTask(), fade, exit);
    } else {
        mSerial.AddTail(mMultiPlay->CreateDisconnectTask(mAid));
        mState = data_ov004_02174150.StateShowCancel_04;
    }
}

DinoUiCannonParentScanner::DinoUiCannonParentScanner(MMultiPlay *multiPlay, u32 *result)
    : MProcess(NULL), mMultiPlay(multiPlay), mResult(result) {
    mState = data_ov004_021741d0.ParentStart_7C;
}

void DinoUiCannonParentScanner::OnConnectMessage(MMessage *msg) {
    if (msg->mId == 5) {
        MMultiPlayChildConnectedMessage *connected = (MMultiPlayChildConnectedMessage *)msg;
        mAid = connected->mUnk0C;
        MI_CpuCopy8(connected->mUnk0E, mSsid, sizeof(mSsid));
        if (mSsid[0] != 2) {
            mSerial.AddTail(mMultiPlay->CreateDisconnectTask(mAid));
        } else {
            mState = data_ov004_02174250.StateSync_74;
        }
    }
}

void DinoUiCannonParentScanner::StateSync() {
    mSerial.AddTail(mMultiPlay->CreateSynchronizeDelayTask());
    mState = data_ov004_021741d0.StateCheckFailed_64;
}

void DinoUiCannonParentScanner::StateCheckFailed() {
    if (mMultiPlay->IsFailed()) {
        mState = data_ov004_02174250.Failed_6C;
    } else {
        mState = data_ov004_021740d0.Parent0216623c_74;
    }
}

void DinoUiCannonParentScanner::StateWait() {
    if (mMultiPlay->IsFailed()) {
        mState = data_ov004_02174250.Failed_5C;
        return;
    }
    Dispatch();
}

void DinoUiCannonChild::StateShootMenu() {
    mSerial.AddTail(new DinoUi2CannonShootMenu(&mUnkB8));
    mState = data_ov004_02174250.StateShot_24;
}

void DinoUiCannonChild::StateShot() {
    if (mUnkB8.size() == 0) {
        mState = data_ov004_02174350.Child021674f8_64;
    } else {
        mState = data_ov004_02174150.Child02166dc0_34;
    }
}

void DinoUiChildScannerBattleConnect::StateCancel() {
    if (mMultiPlay->IsFailed()) {
        mState = data_ov004_021741d0.Unk02163cdc_04;
        return;
    }
    mUnkE8->Clear();
    mScanner->Cancel();
    MTask *exit = MTaskExitTransfer::Create(this);
    MTask *fade = gCurrentDisplay->mBlend->CreateFadeRatioAnime(8, 0, 10, 0);
    MTaskSet &serial = mSerial;
    serial.AddTail(fade);
    serial.AddTail(exit);
}

void DinoUiChildScannerBattleConnect::StateRescan() {
    mScanner->Rescan();
    MTask *exit = MTaskExitTransfer::Create(this);
    MTask *fade = gCurrentDisplay->mBlend->CreateFadeRatioAnime(8, 0, 10, 0);
    MTaskSet &serial = mSerial;
    serial.AddTail(fade);
    serial.AddTail(exit);
}

// Template task instances the game emits in this file. (Instantiated here, after the code that uses them, so that it
// calls them.)
template class MCallTask0<DinoUiBattleParentScanner, MMessageObserver>;
template MTask *MCreateCallTask(DinoUiBattleParentScanner *obj, void (MMessageObserver::*func)());
