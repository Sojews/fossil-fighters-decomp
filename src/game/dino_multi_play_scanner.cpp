// DinoMultiPlayScanner.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_multi_play_scanner.h"
#include "m/multi_message.h"
#include "m/multi_play.h"
#include "m/smart_oam.h"
#include "m/oam_canvas.h"
#include "m/font.h"
#include "game/dino_resource.h"
#include "game/dino_common_item.h"
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

extern "C" const u32 data_0208a43c; // the telop's text
extern "C" const u32 data_0208a440; // the cancel button's

inline DinoMultiPlayScanTelop::DinoMultiPlayScanTelop(MSmartOam *oam) : MProcess(NULL) {
    const char *text = gDinoResource->GetText(data_0208a43c);
    mFont = gDinoResource->CreateFont16(0, 10, 0);
    mFont->AddRef();
    MPoint step = mFont->MeasureText(std::string(text), 0, 0);
    step.x += 16;
    MPoint size(step.x + 256, step.y);
    mView = new MSmartObjCanvas(oam->mDisplay, oam, size, gDinoResource->GetUnkTable(), TRUE);
    mView->Show();
    for (int x = 0; x < size.x; x += step.x) {
        ((MCanvas *) mView)->DrawText(mFont, MPoint(x, 0), 0, std::string(text));
    }
    mFrom = MPoint(0, 64);
    mTo = MPoint(-step.x, 64);
    mParallel.AddTail(mView);
}

// Starts the connection on a random side, with the telop and a cancel button.
void DinoMultiPlayScanner::OnStart() {
    mMultiPlay->AddObserver(this);
    mSerial.AddTail(mMultiPlay->CreateInitializeTask());
    mMode = mRandom.Next(2);
    Start();
    MSmartOam *oam = new MSmartOam(0x14, 0, TRUE);
    oam->SetUnk1C(5);
    MTask *telop = new DinoMultiPlayScanTelop(oam);
    MParallelTaskSet *parallel = &mParallel;
    parallel->AddTail(telop);
    parallel->AddTail(oam);
    mCancel = new DinoCommonItem(0, oam, MPoint(0xE0, 0xB0), 0, data_0208a440, 0);
    mCancel->SetNoFocus(TRUE);
    mCancel->SetKeys(2);
    AddItem(mCancel);
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
