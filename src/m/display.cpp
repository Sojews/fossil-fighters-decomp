// The 2D graphics engines.

#include <algorithm>
#include "m/display.h"
#include "m/window.h"
#include "m/blend.h"
#include "m/bright.h"

// The OBJ and BG controllers (not decompiled yet).
class MBg {
public:
    void Update();
};

class MChr {
public:
    void Update();
};

MDisplay::MDisplay(MLcd *lcd, MDisplayRegs *regs) : mLcd(lcd), mRegs(regs) {
    mBackdrop = 0x7FFF;
    mDispCnt.bgMode = 0;
    mDispCnt.bg0Is3D = FALSE;
    mDispCnt.objTileMapping = 1;
    mDispCnt.objBmpMapping = 0;
    mDispCnt.forceBlank = FALSE;
    mDispCnt.visiblePlanes = 0;
    mDispCnt.visibleWindows = 0;
    mDispCnt.displayMode = 1;
    mDispCnt.objTileBoundary = 0;
    mDispCnt.objBmpBoundary = 0;
    mDispCnt.hblankObj = TRUE;
    mDispCnt.bgCharBase = 0;
    mDispCnt.bgScreenBase = 0;
    mDispCnt.bgExtPalette = FALSE;
    mDispCnt.objExtPalette = FALSE;
    mUnk10.lo = 0;
    mWin = new MWin(this);
    mBlend = new MBlend(this);
    mBright = new MBright(this);
    mBg = NULL;
    mChr = NULL;
}

MDisplay::~MDisplay() {
    delete mWin;
    delete mBlend;
    delete mBright;
}

void MDisplay::Update() {
    *mRegs->backdrop = mBackdrop;
    *mRegs->dispcnt = mDispCnt;
    *mRegs->unk88 = mUnk10;
    if (mBg != NULL) {
        mBg->Update();
    }
    if (mChr != NULL) {
        mChr->Update();
    }
    mWin->Update();
    mBlend->Update();
    mBright->Update();
}

void MDisplay::AddHook(MDisplayHook *hook) {
    mHooks.push_back(hook);
}

void MDisplay::RemoveHook(MDisplayHook *hook) {
    mHooks.erase(std::find(mHooks.begin(), mHooks.end(), hook));
}

u32 MDisplay::GetRegs00() {
    return mRegs->unk00;
}

u32 MDisplay::GetRegs08(int i) {
    return mRegs->unk08[i];
}

u32 MDisplay::GetRegs18(int i) {
    return mRegs->unk18[i];
}

u32 MDisplay::GetRegs28(int i) {
    return mRegs->unk28[i];
}

volatile u8 *MDisplay::GetRegs64(int i) {
    return mRegs->winin[i];
}

volatile u16 *MDisplay::GetRegs74(int i) {
    return mRegs->winh[i];
}

volatile u16 *MDisplay::GetRegs7C() {
    return mRegs->bldcnt;
}

volatile u16 *MDisplay::GetRegs80() {
    return mRegs->bldalpha;
}

volatile u16 *MDisplay::GetRegs84() {
    return mRegs->bldy;
}

volatile u32 *MDisplay::GetRegs8C() {
    return mRegs->masterBright;
}

void MDisplay::ShowPlanes(u32 mask) {
    mDispCnt.visiblePlanes |= mask;
}

void MDisplay::HidePlanes(u32 mask) {
    mDispCnt.visiblePlanes &= ~mask;
}

BOOL MDisplay::ArePlanesHidden(u32 mask) {
    return (mDispCnt.visiblePlanes & mask) == 0;
}

void MDisplay::ShowWindow(int window) {
    if (window == 3) {
        window = 2;
    }
    mDispCnt.visibleWindows |= 1 << window;
}

void MDisplay::HideWindow(int window) {
    if (window == 3) {
        window = 2;
    }
    mDispCnt.visibleWindows &= ~(1 << window);
}

void MDisplay::SetBackdrop(u16 color) {
    mBackdrop = color;
}

volatile u16 *MDisplay::GetBackdropReg() {
    return mRegs->backdrop;
}

void MDisplay::SetBg0Is3D(BOOL on) {
    if (on) {
        mDispCnt.bg0Is3D = TRUE;
    } else {
        mDispCnt.bg0Is3D = FALSE;
    }
}

void MDisplay::SetObjTileBoundary(int boundary) {
    mDispCnt.objTileBoundary = boundary;
}
