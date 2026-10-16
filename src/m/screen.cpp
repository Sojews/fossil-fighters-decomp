// MScreen and the transfers and animations that change a screen.

#include "m/screen.h"

MScreen::MScreen(MDisplay *display, u32 screenBase, u32 base, u32 size, u32 screenSize, int bg)
    : MMemoryBlock(base, size) {
    mDisplay = display;
    mBg = bg;
    mOffset.x = 0;
    mOffset.y = 0;
    mBgCnt = 0;
    mBgCntBits.screenSize = screenSize;
    mBgCntBits.screenBase = screenBase;
}

MScreen::~MScreen() {
    Hide();
}

void MScreen::Update() {
    *(volatile u16 *)mDisplay->GetRegs08(mBg) = mBgCnt;
}

void MScreen::Show() {
    mDisplay->ShowPlanes(1 << mBg);
}

void MScreen::Hide() {
    mDisplay->HidePlanes(1 << mBg);
}

BOOL MScreen::IsHidden() {
    return mDisplay->ArePlanesHidden(1 << mBg);
}

void MScreen::SetChr(MChr *chr) {
    mBgCntBits.charBase = chr->mCharBase;
}

void MScreen::SetBg(int bg) {
    mBg = bg;
}

MTask *MScreen::CreateShowTransfer() {
    return new MScreenShowTransfer(this);
}

MTask *MScreen::CreateHideTransfer() {
    return new MScreenHideTransfer(this);
}

MTask *MScreen::CreateOffsetAnime(MPoint from, MPoint to, int duration, int ease) {
    return new MScreenOffsetAnime(this, from, to, duration, ease);
}

void MScreenOffsetAnime::Apply(int step) {
    MPointAnime::Apply(step);
    mScreen->SetOffset(MPoint(mValue.x, mValue.y));
}

void MScreenHideTransfer::Transfer() {
    mScreen->Hide();
}

void MScreenShowTransfer::Transfer() {
    mScreen->Show();
}
