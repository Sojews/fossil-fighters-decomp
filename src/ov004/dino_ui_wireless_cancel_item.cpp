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
// (MCreateBgCallTask is only declared here: the game calls the copy in dino_ui_exchange.cpp.)
template <class T> MTask *MCreateBgCallTask(T *obj, void (T::*func)(), int thread);
#include "m/call_task.h"
#include "m/observer.h"
#include "m/multi_play.h"
#include "m/text_codec.h"
#include <algorithm>
#include "game/backup_validater.h"
#include "game/dino_message.h"
#include "game/dino_multi_play_icon.h"
#include "m/pal.h"
#include "game/dino_backup.h"
#include "nitro/mi.h"
#include "game/dino_common_window_menu_item_flush_task_menu.h"
#include "m/oam_canvas.h"
#include "m/indicator.h"
#include "m/smart_oam.h"
#include "m/motion.h"
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
    if (mMode) {
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
    u8 unk00[0x10];
    const char *mDigitPath;     // 0x10: the count's digits
    const char *mParentPath2;   // 0x14: the parent items' motions, by mode
    u32 mMaxRockCount;          // 0x18
    const char *mParentPath0;   // 0x1c
    const char *mCancelPath;    // 0x20
    const char *mParentPath1;   // 0x24
    u8 unk28[0x30 - 0x28];
    const char *mCountFramePath; // 0x30
    DinoUiWirelessMatchState StateAfterReset; // 0x34
    void (DinoUiCannonChild::*Leave_3C)(); // 0x3c
    DinoUiWirelessMatchState StateInit; // 0x44
    DinoUiWirelessMatchState StateAsk; // 0x4c
    void (DinoUiCannonParentScanner::*StateLost_54)(); // 0x54
    void (DinoUiBattleParentScanner::*StateStart_5C)(); // 0x5c
    void (DinoUiCannonChild::*State021672f8_64)(); // 0x64
    void (DinoUiChildScannerCannonConnect::*StateConnect_6C)(); // 0x6c
    void (DinoUiCannonParentScanner::*StateListen_74)(); // 0x74
    void (DinoBackup::*Save_7C)(); // 0x7c (its second word is in the next block)
} data_ov004_021740d0;
extern "C" struct {
    u8 unk00[0x4];
    void (DinoUiBattleParentScanner::*StateShowCancel_04)(); // 0x04
    void (DinoUiCannonChild::*Leave_0C)(); // 0x0c
    void (DinoUiCannonChild::*StateSendProfile_14)(); // 0x14
    DinoUiWirelessMatchState StateStartWireless_1C; // 0x1c
    DinoUiWirelessMatchState StateChildScan_24; // 0x24
    void (DinoUiCannonChild::*StateWaitStart_2C)(); // 0x2c
    void (DinoUiCannonChild::*Child02166dc0_34)(); // 0x34
    void (DinoUiCannonChild::*Leave_3C)(); // 0x3c
    void (DinoBackup::*Save_44)(); // 0x44
    DinoUiWirelessMatchState StateRescan; // 0x4c
    void (DinoUiBattleParentScanner::*StateAsk_54)(); // 0x54
    void (MMessageObserver::*ClearQueue_5C)(); // 0x5c
    void (DinoUiCannonParentScanner::*StateWaitChildren_64)(); // 0x64
    MPoint mParentNameFocusedOffset; // 0x6c
    void (DinoUiCannonParentScanner::*OnConnectMessage_74)(MMessage *msg); // 0x74
} data_ov004_02174150;
extern "C" struct {
    u8 unk00[0x4];
    void (DinoUiChildScannerBattleConnect::*Unk02163cdc_04)(); // 0x04
    void (DinoUiCannonParentScanner::*OnCountMessage_0C)(MMessage *msg); // 0x0c
    void (DinoUiChildScannerCannonConnect::*StateChoose_14)(); // 0x14
    MPoint mParentNameOffset; // 0x1c
    void (DinoUiBattleParentScanner::*State02165818_24)(); // 0x24
    u8 unk2C[0x34 - 0x2C];
    void (DinoUiBattleParentScanner::*State021655a0_34)(); // 0x34
    DinoUiWirelessMatchState StateChildScan; // 0x3c
    void (DinoUiChildScannerCannonConnect::*IgnoreMessage_44)(MMessage *msg); // 0x44
    DinoUiWirelessMatchState StateKeep_4C; // 0x4c
    DinoUiWirelessMatchState StateChosen_54; // 0x54
    u8 unk5C[0x64 - 0x5C];
    void (DinoUiCannonParentScanner::*StateCheckFailed_64)(); // 0x64
    void (DinoUiCannonParentScanner::*State021664c0_6C)(); // 0x6c
    u8 unk74[0x7C - 0x74];
    void (DinoUiCannonParentScanner::*StateStart_7C)(); // 0x7c
} data_ov004_021741d0;
extern "C" struct {
    u8 unk00[0x4];
    void (DinoBackup::*Save_04)(); // 0x04
    DinoUiWirelessMatchState StateStart; // 0x0c
    void (DinoUiCannonChild::*StateShootMenu_14)(); // 0x14
    DinoUiWirelessMatchState StateChildScan; // 0x1c
    void (DinoUiCannonChild::*StateShot_24)(); // 0x24
    void (DinoUiCannonParentScanner::*OnConnectMessage_2C)(MMessage *msg); // 0x2c
    void (DinoUiCannonParentScanner::*StateLost_34)(); // 0x34
    u8 unk3C[0x44 - 0x3C];
    void (DinoUiChildScannerBattleConnect::*OnConnectMessage_44)(MMessage *msg); // 0x44
    DinoUiWirelessMatchState StateAnswered_4C; // 0x4c
    void (DinoUiChildScannerCannonConnect::*StateConnecting_54)(); // 0x54
    void (DinoUiCannonParentScanner::*Failed_5C)(); // 0x5c
    void (DinoUiCannonParentScanner::*StateWait_64)(); // 0x64
    void (DinoUiCannonParentScanner::*Failed_6C)(); // 0x6c
    void (DinoUiCannonParentScanner::*StateSync_74)(); // 0x74
    void (DinoUiCannonParentScanner::*StateWaitChildren_7C)(); // 0x7c (its second word is in the next block)
} data_ov004_02174250;
extern "C" struct {
    u8 unk00[0x4];
    void (DinoUiCannonParentScanner::*StateShowCancel_04)(); // 0x04
    void (DinoUiBattleParentScanner::*StateShowCancel_0C)(); // 0x0c
    void (DinoUiChildScannerBattleConnect::*StateConnecting_14)(); // 0x14
    void (DinoUiCannonParentScanner::*StateCancelled_1C)(); // 0x1c
    void (DinoUiBattleParentScanner::*StateShowCancel_24)(); // 0x24
    void (DinoUiChildScannerCannonConnect::*IgnoreMessage_2C)(MMessage *msg); // 0x2c
    void (DinoUiChildScannerCannonConnect::*OnConnectMessage_34)(MMessage *msg); // 0x34
    u8 unk3C[0x44 - 0x3C];
    void (DinoUiChildScannerCannonConnect::*CannonStart_44)(); // 0x44
    u8 unk4C[0x54 - 0x4C];
    DinoUiWirelessMatchState StateChildScan;    // 0x54
    void (DinoUiCannonChild::*ChildStart_5C)(); // 0x5c
    u8 unk64[0x6C - 0x64];
    void (DinoUiChildScannerBattleConnect::*BattleStart_6C)(); // 0x6c
    u8 unk74[0x7C - 0x74];
    void (DinoUiChildScannerBattleConnect::*StateCancel_7C)(); // 0x7c (its second word is in the next block)
} data_ov004_021742d0;
extern "C" struct {
    u8 unk00[0x4];
    void (DinoUiChildScannerBattleConnect::*IgnoreMessage_04)(MMessage *msg); // 0x04
    void (DinoUiChildScannerBattleConnect::*IgnoreMessage_0C)(MMessage *msg); // 0x0c
    void (DinoUiChildScannerBattleConnect::*StateConnect_14)(); // 0x14
    MPoint mMessagePos;  // 0x1c
    void (DinoUiChildScannerBattleConnect::*StateChoose_24)(); // 0x24
    DinoUiWirelessMatchState StateKeep; // 0x2c
    void (DinoUiCannonParentScanner::*StateReceived_34)(); // 0x34
    u8 unk3C[0x44 - 0x3C];
    void (DinoUiCannonChild::*StateSendRocks_44)(); // 0x44
    void (DinoUiCannonParentScanner::*StateShowCancel_4C)(); // 0x4c
    void (DinoUiCannonChild::*Child021674f8_54)(); // 0x54
    DinoUiWirelessMatchState StateParentScanned; // 0x5c
    void (DinoUiCannonChild::*Child021674f8_64)(); // 0x64
    DinoUiWirelessMatchState StateCannonParentScan_6C; // 0x6c
    DinoUiWirelessMatchState StateBattleParentScan_74; // 0x74
    DinoUiWirelessMatchState StateCannonChild_7C; // 0x7c (its second word is in the next block)
    u8 unk84[0x8C - 0x84];
    DinoUiWirelessMatchState StateFinish_8C; // 0x8c
} data_ov004_02174350;
extern "C" struct {
    u8 unk00[0x4];
    DinoUiWirelessMatchState StateChildScan;    // 0x4
} data_ov004_021743d0;

static inline void AddParallel(MProcess *process, MTask *a, MTask *b, MTask *c, MTask *d) {
    MParallelTaskSet *parallel = &process->mParallel;
    parallel->AddTail(a);
    parallel->AddTail(b);
    parallel->AddTail(c);
    parallel->AddTail(d);
}

static inline void AddSerial(MProcess *process, MTask *a, MTask *b) {
    MSerialTaskSet *serial = &process->mSerial;
    serial->AddTail(a);
    serial->AddTail(b);
}

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


void DinoUiWirelessMatch::StateStart() {
    if (MGetResetParameter() == 1) {
        mState = data_ov004_021740d0.StateAfterReset;
    } else if (!mUnk98) {
        mState = data_ov004_021740d0.StateInit;
    } else {
        mState = data_ov004_021740d0.StateAsk;
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
    mState = data_ov004_021741d0.StateChosen_54;
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

DinoUiChildScanner::DinoUiChildScanner(MMultiPlay *multiPlay, u32 *chosen, u32 *choice) : MMenu(0) {
    mMultiPlay = multiPlay;
    mChosen = chosen;
    mChoice = choice;
    SetDeferStart(TRUE);
}

void DinoUiChildScannerParentItem::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(2, 0);
        if (mName) {
            mName->SetPosition(MPoint(mPos.x + data_ov004_02174150.mParentNameFocusedOffset.x,
                                      mPos.y + data_ov004_02174150.mParentNameFocusedOffset.y));
        }
    } else {
        mMotion->Play(1, 0);
        if (mName) {
            mName->SetPosition(MPoint(mPos.x + data_ov004_021741d0.mParentNameOffset.x,
                                      mPos.y + data_ov004_021741d0.mParentNameOffset.y));
        }
    }
}

void DinoUiChildScannerParentItem::SetParent(u32 mode, const std::wstring &name) {
    if (mode == mMode) {
        return;
    }
    if (mMode == 0) {
        DinoSoundPlaySe(0x3B0, 0);
    }
    mMode = mode;
    mText = MEncodeUtf8(name);
    mParallel.Clear();
    const char *path;
    BOOL named;
    switch (mMode) {
    case 0:
        path = data_ov004_021740d0.mParentPath0;
        named = FALSE;
        break;
    case 1:
        path = data_ov004_021740d0.mParentPath1;
        named = TRUE;
        break;
    case 2:
        path = data_ov004_021740d0.mParentPath2;
        named = TRUE;
        break;
    }
    mMotion = new MMotion(path, mOam, 0, 1);
    mMotion->SetPosition(mPos);
    mMotion->SetPriority(3);
    mMotion->SetAnchor(3);
    mMotion->Show();
    mParallel.AddTail(mMotion);
    if (named) {
        MSmartObjCanvas *canvas =
            new MSmartObjCanvas(mOam->mDisplay, mOam, MPoint(0x30, 0xC), gDinoResource->GetUnkTable(), TRUE);
        canvas->SetPriority(3);
        canvas->SetAnchor(2);
        mName = new MIndicator(canvas, gDinoResource->CreateFont12(0, 1, 0xB), 0);
        mName->SetText(std::wstring(name));
        mName->Show();
        mParallel.AddTail(mName);
    } else {
        mName = NULL;
    }
    OnFocusChanged(IsFocused());
}

// Forgets the consoles found and scans again.
void DinoUiChildScanner::Rescan() {
    for (u32 i = 0; i < 9; i++) {
        mParents[i]->SetParent(0, std::wstring(L""));
    }
    MTask *start = mMultiPlay->CreateStartScanTask();
    MTask *reset = mMultiPlay->CreateResetTask();
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(reset);
    serial->AddTail(start);
}

void DinoUiChildScanner::OnMessage(MMessage *msg) {
    switch (msg->mId) {
    case 0: {
        MMultiPlayParentFoundMessage *found = (MMultiPlayParentFoundMessage *)msg;
        mParents[found->mUnk0C]->SetParent(*(u32 *)found->mUnk10, std::wstring((const wchar_t *)((const u32 *)found->mUnk10 + 1)));
        break;
    }
    case 2: {
        MMultiPlayParentUpdatedMessage *updated = (MMultiPlayParentUpdatedMessage *)msg;
        mParents[updated->mUnk0C]->SetParent(*(u32 *)updated->mUnk10,
                                             std::wstring((const wchar_t *)((const u32 *)updated->mUnk10 + 1)));
        break;
    }
    case 1: {
        MMultiPlayParentLostMessage *lost = (MMultiPlayParentLostMessage *)msg;
        mParents[lost->mUnk0C]->SetParent(0, std::wstring(L""));
        break;
    }
    }
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

inline DinoUiWirelessCancelItem::DinoUiWirelessCancelItem() {
    MSmartOam *oam = new MSmartOam(1, 0, 1);
    mMotion = new MMotion(data_ov004_021740d0.mCancelPath, oam, 0, 1);
    mMotion->Play(2, 0);
    mMotion->Show();
    mMotion->SetPosition(0x80, 0x60);
    MParallelTaskSet *parallel = &mParallel;
    parallel->AddTail(mMotion);
    parallel->AddTail(oam);
    SetNoFocus(TRUE);
    SetKeys(2);
}

void DinoUiWirelessCancel::OnStart() {
    *mCancelled = FALSE;
    mButton = new DinoUiWirelessCancelItem;
    AddItem(mButton);
}

// (Not inlined into the battle parent scanner, which calls it.)
#pragma auto_inline off
// NONMATCHING: the game gives the sound task and the set each other's registers
MTask *DinoUiWirelessCancel::CreateCloseTask() {
    DinoUiWirelessCancelItem *button = mButton;
    MTask *wait = button->mMotion->CreateWaitStopTask();
    MTask *play = button->mMotion->CreatePlayTransfer(4, 0);
    MTask *se = DinoSoundCreatePlaySeTask(0x38F, 0);
    MSerialTaskSet *close = new MSerialTaskSet(TRUE);
    close->AddTail(se);
    close->AddTail(play);
    close->AddTail(wait);
    return close;
}
#pragma auto_inline reset

DinoUiCannonChild::DinoUiCannonChild(MMultiPlay *multiPlay, u32 choice)
    : MProcess(NULL), mMultiPlay(multiPlay), mChoice(choice), mBack("/image/m_back00", 0),
      mSenryuDefs(gDinoResource->LoadSenryuDefs()) {
    mState = data_ov004_021742d0.ChildStart_5C;
}

extern const u32 data_0208ab50;
extern MTextPicture *data_ov004_02177284; // the view the cross-fade keeps (the bottom screen's backdrop)

// Fades the backdrop's palette in; with no fossils to shoot, says so first.
void DinoUiCannonChild::StateStart() {
    MPalAnime *fade = new MPalAnime(data_ov004_02177284->GetPalBlock(), 0, 0x7FFF);
    fade->AddFrame((const u16 *)mBack.GetPalette());
    fade->AddBlend((const u16 *)mBack.GetPalette(), 0, 16);
    fade->SetDuration(5);
    mSerial.AddTail(fade);
    if (gDinoBackup->IsUnk14CEmpty()) {
        MTask *box = new DinoCommonWindowMessageBox(MPoint(0x80, 0x60), gDinoResource->GetText(data_0208ab50), 4);
        mSerial.AddTail(box);
        mState = data_ov004_02174350.Child021674f8_54;
    } else {
        mState = data_ov004_02174250.StateShootMenu_14;
    }
}

// Fades the backdrop's palette out, resets the connection and leaves.
void DinoUiCannonChild::Leave() {
    MPalAnime *fade = new MPalAnime(data_ov004_02177284->GetPalBlock(), 0, 0x7FFF);
    fade->AddBlend((const u16 *)mBack.GetPalette(), 0, 16);
    fade->AddFrame((const u16 *)mBack.GetPalette());
    fade->SetDuration(5);
    MTask *reset;
    MTask *exit;
    exit = MTaskExitTransfer::Create(this);
    reset = mMultiPlay->CreateResetTask();
    MParallelTaskSet *closing = new MParallelTaskSet(TRUE);
    closing->AddTail(reset);
    closing->AddTail(fade);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(closing);
    serial->AddTail(exit);
}

// Joins the chosen parent with the owner's name.
void DinoUiCannonChild::StateJoin() {
    DinoPlayerData player;
    gDinoBackup->GetPlayer(&player, TRUE);
    DinoUiWirelessSsid ssid;
    ssid.mode = 2;
    MIi_CpuCopy16(player.name, ssid.name, sizeof(ssid.name));
    AddSerial(this, mMultiPlay->CreateStartChildTask(mChoice, (const u8 *)&ssid), mMultiPlay->CreateSynchronizeDelayTask());
    mState = data_ov004_02174150.StateWaitStart_2C;
}

extern const u32 data_0208ab7c;

// Leaves if joining failed, or listens to the parent.
void DinoUiCannonChild::StateWaitStart() {
    if (mMultiPlay->IsFailed()) {
        MTask *box = new DinoCommonWindowMessageBox(MPoint(0x80, 0x60), gDinoResource->GetText(data_0208ab7c), 4);
        mSerial.AddTail(box);
        mState = data_ov004_02174150.Leave_3C;
    } else {
        mHub = new MMultiPlayMessageHub(0);
        mHub->SetUnk279(TRUE);
        mHub->AddObserver(this);
        mParallel.AddTail(mHub);
        mState = data_ov004_02174350.StateSendRocks_44;
    }
}

void DinoUiWirelessMatch::StateNone() {
}

// Goes on to what the player chose.
void DinoUiWirelessMatch::StateChosen() {
    switch (mChosen) {
    default:
        mState = data_ov004_02174350.StateFinish_8C;
        break;
    case 0:
        mState = data_ov004_021741d0.StateKeep_4C;
        break;
    case 1:
        mState = data_ov004_02174350.StateCannonChild_7C;
        break;
    case 2:
        mState = data_ov004_02174350.StateBattleParentScan_74;
        break;
    case 3:
        mState = data_ov004_02174350.StateCannonParentScan_6C;
        break;
    }
}

extern const u32 data_0208ab5c;

// After a reset during wireless play: restores the teams, says the connection was lost and leaves.
void DinoUiWirelessMatch::StateAfterReset() {
    DinoSoundLoadScene(1);
    DinoSoundPlayBgm(0x77, 0);
    MClearResetParameter();
    for (u32 i = 0; i < 3; i++) {
        DinoTeam team;
        gDinoBackup->GetTeam(i + 1, &team);
        gDinoBackup->SetTeam(i + 4, &team);
    }
    *mResult = 0;
    AddSerial(this, new DinoCommonWindowMessageBox(MPoint(0x80, 0x60), gDinoResource->GetText(data_0208ab5c), 4),
              MTaskExitTransfer::Create(this));
}

extern const u32 data_0208ad5c, data_0208ad60, data_0208ad64;

// Asks whether to start, over the dimmed screen.
void DinoUiWirelessMatch::StateAsk() {
    mAnswer = TRUE;
    DinoCommonWindowYesNoDialog *dialog =
        new DinoCommonWindowYesNoDialog(gDinoResource->GetText(data_0208ad64), &mAnswer);
    dialog->SetLabels2(gDinoResource->GetText(data_0208ad60), gDinoResource->GetText(data_0208ad5c));
    dialog->SetUnkBC(TRUE);
    gCurrentDisplay->mBlend->Reset();
    gCurrentDisplay->mBlend->SetMode(3);
    MBlend *blend = gCurrentDisplay->mBlend;
    blend->SetTarget1(data_ov004_02177284->GetPlaneMask());
    AddSerial(this, gCurrentDisplay->mBlend->CreateFadeRatioAnime(0, 8, 10, 0), dialog);
    mState = data_ov004_02174250.StateAnswered_4C;
}

// No: fades back and leaves. Yes: stops the music and loads the wireless sounds.
void DinoUiWirelessMatch::StateAnswered() {
    if (mAnswer == FALSE) {
        *mResult = 0;
        AddSerial(this, gCurrentDisplay->mBlend->CreateFadeRatioAnime(8, 0, 10, 0), MTaskExitTransfer::Create(this));
        return;
    }
    MTask *load = DinoSoundCreateLoadSceneTask(0x28);
    MTask *timer = new MTimer(10);
    MTask *stopBgm = DinoSoundCreateStopBgmTask(-1, 10);
    MParallelTaskSet *stopping = new MParallelTaskSet(TRUE);
    stopping->AddTail(stopBgm);
    stopping->AddTail(timer);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(stopping);
    serial->AddTail(load);
    mState = data_ov004_02174150.StateStartWireless_1C;
}

extern const u32 data_0208ab48, data_0208ab60;

// Starts the wireless connection and scans for parents.
void DinoUiWirelessMatch::StateStartWireless() {
    mHelpDisabler = new DinoHelpDisabler;
    MDisableSoftReset();
    MDisableSleep();
    mMultiPlay = new MMultiPlay(0x400868, 2, 9, 0x10000, -1, -1);
    mMultiPlay->SetParentLifetime(1200);
    mParallel.AddTail(mMultiPlay);
    mUnk80 = new DinoMultiPlayIcon;
    mParallel.AddTail(mUnk80);
    mChosen = 2;
    AddSerial(this, mMultiPlay->CreateInitializeTask(),
              new DinoCommonWindowMessageBox(MPoint(0x80, 0x60), gDinoResource->GetText(data_0208ab48), 4),
              new DinoCommonWindowMessageBox(MPoint(0x80, 0x60), gDinoResource->GetText(data_0208ab60), 4),
              gCurrentDisplay->mBlend->CreateFadeRatioAnime(8, 0, 10, 0));
    mState = data_ov004_02174150.StateChildScan_24;
}

// Ends wireless play: brings the field's sounds back and leaves.
void DinoUiWirelessMatch::StateFinish() {
    MEnableSoftReset();
    MEnableSleep();
    *mResult = 0;
    MTask *stopBgm;
    MTask *timer;
    MTask *load;
    MTask *bgm;
    MTask *exit;
    exit = MTaskExitTransfer::Create(this);
    bgm = DinoSoundCreatePlayBgmTask(0x77, 0);
    load = DinoSoundCreateLoadSceneTask(1);
    timer = new MTimer(10);
    stopBgm = DinoSoundCreateStopBgmTask(-1, 10);
    MSerialTaskSet *sounds = new MSerialTaskSet(TRUE);
    sounds->AddTail(stopBgm);
    sounds->AddTail(timer);
    sounds->AddTail(load);
    sounds->AddTail(bgm);
    MTask *finish = mMultiPlay->CreateFinishTask();
    MParallelTaskSet *ending = new MParallelTaskSet(TRUE);
    ending->AddTail(finish);
    ending->AddTail(sounds);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(ending);
    serial->AddTail(exit);
}

// Listens for children through a message hub, from no count.
void DinoUiCannonParentScanner::StateListen() {
    mHub = new MMultiPlayMessageHub(0);
    mHub->SetUnk279(TRUE);
    mHub->AddObserver(this);
    mParallel.AddTail(mHub);
    mRocks.clear();
    mProfiles.clear();
    mState = data_ov004_02174250.StateWait_64;
    mOnMessage = data_ov004_021741d0.OnCountMessage_0C;
}

extern const u32 data_0208ab64;

// Dims the screen, shows the count and starts being a parent with the owner's name.
// NONMATCHING: the game swaps the stack slots of the ones digit and the show transfer
void DinoUiCannonParentScanner::StateStart() {
    gCurrentDisplay->mBlend->Reset();
    gCurrentDisplay->mBlend->SetMode(3);
    MBlend *blend = gCurrentDisplay->mBlend;
    blend->SetTarget1(data_ov004_02177284->GetPlaneMask());
    mCount = 0;
    mOam = new MSmartOam(10, 0, 1);
    mOam->mHidden = TRUE;
    mFrame = new MMotion(data_ov004_021740d0.mCountFramePath, mOam, 0, 1);
    mFrame->SetPosition(0x80, 0x60);
    mFrame->SetPriority(3);
    mFrame->Play(1, 0);
    mFrame->Show();
    mOnes = new MMotion(data_ov004_021740d0.mDigitPath, mOam, 0, 1);
    mOnes->SetPosition(0x88, 0x60);
    mOnes->Play(1, 0);
    mOnes->SetPriority(3);
    mOnes->Show();
    mTens = new MMotion(data_ov004_021740d0.mDigitPath, mOam, 0, 1);
    mTens->SetPosition(0x78, 0x60);
    mTens->Play(1, 0);
    mTens->SetPriority(3);
    mTens->Show();
    AddParallel(this, mFrame, mOnes, mTens, mOam);
    mMultiPlay->AddObserver(this);
    DinoPlayerData player;
    gDinoBackup->GetPlayer(&player, TRUE);
    DinoUiWirelessSsid ssid;
    ssid.mode = 2;
    MIi_CpuCopy16(player.name, ssid.name, sizeof(ssid.name));
    MTask *show = mOam->CreateShowTransfer();
    MTask *fade = gCurrentDisplay->mBlend->CreateFadeRatioAnime(0, 8, 10, 0);
    MTask *start = mMultiPlay->CreateStartParentTask(&ssid, sizeof(ssid), 0);
    MSerialTaskSet *starting = new MSerialTaskSet(TRUE);
    starting->AddTail(start);
    MParallelTaskSet *opening = new MParallelTaskSet(TRUE);
    opening->AddTail(starting);
    opening->AddTail(fade);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(opening);
    serial->AddTail(show);
    mTasks = new MParallelTaskSet;
    mParallel.AddTail(mTasks);
    mState = data_ov004_021742d0.StateShowCancel_04;
}

// Keeps the rocks and profiles the children sent, counts them, and saves.
void DinoUiCannonParentScanner::StateReceived() {
    if (mRocks.size() == 0) {
        mOnMessage = data_ov004_02174150.OnConnectMessage_74;
        mState = data_ov004_02174150.StateWaitChildren_64;
        return;
    }
    u32 i;
    for (i = 0; i < mRocks.size(); i++) {
        gDinoBackup->AddUnk138(&mRocks[i], FALSE);
        mCount = std::min(data_ov004_021740d0.mMaxRockCount, mCount + 1);
    }
    for (i = 0; i < mProfiles.size(); i++) {
        gDinoBackup->AddProfile(&mProfiles[i], 0);
    }
    ShowCount();
    mTasks->Clear();
    char text[0x200];
    sprintf(text, gDinoResource->GetText(data_0208ab64),
            MEncodeUtf8(std::wstring((const wchar_t *)&mSsid[1])).c_str());
    MTask *wait = new DinoCommonWindowMessageWait1(std::string(text), data_ov004_02174350.mMessagePos, 4, 0);
    MTask *se = DinoSoundCreatePlaySeTask(0x10B, 0);
    MParallelTaskSet *parallel = &mParallel;
    parallel->AddTail(wait);
    parallel->AddTail(se);
    MTask *waitExit = MTaskExitTransfer::Create(wait);
    MTask *validate = new DinoBackupWriteValidater(wait, mMultiPlay);
    MTask *save = MCreateBgCallTask(gDinoBackup, data_ov004_02174150.Save_44, 1);
    MTask *validateSave = new DinoBackupWriteValidater(wait, mMultiPlay);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(validateSave);
    serial->AddTail(save);
    serial->AddTail(validate);
    serial->AddTail(waitExit);
    mState = data_ov004_02174350.StateShowCancel_4C;
}

// Collects the children's fossil rocks and profiles, and answers their pings.
// NONMATCHING: the game gives the push_back tags other stack bytes
void DinoUiCannonParentScanner::OnCountMessage(MMessage *msg) {
    switch (msg->mId) {
    case 0x100A5: {
        DinoWirelessCannonKasekiRockSignal *rock = (DinoWirelessCannonKasekiRockSignal *)msg;
        if (!gDinoBackup->CanAddUnk138((const DinoBackupEntry *)rock->mUnk0C, rock->mUnk20, rock->mUnk21,
                                       rock->mUnk24)) {
            break;
        }
        mRocks.push_back(*(const DinoBackupEntry *)rock->mUnk0C);
        break;
    }
    case 0x100A9:
        mProfiles.push_back(*(const DinoPlayerProfile *)((DinoExchangeSenryuSignal *)msg)->mUnk0C);
        break;
    case 0x100A6:
        mUnkF9 = FALSE;
        mHub->SendToAll(new DinoWirelessCannonPongSignal);
        mState = data_ov004_021741d0.State021664c0_6C;
        break;
    case 0x100A7:
        mUnkF9 = TRUE;
        break;
    case 0x100A8:
        break;
    }
}

// The connection was lost: says so.
void DinoUiCannonParentScanner::StateLost() {
    mTasks->Clear();
    MTask *box = new DinoCommonWindowMessageBox(data_ov004_02174350.mMessagePos, gDinoResource->GetText(data_0208ab5c), 4);
    mSerial.AddTail(box);
    mState = data_ov004_021742d0.StateCancelled_1C;
}

// Cancelled: hides the counter, resets the connection and leaves.
void DinoUiCannonParentScanner::StateCancelled() {
    mOam->mHidden = TRUE;
    mTasks->Clear();
    MTask *reset;
    MTask *fade;
    MTask *exit;
    exit = MTaskExitTransfer::Create(this);
    fade = GetDisplay()->mBlend->CreateFadeRatioAnime(8, 0, 10, 0);
    reset = mMultiPlay->CreateResetTask();
    MParallelTaskSet *closing = new MParallelTaskSet(TRUE);
    closing->AddTail(reset);
    closing->AddTail(fade);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(closing);
    serial->AddTail(exit);
}

extern const u32 data_0208ab68;

// Shows "waiting for children" with a cancel button.
void DinoUiCannonParentScanner::StateShowCancel() {
    mTasks->AddTail(new DinoCommonWindowMessageWait2(gDinoResource->GetText(data_0208ab68),
                                                     data_ov004_02174350.mMessagePos, 4, 0),
                    mCancel = new DinoUiWirelessCancel(&mCancelled));
    mOnMessage = data_ov004_02174250.OnConnectMessage_2C;
    mState = data_ov004_02174250.StateWaitChildren_7C;
}

void DinoUiCannonParentScanner::State021664c0() {
    if (mMultiPlay->IsFailed()) {
        mState = data_ov004_02174250.StateLost_34;
        return;
    }
    Dispatch();
    if (mUnkF9) {
        AddSerial(this, new MTimer(5), MTaskExitTransfer::Create(mHub), mMultiPlay->CreateReleaseSynchronizeTask(),
                  mMultiPlay->CreateDisconnectTask(mAid));
        mState = data_ov004_02174350.StateReceived_34;
    }
}

// Waits for children; cancelling hides the counter, fades out and leaves.
void DinoUiCannonParentScanner::StateWaitChildren() {
    if (mMultiPlay->IsFailed()) {
        mState = data_ov004_021740d0.StateLost_54;
        return;
    }
    Dispatch();
    if (mCancelled) {
        MTask *exit = MTaskExitTransfer::Create(this);
        MTask *fade = GetDisplay()->mBlend->CreateFadeRatioAnime(8, 0, 10, 0);
        MTask *reset = mMultiPlay->CreateResetTask();
        MSerialTaskSet *resets = new MSerialTaskSet(TRUE);
        resets->AddTail(reset);
        MParallelTaskSet *closing = new MParallelTaskSet(TRUE);
        closing->AddTail(resets);
        closing->AddTail(fade);
        MTask *hide = mOam->CreateHideTransfer();
        MTask *empty = MTaskSetMakeEmptyTask::Create(mTasks);
        MTask *close = mCancel->CreateCloseTask();
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(close);
        serial->AddTail(empty);
        serial->AddTail(hide);
        serial->AddTail(closing);
        serial->AddTail(exit);
    }
}

// Dims the screen and starts being a parent with the owner's name.
void DinoUiBattleParentScanner::StateStart() {
    gCurrentDisplay->mBlend->Reset();
    gCurrentDisplay->mBlend->SetMode(3);
    MBlend *blend = gCurrentDisplay->mBlend;
    blend->SetTarget1(data_ov004_02177284->GetPlaneMask());
    mMultiPlay->AddObserver(this);
    DinoPlayerData player;
    gDinoBackup->GetPlayer(&player, TRUE);
    mStartData.mode = 1;
    MIi_CpuCopy16(player.name, mStartData.name, sizeof(mStartData.name));
    MTask *fade = gCurrentDisplay->mBlend->CreateFadeRatioAnime(0, 8, 10, 0);
    MTask *start = mMultiPlay->CreateStartParentTask(&mStartData, sizeof(mStartData), 0);
    MSerialTaskSet *starting = new MSerialTaskSet(TRUE);
    starting->AddTail(start);
    MParallelTaskSet *opening = new MParallelTaskSet(TRUE);
    opening->AddTail(starting);
    opening->AddTail(fade);
    mSerial.AddTail(opening);
    mTasks = new MParallelTaskSet;
    mParallel.AddTail(mTasks);
    mState = data_ov004_021742d0.StateShowCancel_24;
}

extern const u32 data_0208ab54;

// Sends off the fossil rocks shot: takes them out of the backup and saves.
void DinoUiCannonChild::StateSendRocks() {
    for (u32 i = 0; i < mUnkB8.size(); i++) {
        DinoBackupEntry entry;
        gDinoBackup->GetUnk14C((u32)mUnkB8[i], &entry);
        mRocks.push_back(entry);
        MI_CpuFill8(&entry, 0, sizeof(entry));
        gDinoBackup->SetUnk14C((u32)mUnkB8[i], &entry);
    }
    gDinoBackup->CompactUnk14C();
    MTask *wait = new DinoCommonWindowMessageWait1(std::string(gDinoResource->GetText(data_0208ab54)), MPoint(0x80, 0x60),
                                                   4, 0);
    mParallel.AddTail(wait);
    MTask *waitExit = MTaskExitTransfer::Create(wait);
    MTask *validate = new DinoBackupWriteValidater(wait, mMultiPlay);
    MTask *save = MCreateBgCallTask(gDinoBackup, data_ov004_02174250.Save_04, 1);
    MTask *validateSave = new DinoBackupWriteValidater(wait, mMultiPlay);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(validateSave);
    serial->AddTail(save);
    serial->AddTail(validate);
    serial->AddTail(waitExit);
    mState = data_ov004_02174150.StateSendProfile_14;
}

extern const u32 data_0208ab70;

// Sends the rocks and the owner's profile to the parent, then pings it.
void DinoUiCannonChild::StateSendProfile() {
    for (u32 i = 0; i < mRocks.size(); i++) {
        mHub->Broadcast(new DinoWirelessCannonKasekiRockSignal(mRocks[i]));
    }
    DinoPlayerProfile profile;
    gDinoBackup->MakeProfile(&profile);
    if (profile.unk78) {
        mHub->Broadcast(new DinoExchangeSenryuSignal(profile));
    }
    mHub->Broadcast(new DinoWirelessCannonPingSignal);
    mStarted = FALSE;
    DinoSoundPlaySe(0x10B, 0);
    MTask *wait = new DinoCommonWindowMessageWait1(std::string(gDinoResource->GetText(data_0208ab70)), MPoint(0x80, 0x60),
                                                   4, 0);
    mParallel.AddTail(wait);
    MTask *exit = MTaskExitTransfer::Create(wait);
    AddSerial(this, new MTimer(60), exit);
    mState = data_ov004_021740d0.State021672f8_64;
}

extern const u32 data_0208ab6c;

// Waits for the shot; if the connection fails, says so (after putting the rocks back and saving, if the shot never
// started) and leaves.
void DinoUiCannonChild::State021672f8() {
    Dispatch();
    if (!mMultiPlay->IsFailed()) {
        return;
    }
    if (mStarted) {
        MTask *box = new DinoCommonWindowMessageBox(MPoint(0x80, 0x60), gDinoResource->GetText(data_0208ab6c), 4);
        mSerial.AddTail(box);
        mState = data_ov004_02174150.Leave_0C;
        return;
    }
    for (u32 i = 0; i < mRocks.size(); i++) {
        gDinoBackup->SetUnk14C(gDinoBackup->FindFreeUnk14C(), &mRocks[i]);
    }
    MTask *wait = new DinoCommonWindowMessageWait1(std::string(gDinoResource->GetText(data_0208ab54)), MPoint(0x80, 0x60),
                                                   4, 0);
    mParallel.AddTail(wait);
    MTask *waitExit = MTaskExitTransfer::Create(wait);
    MTask *validate = new DinoBackupWriteValidater(wait, mMultiPlay);
    MTask *save = MCreateBgCallTask(gDinoBackup, data_ov004_021740d0.Save_7C, 1);
    MTask *validateSave = new DinoBackupWriteValidater(wait, mMultiPlay);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(validateSave);
    serial->AddTail(save);
    serial->AddTail(validate);
    serial->AddTail(waitExit);
    MTask *box = new DinoCommonWindowMessageBox(MPoint(0x80, 0x60), gDinoResource->GetText(data_0208ab7c), 4);
    mSerial.AddTail(box);
    mState = data_ov004_021740d0.Leave_3C;
}

// Ends the scan and leaves after half a second.
void DinoUiChildScanner::Leave() {
    MTask *exit = MTaskExitTransfer::Create(this);
    MTask *endScan = mMultiPlay->CreateEndScanTask();
    MSerialTaskSet *ending = new MSerialTaskSet(TRUE);
    ending->AddTail(endScan);
    MTask *timer = new MTimer(30);
    MParallelTaskSet *leaving = new MParallelTaskSet(TRUE);
    leaving->AddTail(timer);
    leaving->AddTail(ending);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(leaving);
    serial->AddTail(exit);
}

// (The game calls the connections' constructors defined below.)
#pragma dont_inline on
void DinoUiChildScanner::OnDecide(MMenuItem *item) {
    if (item == mCancelItem) {
        DinoSoundPlaySe(0x38F, 0);
        *mChosen = 4;
        Leave();
        return;
    }
    if (item == mStartBattleItem) {
        DinoSoundPlaySe(0x390, 0);
        *mChosen = 2;
        Leave();
        return;
    }
    if (item == mStartCannonItem) {
        DinoSoundPlaySe(0x390, 0);
        *mChosen = 3;
        Leave();
        return;
    }
    DinoUiChildScannerParentItem *parent = dynamic_cast<DinoUiChildScannerParentItem *>(item);
    if (parent) {
        if (parent->mMode == 1) {
            DinoSoundPlaySe(0x390, 0);
            mSerial.AddTail(new DinoUiChildScannerBattleConnect(this, mMultiPlay, parent));
        } else if (parent->mMode == 2) {
            DinoSoundPlaySe(0x390, 0);
            mSerial.AddTail(new DinoUiChildScannerCannonConnect(this, mMultiPlay, parent));
        } else {
            DinoSoundPlaySe(0xA4, 0);
        }
    } else {
        DinoSoundPlaySe(0x390, 0);
    }
}

#pragma dont_inline reset

void DinoUiChildScannerBattleConnect::IgnoreMessage(MMessage *msg) {
}

DinoUiChildScannerBattleConnect::DinoUiChildScannerBattleConnect(DinoUiChildScanner *scanner, MMultiPlay *multiPlay,
                                                                 DinoUiChildScannerParentItem *parent)
    : MProcess(NULL), mScanner(scanner), mMultiPlay(multiPlay), mParent(parent), mUnkD0(parent->mUnk8C),
      mPos(parent->mPos.x, parent->mPos.y), mText(parent->GetName()) {
    mState = data_ov004_021742d0.BattleStart_6C;
    mOnMessage = data_ov004_02174350.IgnoreMessage_04;
}

void DinoUiChildScannerCannonConnect::IgnoreMessage(MMessage *msg) {
}

void DinoUiChildScannerBattleConnect::StateStart() {
    mUnkE8 = new MParallelTaskSet;
    mParallel.AddTail(mUnkE8);
    mMultiPlay->AddObserver(this);
    mState = data_ov004_02174350.StateConnect_14;
}

extern const u32 data_0208ab90, data_0208ab94, data_0208ad70, data_0208ad74;


static inline void AddMarkedChoice(DinoCommonWindowMenu *window, u32 id, const std::string &help,
                                   const std::string &text) {
    window->AddChoice(id, text, help);
}

// Offers to connect (or not) to the console, by its item.
// NONMATCHING: the game keeps destroy flags for the marked choice's temporaries, and swaps r6/r7
void DinoUiChildScannerBattleConnect::StateConnect() {
    u32 anchor;
    u32 connect = data_0208ab94;
    mChoice = connect;
    switch (mUnkD0 % 3) {
    case 0:
        anchor = 0;
        break;
    case 1:
        anchor = 5;
        break;
    case 2:
        anchor = 1;
        break;
    }
    DinoCommonWindowMenu *window = new DinoCommonWindowMenu(&mChoice, mPos, NULL, anchor);
    {
        std::string text(gDinoResource->GetText(connect));
        std::string help(gDinoResource->GetText(data_0208ad74));
        window->AddChoice(connect, text, help);
        AddMarkedChoice(window, data_0208ab90, gDinoResource->GetText(data_0208ad70),
                        std::string("*") + gDinoResource->GetText(data_0208ab90));
    }
    mUnkE8->AddTail(window);
    mState = data_ov004_02174350.StateChoose_24;
    mOnMessage = data_ov004_02174250.OnConnectMessage_44;
}

// Drops the window if the console goes away or changes.
void DinoUiChildScannerBattleConnect::OnConnectMessage(MMessage *msg) {
    switch (msg->mId) {
    case 2: {
        MMultiPlayParentUpdatedMessage *updated = (MMultiPlayParentUpdatedMessage *)msg;
        if (updated->mUnk0C == mUnkD0) {
            mUnkE8->Clear();
            mChoice = data_0208ab90;
        }
        break;
    }
    case 1: {
        MMultiPlayParentLostMessage *lost = (MMultiPlayParentLostMessage *)msg;
        if (lost->mUnk0C == mUnkD0) {
            mUnkE8->Clear();
            mChoice = data_0208ab90;
        }
        break;
    }
    }
}

extern const u32 data_0208ab7c, data_0208ab84, data_0208ab88, data_0208ab8c, data_0208ad68, data_0208ad6c;

// Drops the window if the console goes away or changes.
void DinoUiChildScannerCannonConnect::OnConnectMessage(MMessage *msg) {
    switch (msg->mId) {
    case 2: {
        MMultiPlayParentUpdatedMessage *updated = (MMultiPlayParentUpdatedMessage *)msg;
        if (updated->mUnk0C == mUnkD0) {
            mUnkE8->Clear();
            mChoice = data_0208ab88;
        }
        break;
    }
    case 1: {
        MMultiPlayParentLostMessage *lost = (MMultiPlayParentLostMessage *)msg;
        if (lost->mUnk0C == mUnkD0) {
            mUnkE8->Clear();
            mChoice = data_0208ab88;
        }
        break;
    }
    }
}

// Once the window closes: leaves on "don't join", says the console can't be joined unless it's a cannon game, or
// joins.
void DinoUiChildScannerCannonConnect::StateChoose() {
    mScanner->MMessageObserver::Dispatch();
    MMessageObserver::Dispatch();
    if (mUnkE8->IsEmpty()) {
        if (mChoice == data_0208ab88) {
            Exit();
            return;
        }
        if (mParent->mMode != 2) {
            MTask *box = new DinoCommonWindowMessageBox(MPoint(0x80, 0x60), gDinoResource->GetText(data_0208ab7c), 4);
            MTask *exit = MTaskExitTransfer::Create(this);
            MSerialTaskSet *serial = &mSerial;
            serial->AddTail(box);
            serial->AddTail(exit);
        } else {
            mSerial.AddTail(mMultiPlay->CreateEndScanTask());
            mState = data_ov004_02174250.StateConnecting_54;
            mOnMessage = data_ov004_021742d0.IgnoreMessage_2C;
        }
    }
}

// Picks the console once it's still a cannon game; otherwise says so and scans again.
void DinoUiChildScannerCannonConnect::StateConnecting() {
    mScanner->MMessageObserver::Dispatch();
    MMessageObserver::Dispatch();
    if (mParent->mMode != 2) {
        MTask *box = new DinoCommonWindowMessageBox(MPoint(0x80, 0x60), gDinoResource->GetText(data_0208ab7c), 4);
        mScanner->Rescan();
        MTask *exit = MTaskExitTransfer::Create(this);
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(box);
        serial->AddTail(exit);
    } else {
        mScanner->Choose(mParent->mUnk8C);
        mSerial.AddTail(MTaskExitTransfer::Create(this));
    }
}

// Offers to join (or not) the console, by its item.
void DinoUiChildScannerCannonConnect::StateConnect() {
    u32 anchor;
    u32 join = data_0208ab8c;
    mChoice = join;
    switch (mUnkD0 % 3) {
    case 0:
        anchor = 0;
        break;
    case 1:
        anchor = 5;
        break;
    case 2:
        anchor = 1;
        break;
    }
    DinoCommonWindowMenu *window = new DinoCommonWindowMenu(&mChoice, mPos, NULL, anchor);
    window->AddChoice(join, gDinoResource->GetText(join), gDinoResource->GetText(data_0208ad6c));
    window->AddChoice(data_0208ab88, std::string("*") + gDinoResource->GetText(data_0208ab88),
                      gDinoResource->GetText(data_0208ad68));
    mUnkE8->AddTail(window);
    mState = data_ov004_021741d0.StateChoose_14;
    mOnMessage = data_ov004_021742d0.OnConnectMessage_34;
}


// Once the window closes: leaves on "don't connect", says the console can't be joined unless it's a battle, or
// connects.
void DinoUiChildScannerBattleConnect::StateChoose() {
    mScanner->MMessageObserver::Dispatch();
    MMessageObserver::Dispatch();
    if (mUnkE8->IsEmpty()) {
        if (mChoice == data_0208ab90) {
            Exit();
            return;
        }
        if (mParent->mMode != 1) {
            MTask *box = new DinoCommonWindowMessageBox(MPoint(0x80, 0x60), gDinoResource->GetText(data_0208ab7c), 4);
            MTask *exit = MTaskExitTransfer::Create(this);
            MSerialTaskSet *serial = &mSerial;
            serial->AddTail(box);
            serial->AddTail(exit);
        } else {
            mState = data_ov004_021742d0.StateConnecting_14;
            mOnMessage = data_ov004_02174350.IgnoreMessage_0C;
        }
    }
}

// Dims the screen, says it's connecting and joins the console.
void DinoUiChildScannerBattleConnect::StateConnecting() {
    gCurrentDisplay->mBlend->Reset();
    gCurrentDisplay->mBlend->SetMode(3);
    MBlend *blend = gCurrentDisplay->mBlend;
    blend->SetTarget1(data_ov004_02177284->GetPlaneMask() | 0x10);
    gCurrentDisplay->mBlend->SetTarget2(0x3F);
    gCurrentDisplay->mBlend->SetAlpha(16, 0);
    char text[0x100];
    sprintf(text, gDinoResource->GetText(data_0208ab84), mText.c_str());
    mUnkE8->AddTail(new DinoCommonWindowMessageWait1(std::string(text), MPoint(0x80, 0x60), 4, TRUE));
    DinoPlayerData player;
    gDinoBackup->GetPlayer(&player, TRUE);
    DinoUiWirelessSsid ssid;
    ssid.mode = 1;
    MIi_CpuCopy16(player.name, ssid.name, sizeof(ssid.name));
    AddSerial(this, gCurrentDisplay->mBlend->CreateFadeRatioAnime(0, 8, 10, 0), mMultiPlay->CreateEndScanTask(),
              mMultiPlay->CreateStartChildTask(mUnkD0, (const u8 *)&ssid), mMultiPlay->CreateSynchronizeDelayTask());
    mState = data_ov004_021742d0.StateCancel_7C;
}

void DinoUiChildScannerCannonConnect::StateStart() {
    mUnkE8 = new MParallelTaskSet;
    mParallel.AddTail(mUnkE8);
    mMultiPlay->AddObserver(this);
    mState = data_ov004_021740d0.StateConnect_6C;
}

DinoUiChildScannerCannonConnect::DinoUiChildScannerCannonConnect(DinoUiChildScanner *scanner, MMultiPlay *multiPlay,
                                                                 DinoUiChildScannerParentItem *parent)
    : MProcess(NULL), mScanner(scanner), mMultiPlay(multiPlay), mParent(parent), mUnkD0(parent->mUnk8C),
      mPos(parent->mPos.x, parent->mPos.y), mText(parent->GetName()) {
    mState = data_ov004_021742d0.CannonStart_44;
    mOnMessage = data_ov004_021741d0.IgnoreMessage_44;
}

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
                  mMultiPlay->CreateStartParentTask(&mStartData, sizeof(mStartData), 0));
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
    mState = data_ov004_021741d0.StateStart_7C;
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
        mState = data_ov004_021740d0.StateListen_74;
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
