// DinoUiExchange.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_ui_exchange.h"
#include "types.h"
#include "m/blend.h"
#include "m/bg_new_task.h"
#include "game/dino_backup.h"

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

void DinoUiExchange::ShowCount() {
    mOnes->Play(mCount % 10 + 1, 0);
    mTens->Play(mCount / 10 + 1, 0);
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
