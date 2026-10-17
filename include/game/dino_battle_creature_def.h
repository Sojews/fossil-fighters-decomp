#ifndef GAME_DINO_BATTLE_CREATURE_DEF_H
#define GAME_DINO_BATTLE_CREATURE_DEF_H

#include "types.h"

// A vivosaur's definition data as the battle reads it (not identified yet).
struct DinoBattleCreatureDef {
    u8 unk00[0xD];
    u8 mUnk0D;          // 0x0D
    u8 unk0E[0x20 - 0x0E];
    u8 mUnk20;          // 0x20
    u8 mUnk21;          // 0x21
    u8 unk22;
    u8 mUnk23;          // 0x23
    u32 mUnk24;         // 0x24: flags
    u8 unk28[0x61 - 0x28];
    u8 mUnk61;          // 0x61
    u8 unk62[0x68 - 0x62];
    s32 mUnk68;         // 0x68: offset of a byte table
};

#endif
