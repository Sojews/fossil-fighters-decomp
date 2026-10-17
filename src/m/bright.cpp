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

MTask *MBright::CreateRatioAnime(int from, int to, int duration, int ease) {
    return new MBrightRatioAnime(this, from, to, duration, ease);
}

MTask *MBright::CreateFadeInFromBlack(int duration, int ease) {
    MSerialTaskSet *set = new MSerialTaskSet();
    set->mExitWhenEmpty = TRUE;
    set->AddTail(CreateModeTransfer(2));
    set->AddTail(CreateRatioAnime(16, 0, duration, ease));
    return set;
}

MTask *MBright::CreateFadeOutToBlack(int duration, int ease) {
    MSerialTaskSet *set = new MSerialTaskSet();
    set->mExitWhenEmpty = TRUE;
    set->AddTail(CreateModeTransfer(2));
    set->AddTail(CreateRatioAnime(0, 16, duration, ease));
    return set;
}

MTask *MBright::CreateFadeInFromWhite(int duration, int ease) {
    MSerialTaskSet *set = new MSerialTaskSet();
    set->mExitWhenEmpty = TRUE;
    set->AddTail(CreateModeTransfer(1));
    set->AddTail(CreateRatioAnime(16, 0, duration, ease));
    return set;
}

MTask *MBright::CreateFadeOutToWhite(int duration, int ease) {
    MSerialTaskSet *set = new MSerialTaskSet();
    set->mExitWhenEmpty = TRUE;
    set->AddTail(CreateModeTransfer(1));
    set->AddTail(CreateRatioAnime(0, 16, duration, ease));
    return set;
}

void MBrightRatioAnime::Apply(int step) {
    MValueAnime::Apply(step);
    mBright->SetFactor(mValue);
}

void MBrightClearTransfer::Transfer() {
    mBright->Reset();
}

void MBrightModeTransfer::Transfer() {
    mBright->SetMode(mMode);
}

MBrightRatioAnime::~MBrightRatioAnime() {
}
