// DinoEventKasekiRockFound.

#include "game/dino_event_kaseki_rock_found.h"
#include "game/dino_field.h"
#include "m/lcd.h"
#include "m/text_codec.h"
#include "game/dino_common_window_yes_no_dialog.h"
#include "game/dino_resource.h"
#include "game/dino_common_window_message_box.h"
#include "nitro/os.h"
#include <string.h>
#include "game/dino_field_hole.h"
#include "game/dino_event_exec.h"
#include "game/dino_sound.h"
#include "m/polygon_char.h"
#include "m/anm_model.h"
#include "m/call_task.h"
#include <algorithm>
#include "nitro/mi.h"
#include "m/seed_generator.h"
#include "game/dino_weighted_random.h"

DinoEventKasekiRockFound::~DinoEventKasekiRockFound() {
    mEvent->GetField()->SetUnk258(FALSE);
}

void DinoEventKasekiRockFound::Idle() {
    (this->*mState)();
}

// The TU's pooled pointer-to-member constants.
extern "C" struct {
    u8 unk00[0x4];
    s32 mZero_04;   // 0x04: (std::max's 0)
    u8 unk08[0x10 - 0x8];
    void (DinoEventKasekiRockFound::*StateFound_10)();
    u8 unk18[0x38 - 0x18];
    void (DinoEventKasekiRockFound::*State020374d4_38)();
    u8 unk40[0x48 - 0x40];
    void (MPolygonChar::*Load_48)(const char *, const char *);
    u8 unk50[0x68 - 0x50];
    void (DinoEventKasekiRockFound::*Start_68)();
} data_020bd380;

DinoEventKasekiRockFound::DinoEventKasekiRockFound(DinoEventLoop *event, u32 arg, MPoint pos)
    : MProcess(gLcd->GetBottomDisplay()), mEvent(event), mPos(*(MFx32Point *)&pos), mArg(arg) {
    mMessages = MResource<DinoMessageData>("/msg/msg_dig");
    mState = data_020bd380.Start_68;
}

MTask *DinoEventKasekiRockFound::CreateMessageBox(u32 index, const std::string &arg) {
    const u8 *data = (const u8 *)mMessages.Get();
    const u8 *entry = data + ((const u32 *)(data + ((const u32 *)data)[2]))[index];
    const char *format = (const char *)entry + ((const u32 *)entry)[1];
    char *text = new char[strlen(format) + arg.size() + 1];
    OS_SPrintf(text, format, arg.c_str());
    MTask *box = new DinoCommonWindowMessageBox(MPoint(128, 96), std::string(text), 4);
    delete[] text;
    return box;
}

extern const u32 kBackupEntryUnk2Text;
// What the found message calls the rock, by its kind (from 2).
extern "C" const u32 data_0208a348, data_0208a33c, data_0208a344, data_0208a340, data_0208a338;

// Says what was dug up.
MTask *DinoEventKasekiRockFound::CreateFoundMessage() {
    if (mEntry.unk10_0 == 1) {
        return CreateMessageBox(3);
    }
    DinoKasekiRockState rock;
    gDinoBackup->GetKasekiRock(mEntry.id, (u32 *)&rock);
    if (mEntry.type == 1) {
        return CreateMessageBox(0, std::string(gDinoResource->GetText(kBackupEntryUnk2Text)));
    }
    if (rock.unk10) {
        return CreateMessageBox(0, gDinoResource->GetKasekiRockName(mEntry.id));
    }
    if (mEntry.unk10_0 == 0) {
        return CreateMessageBox(2);
    }
    u32 texts[5] = {data_0208a348, data_0208a33c, data_0208a344, data_0208a340, data_0208a338};
    return CreateMessageBox(1, std::string(gDinoResource->GetText(texts[mEntry.unk10_0 - 2])));
}

// The message as a yes/no question: its last two lines are the answers.
MTask *DinoEventKasekiRockFound::CreateYesNo(u32 message) {
    const u8 *data = (const u8 *)mMessages.Get();
    const u8 *entry = data + ((const u32 *)(data + ((const u32 *)data)[2]))[message];
    std::string text((const char *)entry + ((const u32 *)entry)[1]);
    std::vector<std::string> lines = MSplitString(text, '\n');
    std::string yes(*(lines.begin() + (u32)lines.size() - 2));
    std::string no(*(lines.begin() + (u32)lines.size() - 1));
    lines.erase(lines.end() - 2, lines.end());
    text = MJoinStrings(lines, '\n');
    DinoCommonWindowYesNoDialog *dialog = new DinoCommonWindowYesNoDialog(text, &mAnswer);
    dialog->SetLabels(yes, no);
    return dialog;
}

#pragma auto_inline off
// Vector instances the game emits in this file (made explicitly until the code that uses them is decompiled).
template class std::__vec_deleter<std::string, std::allocator<std::string> >;
template std::__vec_deleter<std::string, std::allocator<std::string> &>::~__vec_deleter();

// The finder's digging motion for each kind of rock.
// A dig spot's settings (in the map's M data).
struct DinoDigSpot {
    const u8 *GetRock(u32 id) const { return (const u8 *)this + ((const u32 *)((const u8 *)this + rockOffset))[id]; }
    const u32 *GetGroup(u32 i) const {
        return (const u32 *)((const u8 *)this + ((const u32 *)((const u8 *)this + groupOffset))[i]);
    }
    const u32 *GetLevels() const { return (const u32 *)((const u8 *)this + levelOffset); }

    u8 unk00[0x10];
    s32 battleRate;     // 0x10: the chance of a battle (-1: the default)
    s32 unk14;          // 0x14: (-1: the default)
    u32 levelCount;     // 0x18
    u32 levelOffset;    // 0x1C: by the system var 0x20
    u32 groupCount;     // 0x20
    u32 groupOffset;    // 0x24: each a flag, a count and the offset of that many battle ids
    u32 unk28;
    u32 rockOffset;     // 0x2C
};

// One of the excavation settings' tables of weights (by index).
static inline const s32 *DinoExcavateWeights(MResource<DinoExcavateDefs> &defs, u32 offset, u32 i) {
    const u8 *data = (const u8 *)defs.Get();
    const u8 *entry = data + ((const u32 *)(data + *(const u32 *)(data + offset)))[i];
    return (const s32 *)(entry + ((const u32 *)entry)[1]);
}

static inline u32 DinoExcavateWord(MResource<DinoExcavateDefs> &defs, u32 offset) {
    return *(const u32 *)((const u8 *)defs.Get() + offset);
}

// A table of words at the offset in the word at offset.
static inline u32 DinoTableEntry(const void *data, u32 offset, u32 i) {
    return ((const u32 *)((const u8 *)data + *(const u32 *)((const u8 *)data + offset)))[i];
}

// The map's dig spot.
static inline const DinoDigSpot *DinoGetDigSpot(const DinoMapMData *map, u32 i) {
    const u8 *data = (const u8 *)map;
    return (const DinoDigSpot *)(data + ((const u32 *)(data + *(const u32 *)(data + 0x54)))[i]);
}

// Lists a value in a vector of void *s.
static inline void DinoPushValue(std::vector<void *> &v, const u32 &value) {
    v.push_back((void *const &)value);
}

extern "C" BOOL func_0201b19c(DinoBackup *backup, u32 index, s32 rate);
extern "C" struct {
    u8 unk00[0x20];
    void (DinoEventKasekiRockFound::*StateDig_20)();
} data_020bd400;

// Works out what was found: the saved rock (or, for an unknown one, a rock of the spot's kind at a level weighted by
// the player's settings and team), whether a battle comes first (and whether a battle gives another rock), and
// whether something else happens.
void DinoEventKasekiRockFound::Start() {
    DinoPlayerData player;
    DinoBackupEntry found;
    DinoBackupEntry entry;
    mEvent->GetField()->SetUnk258(TRUE);
    gDinoBackup->GetPlayer(&player, 0);
    gDinoBackup->GetUnk134(mArg, &entry);
    MI_CpuFill8(&found, 0, sizeof(found));
    MResource<DinoExcavateDefs> excavate = gDinoResource->GetExcavateDefs();
    MResource<DinoMapMData> map = gDinoResource->LoadMapM(entry.map);
    const DinoDigSpot *spot = DinoGetDigSpot(map.Get(), entry.spot);
    MRandom *random = gDinoResource->GetRandom();
    if (entry.type != 0) {
        found = entry;
    } else {
        s32 weights[4];
        const u8 *rock = spot->GetRock(entry.id);
        for (u32 i = 0; i < 4; i++) {
            const s32 *rates = (const s32 *)(rock + ((const u32 *)rock)[3]);
            if (rates[i] >= 0) {
                u32 j = i;
                if (j != 0) {
                    j = (i + 2 + player.unk14_14) % 3 + 1;
                }
                weights[i] = rates[j];
            } else {
                weights[i] = DinoExcavateWeights(excavate, 8, player.unk14_14)[i];
            }
        }
        DinoTeam team;
        gDinoBackup->GetTeam(gDinoBackup->GetUnk34_26(), &team);
        for (u32 i = 0; i < 5; i++) {
            u8 slot = team.slots[i];
            if (slot != 0) {
                for (u32 j = 0; j < 4; j++) {
                    weights[j] += DinoExcavateWeights(excavate, 0x3c, slot)[j];
                }
            }
        }
        for (u32 j = 0; j < 4; j++) {
            weights[j] = std::max(weights[j], data_020bd380.mZero_04);
        }
        u32 level;
        if ((int)random->Next(100) < weights[0]) {
            level = 0;
        } else {
            DinoWeightedRandom pick(random);
            pick.Add(weights[1]);
            pick.Add(weights[2]);
            pick.Add(weights[3]);
            level = pick.Pick() + 1;
        }
        found = entry;
        found.id = (*(const u32 *)rock - 1) * 4 + level + 1;
    }
    MI_CpuFill8(&entry, 0, sizeof(entry));
    gDinoBackup->SetUnk134(mArg, &entry);
    s32 rate;
    if (!gDinoBackup->GetSystemFlag(1)) {
        rate = 0;
    } else if (found.unk10_0 == 1) {
        rate = 0;
    } else {
        rate = spot->battleRate;
        if (rate < 0) {
            rate = DinoExcavateWord(excavate, 0x34);
        }
    }
    s32 fightRate = spot->unk14;
    if (fightRate < 0) {
        fightRate = DinoExcavateWord(excavate, 0x28);
    }
    u32 battle = 0;
    if (func_0201b19c(gDinoBackup, 1, rate)) {
        std::vector<void *> battles;
        for (u32 i = 0; i < spot->groupCount; i++) {
            const u32 *group = spot->GetGroup(i);
            if (gDinoBackup->GetFlag(group[0])) {
                for (u32 j = 0; j < group[1]; j++) {
                    DinoPushValue(battles, (u32)((const u32 *)((const u8 *)group + group[2]))[j]);
                }
            }
        }
        if (battles.size() != 0) {
            battle = (u32)battles[random->Next(battles.size())];
            found.unkC_12 = (int)random->Next(100) < fightRate ? 1 : 0;
        }
    }
    mBattle = battle;
    mEntry = found;
    if (mBattle != 0) {
        mBattleData = gDinoResource->LoadBattle(mBattle);
        if (mBattleData.Get()->rockCount != 0) {
            std::vector<void *> bounds;
            u32 total = 0;
            for (u32 i = 0; i < mBattleData.Get()->rockCount; i++) {
                total += mBattleData.Get()->GetRocks()[i].weight;
                DinoPushValue(bounds, total);
            }
            u32 r = gDinoResource->GetRandom()->Next(100);
            for (u32 i = 0; i < mBattleData.Get()->rockCount; i++) {
                if (r < (u32)bounds[i]) {
                    MI_CpuFill8(&mEntry, 0, sizeof(mEntry));
                    mEntry.id = mBattleData.Get()->GetRock(i).id;
                    mEntry.type = 1;
                    break;
                }
            }
        }
    }
    mUnkDC = 0;
    if (mBattle == 0) {
        u32 chance;
        if (spot->levelCount != 0) {
            chance = DinoTableEntry(spot, 0x1c, gDinoBackup->GetSystemVar(0x20) - 1);
        } else {
            chance = DinoTableEntry(excavate.Get(), 0xb4, gDinoBackup->GetSystemVar(0x20) - 1);
        }
        if (func_0201b19c(gDinoBackup, 0, chance)) {
            mUnkDC = 1;
        }
    }
    mState = data_020bd400.StateDig_20;
}

extern const u32 data_0208b2f4[8];
extern const u32 data_0208a1fc;
extern const u32 data_0208a200;

// NONMATCHING: the game keeps the effect's z and height in r6 and r4 the other way round.
void DinoEventKasekiRockFound::StateDig() {
    u32 focus = mEvent->GetField()->GetCamera()->GetFocus();
    mChara = mEvent->GetField()->GetChara(focus);
    u32 motion = data_0208b2f4[mEntry.unk10_0];
    if (mUnkDC) {
        motion = 0x1C;
    }
    mHole = mEvent->GetField()->CreateHole(mPos);
    mAngle = mChara->GetModel()->angle;
    mEffect = new MPolygonChar(mEvent->GetField()->mScene, 0x7FFF, NULL, NULL);
    MPolygonChar *e;
    // (The height as an MFx32, as the game passes it.)
    (e = mEffect)->SetPosition(mHole->GetPos().x.v, MFx32::FromRaw(mHole->mUnk90).v, mHole->GetPos().y.v);
    for (MAnmModelBase *anm = (MAnmModelBase *)e->mParallel.mHead; anm != NULL; anm = (MAnmModelBase *)anm->mNext) {
        anm->mPosRef = &e->mPos;
        anm->mUnk11B = TRUE;
    }
    int angle = (u16)(0x4000 - mChara->GetModel()->angle) * 360 / 16;
    e = mEffect;
    e->mRot.x = 0;
    e->mRot.y = angle;
    e->mRot.z = 0;
    for (MAnmModelBase *anm = (MAnmModelBase *)e->mParallel.mHead; anm != NULL; anm = (MAnmModelBase *)anm->mNext) {
        anm->mRotRef = &e->mRot;
        anm->mUnk11B = TRUE;
    }
    for (MAnmModelBase *anm = (MAnmModelBase *)mEffect->mParallel.mHead; anm != NULL;
         anm = (MAnmModelBase *)anm->mNext) {
        anm->mActive = TRUE;
    }
    e = mEffect;
    e->mUnkAC = 1;
    for (MAnmModelBase *anm = (MAnmModelBase *)e->mParallel.mHead; anm != NULL; anm = (MAnmModelBase *)anm->mNext) {
        anm->mFollowParent = TRUE;
    }
    mParallel.AddTail(mEffect);
    mChara->ClearUnk2E0();
    mChara->SetMotion(4, 0, MFx32(1));
    GetSerial()->AddTail(new MTimer(0x1E),
                    MCreateCallTask(mEffect, data_020bd380.Load_48, "/model/fieldchar/effect03.csv", (const char *)NULL),
                    DinoSoundCreatePlaySeTask(0xCA, 0),
                    new DinoEventExecCamera(mEvent, 0x400, 0x1B53, 0, 0x2B000, 0x3C, 0xC),
                    DinoSoundCreatePlaySeTask(!mUnkDC ? data_0208a200 : data_0208a1fc, 0),
                    mChara->CreateSetMotionTask(motion, 0, MFx32(1)), new MTimer(10),
                    mChara->CreateShowManpuTask(2, MPoint(0, 0)), new MTimer(0x1E),
                    mChara->CreateAngleToTask(0x4000, 0x14, 0xC), mChara->CreateHideManpuTask(), new MTimer(0x14),
                    MTaskExitTransfer::Create(mEffect));
    if (!mUnkDC) {
        mState = data_020bd380.StateFound_10;
    } else {
        mState = data_020bd380.State020374d4_38;
    }
}
