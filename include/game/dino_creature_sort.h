#ifndef GAME_DINO_CREATURE_SORT_H
#define GAME_DINO_CREATURE_SORT_H

// The orders the creature lists (the status menu's, the medal menu's) sort creature ids in.

#include <algorithm>
#include "game/dino_backup.h"
#include "game/dino_resource.h"

extern const u32 kCreatureLevelLimits[2];

extern "C" u32 func_02031104(const DinoCreatureState *creature);

// A creature's experience, up to what the top level needs.
static inline u32 GetCreatureTotal(const DinoCreatureState &creature) {
    if (creature.parts[0].level == 0) {
        return 0;
    }
    u32 max, sum;
    sum = creature.unk0_8 + (creature.parts[0].level + creature.parts[1].level + creature.parts[2].level +
                                 creature.parts[3].level);
    max = kCreatureLevelLimits[0] * 11 + 50;
    return std::min(max, sum);
}

static inline u32 GetCreatureLevel(const DinoCreatureState &creature) {
    if (creature.parts[0].level == 0) {
        return 0;
    }
    return (GetCreatureTotal(creature) - 50) / kCreatureLevelLimits[0] + 1;
}

// By unk0D, in the order 1, 3, 2, 4, 5, then the rest.
struct DinoCreatureByUnk0D {
    bool operator()(const u32 &a, const u32 &b) const {
        const DinoCreatureDef *defs[2];
        u32 ranks[2];
        defs[0] = gDinoResource->GetCreatureDef(a);
        defs[1] = gDinoResource->GetCreatureDef(b);
        for (int i = 0; i < 2; i++) {
            switch (defs[i]->unk0D) {
            case 1:
                ranks[i] = 0;
                break;
            case 2:
                ranks[i] = 2;
                break;
            case 3:
                ranks[i] = 1;
                break;
            case 4:
                ranks[i] = 3;
                break;
            case 5:
                ranks[i] = 4;
                break;
            default:
                ranks[i] = 5;
                break;
            }
        }
        return ranks[0] < ranks[1];
    }
};

// By func_02031104, highest first (creatures not revived yet last).
struct DinoCreatureByUnk02031104 {
    bool operator()(const u32 &a, const u32 &b) const {
        u32 ids[2];
        u32 values[2];
        ids[0] = a;
        ids[1] = b;
        for (int i = 0; i < 2; i++) {
            DinoCreatureState creature;
            gDinoBackup->GetCreature(ids[i], &creature);
            if (creature.parts[0].level != 0) {
                values[i] = func_02031104(&creature);
            } else {
                values[i] = 0;
            }
        }
        return values[0] > values[1];
    }
};

// By level, highest first.
struct DinoCreatureByLevel {
    bool operator()(const u32 &a, const u32 &b) const {
        DinoCreatureState x, y;
        gDinoBackup->GetCreature(a, &x);
        gDinoBackup->GetCreature(b, &y);
        return GetCreatureLevel(x) > GetCreatureLevel(y);
    }
};

struct DinoCreatureByUnk88 {
    static u32 Key(u32 id) { return gDinoResource->GetCreatureDef(id)->unk88; }
    bool operator()(const u32 &a, const u32 &b) const {
        u32 y = Key(b);
        return Key(a) < y;
    }
};

struct DinoCreatureByVoice {
    static u32 Key(u32 id) { return gDinoResource->GetCreatureDef(id)->voice; }
    bool operator()(const u32 &a, const u32 &b) const {
        u32 y = Key(b);
        return Key(a) < y;
    }
};

#endif
