// DinoUi2PartsRockName.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_ui2_parts_rock_name_menu.h"
#include "game/dino_ui2_parts_rock_name.h"
#include "game/dino_ui2_parts_kaseki_button.h"

DinoUi2PartsRockName::~DinoUi2PartsRockName() {
}

DinoUi2PartsKasekiButton::~DinoUi2PartsKasekiButton() {
}

void DinoUi2PartsRockName::Idle() {
}

MRect DinoUi2PartsKasekiButton::GetRect() {
    return GetFrame();
}

MDisplay *DinoUi2PartsKasekiButton::GetDisplay() {
    return MProcess::GetDisplay();
}

const char *DinoUi2PartsKasekiButton::GetText() {
    return gDinoResource->GetText(mTextId);
}
