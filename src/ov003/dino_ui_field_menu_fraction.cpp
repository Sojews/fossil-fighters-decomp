// DinoUiFieldMenuCursor, DinoUiFieldMenuFraction(4), DinoUiFieldMenuNumber(3), DinoUiPartsNumber8,
// DinoUiPartsReviverMedalButton, DinoUiPartsReviverName(CountCheckTask), DinoUiPartsBlackBelt, DinoUiPartsRockName,
// DinoUiPartsKasekiButton, DinoUiPartsTeamInfo, DinoUiPartsTeamTech, DinoUiPartsTeamIndicator.
// One TU: the field menu parts (0x021584d4-0x0215b008).
// NONMATCHING: the constructors of DinoUiFieldMenuNumber, DinoUiFieldMenuFraction4 and DinoUiPartsTeamTech (the game
// keeps this + i * 4 in a stack slot across the first string temporary's destructor), DinoUiFieldMenuNumber3,
// DinoUiPartsNumber8, DinoUiPartsReviverName, DinoUiPartsRockName and DinoUiPartsTeamIndicator (the game loads
// gCurrentDisplay after copying the canvas size), DinoUiPartsReviverName::SetCreature (the game takes the length
// limit's address from its own literal) and DinoUiPartsKasekiButton::Setup (four stack slots in another order).

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
#include "nitro/os.h"
#include "nitro/mi.h"
#include "game/dino_team.h"
#include "game/dino_creature_sort.h"
#include "m/indicator.h"
#include "m/smart_oam.h"
#include "m/motion.h"
#include "m/picture.h"
#include "m/lcd.h"
#include "m/font.h"
#include "m/obj.h"
#include "m/oam_canvas.h"
#include <algorithm>
#include <stdio.h>
#include "game/dino_static_init.h"

extern const char *data_ov003_02160c60[]; // motion file paths
extern const char *data_ov003_02160ce0[]; // more motion file paths

// The medal motions by medal kind (-1 ends the table).
struct MedalMotion {
    int kind;
    const char *path;
};
extern MedalMotion data_ov003_02160fb0[5];
extern const char *data_ov003_02160f60[];
extern const MPoint data_ov003_02160d3c; // where the black belt goes
extern const char *data_ov003_02161028[]; // the kaseki icon motions
extern const u16 data_020b5870[];         // a palette

// Which way of looking a kind uses (-1 ends the table).
struct KindLook {
    int kind;
    int look;
};
extern const KindLook data_ov003_02160210[6];

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
    delete mCanvas;
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

#pragma opt_common_subs off
// (Not auto-inlined: the team info calls it, as it does SetPriority.)
#pragma auto_inline off
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
#pragma auto_inline reset
#pragma opt_common_subs reset

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

#pragma auto_inline off
void DinoUiFieldMenuNumber3::SetPriority(int priority) {
    mText->SetPriority(priority);
}
#pragma auto_inline reset

void DinoUiFieldMenuNumber3::Show() {
    mText->Show();
}

void DinoUiFieldMenuNumber3::Hide() {
    mText->Hide();
}

#pragma opt_common_subs off
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
#pragma opt_common_subs reset

void DinoUiPartsNumber8::SetValue(u32 value) {
    char text[16];
    if (value >= 100000000) {
        value = 100000000 - 1;
    }
    sprintf(text, "%8d", value);
    mText->SetText(std::string(text));
}

#pragma auto_inline off
static int GetKindLook(int kind) {
    for (u32 i = 0; i < 6; i++) {
        if (kind == data_ov003_02160210[i].kind || data_ov003_02160210[i].kind == -1) {
            return data_ov003_02160210[i].look;
        }
    }
    return 1;
}
#pragma auto_inline reset

extern const char data_ov003_02161cd8[];      // the team info's background
extern const MPoint data_ov003_02160d7c;      // the team's name
extern const MPoint data_ov003_02160f88[5];   // by member: the icons
extern const MPoint data_ov003_02160fd8[10];  // the members' numbers
extern const MPoint data_ov003_02160f28[];    // the team tech's positions

// The team's name (two styles), the members' names and numbers, the icons of rows 3 and 4, and the team's tech.
// (opt_common_subs off: the game reads gCurrentDisplay again after copying each canvas's size.)
#pragma opt_common_subs off
DinoUiPartsTeamInfo::DinoUiPartsTeamInfo() : MProcess(NULL) {
    gCurrentDisplay = gLcd->mMain;
    MTextPicture *bg;
    mParallel.AddTail(bg = new MTextPicture(std::string(data_ov003_02161cd8), TRUE, false));
    bg->Show();
    bg->SetPriority(3);
    MFontSet *font = gDinoResource->CreateFont12(0, 10, 1);
    mName = new MIndicator(new MObjCanvas(gCurrentDisplay, gCurrentDisplay->mObj->mOam, MPoint(128, 32), gDinoResource->GetUnkTable(), TRUE), font, 0);
    mParallel.AddTail(mName);
    mName->SetPosition(data_ov003_02160d7c);
    mName->SetPriority(3);
    mName->Hide();
    MFontSet *font2 = gDinoResource->CreateFont12(0, 1, 0);
    mOtherName = new MIndicator(new MObjCanvas(gCurrentDisplay, gCurrentDisplay->mObj->mOam, MPoint(128, 32), gDinoResource->GetUnkTable(), TRUE), font2, 0);
    mParallel.AddTail(mOtherName);
    mOtherName->SetPosition(data_ov003_02160d7c);
    mOtherName->SetPriority(3);
    mOtherName->Hide();
    for (int i = 0; i < 5; i++) {
        mTexts[i] = new MIndicator(new MObjCanvas(gCurrentDisplay, gCurrentDisplay->mObj->mOam, MPoint(128, 32), gDinoResource->GetUnkTable(), TRUE), font, 0);
        mParallel.AddTail(mTexts[i]);
        mTexts[i]->SetPosition(((const MPoint *)data_ov003_02160f60)[i]);
        mTexts[i]->SetPriority(3);
        mTexts[i]->Show();
    }
    for (int i = 0; i < 10; i++) {
        mNumbers[i] = new DinoUiFieldMenuNumber3(data_ov003_02160fd8[i]);
        mNumbers[i]->SetPriority(3);
        mParallel.AddTail(mNumbers[i]);
    }
    mOam = new MSmartOam(10, 0, TRUE);
    mMotionTasks = new MParallelTaskSet(FALSE);
    mParallel.AddTail(mMotionTasks);
    for (int i = 0; i < 3; i++) {
        mMotions[i] = NULL;
    }
    for (int i = 3; i < 5; i++) {
        mMotions[i] = new MMotion(std::string(data_ov003_02160c60[4]), mOam, 2, 1);
        mMotions[i]->SetPosition(data_ov003_02160f88[i]);
        mMotions[i]->SetPriority(3);
        mMotions[i]->Hide();
        mMotionTasks->AddTail(mMotions[i]);
    }
    mParallel.AddTail(mOam);
    mTech = new DinoUiPartsTeamTech(data_ov003_02160f28);
    mParallel.AddTail(mTech);
    mTech->Hide();
    gCurrentDisplay = gLcd->mSub;
}
#pragma opt_common_subs reset

// What the creature stats are worked out from, and the stats (main's func_02030f38).
struct DinoCreatureStatsInput {
    u32 id;         // 0x00
    int rank;       // 0x04: the creature's level
    int bonus[4];   // 0x08: by part
    int levels[5];  // 0x18: the parts', then the creature's own
};
struct DinoCreatureStats {
    int hp;         // 0x00
    int stats[5];   // 0x04
};
extern "C" DinoCreatureStats func_02030f38(const DinoCreatureStatsInput *info, BOOL fixed, MFx32 fraction);

// The creature's level (as GetCreatureLevel, the sum declared before the cap).
static inline u32 DinoTeamCreatureTotal(const DinoCreatureState &creature) {
    if (creature.parts[0].level == 0) {
        return 0;
    }
    u32 sum, max;
    sum = creature.unk0_8 + (creature.parts[0].level + creature.parts[1].level + creature.parts[2].level +
                             creature.parts[3].level);
    max = kCreatureLevelLimits[0] * 11 + 50;
    return std::min(max, sum);
}

static inline u32 DinoTeamCreatureLevel(const DinoCreatureState &creature) {
    if (creature.parts[0].level == 0) {
        return 0;
    }
    return (DinoTeamCreatureTotal(creature) - 50) / kCreatureLevelLimits[0] + 1;
}

void DinoUiPartsTeamInfo::SetTeam(u32 index) {
    const DinoCreatureDef *def;
    DinoCreatureState creature;
    DinoTeam team;
    gDinoBackup->GetTeam(index, &team);
    u32 i;
    u32 count = 0;
    for (i = 0; i < 5; i++) {
        if (team.slots[i] != 0) {
            count++;
        }
    }
    if (count == 0) {
        mName->Hide();
        mOtherName->Hide();
    } else if (index == gDinoBackup->GetUnk34_26()) {
        mOtherName->Hide();
        mName->SetText(std::wstring((const wchar_t *)team.name));
        mName->Show();
    } else {
        mName->Hide();
        mOtherName->SetText(std::wstring((const wchar_t *)team.name));
        mOtherName->Show();
    }
    for (int i = 0; i < 5; i++) {
        mTexts[i]->SetText(std::string(gDinoResource->GetCreatureText08(team.slots[i])));
        if (team.slots[i] != 0) {
            def = gDinoResource->GetCreatureDef(team.slots[i]);
            gDinoBackup->GetCreature(team.slots[i], &creature);
            DinoCreatureStatsInput info;
            info.levels[0] = info.levels[1] = info.levels[2] = info.levels[3] = 0;
            MI_CpuFill8(&info, 0, sizeof(info));
            info.id = team.slots[i];
            info.rank = DinoTeamCreatureLevel(creature);
            info.levels[4] = creature.unk0_8;
            for (int j = 0; j < 4; j++) {
                info.bonus[j] = creature.parts[j].unk12;
                info.levels[j] = creature.parts[j].level;
            }
            DinoCreatureStats stats = func_02030f38(&info, FALSE, MFx32::FromRaw(0));
            mNumbers[i]->SetValue((u16)stats.hp);
            mNumbers[5 + i]->SetValue((u16)DinoTeamCreatureLevel(creature));
        } else {
            mNumbers[i]->SetValue(0xFFFF);
            mNumbers[5 + i]->SetValue(0xFFFE);
        }
        if (i < 3) {
            if (mMotions[i] != NULL) {
                mMotionTasks->Remove(mMotions[i]);
                delete mMotions[i];
                mMotions[i] = NULL;
            }
            if (team.slots[i] != 0) {
                char path[256];
                OS_SPrintf(path, "/motion/ui_reviver_medal/reviver_%03d.bin", team.slots[i]);
                mMotions[i] = new MMotion(std::string(path), mOam, 2, 1);
                if (DinoTeamCreatureLevel(creature) == 12) {
                    mMotions[i]->Play(1, 0);
                } else {
                    mMotions[i]->Play(2, 0);
                }
                mMotions[i]->SetPosition(data_ov003_02160f88[i]);
                mMotions[i]->SetPriority(3);
                mMotions[i]->Show();
                mMotionTasks->AddTail(mMotions[i]);
            }
        } else if (team.slots[i] == 0) {
            mMotions[i]->Hide();
        } else {
            mMotions[i]->Play(GetKindLook(def->unk0D), 0);
            mMotions[i]->Show();
        }
    }
    if (DinoTeamHasTech(&team)) {
        mTech->Show();
    } else {
        mTech->Hide();
    }
}

DinoUiPartsTeamTech::DinoUiPartsTeamTech(const MPoint *positions) : MProcess(NULL) {
    for (int i = 0; i < 3; i++) {
        mPositions[i] = positions[i];
    }
    mOam = new MSmartOam(10, 0, TRUE);
    mIconTasks = new MParallelTaskSet;
    mParallel.AddTail(mIconTasks);
    for (int i = 0; i < 3; i++) {
        mIcons[i] = new MMotion(std::string(data_ov003_02160c60[29]), mOam, 0, 1);
        mIcons[i]->SetPosition(mPositions[i]);
        mIcons[i]->SetPriority(1);
        mIcons[i]->Show();
        mIcons[i]->GetUnit()->SetMode(1);
        mIconTasks->AddTail(mIcons[i]);
        mIcons[i]->Play(1, 0);
    }
    mParallel.AddTail(mOam);
}

void DinoUiPartsTeamTech::Show() {
    for (int i = 0; i < 3; i++) {
        mIcons[i]->Show();
        mIcons[i]->Play(1, 1);
    }
}

void DinoUiPartsTeamTech::Hide() {
    for (int i = 0; i < 3; i++) {
        mIcons[i]->Hide();
    }
}

// Lifts one icon a little.
void DinoUiPartsTeamTech::Raise(u32 index) {
    if (index < 3) {
        mIcons[index]->SetPosition(MPoint(mPositions[index].x, mPositions[index].y - 2));
    }
}

void DinoUiPartsTeamTech::Lower() {
    for (int i = 0; i < 3; i++) {
        mIcons[i]->SetPosition(mPositions[i]);
    }
}

#pragma opt_common_subs off
DinoUiPartsTeamIndicator::DinoUiPartsTeamIndicator(MPoint pos, MFont *font) : MProcess(NULL) {
    mFont = font;
    mCanvas = new MObjCanvas(gCurrentDisplay, gCurrentDisplay->mObj->mOam, MPoint(256, 32), data_020b5870, TRUE);
    mFont->AddRef();
    mCanvas->SetPosition(pos);
    mCanvas->SetPriority(3);
}
#pragma opt_common_subs reset

// Outlined in the font's back color.
void DinoUiPartsTeamIndicator::SetText(const std::wstring &text) {
    int shadow = mFont->GetShadowColor();
    mFont->SetShadowColor(mFont->GetBackColor());
    mFont->SetColor(-1);
    mCanvas->Clear();
    mCanvas->DrawText(mFont, MPoint(1, 0), 0, std::wstring(text));
    mCanvas->DrawText(mFont, MPoint(0, 1), 0, std::wstring(text));
    mFont->SetShadowColor(shadow);
    mCanvas->DrawText(mFont, MPoint(1, 1), 0, std::wstring(text));
}

void DinoUiPartsTeamIndicator::Show() {
    mCanvas->Show();
}

void DinoUiPartsTeamIndicator::Hide() {
    mCanvas->Hide();
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

DinoUiPartsBlackBelt::DinoUiPartsBlackBelt() : MProcess(NULL) {
    MObjCanvas *canvas = new MObjCanvas(gLcd->GetTopDisplay(), MPoint(256, 16), gDinoResource->GetUnkTable(), TRUE);
    mParallel.AddTail(canvas);
    canvas->FillRect(MRect(0, 0, 256, 16), 1);
    canvas->FillRect(MRect(0, 0, 256, 1), 10);
    canvas->SetPosition(data_ov003_02160d3c);
    canvas->SetPriority(1);
    canvas->Show();
}

#pragma opt_common_subs off
DinoUiPartsRockName::DinoUiPartsRockName(MPoint pos) : MProcess(NULL) {
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

// The rock's name, in the selected color or, for a rock not cleaned yet, its own.
void DinoUiPartsRockName::SetEntry(const DinoBackupEntry *entry, BOOL selected) {
    DinoKasekiRockState rock;
    gDinoBackup->GetKasekiRock(entry->id, (u32 *)&rock);
    mText->SetText(std::string(data_ov003_02161cd4));
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

void DinoUiPartsRockName::Clear() {
    mText->SetText(std::string(data_ov003_02161cd4));
}

DinoUiPartsKasekiButton::DinoUiPartsKasekiButton(MSmartOam *oam, MPoint pos, DinoKasekiButtonInfo info, u8 marked) {
    mPos = pos;
    Setup(oam, info);
    SetMarked(marked);
    SetMode(2);
}

void DinoUiPartsKasekiButton::Setup(MSmartOam *oam, DinoKasekiButtonInfo info) {
    mMotion = new MMotion(std::string(data_ov003_02161028[info.icon % 30]), oam, 2, 1);
    mMotion->Play(info.color + 1, 0);
    mMotion->SetPosition(mPos);
    mMotion->SetPriority(2);
    mMotion->GetUnit()->SetMode(1);
    mMotion->Show();
    mParallel.AddTail(mMotion);
    mView = new MMotion(std::string(data_ov003_02160c60[31]), oam, 2, 1);
    mView->Play(1, 0);
    mView->SetPosition(mPos);
    mView->SetPriority(2);
    mView->GetUnit()->SetMode(1);
    mView->Hide();
    mParallel.AddTail(mView);
    mMark = new MMotion(std::string(data_ov003_02160c60[31]), oam, 2, 1);
    mMark->Play(2, 0);
    mMark->SetPosition(mPos);
    mMark->SetPriority(2);
    mMark->GetUnit()->SetMode(1);
    mParallel.AddTail(mMark);
    u8 type = info.type;
    if (type == 3 || ((type == 1 || type == 2) && info.level >= 125)) {
        mBadge = new MMotion(std::string(data_ov003_02160c60[26]), oam, 0, 1);
        if (type == 3) {
            mBadge->Play(3, 0);
        } else {
            mBadge->Play(4, 0);
        }
        mBadge->SetPosition(MPoint(mPos.x - 3, mPos.y + 15));
        mBadge->SetPriority(2);
        mBadge->GetUnit()->SetMode(1);
        mBadge->Show();
        mParallel.AddTail(mBadge);
    } else if (type == 1 || type == 2 || type == 5) {
        for (int i = 0; i < 4; i++) {
            mLevel[i] = new MMotion(std::string(data_ov003_02160c60[6]), oam, 0, 1);
            mLevel[i]->SetPosition(mPos.Offset(i * 8 - 14, 16));
            mLevel[i]->SetPriority(2);
            mLevel[i]->GetUnit()->SetMode(1);
            mLevel[i]->Show();
            mParallel.AddTail(mLevel[i]);
        }
        mLevel[3]->SetPosition(MPoint(mPos.x + 9, mPos.y + 15));
        int level = info.level;
        mLevel[0]->Play(level / 100 % 10 + 3, 0);
        mLevel[1]->Play(level / 10 % 10 + 3, 0);
        mLevel[2]->Play(level % 10 + 3, 0);
        if (type == 5) {
            mLevel[3]->Play(2, 0);
        } else {
            mLevel[3]->Play(1, 0);
        }
    }
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

MTask *DinoUiPartsReviverMedalButton::CreateFlyInTask(MPoint to) {
    MSerialTaskSet *tasks = new MSerialTaskSet(TRUE);
    MTask *front = mMotion->CreatePriorityTransfer(2);
    MTask *wait = mMotion->CreateWaitStopTask();
    MTask *landSe = DinoSoundCreatePlaySeTask(0xA2, 0);
    MTask *land = mMotion->CreatePlayTransfer(mKind + 7, 0);
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

#pragma opt_common_subs off
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
#pragma opt_common_subs reset

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

// The file's static initialization (see dino_static_init.h).
extern "C" u32 data_ov003_02160de0[];
extern "C" u32 data_ov003_02160d60[];
extern "C" u32 data_ov003_02160fe0[];
extern "C" u32 data_ov003_02161060[];
extern "C" u32 data_ov003_02160ee0[];
// NONMATCHING: two stores of reused constants are scheduled one instruction later.
static struct DinoUiFieldMenuFractionStaticInit {
    DinoUiFieldMenuFractionStaticInit() {
        DINO_POOL_WORD(data_ov003_02160de0, 0x14) = 128;
        DINO_POOL_WORD(data_ov003_02160de0, 0x18) = 176;
        DINO_POOL_WORD(data_ov003_02160ce0, 0x3c) = 8;
        DINO_POOL_WORD(data_ov003_02160ce0, 0x40) = 150;
        DINO_POOL_WORD(data_ov003_02160d60, 0x14) = 64;
        DINO_POOL_WORD(data_ov003_02160d60, 0x18) = 112;
        s32 dup = ((s32 *)&data_ov003_02160c60)[0x60 / 4];
        DINO_POOL_WORD(data_ov003_02160fe0, 0x48) = dup;
        DINO_POOL_WORD(data_ov003_02160fe0, 0x4c) = dup;
        DINO_POOL_WORD(data_ov003_02160fe0, 0x50) = DINO_POOL_WORD(data_ov003_02160c60, 0x34);
        DINO_POOL_WORD(data_ov003_02160fe0, 0x54) = DINO_POOL_WORD(data_ov003_02160c60, 0x28);
        DINO_POOL_WORD(data_ov003_02160fe0, 0x58) = DINO_POOL_WORD(data_ov003_02160ce0, 0x18);
        DINO_POOL_WORD(data_ov003_02160fe0, 0x5c) = DINO_POOL_WORD(data_ov003_02160ce0, 0x28);
        DINO_POOL_WORD(data_ov003_02160fe0, 0x60) = DINO_POOL_WORD(data_ov003_02160c60, 0x48);
        DINO_POOL_WORD(data_ov003_02160fe0, 0x64) = DINO_POOL_WORD(data_ov003_02160ce0, 0x20);
        DINO_POOL_WORD(data_ov003_02160fe0, 0x68) = DINO_POOL_WORD(data_ov003_02160c60, 0x20);
        DINO_POOL_WORD(data_ov003_02160fe0, 0x6c) = DINO_POOL_WORD(data_ov003_02160c60, 0x38);
        DINO_POOL_WORD(data_ov003_02160fe0, 0x70) = DINO_POOL_WORD(data_ov003_02160c60, 0x64);
        DINO_POOL_WORD(data_ov003_02160fe0, 0x74) = DINO_POOL_WORD(data_ov003_02160c60, 0x3c);
        DINO_POOL_WORD(data_ov003_02160fe0, 0x78) = DINO_POOL_WORD(data_ov003_02160c60, 0x78);
        DINO_POOL_WORD(data_ov003_02160fe0, 0x7c) = DINO_POOL_WORD(data_ov003_02160ce0, 0x14);
        DINO_POOL_WORD(data_ov003_02161060, 0x0) = DINO_POOL_WORD(data_ov003_02160ce0, 0x1c);
        DINO_POOL_WORD(data_ov003_02161060, 0x4) = DINO_POOL_WORD(data_ov003_02160c60, 0x4c);
        DINO_POOL_WORD(data_ov003_02161060, 0x8) = DINO_POOL_WORD(data_ov003_02160c60, 0x8);
        DINO_POOL_WORD(data_ov003_02161060, 0xc) = DINO_POOL_WORD(data_ov003_02160c60, 0xc);
        DINO_POOL_WORD(data_ov003_02161060, 0x10) = DINO_POOL_WORD(data_ov003_02160ce0, 0x8);
        DINO_POOL_WORD(data_ov003_02161060, 0x14) = DINO_POOL_WORD(data_ov003_02160c60, 0x5c);
        DINO_POOL_WORD(data_ov003_02161060, 0x18) = DINO_POOL_WORD(data_ov003_02160ce0, 0xc);
        DINO_POOL_WORD(data_ov003_02161060, 0x1c) = DINO_POOL_WORD(data_ov003_02160c60, 0x50);
        DINO_POOL_WORD(data_ov003_02161060, 0x20) = DINO_POOL_WORD(data_ov003_02160c60, 0x44);
        DINO_POOL_WORD(data_ov003_02161060, 0x24) = DINO_POOL_WORD(data_ov003_02160c60, 0x2c);
        DINO_POOL_WORD(data_ov003_02161060, 0x28) = DINO_POOL_WORD(data_ov003_02160c60, 0x30);
        DINO_POOL_WORD(data_ov003_02161060, 0x2c) = DINO_POOL_WORD(data_ov003_02160ce0, 0x4);
        DINO_POOL_WORD(data_ov003_02161060, 0x30) = DINO_POOL_WORD(data_ov003_02160c60, 0x54);
        DINO_POOL_WORD(data_ov003_02161060, 0x34) = DINO_POOL_WORD(data_ov003_02160c60, 0x6c);
        DINO_POOL_WORD(data_ov003_02161060, 0x38) = DINO_POOL_WORD(data_ov003_02160ce0, 0x34);
        DINO_POOL_WORD(data_ov003_02161060, 0x3c) = DINO_POOL_WORD(data_ov003_02160c60, 0x1c);
        DINO_POOL_WORD(data_ov003_02160ce0, 0x5c) = 0;
        DINO_POOL_WORD(data_ov003_02160ce0, 0x60) = 176;
        DINO_POOL_WORD(data_ov003_02160ce0, 0x54) = 0;
        DINO_POOL_WORD(data_ov003_02160ce0, 0x58) = 178;
        DINO_POOL_WORD(data_ov003_02160d60, 0x1c) = 35;
        DINO_POOL_WORD(data_ov003_02160d60, 0x20) = 3;
        DINO_POOL_WORD(data_ov003_02160ce0, 0x64) = 178;
        DINO_POOL_WORD(data_ov003_02160ce0, 0x68) = 33;
        DINO_POOL_WORD(data_ov003_02160de0, 0xc) = 178;
        DINO_POOL_WORD(data_ov003_02160de0, 0x10) = 70;
        DINO_POOL_WORD(data_ov003_02160ce0, 0x6c) = 178;
        DINO_POOL_WORD(data_ov003_02160ce0, 0x70) = 102;
        DINO_POOL_WORD(data_ov003_02160d60, 0x74) = 178;
        DINO_POOL_WORD(data_ov003_02160d60, 0x78) = 136;
        DINO_POOL_WORD(data_ov003_02160ce0, 0x74) = 178;
        DINO_POOL_WORD(data_ov003_02160ce0, 0x78) = 153;
        DINO_POOL_WORD(data_ov003_02160ce0, 0x7c) = 213;
        DINO_POOL_WORD(data_ov003_02160d60, 0x0) = 33;
        DINO_POOL_WORD(data_ov003_02160d60, 0x4) = 213;
        DINO_POOL_WORD(data_ov003_02160d60, 0x8) = 70;
        DINO_POOL_WORD(data_ov003_02160d60, 0xc) = 213;
        DINO_POOL_WORD(data_ov003_02160d60, 0x10) = 102;
        DINO_POOL_WORD(data_ov003_02160ce0, 0x44) = 213;
        DINO_POOL_WORD(data_ov003_02160ce0, 0x48) = 136;
        DINO_POOL_WORD(data_ov003_02160d60, 0x64) = 213;
        DINO_POOL_WORD(data_ov003_02160d60, 0x68) = 153;
        DINO_POOL_WORD(data_ov003_02160f60, 0x78) = 178;
        DINO_POOL_WORD(data_ov003_02160f60, 0x7c) = 33;
        DINO_POOL_WORD(data_ov003_02160fe0, 0x0) = 178;
        DINO_POOL_WORD(data_ov003_02160fe0, 0x4) = 70;
        DINO_POOL_WORD(data_ov003_02160fe0, 0x8) = 178;
        DINO_POOL_WORD(data_ov003_02160fe0, 0xc) = 102;
        DINO_POOL_WORD(data_ov003_02160fe0, 0x10) = 178;
        DINO_POOL_WORD(data_ov003_02160fe0, 0x14) = 136;
        DINO_POOL_WORD(data_ov003_02160fe0, 0x18) = 178;
        DINO_POOL_WORD(data_ov003_02160fe0, 0x1c) = 153;
        DINO_POOL_WORD(data_ov003_02160fe0, 0x20) = 213;
        DINO_POOL_WORD(data_ov003_02160fe0, 0x24) = 33;
        DINO_POOL_WORD(data_ov003_02160fe0, 0x28) = 213;
        DINO_POOL_WORD(data_ov003_02160fe0, 0x2c) = 70;
        DINO_POOL_WORD(data_ov003_02160fe0, 0x30) = 213;
        DINO_POOL_WORD(data_ov003_02160fe0, 0x34) = 102;
        DINO_POOL_WORD(data_ov003_02160fe0, 0x38) = 213;
        DINO_POOL_WORD(data_ov003_02160fe0, 0x3c) = 136;
        DINO_POOL_WORD(data_ov003_02160fe0, 0x40) = 213;
        DINO_POOL_WORD(data_ov003_02160fe0, 0x44) = 153;
        DINO_POOL_WORD(data_ov003_02160de0, 0x4) = 53;
        DINO_POOL_WORD(data_ov003_02160de0, 0x8) = 34;
        DINO_POOL_WORD(data_ov003_02160d60, 0x6c) = 53;
        DINO_POOL_WORD(data_ov003_02160d60, 0x70) = 71;
        DINO_POOL_WORD(data_ov003_02160d60, 0x24) = 53;
        DINO_POOL_WORD(data_ov003_02160d60, 0x28) = 103;
        DINO_POOL_WORD(data_ov003_02160d60, 0x54) = 54;
        DINO_POOL_WORD(data_ov003_02160d60, 0x58) = 138;
        DINO_POOL_WORD(data_ov003_02160d60, 0x5c) = 54;
        DINO_POOL_WORD(data_ov003_02160d60, 0x60) = 155;
        DINO_POOL_WORD(data_ov003_02160f60, 0x0) = 53;
        DINO_POOL_WORD(data_ov003_02160f60, 0x4) = 34;
        DINO_POOL_WORD(data_ov003_02160f60, 0x8) = 53;
        DINO_POOL_WORD(data_ov003_02160f60, 0xc) = 71;
        DINO_POOL_WORD(data_ov003_02160f60, 0x10) = 53;
        DINO_POOL_WORD(data_ov003_02160f60, 0x14) = 103;
        DINO_POOL_WORD(data_ov003_02160f60, 0x18) = 54;
        DINO_POOL_WORD(data_ov003_02160f60, 0x1c) = 138;
        DINO_POOL_WORD(data_ov003_02160f60, 0x20) = 54;
        DINO_POOL_WORD(data_ov003_02160f60, 0x24) = 155;
        DINO_POOL_WORD(data_ov003_02160d60, 0x4c) = 32;
        DINO_POOL_WORD(data_ov003_02160d60, 0x50) = 40;
        DINO_POOL_WORD(data_ov003_02160d60, 0x3c) = 32;
        DINO_POOL_WORD(data_ov003_02160d60, 0x40) = 77;
        DINO_POOL_WORD(data_ov003_02160d60, 0x44) = 32;
        DINO_POOL_WORD(data_ov003_02160d60, 0x48) = 109;
        DINO_POOL_WORD(data_ov003_02160d60, 0x2c) = 92;
        DINO_POOL_WORD(data_ov003_02160d60, 0x30) = 229;
        DINO_POOL_WORD(data_ov003_02160d60, 0x34) = 92;
        DINO_POOL_WORD(data_ov003_02160d60, 0x38) = 246;
        DINO_POOL_WORD(data_ov003_02160f60, 0x28) = 32;
        DINO_POOL_WORD(data_ov003_02160f60, 0x2c) = 40;
        DINO_POOL_WORD(data_ov003_02160f60, 0x30) = 32;
        DINO_POOL_WORD(data_ov003_02160f60, 0x34) = 77;
        DINO_POOL_WORD(data_ov003_02160f60, 0x38) = 32;
        DINO_POOL_WORD(data_ov003_02160f60, 0x3c) = 109;
        DINO_POOL_WORD(data_ov003_02160f60, 0x40) = 92;
        DINO_POOL_WORD(data_ov003_02160f60, 0x44) = 229;
        DINO_POOL_WORD(data_ov003_02160f60, 0x48) = 92;
        DINO_POOL_WORD(data_ov003_02160f60, 0x4c) = 246;
        DINO_POOL_WORD(data_ov003_02160ee0, 0x48) = 32;
        DINO_POOL_WORD(data_ov003_02160ee0, 0x4c) = 40;
        DINO_POOL_WORD(data_ov003_02160ee0, 0x50) = 32;
        DINO_POOL_WORD(data_ov003_02160ee0, 0x54) = 77;
        DINO_POOL_WORD(data_ov003_02160ee0, 0x58) = 32;
        DINO_POOL_WORD(data_ov003_02160ee0, 0x5c) = 109;
        DINO_POOL_WORD(data_ov003_02160f60, 0x54) = DINO_POOL_WORD(data_ov003_02160ce0, 0x30);
        DINO_POOL_WORD(data_ov003_02160f60, 0x5c) = DINO_POOL_WORD(data_ov003_02160c60, 0x14);
        DINO_POOL_WORD(data_ov003_02160f60, 0x64) = DINO_POOL_WORD(data_ov003_02160ce0, 0x24);
        DINO_POOL_WORD(data_ov003_02160f60, 0x6c) = DINO_POOL_WORD(data_ov003_02160ce0, 0x10);
        DINO_POOL_WORD(data_ov003_02160f60, 0x74) = DINO_POOL_WORD(data_ov003_02160c60, 0x58);
    }
} sDinoUiFieldMenuFractionStaticInit;
