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

// NONMATCHING: register allocation (the game keeps the red accumulator on the stack and blue in lr) and 8 bytes.
void DinoGradationBg::SetGradation(int r0, int g0, int b0, int r1, int g1, int b1) {
    int rAcc = 0;
    int gAcc = 0;
    int bAcc = 0;
    u32 i;

    for (i = 0; i < M_RASTER_LINES; i++) {
        u32 r4;
        u32 b4;
        u32 g4;
        int t = M_RASTER_LINES - i;
        u32 r = (u32)(0x155 * (r0 * t + rAcc)) >> 16;
        u32 g = (u32)(0x155 * (g0 * t + gAcc)) >> 16;
        u32 b = (u32)(0x155 * (t * b0 + bAcc)) >> 16;
        mFront[i] = (r >> 3) | ((g >> 3) << 5) | ((b >> 3) << 10);
        g4 = g + 4;
        const u32 &gc = std::min(g4, data_020b5e14[2]);
        b4 = b + 4;
        const u32 &bc = std::min(b4, data_020b5e14[1]);
        r4 = r + 4;
        const u32 &rc = std::min(r4, data_020b5e14[0]);
        mBack[i] = (rc >> 3) | ((gc >> 3) << 5) | ((bc >> 3) << 10);
        rAcc += r1;
        gAcc += g1;
        bAcc += b1;
    }
}

DinoGradationBg::~DinoGradationBg() {
    delete mRaster;
    mDisplay->SetBackdrop(0);
}

void DinoGradationBg::Execute() {
}
