// DinoUiExchange.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_ui_exchange.h"
#include "types.h"
#include "m/blend.h"
#include "m/bg_new_task.h"
#include "game/dino_backup.h"
#include "game/dino_resource.h"
#include "m/seed_generator.h"

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
