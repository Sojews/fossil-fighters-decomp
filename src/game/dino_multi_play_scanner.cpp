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

void DinoMultiPlayScanTelop::Idle() {
    mSerial.AddTail(mView->CreateCoordAnime(mFrom, mTo, 200, 0));
}
