// DinoUi3PartsFraction, DinoUi3PartsRockName, DinoUi3PartsMaskName, DinoUi3PartsKasekiButton, DinoUi3PartsMoney.
// Only the destructors are decompiled so far.

#include "game/dino_ui3_parts_fraction.h"
#include "m/smart_oam.h"
#include "game/dino_ui3_parts_rock_name.h"
#include "game/dino_ui3_parts_mask_name.h"
#include "game/dino_ui3_parts_kaseki_button.h"
#include "game/dino_ui3_parts_money.h"
#include "game/dino_ui3_parts_cursor.h"
#include "game/dino_backup.h"
#include "game/dino_resource.h"
#include <printf.h>
#include "game/dino_sound.h"
#include "m/indicator.h"
#include "m/font.h"
#include "m/oam_canvas.h"
#include "m/obj.h"
#include "m/display.h"

// The TU's pooled constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x34];
    const char *pageNumberPath; // 0x34
} data_ov005_0216561c;

// "n/m": the two digits sit a little up-left and down-right of pos.
DinoUi3PartsFraction::DinoUi3PartsFraction(MSmartOam *oam, MPoint pos) : MProcess(NULL) {
    mNumerator = new MMotion(data_ov005_0216561c.pageNumberPath, oam, 2, 1);
    mNumerator->Play(1, 0);
    mNumerator->SetPosition(pos.x - 4, pos.y - 4);
    mNumerator->SetPriority(2);
    mNumerator->Show();
    mNumerator->GetUnit()->SetMode(1);
    mParallel.AddTail(mNumerator);
    mDenominator = new MMotion(data_ov005_0216561c.pageNumberPath, oam, 2, 1);
    mDenominator->Play(1, 0);
    mDenominator->SetPosition(pos.x + 4, pos.y + 4);
    mDenominator->SetPriority(2);
    mDenominator->Show();
    mDenominator->GetUnit()->SetMode(1);
    mParallel.AddTail(mDenominator);
}

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

// The fossil's name, in a color by its state.
void DinoUi3PartsRockName::SetEntry(const DinoBackupEntry *entry, BOOL selected) {
    DinoKasekiRockState rock;
    gDinoBackup->GetKasekiRock(entry->id, (u32 *)&rock);
    mText->SetText(std::string(data_ov005_02165f58));
    if (mText->mState == STATE_RUN) {
        mText->Execute();
    }
    if (selected) {
        mFont->SetShadowColor(3);
    } else if (!rock.unk10 && entry->unk10_3) {
        mFont->SetShadowColor(5);
    } else {
        mFont->SetShadowColor(1);
    }
    mText->SetText(gDinoBackup->GetEntryName(entry));
}

void DinoUi3PartsRockName::Clear() {
    mText->SetText(std::string(data_ov005_02165f58));
}

// The mask's name, greyed if it can't be bought.
void DinoUi3PartsMaskName::SetMask(u32 id, BOOL greyed) {
    mText->SetText(std::string(data_ov005_02165f58));
    if (mText->mState == STATE_RUN) {
        mText->Execute();
    }
    if (greyed) {
        mFont->SetShadowColor(3);
    } else {
        mFont->SetShadowColor(1);
    }
    MResource<DinoHeadMaskDefs> defs = gDinoResource->LoadHeadMaskDefs();
    const DinoHeadMaskDefs *data = defs.Get();
    mText->SetText(std::string(gDinoResource->GetText(
        ((const DinoHeadMaskDef *)((const u8 *)data + ((const u32 *)((const u8 *)data + data->tableOffset))[id]))
            ->nameText)));
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

DinoUi3PartsKasekiButton::DinoUi3PartsKasekiButton(MSmartOam *oam, MPoint pos, DinoUiKasekiIcon icon, u8 marked) {
    mPos = pos;
    Setup(oam, icon);
    SetMarked(marked);
    SetMode(2);
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

MTask *DinoUi3PartsCursor::CreateMoveTask(MRect from, MRect to, int duration, int ease) {
    DinoSoundPlaySe(0x96, 0);
    return CreateFrameAnime(from, to, duration, ease);
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

#pragma opt_common_subs off
DinoUi3PartsRockName::DinoUi3PartsRockName(MPoint pos) : MProcess(NULL) {
    mFont = gDinoResource->CreateFont12(0, 1, 0);
    MObjCanvas *canvas = new MObjCanvas(gCurrentDisplay, gCurrentDisplay->mObj->mOam, MPoint(256, 32),
                                        gDinoResource->GetUnkTable(), TRUE);
    canvas->GetOamUnit()->SetMode(1);
    mText = new MIndicator(canvas, mFont, 0);
    mParallel.AddTail(mText);
    mText->SetPosition(pos);
    mText->SetPriority(2);
    mText->Show();
}
#pragma opt_common_subs reset
