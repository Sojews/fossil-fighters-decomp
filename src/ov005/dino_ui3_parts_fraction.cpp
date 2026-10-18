// DinoUi3PartsFraction, DinoUi3PartsRockName, DinoUi3PartsMaskName, DinoUi3PartsKasekiButton, DinoUi3PartsMoney.
// Only the destructors are decompiled so far.

#include "game/dino_ui3_parts_fraction.h"
#include "game/dino_ui3_parts_rock_name.h"
#include "game/dino_ui3_parts_mask_name.h"
#include "game/dino_ui3_parts_kaseki_button.h"
#include "game/dino_ui3_parts_money.h"
#include "game/dino_ui3_parts_cursor.h"
#include "game/dino_backup.h"
#include <printf.h>

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

extern "C" const char data_ov005_02165f58[]; // ""
extern "C" const char data_ov005_02165f74[]; // the number format

void DinoUi3PartsRockName::Clear() {
    mText->SetText(std::string(data_ov005_02165f58));
}

void DinoUi3PartsMaskName::Clear() {
    mText->SetText(std::string(data_ov005_02165f58));
}

void DinoUi3PartsMoney::RefreshMoney() {
    char text[12];
    sprintf(text, data_ov005_02165f74, gDinoBackup->GetSystemVar(3));
    mMoney->SetText(std::string(text));
}

void DinoUi3PartsMoney::SetPrice(int price) {
    if (price == 0) {
        mPrice->SetText(std::string(data_ov005_02165f58));
        return;
    }
    char text[12];
    sprintf(text, data_ov005_02165f74, price);
    mPrice->SetText(std::string(text));
}

void DinoUi3PartsMoney::SetChange(int change) {
    char text[12];
    sprintf(text, data_ov005_02165f74, change);
    mChange->SetText(std::string(text));
}

void DinoUi3PartsKasekiButton::SetMarked(BOOL marked) {
    if (marked) {
        mMark->Show();
    } else {
        mMark->Hide();
    }
}

void DinoUi3PartsKasekiButton::SetActive(BOOL active) {
    if (active) {
        mMotion->GetUnit()->SetMode(1);
    } else {
        mMotion->GetUnit()->SetMode(0);
    }
}

void DinoUi3PartsKasekiButton::HideMotion() {
    mMotion->Hide();
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

void DinoUi3PartsFraction::SetNumerator(u8 n) {
    mNumerator->Play(n % 10 + 1, 0);
}

void DinoUi3PartsFraction::SetDenominator(u8 n) {
    mDenominator->Play(n % 10 + 1, 0);
}

void DinoUi3PartsFraction::Show() {
    mNumerator->Show();
    mDenominator->Show();
}

void DinoUi3PartsFraction::Hide() {
    mNumerator->Hide();
    mDenominator->Hide();
}
