// DinoUiLicenseCard.
// Only the destructors are decompiled so far.

#include "m/lcd.h"
#include "game/dino_ui_license_card.h"
#include "m/font.h"

DinoUiLicenseCard::~DinoUiLicenseCard() {
}

void DinoUiLicenseCard::Close() {
    mFont->Release();
    Exit();
}

void DinoUiLicenseCard::Idle() {
    (this->*mState)();
}

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x28];
    void (DinoUiLicenseCard::*Start_28)();
} data_ov005_021654e0;

DinoUiLicenseCard::DinoUiLicenseCard(u8 arg) : MProcess(gLcd->mMain), mUnk7C(arg) {
    mState = data_ov005_021654e0.Start_28;
}
