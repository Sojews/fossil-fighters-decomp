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
#include "game/dino_static_init.h"

// The TU's pooled constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x4];
    const char *digitPath;      // 0x04
    u8 unk08[0x14 - 0x8];
    const char *badgePath;      // 0x14
    u8 unk18[0x34 - 0x18];
    const char *pageNumberPath; // 0x34
    u8 unk38[0x48 - 0x38];
    const char *buttonPath;     // 0x48
} data_ov005_0216561c;
extern "C" const char *data_ov005_0216574c[]; // the fossils' motions (30), filled by the static initializer

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

extern "C" const char data_ov005_02165f5c[]; // the digits' font
extern "C" const char data_ov005_02165f58[]; // ""
extern "C" const char data_ov005_02165f74[]; // the number format
extern const MPoint data_020b5820, data_020b5828, data_020b5830; // the price's, change's and money's places

// The price, the player's money and the change, each its own line of digits.
// (No common subexpressions: the game reads gCurrentDisplay for each canvas after copying its size.)
#pragma opt_common_subs off
DinoUi3PartsMoney::DinoUi3PartsMoney() : MProcess(NULL) {
    MFontSet *font = new MFontSet(std::string(data_ov005_02165f5c), 0, 0, 0);
    MObjCanvas *canvas = new MObjCanvas(gCurrentDisplay, MPoint(0x40, 0x10), font->GetPalette(), TRUE);
    canvas->GetOamUnit()->SetMode(1);
    mPrice = new MIndicator(canvas, font, 0);
    mParallel.AddTail(mPrice);
    mPrice->SetPosition(data_020b5820);
    mPrice->SetPriority(2);
    mPrice->Show();
    mPrice->SetText(std::string(data_ov005_02165f58));
    canvas = new MObjCanvas(gCurrentDisplay, MPoint(0x40, 0x10), font->GetPalette(), TRUE);
    canvas->GetOamUnit()->SetMode(1);
    mMoney = new MIndicator(canvas, font, 0);
    mParallel.AddTail(mMoney);
    mMoney->SetPosition(data_020b5830);
    mMoney->SetPriority(2);
    mMoney->Show();
    char text[12];
    sprintf(text, data_ov005_02165f74, gDinoBackup->GetSystemVar(3));
    mMoney->SetText(std::string(text));
    canvas = new MObjCanvas(gCurrentDisplay, MPoint(0x40, 0x10), font->GetPalette(), TRUE);
    canvas->GetOamUnit()->SetMode(1);
    mChange = new MIndicator(canvas, font, 0);
    mParallel.AddTail(mChange);
    mChange->SetPosition(data_020b5828);
    mChange->SetPriority(2);
    mChange->Show();
    SetChange(0);
}
#pragma opt_common_subs reset

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
#pragma opt_common_subs off
DinoUi3PartsMaskName::DinoUi3PartsMaskName(MPoint pos) : MProcess(NULL) {
    mFont = gDinoResource->CreateFont12(0, 1, 0);
    MObjCanvas *canvas = new MObjCanvas(gCurrentDisplay, gCurrentDisplay->mObj->mOam, MPoint(160, 16),
                                        gDinoResource->GetUnkTable(), TRUE);
    canvas->GetOamUnit()->SetMode(1);
    mText = new MIndicator(canvas, mFont, 0);
    mParallel.AddTail(mText);
    mText->SetPosition(pos);
    mText->SetPriority(2);
    mText->Show();
}
#pragma opt_common_subs reset

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

// The fossil's motion, its focus highlight and mark, and its badge or value.
// NONMATCHING: in the digit loop the game spills &mParallel below the string temp's flag (slots 0xC/0x10 swapped);
// declaration order, ipa and the opt pragmas don't move it.
void DinoUi3PartsKasekiButton::Setup(MSmartOam *oam, DinoUiKasekiIcon icon) {
    mMotion = new MMotion(std::string(data_ov005_0216574c[icon.unk00 % 30]), oam, 2, 1);
    mMotion->Play(icon.unk01 + 1, 0);
    mMotion->SetPosition(mPos);
    mMotion->SetPriority(2);
    mMotion->GetUnit()->SetMode(1);
    mMotion->Show();
    mParallel.AddTail(mMotion);
    mView = new MMotion(std::string(data_ov005_0216561c.buttonPath), oam, 2, 1);
    mView->Play(1, 0);
    mView->SetPosition(mPos);
    mView->SetPriority(2);
    mView->GetUnit()->SetMode(1);
    mView->Hide();
    mParallel.AddTail(mView);
    mMark = new MMotion(std::string(data_ov005_0216561c.buttonPath), oam, 2, 1);
    mMark->Play(2, 0);
    mMark->SetPosition(mPos);
    mMark->SetPriority(2);
    mMark->GetUnit()->SetMode(1);
    mParallel.AddTail(mMark);
    if (icon.kind == 3 || ((icon.kind == 1 || icon.kind == 2) && icon.value >= 125)) {
        mBadge = new MMotion(std::string(data_ov005_0216561c.badgePath), oam, 0, 1);
        if (icon.kind == 3) {
            mBadge->Play(3, 0);
        } else {
            mBadge->Play(4, 0);
        }
        mBadge->SetPosition(MPoint(mPos.x - 3, mPos.y + 15));
        mBadge->SetPriority(2);
        mBadge->GetUnit()->SetMode(1);
        mBadge->Show();
        mParallel.AddTail(mBadge);
    } else if (icon.kind == 1 || icon.kind == 2 || icon.kind == 5) {
        for (int i = 0; i < 4; i++) {
            MTaskSet *tasks = &mParallel;
            mDigits[i] = new MMotion(std::string(data_ov005_0216561c.digitPath), oam, 0, 1);
            mDigits[i]->SetPosition(MPoint(mPos.x + (i * 8 - 14), mPos.y + 16));
            mDigits[i]->SetPriority(2);
            mDigits[i]->GetUnit()->SetMode(1);
            mDigits[i]->Show();
            tasks->AddTail(mDigits[i]);
        }
        mDigits[3]->SetPosition(MPoint(mPos.x + 9, mPos.y + 15));
        int value = icon.value;
        mDigits[0]->Play(value / 100 % 10 + 3, 0);
        mDigits[1]->Play(value / 10 % 10 + 3, 0);
        mDigits[2]->Play(value % 10 + 3, 0);
        if (icon.kind == 5) {
            mDigits[3]->Play(2, 0);
        } else {
            mDigits[3]->Play(1, 0);
        }
    }
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

// The file's static initialization (see dino_static_init.h).
extern "C" u32 data_ov005_0216571c[];
extern "C" u32 data_ov005_0216569c[];
extern "C" u32 data_ov005_0216579c[];
static struct DinoUi3PartsFractionStaticInit {
    DinoUi3PartsFractionStaticInit() {
        DINO_POOL_WORD(data_ov005_0216571c, 0x30) = ((s32 *) &data_ov005_0216561c)[0x18 / 4];
        DINO_POOL_WORD(data_ov005_0216571c, 0x34) = ((s32 *) &data_ov005_0216561c)[0x18 / 4];
        DINO_POOL_WORD(data_ov005_0216571c, 0x38) = DINO_POOL_WORD(data_ov005_0216569c, 0x8);
        DINO_POOL_WORD(data_ov005_0216571c, 0x3c) = DINO_POOL_WORD(data_ov005_0216561c, 0x60);
        DINO_POOL_WORD(data_ov005_0216571c, 0x40) = DINO_POOL_WORD(data_ov005_0216561c, 0x7c);
        DINO_POOL_WORD(data_ov005_0216571c, 0x44) = DINO_POOL_WORD(data_ov005_0216561c, 0x28);
        DINO_POOL_WORD(data_ov005_0216571c, 0x48) = DINO_POOL_WORD(data_ov005_0216561c, 0x2c);
        DINO_POOL_WORD(data_ov005_0216571c, 0x4c) = DINO_POOL_WORD(data_ov005_0216561c, 0x30);
        DINO_POOL_WORD(data_ov005_0216571c, 0x50) = DINO_POOL_WORD(data_ov005_0216561c, 0x64);
        DINO_POOL_WORD(data_ov005_0216571c, 0x54) = DINO_POOL_WORD(data_ov005_0216561c, 0x54);
        DINO_POOL_WORD(data_ov005_0216571c, 0x58) = DINO_POOL_WORD(data_ov005_0216561c, 0x20);
        DINO_POOL_WORD(data_ov005_0216571c, 0x5c) = DINO_POOL_WORD(data_ov005_0216561c, 0x8);
        DINO_POOL_WORD(data_ov005_0216571c, 0x60) = DINO_POOL_WORD(data_ov005_0216561c, 0x5c);
        DINO_POOL_WORD(data_ov005_0216571c, 0x64) = DINO_POOL_WORD(data_ov005_0216561c, 0x6c);
        DINO_POOL_WORD(data_ov005_0216571c, 0x68) = DINO_POOL_WORD(data_ov005_0216561c, 0x70);
        DINO_POOL_WORD(data_ov005_0216571c, 0x6c) = DINO_POOL_WORD(data_ov005_0216561c, 0x10);
        DINO_POOL_WORD(data_ov005_0216571c, 0x70) = DINO_POOL_WORD(data_ov005_0216561c, 0x44);
        DINO_POOL_WORD(data_ov005_0216571c, 0x74) = DINO_POOL_WORD(data_ov005_0216561c, 0x50);
        DINO_POOL_WORD(data_ov005_0216571c, 0x78) = DINO_POOL_WORD(data_ov005_0216561c, 0x1c);
        DINO_POOL_WORD(data_ov005_0216571c, 0x7c) = DINO_POOL_WORD(data_ov005_0216569c, 0x4);
        DINO_POOL_WORD(data_ov005_0216579c, 0x0) = DINO_POOL_WORD(data_ov005_0216561c, 0x4c);
        DINO_POOL_WORD(data_ov005_0216579c, 0x4) = DINO_POOL_WORD(data_ov005_0216561c, 0x24);
        DINO_POOL_WORD(data_ov005_0216579c, 0x8) = DINO_POOL_WORD(data_ov005_0216561c, 0x68);
        DINO_POOL_WORD(data_ov005_0216579c, 0xc) = DINO_POOL_WORD(data_ov005_0216561c, 0x78);
        DINO_POOL_WORD(data_ov005_0216579c, 0x10) = DINO_POOL_WORD(data_ov005_0216561c, 0x38);
        DINO_POOL_WORD(data_ov005_0216579c, 0x14) = DINO_POOL_WORD(data_ov005_0216561c, 0x3c);
        DINO_POOL_WORD(data_ov005_0216579c, 0x18) = DINO_POOL_WORD(data_ov005_0216561c, 0x74);
        DINO_POOL_WORD(data_ov005_0216579c, 0x1c) = DINO_POOL_WORD(data_ov005_0216569c, 0x0);
        DINO_POOL_WORD(data_ov005_0216579c, 0x20) = DINO_POOL_WORD(data_ov005_0216561c, 0xc);
        DINO_POOL_WORD(data_ov005_0216579c, 0x24) = DINO_POOL_WORD(data_ov005_0216561c, 0x58);
    }
} sDinoUi3PartsFractionStaticInit;
