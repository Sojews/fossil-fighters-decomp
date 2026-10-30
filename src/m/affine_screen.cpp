// MAffineScreen and the transfers and animations that change one.

#include "m/affine_screen.h"

static u16 sAffineUpdateCount;

MAffineScreen::MAffineScreen(MDisplay *display, u32 screenBase, u32 base, u32 size, u32 screenSize, int bg)
    : MScreen(display, screenBase, base, size, screenSize, bg) {
    mRot = 0;
    mScale.Set(MFx32(1), MFx32(1));
    mCenter.x = 0;
    mCenter.y = 0;
}

void MAffineScreen::SetRot(u16 rot) {
    mRot = rot;
}

void MAffineScreen::SetScale(MFx32 x, MFx32 y) {
    mScale.x = x;
    mScale.y = y;
}

void MAffineScreen::SetScale(MFx32Point scale) {
    mScale = scale;
}

void MAffineScreen::SetCenter(int x, int y) {
    mCenter.x = x;
    mCenter.y = y;
}

void MAffineScreen::SetCenter(MPoint center) {
    mCenter = center;
}

// Writes a BG's affine parameters (PA-PD) and reference point so that the screen is transformed by mtx about
// (centerX, centerY) and scrolled to (x, y).
static inline void SetBgAffine(volatile u32 *regs, const MtxFx22 *mtx, int centerX, int centerY, int x, int y) {
    regs[0] = (u16)(s16)(mtx->_00 >> 4) | ((u16)(s16)(mtx->_01 >> 4) << 16);
    regs[1] = (u16)(s16)(mtx->_10 >> 4) | ((u16)(s16)(mtx->_11 >> 4) << 16);
    x -= centerX;
    y -= centerY;
    fx32 refY = (centerY << 12) + (mtx->_10 * x + mtx->_11 * y) + 0x800;
    fx32 refX = (centerX << 12) + (mtx->_00 * x + mtx->_01 * y) + 0x800;
    regs[2] = refX >> 4;
    regs[3] = refY >> 4;
}

void MAffineScreen::Update() {
    MtxFx22 mtx;

    MScreen::Update();
    sAffineUpdateCount += 0x10;
    MTX_Rot22_(&mtx, FX_SinIdx(mRot), FX_CosIdx(mRot));
    MTX_ScaleApply22(&mtx, &mtx, (MFx32(1) / mScale.x).v, (MFx32(1) / mScale.y).v);
    MPoint center = mCenter;
    SetBgAffine((volatile u32 *)mDisplay->GetRegs28(mBg), &mtx, center.x, center.y, mOffset.x, mOffset.y);
}

MTask *MAffineScreen::CreateRotTransfer(u16 rot) {
    return new MAffineScreenRotTransfer(this, rot);
}

// Turns the shorter way round.
MTask *MAffineScreen::CreateRotAnime(int from, int to, int duration, int ease) {
    if ((u16)(to - from) < 0x8000) {
        if (to < from) {
            to += 0x10000;
        }
    } else if (to > from) {
        to -= 0x10000;
    }
    return new MAffineScreenRotAnime(this, from, to, duration, ease);
}

MTask *MAffineScreen::CreateScaleAnime(MFx32Point from, MFx32Point to, int duration, int ease) {
    return new MAffineScreenScaleAnime(this, from, to, duration, ease);
}

void MAffineScreenScaleAnime::Apply(int step) {
    MFx32PointAnime::Apply(step);
    MFx32Point value = mValue;
    mScreen->SetScale(value);
}

void MAffineScreenRotAnime::Apply(int step) {
    MValueAnime::Apply(step);
    mScreen->SetRot(mValue);
}

void MAffineScreenRotTransfer::Transfer() {
    mScreen->SetRot(mRot);
}
