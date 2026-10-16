// Display windows.

#include "m/window.h"
#include "m/win_control_shape_anime.h"

MWinControl::MWinControl(MDisplay *display, int index) {
    mDisplay = display;
    mIndex = index;
    Reset();
}

void MWinControl::SwapPlanes(u32 unused, const int *a, const int *b, u32 count) {
    u32 i;

    for (i = 0; i < count; i++) {
        u8 bitA = 1 << a[i];
        u8 bitB = 1 << b[i];
        mInside = (mInside & ~(bitB | bitA)) | ((mInside & bitB) ? bitA : 0) | ((mInside & bitA) ? bitB : 0);
    }
}

void MWinControl::Show() {
    mDisplay->ShowWindow(mIndex);
}

void MWinControl::Hide() {
    mDisplay->HideWindow(mIndex);
}

void MWinControl::SetTarget(int planes) {
    mInside = (mInside & 0x20) | (u8)(planes & ~0x20);
}

void MWinControl::SetBlendEnabled(BOOL enabled) {
    if (enabled) {
        mInside |= 0x20;
    } else {
        mInside &= ~0x20;
    }
}

void MWinControl::SetShape(int left, int top, int right, int bottom) {
    mShape.left = left;
    mShape.top = top;
    mShape.right = right;
    mShape.bottom = bottom;
}

void MWinControl::SetShape(MRect rect) {
    mShape.left = rect.left;
    mShape.top = rect.top;
    mShape.right = rect.right;
    mShape.bottom = rect.bottom;
}

volatile u16 *MWinControl::GetShapeRegs() {
    return mDisplay->GetRegs74(mIndex);
}

void MWinControl::Reset() {
    mInside = 0;
    mShape.left = 0;
    mShape.top = 0;
    mShape.right = 0;
    mShape.bottom = 0;
    if (mIndex != 2) {
        Hide();
    }
}

void MWinControl::Update() {
    *mDisplay->GetRegs64(mIndex) = mInside;
    if ((u32)mIndex < 2) {
        volatile u16 *regs = mDisplay->GetRegs74(mIndex);
        regs[0] = (mShape.left << 8) | mShape.right;
        regs[2] = (mShape.top << 8) | mShape.bottom;
    }
}

MWin::MWin(MDisplay *display) {
    u32 i;

    for (i = 0; i < 4; i++) {
        mWindows[i] = new MWinControl(display, i);
    }
}

MWin::~MWin() {
    int i;

    for (i = 0; i < 4; i++) {
        delete mWindows[i];
    }
}

void MWin::SwapPlanes(u32 unused, const int *a, const int *b, u32 count) {
    int i;

    for (i = 0; i < 4; i++) {
        mWindows[i]->SwapPlanes(unused, a, b, count);
    }
}

void MWin::Reset() {
    int i;

    for (i = 0; i < 4; i++) {
        mWindows[i]->Reset();
    }
}

void MWin::Update() {
    int i;

    for (i = 0; i < 4; i++) {
        mWindows[i]->Update();
    }
}

MWinControl *MWin::GetWindow0() {
    return mWindows[0];
}

MWinControl *MWin::GetWindow1() {
    return mWindows[1];
}

MWinControl *MWin::GetObjWindow() {
    return mWindows[2];
}

MTask *MWinControl::CreateShowTransfer() {
    return new MWinControlShowTransfer(this);
}

MTask *MWinControl::CreateTargetTransfer(int planes) {
    return new MWinControlTargetTransfer(this, planes);
}

MTask *MWinControl::CreateBlendEnabledTransfer(bool enabled) {
    return new MWinControlBlendEnabledTransfer(this, enabled);
}

MTask *MWinControl::CreateShapeTransfer(MRect rect) {
    return new MWinControlShapeTransfer(this, rect);
}

MTask *MWin::CreateClearTransfer() {
    return new MWinClearTransfer(this);
}

void MWinClearTransfer::Transfer() {
    mWin->Reset();
}

void MWinControlShapeTransfer::Transfer() {
    mWin->SetShape(mRect);
}

void MWinControlBlendEnabledTransfer::Transfer() {
    mWin->SetBlendEnabled(mEnabled);
}

void MWinControlTargetTransfer::Transfer() {
    mWin->SetTarget(mPlanes);
}

void MWinControlShowTransfer::Transfer() {
    mWin->Show();
}

MWinControlShapeAnime::~MWinControlShapeAnime() {
}
