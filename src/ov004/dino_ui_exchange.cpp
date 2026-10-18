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
#include "game/dino_common_window_message_box.h"

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
    u8 unk00[0x3C];
    void (DinoUiExchange::*State02155e10_3C)(); // 0x3c
} data_ov004_02171de0;
extern "C" struct {
    u8 unk00[0x34];
    void (DinoUiExchange::*Unk02155f24_34)();
    u8 unk3C[0x44 - 0x3C];
    void (DinoUiExchange::*State0215639c_44)(); // 0x44
    u8 unk4C[0x54 - 0x4C];
    void (DinoUiExchange::*StateCheck_54)(); // 0x54
    u8 unk5C[0x6C - 0x5C];
    void (DinoUiExchange::*StateStart_6C)(); // 0x6c
} data_ov004_02171e60;

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
        mState = data_ov004_02171de0.State02155e10_3C;
    }
}

void DinoUiExchange::StateWait() {
    SetNextTime();
    mState = data_ov004_02171e60.State0215639c_44;
}

void DinoUiExchange::StateEnd() {
    if (mUnkC0 == 0) {
        MTask *exit = MTaskExitTransfer::Create(this);
        MTask *fade = gCurrentDisplay->mBlend->CreateFadeRatioAnime(8, 0, 10, 0);
        MTaskSet &serial = mSerial;
        serial.AddTail(fade);
        serial.AddTail(exit);
    } else {
        mState = data_ov004_02171e60.Unk02155f24_34;
    }
}
