// DinoGradationBg.

#include <algorithm>
#include "game/dino_gradation_bg.h"
#include "m/display.h"
#include "nitro/mi.h"

// The 255s the brighter table's channels are clamped to (r, b, g).
extern u32 data_020b5e14[3];

DinoGradationBg::DinoGradationBg(MDisplay *display) {
    mDisplay = display;
    mRaster = new MRasterPal((u32)-1, mDisplay);
    mFront = (u16 *)mRaster->mFront;
    mBack = (u16 *)mRaster->mBack;
    MI_CpuFill8(mFront, 0, M_RASTER_LINES * 2);
    MI_CpuFill8(mBack, 0, M_RASTER_LINES * 2);
    mRaster->SetFlipOnVBlank(TRUE);
}

// The red limit's address gets its own literal (a separate object at the anchor), so name it through the word before.
extern u32 data_020b5e10[];

// std::min(a, value) that returns the address of `named` (the same word as `value`) when it clamps.
static inline const u32 &MinNamed(const u32 &a, const u32 &value, const u32 &named) {
    return value < a ? named : a;
}

// Blends each raster line from (r0, g0, b0) to (r1, g1, b1); the back table is 4 brighter, clamped to 255.
// NONMATCHING: scheduling only (the game adds red's +4 before loading the red limit's base).

void DinoGradationBg::SetGradation(int r0, int g0, int b0, int r1, int g1, int b1) {
    int i;
    for (i = 0; (u32)i < M_RASTER_LINES; i++) {
        int t = M_RASTER_LINES - i;
        u32 r = (u32)(0x155 * (r0 * t + r1 * i)) >> 16;
        u32 g = (u32)(0x155 * (g0 * t + g1 * i)) >> 16;
        u32 b = (u32)(0x155 * (b0 * t + b1 * i)) >> 16;
        mFront[i] = (r >> 3) | (((b >> 3) << 10) | ((g >> 3) << 5));
        mBack[i] = (MinNamed(r + 4, data_020b5e14[0], data_020b5e10[1]) >> 3) |
                   (((std::min(b + 4, data_020b5e14[1]) >> 3) << 10) | ((std::min(g + 4, data_020b5e14[2]) >> 3) << 5));
    }
}

DinoGradationBg::~DinoGradationBg() {
    delete mRaster;
    mDisplay->SetBackdrop(0);
}

void DinoGradationBg::Execute() {
}
