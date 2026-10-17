// DinoCommonItem.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_common_item_menu.h"
#include "game/dino_common_item.h"

DinoCommonItem::~DinoCommonItem() {
}

MRect DinoCommonItem::GetRect() {
    return GetFrame();
}

MDisplay *DinoCommonItem::GetDisplay() {
    return MProcess::GetDisplay();
}

const char *DinoCommonItem::GetText() {
    return gDinoResource->GetText(mTextId);
}

void DinoCommonItem::OnDecide() {
}

MRect DinoCommonItem::GetFrame() {
    MPoint d(-mOam->mOffset.x, -mOam->mOffset.y);
    MRect r = mFrame;
    r.Offset(d);
    return r;
}

void DinoCommonItem::OnSelectChanged(BOOL selected) {
    UpdateLook();
    if (mView) {
        if (selected) {
            mView->SetPosition(MPoint(mPos.x + 1, mPos.y + 1));
        } else {
            mView->SetPosition(mPos);
        }
    }
}


void DinoCommonItem::OnFocusChanged(BOOL focused) {
    UpdateLook();
}

void DinoCommonItem::UpdateLook() {
    if (IsSelected()) {
        mMotion->Play(3, 0);
    } else if (IsFocused()) {
        mMotion->Play(2, 0);
    } else {
        mMotion->Play(1, 0);
    }
}

void DinoCommonItem::OnEnableChanged(BOOL enabled) {
    if (mView) {
        if (enabled) {
            mView->SetColors(0, 1, 10);
        } else {
            mView->SetColors(0, 11, 10);
        }
    }
}
