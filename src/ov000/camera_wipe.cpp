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

extern "C" void G3X_SetFog(BOOL enable, int fogMode, int fogSlope, int fogOffset);
extern "C" void G3X_SetFogTable(const u32 *fogTable);
extern u8 data_ov000_02197934[32];  // the fog density table

static inline void FillFogTable(u8 density) {
    for (int i = 0; i < 32; i++) {
        data_ov000_02197934[i] = density;
    }
}

M3DFadeTask::~M3DFadeTask() {
    if (mUnk14 < 0) {
        FillFogTable(0);
        G3X_SetFog(FALSE, 0, 0, 0);
    } else {
        FillFogTable(0x7F);
        G3X_SetFog(TRUE, 0, 0, 0);
        *(vu32 *)0x04000358 = 0x1F << 16;   // fog color: black, alpha 31
        G3X_SetFogTable((const u32 *)data_ov000_02197934);
    }
}

// NONMATCHING: the game calls std::vector<std::string>'s out-of-line destructor for mLines; ours inlines it into a
// __vec_deleter call. The bytes are identical, so only callcheck sees it.
DinoBattleViewActionInfoBar::~DinoBattleViewActionInfoBar() {
    mFont->Release();
}
