// DinoUi3PartsFraction, DinoUi3PartsRockName, DinoUi3PartsMaskName, DinoUi3PartsKasekiButton, DinoUi3PartsMoney.
// Only the destructors are decompiled so far.

#include "game/dino_ui3_parts_fraction.h"
#include "game/dino_ui3_parts_rock_name.h"
#include "game/dino_ui3_parts_mask_name.h"
#include "game/dino_ui3_parts_kaseki_button.h"
#include "game/dino_ui3_parts_money.h"

DinoUi3PartsFraction::~DinoUi3PartsFraction() {
}

DinoUi3PartsRockName::~DinoUi3PartsRockName() {
}

DinoUi3PartsMaskName::~DinoUi3PartsMaskName() {
}

DinoUi3PartsKasekiButton::~DinoUi3PartsKasekiButton() {
}

DinoUi3PartsMoney::~DinoUi3PartsMoney() {
}

void DinoUi3PartsMoney::Idle() {
}

void DinoUi3PartsMaskName::Idle() {
}

void DinoUi3PartsRockName::Idle() {
}

void DinoUi3PartsFraction::Idle() {
}

void DinoUi3PartsKasekiButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mView->Show();
    } else {
        mView->Hide();
    }
}

MRect DinoUi3PartsKasekiButton::GetFrame() {
    MPoint p(mPos.x, mPos.y);
    return MRect(MPoint(p.x - 24, p.y - 24), MPoint(p.x + 24, p.y + 24));
}
