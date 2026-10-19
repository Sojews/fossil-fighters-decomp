// DinoUiExchange.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "m/display.h"
#include "game/dino_ui_exchange.h"
#include "types.h"
#include "m/blend.h"
#include "m/bg_new_task.h"
#include "game/dino_backup.h"
#include "game/dino_resource.h"
#include "m/seed_generator.h"
#include "game/dino_message.h"
#include "m/smart_oam.h"
#include "game/dino_sound.h"
#include "m/sleep.h"
#include "m/soft_reset.h"
#include "game/dino_common_window_message_box.h"
#include "game/dino_common_window_yes_no_dialog.h"
#include "game/dino_ui_wireless_cancel_item_menu.h"

DinoUiExchange::~DinoUiExchange() {
    GetDisplay()->mBlend->Reset();
}

void DinoUiExchange::Idle() {
    (this->*mState)();
}

void DinoUiExchange::OnMessage(MMessage *msg) {
    (this->*mOnMessage)(msg);
}

#pragma auto_inline off

static inline const u32 *ExchangeIds(const DinoExcavateDefs *data) {
    return (const u32 *)((const u8 *)data + data->exchangeOffset);
}

static inline u32 RandomExchangeId() {
    MResource<DinoExcavateDefs> defs = gDinoResource->GetExcavateDefs();
    MResource<DinoExcavateDefs> defs2 = gDinoResource->GetExcavateDefs();
    u32 index = gDinoResource->GetRandom()->Next(defs2.Get()->exchangeCount);
    return ExchangeIds(defs.Get())[index];
}

// NONMATCHING: the two excavate resources swap stack slots.
void DinoUiExchange::MakeExchangeEntry(DinoBackupEntry *entry) {
    u32 id = RandomExchangeId();
    MResource<DinoKasekiDefs> kaseki = gDinoResource->LoadKasekiDefs();
    const DinoKasekiDef *def = kaseki.Get()->GetDef(id);
    MI_CpuFill8(entry, 0, sizeof(DinoBackupEntry));
    entry->id = id;
    entry->type = def->unk24;
    entry->unk10_0 = gDinoResource->GetCreatureUnk0D((id - 1) / 4 + 1);
}

void DinoUiExchange::InitProfile(DinoPlayerProfile *profile) {
    gDinoBackup->InitProfile(profile);
}

void DinoUiExchange::ShowCount() {
    mOnes->Play(mCount % 10 + 1, 0);
    mTens->Play(mCount / 10 + 1, 0);
}

// Starts over as parent or child at random, after a random wait.
void DinoUiExchange::Start() {
    ClearQueue();
    mUnkE4 = gDinoResource->GetRandom()->Next(60) + 60;
    mMode = gDinoResource->GetRandom()->Next(2);
    mSerial.AddTail(mMultiPlay->CreateResetTask());
    if (mMode == 1) {
        mSerial.AddTail(mMultiPlay->CreateStartParentTask(mBeacon, sizeof(mBeacon), 0));
    } else {
        mSerial.AddTail(mMultiPlay->CreateStartScanTask());
    }
}

// Ends the wireless session (and the scan, unless it is all there is).
void DinoUiExchange::Reset() {
    if (mMode == 1) {
        mSerial.AddTail(mMultiPlay->CreateResetTask());
    } else {
        MTask *reset = mMultiPlay->CreateResetTask();
        MTask *endScan = mMultiPlay->CreateEndScanTask();
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(endScan);
        serial->AddTail(reset);
    }
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MBgCallTask0<DinoBackup>;
template MTask *MCreateBgCallTask(DinoBackup *obj, void (DinoBackup::*func)(), int thread);

// The set of partners' addresses. The game built its search loops without strength reduction.
// NONMATCHING: insert_node_at (register allocation) and insert_unique (its true/false constants sit one byte
// further into the file's pooled data, after one an undecompiled function uses).
#pragma opt_strength_reduction off
template class std::__red_black_tree<DinoMacAddress, DinoMacAddress, std::__identity<DinoMacAddress>,
                                     std::less<DinoMacAddress>, std::allocator<DinoMacAddress> >;
#pragma opt_strength_reduction reset

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x14];
    void (DinoUiExchange::*State02156acc_14)(); // 0x14
    void (DinoUiExchange::*StateExchange_1C)(); // 0x1c
    void (DinoUiExchange::*OnScanMessage_24)(MMessage *msg); // 0x24
    void (DinoUiExchange::*StateRestart_2C)(); // 0x2c
    u8 unk34[0x3C - 0x34];
    void (DinoUiExchange::*StateAsk_3C)(); // 0x3c
    u8 unk44[0x4C - 0x44];
    void (DinoUiExchange::*StateQuit_4C)(); // 0x4c
    void (DinoUiExchange::*StateRestart_54)(); // 0x54
    u8 unk5C[0x74 - 0x5C];
    void (DinoUiExchange::*OnScanMessage_74)(MMessage *msg); // 0x74
    void (DinoUiExchange::*StateCancelled_7C)(); // 0x7c (its second word is in the next block)
} data_ov004_02171de0;
extern "C" struct {
    u8 unk00[0x4];
    void (DinoUiExchange::*OnExchangeMessage_04)(MMessage *msg); // 0x04
    void (DinoUiExchange::*OnNoMessage_0C)(MMessage *msg); // 0x0c
    void (DinoUiExchange::*StateSearch_14)(); // 0x14
    void (DinoUiExchange::*State02156718_1C)(); // 0x1c
    void (DinoUiExchange::*State02156acc_24)(); // 0x24
    void (DinoUiExchange::*StateEnd_2C)(); // 0x2c
    void (DinoUiExchange::*Unk02155f24_34)();
    MPoint mMessagePos;  // 0x3c
    void (DinoUiExchange::*StateRestart_44)(); // 0x44
    void (DinoUiExchange::*OnNoMessage_4C)(MMessage *msg); // 0x4c
    void (DinoUiExchange::*StateCheck_54)(); // 0x54
    u8 unk5C[0x6C - 0x5C];
    void (DinoUiExchange::*StateStart_6C)(); // 0x6c
    void (DinoUiExchange::*StateSearch_74)(); // 0x74
    void (DinoUiExchange::*StateFinish_7C)(); // 0x7c (its second word is in the next block)
} data_ov004_02171e60;
extern "C" struct {
    u8 unk00[0x4];
    void (DinoUiExchange::*OnNoMessage_04)(MMessage *msg); // 0x04
    void (DinoUiExchange::*StateLeave_0C)(); // 0x0c
    void (DinoUiExchange::*State02156718_14)(); // 0x14
} data_ov004_02171ee0;

DinoUiExchange::DinoUiExchange() : MProcess(NULL) {
    mState = data_ov004_02171e60.StateStart_6C;
}

extern MView *data_ov004_02177284; // the view the cross-fade keeps
extern const u32 data_0208ab50;

// Dims the screen and fades it in.
void DinoUiExchange::StateStart() {
    gCurrentDisplay->mBlend->Reset();
    gCurrentDisplay->mBlend->SetMode(3);
    MBlend *blend = gCurrentDisplay->mBlend;
    blend->SetTarget1(data_ov004_02177284->GetPlaneMask());
    mCount = 0;
    SetNextTime();
    mSerial.AddTail(gCurrentDisplay->mBlend->CreateFadeRatioAnime(0, 8, 10, 0));
    mState = data_ov004_02171e60.StateCheck_54;
}

// With no fossils to exchange, says so and leaves.
void DinoUiExchange::StateCheck() {
    if (gDinoBackup->IsUnk14CEmpty()) {
        MTask *box = new DinoCommonWindowMessageBox(MPoint(0x80, 0x60), gDinoResource->GetText(data_0208ab50), 4);
        MTask *exit = MTaskExitTransfer::Create(this);
        MTask *fade = gCurrentDisplay->mBlend->CreateFadeRatioAnime(8, 0, 10, 0);
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(box);
        serial->AddTail(fade);
        serial->AddTail(exit);
    } else {
        mState = data_ov004_02171de0.StateAsk_3C;
    }
}

extern const u32 data_0208ab44, data_0208ab4c, data_0208ad54, data_0208ad58;

// Asks whether to exchange.
void DinoUiExchange::StateAsk() {
    mAnswer = TRUE;
    DinoCommonWindowYesNoDialog *dialog = new DinoCommonWindowYesNoDialog(gDinoResource->GetText(data_0208ab4c), &mAnswer);
    dialog->SetLabels2(gDinoResource->GetText(data_0208ad58), gDinoResource->GetText(data_0208ad54));
    dialog->SetUnkBC(TRUE);
    mSerial.AddTail(dialog);
    mState = data_ov004_02171e60.StateEnd_2C;
}

// Starts over; shows "searching" with a cancel button unless already showing.
void DinoUiExchange::StateRestart() {
    Start();
    if (mTasks->IsEmpty()) {
        mTasks->AddTail(new DinoCommonWindowMessageWait2(gDinoResource->GetText(data_0208ab44),
                                                         data_ov004_02171e60.mMessagePos, 4, 0),
                        mCancel = new DinoUiWirelessCancel(&mCancelled));
    }
    mState = data_ov004_02171e60.StateSearch_74;
    mOnMessage = data_ov004_02171de0.OnScanMessage_24;
}

// Searches until a partner turns up; restarts as the other side every so often, and on failure.
void DinoUiExchange::StateSearch() {
    if (mMultiPlay->IsFailed()) {
        mSerial.AddTail(mMultiPlay->CreateResetTask());
        Start();
        return;
    }
    if (mCancelled) {
        mSerial.AddTail(mCancel->CreateCloseTask());
        Reset();
        mState = data_ov004_02171de0.StateCancelled_7C;
        return;
    }
    mUnkE4--;
    if (mUnkE4 == 0) {
        Reset();
        Start();
    } else {
        Dispatch();
    }
}

extern const u32 data_0208ab40, data_0208ad4c, data_0208ad50;

// A partner found while scanning (taken once the retry wait is over), or a child's join as the parent or the child.
void DinoUiExchange::OnScanMessage(MMessage *msg) {
    switch (msg->mId) {
    case 0x1A:
        if (gDinoBackup->GetPlayFrames() < mNextTime) {
            return;
        }
        {
            MI_CpuCopy8((u8 *)msg + 0x80, mPartnerMac, 6);
            {
                MResource<DinoExcavateDefs> defs = gDinoResource->GetExcavateDefs();
                u32 wait = defs.Get()->exchangeRetryWait;
                u64 frames = gDinoBackup->GetPlayFrames();
                mNextTime = frames + wait;
            }
            mIsParent = TRUE;
            MakeExchangeEntry(&mRock);
            InitProfile(&mProfile);
            for (u32 i = 0; i < 9; i++) {
                mPartnerName[i] = mProfile.name[i];
            }
            Reset();
            mOnMessage = data_ov004_02171e60.OnNoMessage_4C;
            mState = data_ov004_02171e60.State02156acc_24;
        }
        break;
    case 0:
        if (mMode == 0) {
            MI_CpuCopy8((u8 *)msg + 0x80, mPartnerMac, 6);
            MI_CpuCopy8((u8 *)msg + 0x10, mPartnerInfo, 0x18);
            GetSerial()->AddTail(mMultiPlay->CreateEndScanTask(),
                                 mMultiPlay->CreateStartChildTask(*(u32 *)((u8 *)msg + 0xC), mBeacon),
                                 mMultiPlay->CreateSynchronizeDelayTask());
            mOnMessage = data_ov004_02171e60.OnNoMessage_0C;
            mState = data_ov004_02171e60.State02156718_1C;
        }
        break;
    case 5:
        if (mMode == 1) {
            MI_CpuCopy8((u8 *)msg + 0x26, mPartnerMac, 6);
            MI_CpuCopy8((u8 *)msg + 0xE, mPartnerInfo, 0x18);
            mSerial.AddTail(mMultiPlay->CreateSynchronizeDelayTask());
            mOnMessage = data_ov004_02171ee0.OnNoMessage_04;
            mState = data_ov004_02171ee0.State02156718_14;
        }
        break;
    }
}

void DinoUiExchange::StateJoined() {
    if (mMultiPlay->IsFailed()) {
        mSerial.AddTail(mMultiPlay->CreateResetTask());
        Start();
        mOnMessage = data_ov004_02171de0.OnScanMessage_74;
        mState = data_ov004_02171e60.StateSearch_14;
        return;
    }
    mHub = new MMultiPlayMessageHub(0);
    mHub->SetUnk279(TRUE);
    mHub->AddObserver(this);
    mParallel.AddTail(mHub);
    mIsParent = FALSE;
    MI_CpuFill8(&mRock, 0, sizeof(mRock));
    MI_CpuFill8(&mProfile, 0, sizeof(mProfile));
    DinoBackupEntry entry;
    gDinoBackup->GetUnk14C(0, &entry);
    mHub->Broadcast(new DinoExchangeKasekiRockSignal(entry));
    DinoPlayerProfile profile;
    gDinoBackup->MakeProfile(&profile);
    if (profile.unk78) {
        mHub->Broadcast(new DinoExchangeSenryuSignal(profile));
    }
    mHub->SendToAll(new DinoExchangeDoneSignal(mMultiPlay->GetPlayerIndex()));
    mDone[0] = mDone[1] = FALSE;
    mState = data_ov004_02171de0.StateExchange_1C;
    mOnMessage = data_ov004_02171e60.OnExchangeMessage_04;
}

// Cancelled: asks whether to stop exchanging.
void DinoUiExchange::StateCancelled() {
    mTasks->Clear();
    mAnswer = TRUE;
    DinoCommonWindowYesNoDialog *dialog = new DinoCommonWindowYesNoDialog(gDinoResource->GetText(data_0208ab40), &mAnswer);
    dialog->SetLabels2(gDinoResource->GetText(data_0208ad50), gDinoResource->GetText(data_0208ad4c));
    mSerial.AddTail(dialog);
    mState = data_ov004_02171de0.StateQuit_4C;
}

void DinoUiExchange::StateQuit() {
    if (mAnswer == TRUE) {
        mState = data_ov004_02171ee0.StateLeave_0C;
    } else {
        mState = data_ov004_02171de0.StateRestart_54;
    }
}

void DinoUiExchange::StateLeave() {
    mSerial.AddTail(mMultiPlay->CreateFinishTask());
    mState = data_ov004_02171e60.StateFinish_7C;
}

extern const u32 data_0208ab38, data_0208ab3c;

// Ends wireless play and says how many exchanges were made (if any), then fades out and leaves.
void DinoUiExchange::StateFinish() {
    MEnableSoftReset();
    MEnableSleep();
    DinoSoundPlayBgm(0x77, 0);
    if (mCount != 0) {
        MTask *box = new DinoCommonWindowMessageBox(data_ov004_02171e60.mMessagePos, gDinoResource->GetText(data_0208ab3c), 4);
        MTask *exit = MTaskExitTransfer::Create(this);
        MTask *fade = gCurrentDisplay->mBlend->CreateFadeRatioAnime(8, 0, 10, 0);
        MTask *hide = mOam->CreateHideTransfer();
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(box);
        serial->AddTail(hide);
        serial->AddTail(fade);
        serial->AddTail(exit);
    } else {
        MTask *box = new DinoCommonWindowMessageBox(data_ov004_02171e60.mMessagePos, gDinoResource->GetText(data_0208ab38), 4);
        MTask *exit = MTaskExitTransfer::Create(this);
        MTask *fade = gCurrentDisplay->mBlend->CreateFadeRatioAnime(8, 0, 10, 0);
        MTask *hide = mOam->CreateHideTransfer();
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(box);
        serial->AddTail(hide);
        serial->AddTail(fade);
        serial->AddTail(exit);
    }
}

static inline void AddSerial(MProcess *process, MTask *a, MTask *b, MTask *c) {
    MSerialTaskSet *serial = &process->mSerial;
    serial->AddTail(a);
    serial->AddTail(b);
    serial->AddTail(c);
}

// Does nothing (what the pools point at while there is nothing to do).
void DinoUiExchange::StateNone() {
}

// Waits until both sides are done, then ends the session.
void DinoUiExchange::StateExchange() {
    Dispatch();
    if (mMultiPlay->IsFailed()) {
        mHub->Exit();
        mState = data_ov004_02171de0.StateRestart_2C;
        return;
    }
    if (mDone[0] && mDone[1]) {
        AddSerial(this, new MTimer(5), MTaskExitTransfer::Create(mHub), mMultiPlay->CreateResetTask());
        mState = data_ov004_02171de0.State02156acc_14;
    }
}

void DinoUiExchange::OnExchangeMessage(MMessage *msg) {
    switch (msg->mId) {
    case 0x100A8:
        mRock = *(DinoBackupEntry *)((DinoExchangeKasekiRockSignal *)msg)->mUnk0C;
        break;
    case 0x100A9:
        mProfile = *(DinoPlayerProfile *)((DinoExchangeSenryuSignal *)msg)->mUnk0C;
        break;
    case 0x100AA:
        mDone[((DinoExchangeDoneSignal *)msg)->mUnk0C] = TRUE;
        break;
    }
}

void DinoUiExchange::StateWait() {
    SetNextTime();
    mState = data_ov004_02171e60.StateRestart_44;
}

void DinoUiExchange::StateEnd() {
    if (mAnswer == FALSE) {
        MTask *exit = MTaskExitTransfer::Create(this);
        MTask *fade = gCurrentDisplay->mBlend->CreateFadeRatioAnime(8, 0, 10, 0);
        MTaskSet &serial = mSerial;
        serial.AddTail(fade);
        serial.AddTail(exit);
    } else {
        mState = data_ov004_02171e60.Unk02155f24_34;
    }
}
