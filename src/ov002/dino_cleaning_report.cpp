// DinoCleaningReport.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_cleaning_report.h"
#include "game/dino_cleaning.h"
#include "game/dino_backup.h"
#include "types.h"
#include "game/dino_event_talk_window.h"
#include "game/dino_common_window_message_box.h"
#include "m/lcd.h"
#include <string.h>
#include "game/dino_static_init.h"
#include "game/dino_sound.h"
#include "m/bg.h"
#include "m/window.h"
#include "m/blend.h"
#include "nitro/mi.h"

DinoCleaningReport::~DinoCleaningReport() {
}

void DinoCleaningReport::Idle() {
    (this->*mState)();
}

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x4];
    void (DinoCleaningReport::*State0215670c_04)(); // 0x04
    u8 unk0C[0x14 - 0xC];
    void (DinoCleaningReport::*State02155b28_14)(); // 0x14
    u8 unk1C[0x34 - 0x1C];
    void (DinoCleaningReport::*State02156808_34)(); // 0x34
} data_ov002_02163e60;

DinoCleaningReport::DinoCleaningReport(u32 arg) : MProcess(NULL), mUnk7C(arg) {
    mMessages = MResource<DinoMessageData>("/msg/msg_cleaning");
    mState = data_ov002_02163e60.State02155b28_14;
}

// A message box when the text starts a new line; otherwise a talk window with the choices.
void DinoCleaningReport::ShowMessage(u32 id, const std::string &arg, u32 *result, const std::string &a,
                                     const std::string &b, const std::string &c, const std::string &d) {
    const u8 *data = (const u8 *)mMessages.Get();
    u32 format;
    const u8 *entry = data + ((const u32 *)(data + ((const u32 *)data)[2]))[id];
    format = ((const u32 *)entry)[1];
    char *text = new char[strlen((const char *)entry + format) + arg.size() + 1];
    OS_SPrintf(text, (const char *)entry + format, arg.c_str());
    if (text[0] == '\n') {
        MTask *box = new DinoCommonWindowMessageBox(gLcd->GetBottomDisplay(), MPoint(128, 96), std::string(text), 4);
        mSerial.AddTail(box);
    } else {
        DinoEventTalkWindow *window =
            new DinoEventTalkWindow(gLcd->GetBottomDisplay(), std::string(text), ((DinoCleaning *)mUnk7C)->mUnkD4, 0);
        window->SetYesNo((u32)result, MPoint(254, 142), 3, std::string(a), std::string(b), std::string(c),
                         std::string(d));
        mSerial.AddTail(window);
    }
    delete[] text;
}

// What the cleaning hands the report (DinoCleaning::mUnkA0).
struct DinoCleaningResult {
    u32 score;                  // 0x00
    u32 unk04;                  // 0x04
    u32 unk08;                  // 0x08
    u32 cleaned;                // 0x0C: the fossil came out
    u32 next;                   // 0x10: what follows the report
    DinoCreatureState before;   // 0x14: the creature before the new part
    DinoCreatureState after;    // 0x2C
};

extern "C" u8 data_0208a234;

// Says how the cleaning went and keeps the result: a part better than the one the creature had replaces it (asking
// about reviving for a new head), otherwise the score counts towards the excavation level; then the dig level, and
// the rock's entry is updated (or cleared).
void DinoCleaningReport::State02155b28() {
    DinoSoundPlayBgm(0x3B3, 0);
    gLcd->GetBottomDisplay()->mBg->SetMode(3);
    gLcd->GetBottomDisplay()->mWin->Reset();
    gLcd->GetBottomDisplay()->mBlend->Reset();
    u32 index;
    u32 range;
    u32 next;
    u32 part;
    BOOL addRange;
    BOOL update;
    BOOL revived;
    u32 score = ((DinoCleaningResult *)GetCleaning()->mUnkA0)->score;
    range = 0;
    gDinoBackup->SetSystemVar(0x11, score);
    gDinoBackup->SetSystemVar(0x27, ((DinoCleaningResult *)GetCleaning()->mUnkA0)->unk04);
    next = 0;
    if (((DinoCleaningResult *)GetCleaning()->mUnkA0)->cleaned == 0) {
        ShowMessage(6);
    } else {
        if (GetCleaning()->mUnkA8 == 1) {
            ShowMessage(4);
        }
        if (data_0208a234 && ((DinoCleaningResult *)GetCleaning()->mUnkA0)->unk04) {
            ShowMessage(5);
        }
        if (GetCleaning()->mUnkA8 == 1) {
            ShowMessage(8, gDinoResource->GetKasekiRockName(GetCleaning()->mUnk9C));
        } else {
            switch (GetCleaning()->mUnkA4) {
            case 0:
                ShowMessage(7, gDinoResource->GetKasekiRockName(GetCleaning()->mUnk9C));
                break;
            case 1:
                ShowMessage(0x18, gDinoResource->GetKasekiName(GetCleaning()->mUnk9C));
                break;
            case 2:
                ShowMessage(0x19);
                break;
            }
        }
        if (GetCleaning()->mUnkA4 == 0) {
            index = (GetCleaning()->mUnk9C - 1) / 4;
            part = (GetCleaning()->mUnk9C - 1) & 3;
            DinoCreatureState creature;
            gDinoBackup->GetCreature(index + 1, &creature);
            revived = creature.parts[0].level != 0;
            u32 level = creature.parts[part].level;
            gDinoBackup->SetSystemVar(0x12, level);
            BOOL better = score > level;
            addRange = FALSE;
            update = FALSE;
            if (part == 0) {
                if (!revived) {
                    ShowMessage(0xE);
                    update = TRUE;
                    next = 2;
                } else if (better) {
                    ShowMessage(0xD);
                    update = TRUE;
                    next = 1;
                } else {
                    ShowMessage(9);
                    addRange = TRUE;
                    range = gDinoResource->FindExcavateRange(score);
                }
            } else if (!revived) {
                if (level == 0) {
                    ShowMessage(0xB);
                    update = TRUE;
                } else if (better) {
                    ShowMessage(0xA);
                    update = TRUE;
                    addRange = TRUE;
                    range = gDinoResource->FindExcavateRange(level);
                } else {
                    ShowMessage(9);
                    addRange = TRUE;
                    range = gDinoResource->FindExcavateRange(score);
                }
            } else if (level == 0) {
                if (gDinoBackup->GetSystemFlag(0x2D)) {
                    ShowMessage(0xC);
                } else {
                    gDinoBackup->SetSystemFlag(0x2D, TRUE);
                    ShowMessage(0x24, gDinoResource->GetKasekiRockName(GetCleaning()->mUnk9C));
                }
                update = TRUE;
                next = 1;
            } else if (better) {
                if (gDinoBackup->GetSystemFlag(0x2E)) {
                    ShowMessage(0xD);
                } else {
                    gDinoBackup->SetSystemFlag(0x2E, TRUE);
                    ShowMessage(0x25);
                }
                update = TRUE;
                next = 1;
            } else {
                ShowMessage(9);
                range = gDinoResource->FindExcavateRange(score);
                addRange = TRUE;
            }
            if (update) {
                ((DinoCleaningResult *)GetCleaning()->mUnkA0)->before = creature;
                creature.parts[part].level = score;
                creature.parts[part].unk19 = GetCleaning()->mUnkA8 == 1;
                gDinoBackup->SetCreature(index + 1, &creature);
                ((DinoCleaningResult *)GetCleaning()->mUnkA0)->after = creature;
                DinoKasekiRockState rock;
                gDinoBackup->GetKasekiRock(GetCleaning()->mUnk9C, (u32 *)&rock);
                rock.unk10 = 1;
                rock.score = score;
                gDinoBackup->SetKasekiRock(GetCleaning()->mUnk9C, (u32 *)&rock);
            }
            if (addRange) {
                gDinoBackup->SetSystemVar(0x2E, range);
                gDinoBackup->SetSystemVar(0x13, gDinoBackup->GetSystemVar(0x13) + range);
            }
        }
    }
    ((DinoCleaningResult *)GetCleaning()->mUnkA0)->next = next;
    u32 level = gDinoBackup->GetExcavateLevelUnk0();
    if (((DinoCleaningResult *)GetCleaning()->mUnkA0)->cleaned) {
        gDinoBackup->SetSystemVar(0x19, gDinoBackup->GetSystemVar(0x19) + 1);
    }
    u32 newLevel = gDinoBackup->GetExcavateLevelUnk0();
    if (newLevel != level) {
        if (newLevel == gDinoBackup->GetMaxExcavateLevelUnk0()) {
            ShowMessage(0x1C);
        } else {
            ShowMessage(0x1B);
        }
    }
    if (GetCleaning()->mUnkA4 != 0 && ((DinoCleaningResult *)GetCleaning()->mUnkA0)->cleaned) {
        DinoBackupEntry entry;
        gDinoBackup->GetUnk98(GetCleaning()->mUnk140, &entry);
        gDinoBackup->CompactUnk98();
        entry.active = 1;
        entry.score = score;
        gDinoBackup->SetUnk98(GetCleaning()->mUnk140, &entry);
    } else {
        DinoBackupEntry entry;
        MI_CpuFill8(&entry, 0, sizeof(entry));
        gDinoBackup->SetUnk98(GetCleaning()->mUnk140, &entry);
        gDinoBackup->CompactUnk98();
    }
    if (next != 0) {
        mSerial.AddTail(MTaskExitTransfer::Create(this));
        return;
    }
    mState = data_ov002_02163e60.State0215670c_04;
}

extern const u32 data_0208acc8, data_0208ad24, data_0208ad28;

// With rocks waiting, asks about them; otherwise ends the report.
void DinoCleaningReport::State0215670c() {
    if (!gDinoBackup->HasInactiveUnk98()) {
        ((u32 *)((DinoCleaning *)mUnk7C)->mUnkA0)[4] = 3;
        Exit();
        return;
    }
    mAnswer = 0;
    ShowMessage(0x11, "", &mAnswer, "", std::string("*") + gDinoResource->GetText(data_0208acc8),
                gDinoResource->GetText(data_0208ad28), gDinoResource->GetText(data_0208ad24));
    mState = data_ov002_02163e60.State02156808_34;
}

// Ends the report with the answer.
void DinoCleaningReport::State02156808() {
    ((u32 *)((DinoCleaning *)mUnk7C)->mUnkA0)[4] = mAnswer == 1 ? 3 : 4;
    Exit();
}

// The file's static initialization (see dino_static_init.h).
#pragma always_inline on
static struct DinoCleaningReportStaticInit {
    DinoCleaningReportStaticInit() {
        DINO_COMMON_LAYOUT_INIT(data_ov002_02163e60, 0xc, 0x74, 0x24, 0x2c, 0x84, 0x54, 0x5c, 0x64);
    }
} sDinoCleaningReportStaticInit;
