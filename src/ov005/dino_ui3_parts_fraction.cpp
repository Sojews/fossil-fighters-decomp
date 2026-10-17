// DinoUi3PartsFraction, DinoUi3PartsRockName, DinoUi3PartsMaskName, DinoUi3PartsKasekiButton, DinoUi3PartsMoney.
// Only the destructors are decompiled so far.

#include "game/dino_ui3_parts_fraction.h"
#include "game/dino_ui3_parts_rock_name.h"
#include "game/dino_ui3_parts_mask_name.h"
#include "game/dino_ui3_parts_kaseki_button.h"
#include "game/dino_ui3_parts_money.h"
#include "game/dino_ui3_parts_cursor.h"

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
    return MRect(mPos.Offset(-24, -24), mPos.Offset(24, 24));
}

void DinoUi3PartsCursor::Execute() {
    if (mUnk10->mState == STATE_RUN) {
        mUnk10->Execute();
    }
}

void DinoUi3PartsCursor::SetPosition(MPoint pos) {
    mFrame.MoveTo(pos);
    mUnk10->SetPosition(mFrame.GetTopLeft());
    mTime = 0;
}

MPoint DinoUi3PartsCursor::GetPosition() {
    return mFrame.GetTopLeft();
}

void DinoUi3PartsCursor::SetSize(MPoint size) {
    mFrame.right = mFrame.left + size.x;
    mFrame.bottom = mFrame.top + size.y;
    mUnk10->SetPosition(mFrame.GetTopLeft());
    mTime = 0;
}

MPoint DinoUi3PartsCursor::GetSize() {
    return mFrame.GetSize();
}

void DinoUi3PartsCursor::SetPriority(int priority) {
    mUnk10->SetPriority(priority);
}

int DinoUi3PartsCursor::GetPriority() {
    return mUnk10->GetPriority();
}

void DinoUi3PartsCursor::Show() {
    mUnk10->Show();
    mHidden = FALSE;
    mTime = 0;
}

void DinoUi3PartsCursor::Hide() {
    mUnk10->Hide();
    mHidden = TRUE;
    mTime = 0;
}

BOOL DinoUi3PartsCursor::IsHidden() {
    return mHidden;
}

u32 DinoUi3PartsCursor::GetPlaneMask() {
    return 0x10;
}
