// The screens and VRAM banks.

#include "m/lcd.h"

extern "C" {
extern void *data_0208b378[];   // VRAMCNT register addresses
extern u8 data_0208b410[];      // texture VRAM configurations, 0x1C bytes each
extern u32 data_0208bcc8[][5][4];   // bank addresses, by bank, MST and offset
}

void MLcd::VSync();

BOOL MLcd::IsBankUsed(int bank) {
    return mBankUsed[bank];
}

void MLcd::UseBank(int bank, int mst, int offset) {
    if (mBankUsed[bank]) {
        OS_Terminate();
    }
    mBankUsed[bank] = TRUE;
    mBankCnt[bank].mst = mst;
    mBankCnt[bank].offset = offset;
    mBankCnt[bank].enable = TRUE;
}

void MLcd::FreeBank(int bank) {
    if (!mBankUsed[bank]) {
        OS_Terminate();
    }
    mBankUsed[bank] = FALSE;
    mBankCnt[bank].raw = 0;
}

void MLcd::SetSwapped(BOOL swapped) {
    mSwapped = swapped;
}

BOOL MLcd::IsSwapped() {
    return mSwapped;
}

void MLcd::SetTopDisplay(MDisplay *display) {
    if (display == mMain) {
        mPowCnt |= 0x8000;
    } else {
        mPowCnt &= ~0x8000;
    }
}

void MLcd::SetBottomDisplay(MDisplay *display) {
    if (display == mSub) {
        mPowCnt |= 0x8000;
    } else {
        mPowCnt &= ~0x8000;
    }
}

MDisplay *MLcd::GetTopDisplay() {
    if (mPowCnt & 0x8000) {
        return mMain;
    }
    return mSub;
}

MDisplay *MLcd::GetBottomDisplay() {
    if (mPowCnt & 0x8000) {
        return mSub;
    }
    return mMain;
}

MVramCnt MLcd::SetBankCnt(int bank, MVramCnt cnt) {
    MVramCnt old;

    old.raw = mBankCnt[bank].raw;
    mBankCnt[bank].raw = cnt.raw;
    *(u8 *)data_0208b378[bank] = cnt.raw;
    return old;
}

void MLcd::UseTextureBanks(int config) {
    UseBanks(data_0208b410 + config * 0x1C, &mTexBanks);
}

void MLcd::FreeTextureBanks() {
    FreeBanks(&mTexBanks);
}

int MLcd::GetTextureSlotFlags() {
    if (mTexBanks.banks[0] == 3 && mTexBanks.banks[1] == 4) {
        return 0xC;
    }
    if (mTexBanks.banks[0] == 3) {
        return 4;
    }
    if (mTexBanks.banks[0] == 4) {
        return 8;
    }
    return 0;
}

u32 MLcd::GetBankAddress(int bank) {
    return data_0208bcc8[bank][mBankCnt[bank].mst][mBankCnt[bank].offset];
}
