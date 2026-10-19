// Helpers between the field menu fraction and the revive status: team checks and text tags.
// Only some functions are decompiled so far.

#include <string>
#include "game/dino_team.h"
#include "game/dino_resource.h"
#include "m/resource.h"
#include "game/dino_ui_parts_kaseki_button.h"
#include "game/dino_ui_common.h"
#include "m/blend.h"
#include "m/display.h"
#include "m/task.h"
#include "m/fx32.h"

// TRUE if the team's first three members are all set and share a type, which gives the team a technique.
BOOL DinoTeamHasTech(const DinoTeam *team) {
    for (int i = 0; i < 3; i++) {
        if (team->slots[i] == 0) {
            return FALSE;
        }
    }
    u32 types = gDinoResource->GetCreatureDef(team->slots[0])->types;
    for (int i = 1; i < 3; i++) {
        types &= gDinoResource->GetCreatureDef(team->slots[i])->types;
    }
    return types != 0 ? TRUE : FALSE;
}

// TRUE if the two teams' members differ.
BOOL DinoTeamDiffers(const DinoTeam *a, const DinoTeam *b) {
    for (int i = 0; i < 5; i++) {
        if (a->slots[i] != b->slots[i]) {
            return TRUE;
        }
    }
    return FALSE;
}

// What the container entry's button shows: a kaseki rock, a fossil (cleaned or not, and whether the creature is
// revived), or one of the two special kinds of rock.
DinoKasekiButtonInfo DinoGetKasekiButtonInfo(const DinoBackupEntry *entry, MResource<DinoKasekiDefs> defs) {
    DinoKasekiButtonInfo info;
    info.type = 0;
    info.level = 0;
    u32 id = entry->id;
    if (id >= 501 && id <= 530 && entry->active == 1) {
        info.icon = (id - 501) / 3 + 19;
        info.color = 0;
        info.type = 5;
        info.level = entry->score;
    } else if (id >= 601 && id <= 630 && entry->active == 1) {
        info.icon = 29;
        info.color = 0;
    } else {
        info.icon = defs.Get()->GetDef(id)->icon;
        info.color = entry->unk10_0;
        DinoKasekiRockState rock;
        gDinoBackup->GetKasekiRock(entry->id, (u32 *)&rock);
        if (rock.unk10) {
            DinoCreatureState creature;
            gDinoBackup->GetCreature((entry->id - 1) / 4 + 1, &creature);
            if (creature.parts[0].level != 0) {
                info.type = 1;
            } else {
                info.type = 2;
            }
            info.level = rock.score;
        } else {
            info.type = 3;
        }
        if (entry->unk10_0 == 1) {
            info.type = 4;
        }
    }
    return info;
}

// The text with every tag replaced by the value (unless the value has the tag itself).
// NONMATCHING: ours keeps an unreachable epilogue after the loop.
std::string DinoReplaceTag(std::string text, std::string tag, std::string value) {
    if (value.find(tag, 0) != std::string::npos) {
        return text;
    }
    for (;;) {
        std::string::size_type pos = text.find(tag, 0);
        if (pos == std::string::npos) {
            return text;
        }
        text = text.replace(pos, tag.size(), value);
    }
}

extern "C" u8 data_ov003_021624c0;  // the slow revive effect (which can't be skipped)

// A duration of the revive effect, scaled for which version of it plays.
int DinoReviveFrames(int frames) {
    if (data_ov003_021624c0) {
        return MFx32ToInt(MFx32::FromRaw(0xCCD) * MFx32(frames));
    }
    return MFx32ToInt(MFx32::FromRaw(0x548) * MFx32(frames));
}

#pragma auto_inline off

void DinoDimScreen() {
    MBlend *blend = gCurrentDisplay->mBlend;
    blend->Reset();
    blend->SetMode(3);
    blend->SetBrightness(8);
    blend->SetAlpha(16, 0);
    blend->SetTarget1(0x10);
    blend->SetTarget2(0x3F);
}

// (Emits the out-of-line copies of inline functions the game emits in this file.)
// (Taking its address emits it, with its own calls inlined.)
const DinoKasekiDefs *(MResource<DinoKasekiDefs>::*DinoKasekiDefsGetAnchor())() {
    return &MResource<DinoKasekiDefs>::Get;
}
#pragma dont_inline on
const DinoKasekiDef *DinoKasekiDefsAnchor(const DinoKasekiDefs *defs, u32 id) {
    return defs->GetDef(id);
}
MSerialTaskSet *DinoSerialAddAnchor(MSerialTaskSet *set, MTask *task) {
    return set->Add(task);
}
#pragma dont_inline reset

// The battle KP table: entries chosen by the largest minimum not above a value.
struct DinoBattleKPDef {
    u32 min;    // 0x00
    u32 unk4;
    u32 unk8;   // 0x08
    u32 unkC;   // 0x0C
    u32 unk10[2];
};
struct DinoBattleKPDefs {
    u32 unk0;
    u32 count;      // 0x04
    u32 entries;    // 0x08: offset of count DinoBattleKPDef
};

extern "C" const char data_ov003_02161d08[];   // "/etc/btl_kp_defs"

// The KP values for the value (0 and 0 when no entry applies).
void DinoGetBattleKP(u32 value, u32 *a, u32 *b) {
    MResource<DinoBattleKPDefs> defs((std::string(data_ov003_02161d08)));
    const DinoBattleKPDef *best = 0;
    for (u32 i = 0; i < defs.Get()->count; i++) {
        const DinoBattleKPDefs *data = defs.Get();
        const DinoBattleKPDef *def = &((const DinoBattleKPDef *)((const u8 *)data + data->entries))[i];
        if (value >= def->min) {
            if (best == 0) {
                best = def;
            } else if (best->min < def->min) {
                best = def;
            }
        }
    }
    if (best != 0) {
        *a = best->unkC;
        *b = best->unk8;
    } else {
        *a = 0;
        *b = 0;
    }
}
