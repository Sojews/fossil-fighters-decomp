// DinoUiWirelessCancelItem.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

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

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask0<DinoUiBattleParentScanner, MMessageObserver>;
template MTask *MCreateCallTask(DinoUiBattleParentScanner *obj, void (MMessageObserver::*func)());

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
    DinoUiWirelessMatchState state_02167e3c;    // 0x34
    u8 unk3C[0x44 - 0x3C];
    DinoUiWirelessMatchState StateInit;         // 0x44
    DinoUiWirelessMatchState state_021676f8;    // 0x4C
} data_ov004_021740d0;
extern "C" struct {
    u8 unk00[0x4C];
    DinoUiWirelessMatchState StateRescan;       // 0x4C
} data_ov004_02174150;
extern "C" struct {
    u8 unk00[0x3C];
    DinoUiWirelessMatchState StateChildScan;    // 0x3C
    u8 unk44[0x54 - 0x44];
    DinoUiWirelessMatchState state_02167b38;    // 0x54
} data_ov004_021741d0;
extern "C" struct {
    u8 unk00[0x1C];
    DinoUiWirelessMatchState StateChildScan;    // 0x1C
} data_ov004_02174250;
extern "C" struct {
    u8 unk00[0x54];
    DinoUiWirelessMatchState StateChildScan;    // 0x54
} data_ov004_021742d0;
extern "C" struct {
    u8 unk00[0x2C];
    DinoUiWirelessMatchState StateKeep;         // 0x2C
    u8 unk34[0x5C - 0x34];
    DinoUiWirelessMatchState StateParentScanned; // 0x5C
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
