// Display color effects.

#include "m/blend.h"
#include "m/blend_fade_ratio_anime.h"
#include "m/blend_alpha_ratio_anime.h"

void MBlend::Update() {
    *mDisplay->GetRegs7C() = mBldCnt;
    *mDisplay->GetRegs80() = mBldAlpha;
    *mDisplay->GetRegs84() = mBldY;
}

void MBlend::Reset() {
    SetMode(0);
    SetAlpha(0, 0);
    SetBrightness(0);
    SetTarget1(0);
    SetTarget2(0);
}

// Swaps the target bits of pairs of planes, as when two BG layers trade places.
void MBlend::SwapPlanes(u32 unused, const int *a, const int *b, u32 count) {
    u32 i;

    for (i = 0; i < count; i++) {
        u16 bitA = 1 << a[i];
        u16 bitB = 1 << b[i];
        mBldCnt = (mBldCnt & ~(bitB | bitA)) | ((mBldCnt & bitB) ? bitA : 0) | ((mBldCnt & bitA) ? bitB : 0);
        bitA = 1 << (a[i] + 8);
        bitB = 1 << (b[i] + 8);
        mBldCnt = (mBldCnt & ~(bitB | bitA)) | ((mBldCnt & bitB) ? bitA : 0) | ((mBldCnt & bitA) ? bitB : 0);
    }
}

void MBlend::SetMode(int mode) {
    mBldCnt = (mBldCnt & ~0xC0) | (mode << 6);
}

int MBlend::GetMode() {
    return (mBldCnt & 0xC0) >> 6;
}

void MBlend::SetAlpha(int eva, int evb) {
    mBldAlpha = eva | (evb << 8);
}

u8 MBlend::GetEva() {
    return mBldAlpha;
}

u8 MBlend::GetEvb() {
    return mBldAlpha >> 8;
}

void MBlend::SetBrightness(u16 evy) {
    mBldY = evy;
}

void MBlend::SetTarget1(int planes) {
    mBldCnt = (mBldCnt & 0xFFC0) | planes;
}

void MBlend::SetTarget2(int planes) {
    mBldCnt = (mBldCnt & 0xC0FF) | (planes << 8);
}

int MBlend::GetTarget1() {
    return mBldCnt & 0x3F;
}

MTask *MBlend::CreateModeTransfer(int mode) {
    return new BlendModeTransfer(this, mode);
}

MTask *MBlend::CreateAlphaTransfer(int eva, int evb) {
    return new BlendAlphaRatioTransfer(this, eva, evb);
}

MTask *MBlend::CreateTarget1Transfer(int planes) {
    return new BlendTarget1Transfer(this, planes);
}

MTask *MBlend::CreateTarget2Transfer(int planes) {
    return new BlendTarget2Transfer(this, planes);
}

MTask *MBlend::CreateClearTransfer() {
    return new MBlendClearTransfer(this);
}

void MBlendClearTransfer::Transfer() {
    mBlend->Reset();
}

void BlendTarget2Transfer::Transfer() {
    mBlend->SetTarget2(mPlanes);
}

void BlendTarget1Transfer::Transfer() {
    mBlend->SetTarget1(mPlanes);
}

void BlendAlphaRatioTransfer::Transfer() {
    mBlend->SetAlpha(mEva, mEvb);
}

void BlendModeTransfer::Transfer() {
    mBlend->SetMode(mMode);
}

MBlendFadeRatioAnime::~MBlendFadeRatioAnime() {
}

MBlendAlphaRatioAnime::~MBlendAlphaRatioAnime() {
}
