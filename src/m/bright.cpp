// Display master brightness.

#include "m/bright.h"
#include "m/bright_ratio_anime.h"

MBright::MBright(MDisplay *display) {
    mDisplay = display;
    Reset();
}

void MBright::Reset() {
    mValue.raw = 0;
}

void MBright::SetMode(int mode) {
    mValue.mode = mode;
}

int MBright::GetMode() {
    return mValue.mode;
}

void MBright::SetFactor(int factor) {
    mValue.factor = factor;
}

u32 MBright::GetFactor() {
    return mValue.factor;
}

void MBright::Update() {
    *mDisplay->GetRegs8C() = *(u32 *)&mValue;
}

MTask *MBright::CreateModeTransfer(int mode) {
    return new MBrightModeTransfer(this, mode);
}

MTask *MBright::CreateClearTransfer() {
    return new MBrightClearTransfer(this);
}

void MBrightClearTransfer::Transfer() {
    mBright->Reset();
}

void MBrightModeTransfer::Transfer() {
    mBright->SetMode(mMode);
}

MBrightRatioAnime::~MBrightRatioAnime() {
}
