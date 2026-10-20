// DinoUiLicenseCard.
// Only the destructors are decompiled so far.

#include "game/dino_static_init.h"
#include "m/bright.h"
#include "m/display.h"
#include "m/input.h"
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
    u8 unk00[0x20];
    void (DinoUiLicenseCard::*Close_20)();
    void (DinoUiLicenseCard::*Start_28)();
} data_ov005_021654e0;

DinoUiLicenseCard::DinoUiLicenseCard(u8 arg) : MProcess(gLcd->mMain), mUnk7C(arg) {
    mState = data_ov005_021654e0.Start_28;
}

// Until A, B, Start or a touch, then fades out.
void DinoUiLicenseCard::StateWait() {
    if (mInput->GetPressed() & 0x1007) {
        mSerial.AddTail(gCurrentDisplay->mBright->CreateFadeOutToBlack(60, 0));
        mState = data_ov005_021654e0.Close_20;
    }
}

// The file's static initialization (see dino_static_init.h).
static struct DinoUiLicenseCardStaticInit {
    DinoUiLicenseCardStaticInit() {
        *(u16 *)((u8 *)&data_ov005_021654e0 + 0x12) = 0xFFFF;
        *(u16 *)((u8 *)&data_ov005_021654e0 + 0x14) = 0x996E;
    }
} sDinoUiLicenseCardStaticInit;
