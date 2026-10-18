// DinoMultiPlayScanner.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_multi_play_scanner.h"
#include "types.h"

DinoMultiPlayScanner::~DinoMultiPlayScanner() {
}

DinoMultiPlayScanTelop::~DinoMultiPlayScanTelop() {
    mFont->Release();
}

#pragma auto_inline off

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

#pragma auto_inline on

void DinoMultiPlayScanTelop::Idle() {
    mSerial.AddTail(mView->CreateCoordAnime(mFrom, mTo, 200, 0));
}
