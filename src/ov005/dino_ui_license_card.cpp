// DinoUiLicenseCard.
// Only the destructors are decompiled so far.

#include "game/dino_ui_license_card.h"

DinoUiLicenseCard::~DinoUiLicenseCard() {
}

void DinoUiLicenseCard::Idle() {
    (this->*mState)();
}
