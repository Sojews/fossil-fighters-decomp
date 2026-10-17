// DinoUi2PartsRockName.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_ui2_parts_rock_name_menu.h"
#include "game/dino_ui2_parts_rock_name.h"
#include "game/dino_ui2_parts_kaseki_button.h"
#include "game/dino_ui2_field_menu_cursor.h"

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

void DinoUi2PartsKasekiButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mView->Show();
    } else {
        mView->Hide();
    }
}

MRect DinoUi2PartsKasekiButton::GetFrame() {
    return MRect(mPos.Offset(-24, -24), mPos.Offset(24, 24));
}

void DinoUi2FieldMenuCursor::Execute() {
    if (mUnk10->mState == STATE_RUN) {
        mUnk10->Execute();
    }
    if (mUnk14->mState == STATE_RUN) {
        mUnk14->Execute();
    }
    if (mMode == 2 && mTime++ > 120) {
        mUnk14->Hide();
    }
}

void DinoUi2FieldMenuCursor::SetPosition(MPoint pos) {
    mFrame.MoveTo(pos);
    mUnk10->SetPosition(mFrame.GetTopLeft());
    mUnk14->SetPosition(mFrame.GetTopLeft());
    mTime = 0;
}

MPoint DinoUi2FieldMenuCursor::GetPosition() {
    return mFrame.GetTopLeft();
}

void DinoUi2FieldMenuCursor::SetSize(MPoint size) {
    mFrame.right = mFrame.left + size.x;
    mFrame.bottom = mFrame.top + size.y;
    mUnk10->SetPosition(mFrame.GetTopLeft());
    mUnk14->SetPosition(mFrame.GetTopLeft());
    mTime = 0;
}

MPoint DinoUi2FieldMenuCursor::GetSize() {
    return mFrame.GetSize();
}

void DinoUi2FieldMenuCursor::SetPriority(int priority) {
    mUnk10->SetPriority(priority);
    mUnk14->SetPriority(priority);
}

int DinoUi2FieldMenuCursor::GetPriority() {
    return mUnk10->GetPriority();
}

void DinoUi2FieldMenuCursor::Show() {
    switch (mMode) {
    case 1:
        mUnk10->Show();
        break;
    case 2:
        mUnk14->Show();
        break;
    }
    mHidden = FALSE;
    mTime = 0;
}

void DinoUi2FieldMenuCursor::Hide() {
    switch (mMode) {
    case 1:
        mUnk10->Hide();
        break;
    case 2:
        mUnk14->Hide();
        break;
    }
    mHidden = TRUE;
    mTime = 0;
}

BOOL DinoUi2FieldMenuCursor::IsHidden() {
    return mHidden;
}

u32 DinoUi2FieldMenuCursor::GetPlaneMask() {
    return 0x10;
}
