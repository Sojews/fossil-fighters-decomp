// DinoUiOrderSelect.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_ui_order_select.h"
#include "types.h"

DinoUiOrderSelect::~DinoUiOrderSelect() {
}

DinoUiOrderSelectMenuItem::~DinoUiOrderSelectMenuItem() {
}

DinoUiOrderSelectBackButton::~DinoUiOrderSelectBackButton() {
}

DinoUiOrderSelectDecideButton::~DinoUiOrderSelectDecideButton() {
}

MRect DinoUiOrderSelectMenuItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoUiOrderSelectMenuItem::GetDisplay() {
    return MProcess::GetDisplay();
}

MRect DinoUiOrderSelectMenuItem::GetFrame() {
    return mMotion->GetFrame();
}

void DinoUiOrderSelectBackButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(3, 0);
    } else {
        mMotion->Play(2, 0);
    }
}

void DinoUiOrderSelectMenuItem::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(2, 0);
    } else {
        mMotion->Play(1, 0);
    }
}

const char *DinoUiOrderSelectMenuItem::GetText() {
    return gDinoResource->GetText(mTextId);
}
