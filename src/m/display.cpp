// The 2D graphics engines.

#include "m/display.h"

void MDisplay::AddHook(MDisplayHook *hook) {
    mHooks.push_back(hook);
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

u32 MDisplay::GetRegs64(int i) {
    return mRegs->unk64[i];
}

u32 MDisplay::GetRegs74(int i) {
    return mRegs->unk74[i];
}

u32 MDisplay::GetRegs7C() {
    return mRegs->unk7C;
}

u32 MDisplay::GetRegs80() {
    return mRegs->unk80;
}

u32 MDisplay::GetRegs84() {
    return mRegs->unk84;
}

u32 MDisplay::GetRegs8C() {
    return mRegs->unk8C;
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
