// DinoUiLicenseCard.
// Only the destructors are decompiled so far.

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
