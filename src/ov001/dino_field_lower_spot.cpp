// DinoFieldLowerSpot.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_field_lower_spot.h"
#include "m/lcd.h"
#include "m/display.h"
#include "m/window.h"
#include "m/bright.h"

DinoFieldLowerSpotIn::DinoFieldLowerSpotIn(int duration) : MProcess(NULL) {
    mDuration = duration;
    mUnk7C = 0;
    mUnk80 = 0;
}

DinoFieldLowerSpotIn::~DinoFieldLowerSpotIn() {
    gLcd->mMain->mWin->Reset();
}

DinoFieldLowerSpotOut::DinoFieldLowerSpotOut(int duration) : MProcess(NULL) {
    mDuration = duration;
    mUnk7C = 0;
    mUnk80 = 0;
}

DinoFieldLowerSpotOut::~DinoFieldLowerSpotOut() {
    gLcd->mMain->mWin->Reset();
    gLcd->mMain->mBright->SetMode(2);
    gLcd->mMain->mBright->SetFactor(16);
}
