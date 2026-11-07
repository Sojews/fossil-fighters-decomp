// DinoGradationBg.
#pragma ipa file  // (the clamp limits are file statics; the game reads them through the .data anchor)

#include <algorithm>
#include "game/dino_gradation_bg.h"
#include "m/display.h"
#include "nitro/mi.h"

// The 255s the brighter table's channels are clamped to; ipa lays them out r, b, g (0x020b5e14..1c).
static u32 sLimitB = 255;
static u32 sLimitG = 255;
static u32 sLimitR = 255;

DinoGradationBg::DinoGradationBg(MDisplay *display) {
    mDisplay = display;
    mRaster = new MRasterPal((u32)-1, mDisplay);
    mFront = (u16 *)mRaster->mFront;
    mBack = (u16 *)mRaster->mBack;
    MI_CpuFill8(mFront, 0, M_RASTER_LINES * 2);
    MI_CpuFill8(mBack, 0, M_RASTER_LINES * 2);
    mRaster->SetFlipOnVBlank(TRUE);
}

// Blends each raster line from (r0, g0, b0) to (r1, g1, b1); the back table is 4 brighter, clamped to 255.
void DinoGradationBg::SetGradation(int r0, int g0, int b0, int r1, int g1, int b1) {
    int i;
    for (i = 0; (u32)i < M_RASTER_LINES; i++) {
        int t = M_RASTER_LINES - i;
        u32 r = (u32)(0x155 * (r0 * t + r1 * i)) >> 16;
        u32 g = (u32)(0x155 * (g0 * t + g1 * i)) >> 16;
        u32 b = (u32)(0x155 * (b0 * t + b1 * i)) >> 16;
        mFront[i] = (r >> 3) | (((b >> 3) << 10) | ((g >> 3) << 5));
        mBack[i] = (std::min(r + 4, sLimitR) >> 3) |
                   (((std::min(b + 4, sLimitB) >> 3) << 10) | ((std::min(g + 4, sLimitG) >> 3) << 5));
    }
}

DinoGradationBg::~DinoGradationBg() {
    delete mRaster;
    mDisplay->SetBackdrop(0);
}

void DinoGradationBg::Execute() {
}
