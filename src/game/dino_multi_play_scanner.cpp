// DinoMultiPlayScanner.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_multi_play_scanner.h"
#include "m/multi_message.h"
#include "types.h"

DinoMultiPlayScanner::~DinoMultiPlayScanner() {
}

DinoMultiPlayScanTelop::~DinoMultiPlayScanTelop() {
    mFont->Release();
}

#pragma auto_inline off

DinoMultiPlayScanner::DinoMultiPlayScanner(MMultiPlay *multiPlay, u8 *result)
    : MMenu(1), mMultiPlay(multiPlay), mResult(result) {
    mFound = FALSE;
}

void DinoMultiPlayScanner::Start() {
    if (mMode == 1) {
        mSerial.AddTail(mMultiPlay->CreateStartParentTask(NULL, 0, 0));
    } else {
        mSerial.AddTail(mMultiPlay->CreateStartScanTask());
    }
    mWait = mRandom.Next(60) + 60;
}

void DinoMultiPlayScanner::Reset() {
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

void DinoMultiPlayScanner::Cancel() {
    *mResult = 0;
    Reset();
    mSerial.AddTail(CreateExitTransfer());
}

// Any decide cancels the scan.
void DinoMultiPlayScanner::OnDecide(MMenuItem *item) {
    Cancel();
}

// Restarts after a failure, finishes once a partner is found, and otherwise switches sides every so often.
void DinoMultiPlayScanner::OnIdle() {
    if (mMultiPlay->IsFailed()) {
        mFound = FALSE;
        mSerial.AddTail(mMultiPlay->CreateResetTask());
        Start();
        return;
    }
    if (mFound == TRUE) {
        *mResult = 1;
        Exit();
        return;
    }
    if (--mWait <= 0) {
        Reset();
        mMode = (mMode + 1) % 2;
        Start();
        return;
    }
    Dispatch();
}

void DinoMultiPlayScanner::OnMessage(MMessage *msg) {
    switch (msg->mId) {
    case 0:
        if (mMode == 0) {
            mFound = TRUE;
            MMultiPlayParentFoundMessage *found = dynamic_cast<MMultiPlayParentFoundMessage *>(msg);
            MTask *delay = mMultiPlay->CreateSynchronizeDelayTask();
            MTask *start = mMultiPlay->CreateStartChildTask(found->mUnk0C, NULL);
            MTask *end = mMultiPlay->CreateEndScanTask();
            MSerialTaskSet *serial = &mSerial;
            serial->AddTail(end);
            serial->AddTail(start);
            serial->AddTail(delay);
        }
        break;
    case 5:
        if (mMode == 1) {
            mFound = TRUE;
            mSerial.AddTail(mMultiPlay->CreateSynchronizeDelayTask());
        }
        break;
    }
}

#pragma auto_inline on

void DinoMultiPlayScanTelop::Idle() {
    mSerial.AddTail(mView->CreateCoordAnime(mFrom, mTo, 200, 0));
}
