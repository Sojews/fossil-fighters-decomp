// DinoUiFieldMenuFraction, DinoUiFieldMenuNumber, DinoUiFieldMenuNumber3, DinoUiPartsNumber8, DinoUiPartsReviverMedalButton, DinoUiPartsReviverNameCountCheckTask, DinoUiPartsBlackBelt, DinoUiPartsRockName, DinoUiPartsKasekiButton, DinoUiPartsTeamInfo, DinoUiPartsTeamTech.
// One TU: the field menu parts (0x021584d4-0x0215b008).

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_ui_field_menu_fraction_menu.h"
#include "game/dino_ui_field_menu_fraction.h"
#include "game/dino_ui_field_menu_number.h"
#include "game/dino_ui_field_menu_number3.h"
#include "game/dino_ui_parts_number8.h"
#include "game/dino_ui_parts_reviver_medal_button.h"
#include "game/dino_ui_parts_reviver_name_count_check_task.h"
#include "game/dino_ui_parts_black_belt.h"
#include "game/dino_ui_parts_rock_name.h"
#include "game/dino_ui_parts_kaseki_button.h"
#include "game/dino_ui_parts_team_info.h"
#include "game/dino_ui_parts_team_tech.h"
#include "game/dino_ui_parts_reviver_name.h"
#include "game/dino_ui_parts_team_indicator.h"
#include "game/dino_ui_field_menu_fraction4.h"
#include "game/dino_ui_field_menu_cursor.h"

DinoUiFieldMenuFraction::~DinoUiFieldMenuFraction() {
}

DinoUiFieldMenuNumber::~DinoUiFieldMenuNumber() {
}

DinoUiFieldMenuNumber3::~DinoUiFieldMenuNumber3() {
}

DinoUiPartsNumber8::~DinoUiPartsNumber8() {
}

DinoUiPartsReviverMedalButton::~DinoUiPartsReviverMedalButton() {
}

DinoUiPartsReviverNameCountCheckTask::~DinoUiPartsReviverNameCountCheckTask() {
}

DinoUiPartsBlackBelt::~DinoUiPartsBlackBelt() {
}

DinoUiPartsRockName::~DinoUiPartsRockName() {
}

DinoUiPartsKasekiButton::~DinoUiPartsKasekiButton() {
}

DinoUiPartsTeamInfo::~DinoUiPartsTeamInfo() {
}

DinoUiPartsTeamTech::~DinoUiPartsTeamTech() {
}

DinoUiPartsReviverName::~DinoUiPartsReviverName() {
}

void DinoUiFieldMenuFraction::Idle() {
}

DinoUiPartsTeamIndicator::~DinoUiPartsTeamIndicator() {
    mFont->Release();
    delete mView;
}

void DinoUiPartsTeamIndicator::Idle() {
}

DinoUiFieldMenuFraction4::~DinoUiFieldMenuFraction4() {
}

void DinoUiFieldMenuFraction4::Idle() {
}

void DinoUiPartsReviverMedalButton::OnDecide() {
}

void DinoUiPartsTeamTech::Idle() {
}

void DinoUiPartsTeamInfo::Idle() {
}

void DinoUiPartsRockName::Idle() {
}

void DinoUiPartsBlackBelt::Idle() {
}

void DinoUiPartsNumber8::Idle() {
}

void DinoUiFieldMenuNumber3::Idle() {
}

void DinoUiFieldMenuNumber::Idle() {
}

void DinoUiPartsKasekiButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mView->Show();
    } else {
        mView->Hide();
    }
}

MRect DinoUiPartsKasekiButton::GetFrame() {
    return MRect(mPos.Offset(-24, -24), mPos.Offset(24, 24));
}

MRect DinoUiPartsReviverMedalButton::GetFrame() {
    return mFrame;
}

void DinoUiPartsReviverMedalButton::OnFocusChanged(BOOL focused) {
    if (focused) {
        mMotion->Play(mKind + 3, 0);
    } else {
        mMotion->Play(mKind + 1, 0);
    }
    mMotion->SetPriority(2);
    mSerial.Clear();
}

void DinoUiFieldMenuCursor::Execute() {
    if (mUnk10->mState == STATE_RUN) {
        mUnk10->Execute();
    }
    if (mUnk14->mState == STATE_RUN) {
        mUnk14->Execute();
    }
}

void DinoUiFieldMenuCursor::SetPosition(MPoint pos) {
    mFrame.MoveTo(pos);
    mUnk10->SetPosition(mFrame.GetTopLeft());
    mUnk14->SetPosition(mFrame.GetTopLeft());
    mUnk2C = 0;
}

MPoint DinoUiFieldMenuCursor::GetPosition() {
    return mFrame.GetTopLeft();
}

void DinoUiFieldMenuCursor::SetSize(MPoint size) {
    mFrame.right = mFrame.left + size.x;
    mFrame.bottom = mFrame.top + size.y;
    mUnk10->SetPosition(mFrame.GetTopLeft());
    mUnk14->SetPosition(mFrame.GetTopLeft());
    mUnk2C = 0;
}

MPoint DinoUiFieldMenuCursor::GetSize() {
    return mFrame.GetSize();
}

void DinoUiFieldMenuCursor::SetPriority(int priority) {
    mUnk10->SetPriority(priority);
    mUnk14->SetPriority(priority);
}

int DinoUiFieldMenuCursor::GetPriority() {
    return mUnk10->GetPriority();
}

void DinoUiFieldMenuCursor::Show() {
    switch (mMode) {
    case 1:
        mUnk10->Show();
        break;
    case 2:
        mUnk14->Show();
        break;
    }
    mHidden = FALSE;
    mUnk2C = 0;
}

void DinoUiFieldMenuCursor::Hide() {
    switch (mMode) {
    case 1:
        mUnk10->Hide();
        break;
    case 2:
        mUnk14->Hide();
        break;
    }
    mHidden = TRUE;
    mUnk2C = 0;
}

BOOL DinoUiFieldMenuCursor::IsHidden() {
    return mHidden;
}

u32 DinoUiFieldMenuCursor::GetPlaneMask() {
    return 0x10;
}
