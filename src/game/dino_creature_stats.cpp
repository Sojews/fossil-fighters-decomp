// A creature's stats, worked out from its rank and its parts.

#include "types.h"
#include "m/fx32.h"
#include <algorithm>
#include "game/dino_backup.h"
#include "game/dino_resource.h"
#include "game/dino_static_init.h"

// How far towards the high value a stat is at each rank, along each curve (by curve, then rank).
extern "C" const MFx32 data_020bc5b4[4][8];

extern const u32 kCreatureLevelLimits[2];

// A creature's experience, up to what the top level needs.
static inline u32 GetCreatureTotal(const DinoCreatureState &creature) {
    if (creature.parts[0].level == 0) {
        return 0;
    }
    u32 sum = creature.unk0_8 + (creature.parts[0].level + creature.parts[1].level + creature.parts[2].level +
                                 creature.parts[3].level);
    return std::min(kCreatureLevelLimits[0] * 11 + 50, sum);
}

static inline u32 GetCreatureRank(const DinoCreatureState &creature) {
    if (creature.parts[0].level == 0) {
        return 0;
    }
    return (GetCreatureTotal(creature) - 50) / kCreatureLevelLimits[0] + 1;
}

// What the stats are worked out from.
struct DinoCreatureStatsInput {
    u32 id;         // 0x00
    int rank;       // 0x04: 0-12
    int bonus[4];   // 0x08: by part
    int levels[5];  // 0x18: the parts', then the creature's own
};

struct DinoCreatureStats {
    int hp;         // 0x00
    int stats[5];   // 0x04
};

// A stat at a rank: from low (rank 0) to high (rank 8) along a curve, then a quarter of the way on towards max at each
// rank after. bonus adds that many percent of a quarter of the way.
extern "C" int func_02030e7c(int rank, int low, int high, int max, int curve, int bonus) {
    MFx32 value;
    MFx32 fLow(low);
    MFx32 fMax(max);
    MFx32 fHigh(high);
    curve--;
    if (curve > 3 || curve < 0) {
        curve = 0;
    }
    if (rank == 8) {
        return high;
    }
    if (rank < 8) {
        value = fLow + (fHigh - fLow) * data_020bc5b4[curve][rank];
    } else {
        value = fHigh + (fMax - fHigh) * MFx32(rank - 8) / MFx32(4);
    }
    int result = MFx32ToInt(value);
    if (bonus != 0) {
        result += (max - high) * bonus / 100 / 4;
    }
    return result;
}

static inline int GetStat(const DinoCreatureStatsInput *info, const DinoCreatureStatDef &def, int bonus) {
    return func_02030e7c(info->rank, def.low, def.high, def.max, def.curve, bonus);
}

// The parts' experience, up to the most the hit points count.
static inline int GetTotal(const DinoCreatureStatsInput *info) {
    int total = info->levels[0] + info->levels[1] + info->levels[2] + info->levels[3] + info->levels[4];
    return std::min(600, total);
}

// The stats: the hit points go from one rank's to the next by how much experience the parts have past the rank's
// (fraction, if fixed).
extern "C" DinoCreatureStats func_02030f38(const DinoCreatureStatsInput *info, bool fixed, MFx32 fraction) {
    DinoCreatureStats out;
    const DinoCreatureDef *def = gDinoResource->GetCreatureDef(info->id);
    out.stats[0] = GetStat(info, def->stats[0], info->bonus[2]);
    out.stats[1] = GetStat(info, def->stats[1], 0);
    out.stats[2] = GetStat(info, def->stats[2], info->bonus[0]);
    out.stats[3] = GetStat(info, def->stats[3], info->bonus[3]);
    out.stats[4] = GetStat(info, def->stat4, 0);
    fx32 scaleLow = gDinoResource->GetBattleAdjustDefs()->rankScaleLow;
    fx32 scaleHigh = gDinoResource->GetBattleAdjustDefs()->rankScaleHigh;
    MFx32 scale;
    if (info->rank != 8) {
        if (info->rank > 8) {
            scale = MFx32::FromRaw(scaleHigh - FX32_ONE) * MFx32(info->rank - 8) / MFx32(4);
        } else {
            scale = MFx32::FromRaw(FX32_ONE - scaleLow) * MFx32(info->rank) / MFx32(8);
        }
    }
    if (info->rank == 12) {
        out.hp = def->GetHpTable()[11];
    } else {
        MFx32 f;
        if (fixed) {
            f = fraction;
        } else {
            int total = GetTotal(info);
            f = MFx32(total - info->rank * 50) / MFx32(50);
        }
        int i = info->rank != 0 ? info->rank - 1 : 1;
        out.hp = def->GetHpTable()[i] + MFx32ToInt(MFx32(def->GetHpTable()[i + 1] - def->GetHpTable()[i]) * f);
    }
    return out;
}

// A creature's hit points.
extern "C" u32 func_02031104(const DinoCreatureState *creature) {
    DinoCreatureStatsInput info;
    info.levels[0] = info.levels[1] = info.levels[2] = info.levels[3] = 0;
    info.id = creature->index;
    info.rank = GetCreatureRank(*creature);
    info.levels[4] = creature->unk0_8;
    for (u32 i = 0; i < 4; i++) {
        info.bonus[i] = creature->parts[i].unk12;
        info.levels[i] = creature->parts[i].level;
    }
    return func_02030f38(&info, false, MFx32::FromRaw(0)).hp;
}

// The file's static initialization (see dino_static_init.h): three points, and the curves (in sevenths, as fx32).
extern "C" u8 data_020bc59c[];
extern "C" u8 data_020bc61c[];

static struct DinoCreatureStatsStaticInit {
    DinoCreatureStatsStaticInit() {
        DINO_POOL(MPoint, data_020bc59c, 0x14) = MPoint(128, 176);
        DINO_POOL(MPoint, data_020bc59c, 0xc) = MPoint(8, 150);
        DINO_POOL(MPoint, data_020bc59c, 4) = MPoint(64, 112);
        DINO_POOL_WORD(data_020bc59c, 0x1c) = 0x0;
        DINO_POOL_WORD(data_020bc59c, 0x20) = 0x249;
        DINO_POOL_WORD(data_020bc59c, 0x24) = 0x492;
        DINO_POOL_WORD(data_020bc59c, 0x28) = 0x6db;
        DINO_POOL_WORD(data_020bc59c, 0x2c) = 0x925;
        DINO_POOL_WORD(data_020bc59c, 0x30) = 0xb6e;
        DINO_POOL_WORD(data_020bc59c, 0x34) = 0xdb7;
        DINO_POOL_WORD(data_020bc59c, 0x38) = 0x1000;
        DINO_POOL_WORD(data_020bc59c, 0x3c) = 0x0;
        DINO_POOL_WORD(data_020bc59c, 0x40) = 0x43f;
        DINO_POOL_WORD(data_020bc59c, 0x44) = 0x7d6;
        DINO_POOL_WORD(data_020bc59c, 0x48) = 0xac7;
        DINO_POOL_WORD(data_020bc59c, 0x4c) = 0xd10;
        DINO_POOL_WORD(data_020bc59c, 0x50) = 0xeb2;
        DINO_POOL_WORD(data_020bc59c, 0x54) = 0xfac;
        DINO_POOL_WORD(data_020bc59c, 0x58) = 0x1000;
        DINO_POOL_WORD(data_020bc59c, 0x5c) = 0x0;
        DINO_POOL_WORD(data_020bc59c, 0x60) = 0x54;
        DINO_POOL_WORD(data_020bc59c, 0x64) = 0x14e;
        DINO_POOL_WORD(data_020bc59c, 0x68) = 0x2f0;
        DINO_POOL_WORD(data_020bc59c, 0x6c) = 0x539;
        DINO_POOL_WORD(data_020bc59c, 0x70) = 0x82a;
        DINO_POOL_WORD(data_020bc59c, 0x74) = 0xbc1;
        DINO_POOL_WORD(data_020bc59c, 0x78) = 0x1000;
        DINO_POOL_WORD(data_020bc59c, 0x7c) = 0x0;
        DINO_POOL_WORD(data_020bc61c, 0x0) = 0x249;
        DINO_POOL_WORD(data_020bc61c, 0x4) = 0x249;
        DINO_POOL_WORD(data_020bc61c, 0x8) = 0x6db;
        DINO_POOL_WORD(data_020bc61c, 0xc) = 0x6db;
        DINO_POOL_WORD(data_020bc61c, 0x10) = 0xb6e;
        DINO_POOL_WORD(data_020bc61c, 0x14) = 0xb6e;
        DINO_POOL_WORD(data_020bc61c, 0x18) = 0x1000;
    }
} sDinoCreatureStatsStaticInit;
