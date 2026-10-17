// CameraWipe.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "m/window.h"
#include "m/lcd.h"
#include "game/camera_wipe.h"
#include "types.h"

extern u8 data_ov000_02197930;

CameraWipe::~CameraWipe() {
    if ((u32)(mMode - 2) <= 1) {
        mOwner->mScreen->Hide();
        data_ov000_02197930 = TRUE;
    } else {
        data_ov000_02197930 = FALSE;
    }
    gLcd->mMain->mWin->Reset();
}

M3DFadeTask::~M3DFadeTask() {
}

// NONMATCHING: the game calls std::vector<std::string>'s out-of-line destructor for mLines; ours inlines it into a
// __vec_deleter call. The bytes are identical, so only callcheck sees it.
DinoBattleViewActionInfoBar::~DinoBattleViewActionInfoBar() {
    mFont->Release();
}
