// DinoUiFieldMenuFraction, DinoUiFieldMenuNumber, DinoUiFieldMenuNumber3, DinoUiPartsNumber8, DinoUiPartsReviverMedalButton, DinoUiPartsReviverNameCountCheckTask, DinoUiPartsBlackBelt, DinoUiPartsRockName, DinoUiPartsKasekiButton, DinoUiPartsTeamInfo, DinoUiPartsTeamTech.
// One TU: the field menu parts (0x021584d4-0x0215b008).
// NONMATCHING: the constructors of DinoUiFieldMenuNumber and DinoUiFieldMenuFraction4 (the game keeps this + i * 4
// in a stack slot across the first string temporary's destructor), DinoUiFieldMenuNumber3, DinoUiPartsNumber8 and
// DinoUiPartsReviverName (the game loads gCurrentDisplay after copying the canvas size), and
// DinoUiPartsReviverName::SetCreature (the game takes the length limit's address from its own literal).

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_resource.h"
#include "game/dino_sound.h"
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
#include "m/display.h"
#include "m/font.h"
#include "m/obj.h"
#include "m/oam_canvas.h"
#include <algorithm>
#include <stdio.h>

extern const char *data_ov003_02160c60[]; // motion file paths
extern const char *data_ov003_02160ce0[]; // more motion file paths

// The medal motions by medal kind (-1 ends the table).
struct MedalMotion {
    int kind;
    const char *path;
};
extern MedalMotion data_ov003_02160fb0[5];
extern const char *data_ov003_02160f60[];

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

DinoUiFieldMenuNumber::DinoUiFieldMenuNumber(MSmartOam *oam, MPoint pos) : MProcess(NULL) {
    for (int i = 0; i < 5; i++) {
        mDigits[i] = new MMotion(std::string(data_ov003_02160c60[28]), oam, 2, 1);
        mDigits[i]->Play(1, 0);
        mDigits[i]->SetPosition(pos.x + i * 8, pos.y);
        mDigits[i]->SetPriority(2);
        mDigits[i]->Show();
        mParallel.AddTail(mDigits[i]);
        mDigits[i]->GetUnit()->SetMode(1);
    }
}

void DinoUiFieldMenuNumber::SetValue(int value) {
    if (value >= 100000) {
        value = 100000 - 1;
    }
    if (value < 0) {
        value = 0;
    }
    BOOL shown = FALSE;
    int unit = 100000;
    for (int i = 0; i < 5; i++) {
        unit /= 10;
        if (shown || value / unit % 10 > 0) {
            mDigits[i]->Play(value / unit % 10 + 1, 0);
            mDigits[i]->Show();
            shown = TRUE;
        } else {
            mDigits[i]->Hide();
        }
    }
    // Zero still shows one digit.
    if (!shown) {
        mDigits[4]->Play(1, 0);
        mDigits[4]->Show();
    }
}

DinoUiFieldMenuNumber3::DinoUiFieldMenuNumber3(MPoint pos) : MProcess(NULL) {
    MFontSet *font = new MFontSet(std::string("/font/btl_fontset_8x16"), 0, 0, 0);
    mText = new MIndicator(new MObjCanvas(gCurrentDisplay, gCurrentDisplay->mObj->mOam, MPoint(0x20, 0x10),
                                          font->GetPalette(), TRUE),
                           font, 0);
    mParallel.AddTail(mText);
    mText->SetPosition(pos.x, pos.y);
    mText->SetPriority(2);
    mText->Show();
}

void DinoUiFieldMenuNumber3::SetValue(u32 value) {
    char text[4];
    if (value == VALUE_MANY) {
        sprintf(text, "+++");
    } else if (value == VALUE_MORE) {
        sprintf(text, "  +");
    } else if (value == VALUE_UNKNOWN) {
        sprintf(text, "???");
    } else if (value == VALUE_UNKNOWN2) {
        sprintf(text, " ??");
    } else {
        if (value >= 1000) {
            value = 1000 - 1;
        }
        sprintf(text, "%3d", value);
    }
    mText->SetText(std::string(text));
}

void DinoUiFieldMenuNumber3::SetPriority(int priority) {
    mText->SetPriority(priority);
}

void DinoUiFieldMenuNumber3::Show() {
    mText->Show();
}

void DinoUiFieldMenuNumber3::Hide() {
    mText->Hide();
}

DinoUiPartsNumber8::DinoUiPartsNumber8(MPoint pos) : MProcess(NULL) {
    MFontSet *font = new MFontSet(std::string("/font/kp_number"), 0, 0, 0);
    MObjCanvas *canvas = new MObjCanvas(gCurrentDisplay, gCurrentDisplay->mObj->mOam, MPoint(0x40, 0x20),
                                        font->GetPalette(), TRUE);
    canvas->GetOamUnit()->SetMode(1);
    mText = new MIndicator(canvas, font, 0);
    mParallel.AddTail(mText);
    mText->SetPosition(pos.x, pos.y);
    mText->SetPriority(2);
    mText->Show();
}

void DinoUiPartsNumber8::SetValue(u32 value) {
    char text[16];
    if (value >= 100000000) {
        value = 100000000 - 1;
    }
    sprintf(text, "%8d", value);
    mText->SetText(std::string(text));
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

extern "C" const char data_ov003_02161cd4[]; // ""

void DinoUiPartsRockName::Clear() {
    mText->SetText(std::string(data_ov003_02161cd4));
}

void DinoUiPartsKasekiButton::SetMarked(BOOL marked) {
    if (marked) {
        mMark->Show();
    } else {
        mMark->Hide();
    }
}

void DinoUiPartsKasekiButton::HideMotion() {
    mMotion->Hide();
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

#pragma auto_inline off
static const char *GetMedalMotion(int kind) {
    for (u32 i = 0; i < 5; i++) {
        if (kind == data_ov003_02160fb0[i].kind || data_ov003_02160fb0[i].kind == -1) {
            return data_ov003_02160fb0[i].path;
        }
    }
    return data_ov003_02160f60[21];
}
#pragma auto_inline reset

DinoUiPartsReviverMedalButton::DinoUiPartsReviverMedalButton(MSmartOam *oam, MPoint pos, u32 creature, u8 lit,
                                                             u8 byKind) {
    mKind = lit == 0 ? 1 : 0;
    mEmpty = creature == 0xFFFF ? 1 : 0;
    if (creature == 0xFFFF) {
        creature = 1;
    }
    if (byKind) {
        const DinoCreatureDef *def = gDinoResource->GetCreatureDef(creature);
        mMotion = new MMotion(std::string(GetMedalMotion(def->unk0D)), oam, 2, 1);
    } else if (creature == 0) {
        mMotion = new MMotion(std::string(data_ov003_02160ce0[11]), oam, 2, 1);
    } else {
        char path[256];
        OS_SPrintf(path, "/motion/ui_reviver_medal/reviver_%03d.bin", creature);
        mMotion = new MMotion(std::string(path), oam, 2, 1);
    }
    mMotion->Play(mKind + 1, 0);
    mMotion->SetPosition(pos);
    mMotion->SetPriority(2);
    mParallel.AddTail(mMotion);
    if (mEmpty) {
        mMotion->Hide();
    } else {
        mMotion->Show();
    }
    mMotion->GetUnit()->SetMode(1);
    mFrame = MRect(pos.Offset(-16, -16), pos.Offset(16, 16));
}

void DinoUiPartsReviverMedalButton::PlayNormal() {
    mMotion->Play(mKind + 1, 0);
    mMotion->SetPriority(2);
}

void DinoUiPartsReviverMedalButton::PlayFocused() {
    mMotion->Play(mKind + 3, 0);
    mMotion->SetPriority(2);
}

void DinoUiPartsReviverMedalButton::PlayDecided() {
    mMotion->Play(mKind + 5, 0);
    mMotion->SetPriority(1);
    if (!mEmpty) {
        DinoSoundPlaySe(0x9E, 0);
    }
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

void DinoUiFieldMenuCursor::SetMode(u32 mode) {
    if (mode >= 1 && mode <= 2) {
        BOOL hidden = IsHidden();
        Hide();
        mMode = mode;
        if (hidden) {
            Hide();
        } else {
            Show();
        }
        mUnk2C = 0;
    }
}

// Plays both motions once.
MTask *DinoUiFieldMenuCursor::CreatePlayTask() {
    MParallelTaskSet *tasks = new MParallelTaskSet(TRUE);
    tasks->AddTail(mUnk10->CreatePlayTransfer(1, 1));
    tasks->AddTail(mUnk14->CreatePlayTransfer(1, 1));
    return tasks;
}

MTask *DinoUiFieldMenuCursor::CreateMoveTask(MRect from, MRect to, int duration, int ease) {
    DinoSoundPlaySe(0x96, 0);
    return CreateFrameAnime(from, to, duration, ease);
}

void DinoUiFieldMenuFraction::SetPriority(int priority) {
    mNumerator->SetPriority(priority);
    mDenominator->SetPriority(priority);
}

MTask *DinoUiPartsReviverName::CreateFlyInTask(MPoint to) {
    MSerialTaskSet *tasks = new MSerialTaskSet(TRUE);
    MTask *front = mMotion->CreatePriorityTransfer(2);
    MTask *wait = mMotion->CreateWaitStopTask();
    MTask *landSe = DinoSoundCreatePlaySeTask(0xA2, 0);
    MTask *land = mMotion->CreatePlayTransfer(mLength + 7, 0);
    MTask *fly = mMotion->CreateCoordAnime(mMotion->GetPosition(), to, 30, 0);
    MTask *flySe = DinoSoundCreatePlaySeTask(0xA0, 0);
    tasks->AddTail(mMotion->CreatePriorityTransfer(1));
    tasks->AddTail(flySe);
    tasks->AddTail(fly);
    tasks->AddTail(land);
    tasks->AddTail(landSe);
    tasks->AddTail(wait);
    tasks->AddTail(front);
    return tasks;
}

DinoUiPartsReviverName::DinoUiPartsReviverName() : MProcess(NULL) {
    mLength = 0;
    mLeft = 0;
    mMotionHidden = FALSE;
    mTextHidden = FALSE;
    mOam = new MSmartOam(20, 0, TRUE);
    mMotion = new MMotion(std::string(data_ov003_02160c60[14]), mOam, 2, 1);
    mMotion->Play(1, 0);
    mMotion->SetPosition(0, 0);
    mMotion->SetPriority(1);
    mMotion->Hide();
    mParallel.AddTail(mMotion);
    mMotion->GetUnit()->SetMode(1);
    mFont = gDinoResource->CreateFont12(0, 1, 0);
    MObjCanvas *canvas = new MObjCanvas(gCurrentDisplay, gCurrentDisplay->mObj->mOam, MPoint(160, 16),
                                        gDinoResource->GetUnkTable(), TRUE);
    canvas->GetOamUnit()->SetMode(1);
    mText = new MIndicator(canvas, mFont, 0);
    mParallel.AddTail(mText);
    mText->SetPosition(0, 0);
    mText->SetPriority(1);
    mText->Hide();
    mParallel.AddTail(mOam);
    Activate();
}

void DinoUiPartsReviverName::SetCreature(u32 id) {
    std::string name(gDinoResource->GetCreatureText08(id));
    mText->SetText(std::string(name));
    MPoint size = mFont->MeasureText(name, 0, 0);
    u8 length = (size.x + 7) / 8;
    // The longest plate (16 letters) is a constant in the file's data.
    mLength = std::min(length, ((const u8 *)data_ov003_02160c60)[2]);
    mMotion->Hide();
    mText->Hide();
    mLeft = mLength + 15;
    mTextWidth = size.x;
}

// Keeps the plate on the screen.
void DinoUiPartsReviverName::SetPosition(MPoint pos) {
    int x = pos.x;
    int y = pos.y;
    if (x < 8) {
        x = 8;
    }
    if (x + mLength * 8 + 16 > 248) {
        x = 232 - mLength * 8;
    }
    mMotion->SetPosition(MPoint(x + 16, y));
    x += 8;
    mText->SetPosition(MPoint(x + (u8)((mLength * 8 - mTextWidth) / 2), y + 6));
}

void DinoUiPartsReviverName::Hide() {
    mMotion->Hide();
    mText->Hide();
    mLeft = 0;
}

void DinoUiPartsReviverName::Idle() {
    if (mLeft) {
        mLeft--;
        if (mLength >= mLeft) {
            mMotion->Play(mLength - mLeft + 1, 0);
            mMotion->Show();
            if (mLeft == 0) {
                mText->Show();
            }
        }
    }
}

// While the help is open, the name is hidden.
void DinoUiPartsReviverName::Unk08() {
    if (!mMotion->IsHidden()) {
        mMotion->Hide();
        mMotionHidden = TRUE;
        if (mMotion->mState == STATE_RUN) {
            mMotion->Execute();
        }
        if (mOam->mState == STATE_RUN) {
            mOam->Execute();
        }
    } else {
        mMotionHidden = FALSE;
    }
    if (!mText->IsHidden()) {
        mText->Hide();
        mTextHidden = TRUE;
    } else {
        mTextHidden = FALSE;
    }
}

void DinoUiPartsReviverName::Unk0C() {
    if (mMotionHidden) {
        mMotion->Show();
    }
    if (mTextHidden) {
        mText->Show();
    }
}

DinoUiPartsReviverNameCountCheckTask::DinoUiPartsReviverNameCountCheckTask(DinoUiPartsReviverName *name) {
    mName = name;
}

void DinoUiPartsReviverNameCountCheckTask::Execute() {
    if (mName->mLeft == 0) {
        Exit();
    }
}

// The numerator up and left of pos, the denominator down and right.
DinoUiFieldMenuFraction::DinoUiFieldMenuFraction(MSmartOam *oam, MPoint pos) : MProcess(NULL) {
    mNumerator = new MMotion(std::string(data_ov003_02160c60[9]), oam, 2, 1);
    mNumerator->Play(1, 0);
    mNumerator->SetPosition(pos.x - 4, pos.y - 4);
    mNumerator->SetPriority(2);
    mNumerator->Show();
    mNumerator->GetUnit()->SetMode(1);
    mParallel.AddTail(mNumerator);
    mDenominator = new MMotion(std::string(data_ov003_02160c60[9]), oam, 2, 1);
    mDenominator->Play(1, 0);
    mDenominator->SetPosition(pos.x + 4, pos.y + 4);
    mDenominator->SetPriority(2);
    mDenominator->Show();
    mDenominator->GetUnit()->SetMode(1);
    mParallel.AddTail(mDenominator);
}

void DinoUiFieldMenuFraction::SetNumerator(u8 n) {
    mNumerator->Play(n % 10 + 1, 0);
}

void DinoUiFieldMenuFraction::SetDenominator(u8 n) {
    mDenominator->Play(n % 10 + 1, 0);
}

DinoUiFieldMenuFraction4::DinoUiFieldMenuFraction4(MSmartOam *oam, MPoint pos, u8 digits) : MProcess(NULL) {
    mDigits = digits;
    if (mDigits < 1) {
        mDigits = 1;
    }
    if (mDigits > 3) {
        mDigits = 3;
    }
    for (int i = 0; i < mDigits; i++) {
        mNumerator[i] = new MMotion(std::string(data_ov003_02160c60[9]), oam, 2, 1);
        mNumerator[i]->Play(1, 0);
        mNumerator[i]->SetPosition(pos.x - 6 - i * 8, pos.y);
        mNumerator[i]->SetPriority(2);
        mNumerator[i]->Show();
        mNumerator[i]->GetUnit()->SetMode(1);
        mParallel.AddTail(mNumerator[i]);
        mDenominator[i] = new MMotion(std::string(data_ov003_02160c60[9]), oam, 2, 1);
        mDenominator[i]->Play(1, 0);
        mDenominator[i]->SetPosition(pos.x + 7 + (mDigits - 1 - i) * 8, pos.y);
        mDenominator[i]->SetPriority(2);
        mDenominator[i]->Show();
        mDenominator[i]->GetUnit()->SetMode(1);
        mParallel.AddTail(mDenominator[i]);
    }
    mSlash = new MMotion(std::string(data_ov003_02160c60[9]), oam, 2, 1);
    mSlash->Play(11, 0);
    mSlash->SetPosition(pos.x, pos.y);
    mSlash->SetPriority(2);
    mSlash->Show();
    mParallel.AddTail(mSlash);
}

void DinoUiFieldMenuFraction4::SetNumerator(u8 n) {
    for (int i = 0; i < mDigits; i++) {
        mNumerator[i]->Play(n % 10 + 1, 0);
        n /= 10;
    }
}

void DinoUiFieldMenuFraction4::SetDenominator(u8 n) {
    for (int i = 0; i < mDigits; i++) {
        mDenominator[i]->Play(n % 10 + 1, 0);
        n /= 10;
    }
}

void DinoUiFieldMenuFraction4::SetPriority(int priority) {
    for (int i = 0; i < mDigits; i++) {
        mNumerator[i]->SetPriority(priority);
        mDenominator[i]->SetPriority(priority);
    }
    mSlash->SetPriority(priority);
}
