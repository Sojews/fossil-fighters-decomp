#ifndef GAME_DINO_UI_FOSSIL_ENTRY_H
#define GAME_DINO_UI_FOSSIL_ENTRY_H

#include "types.h"

// An entry of the fossil lists the shop and container menus build from the backup (what the fields are is unknown).
struct DinoUiFossilEntry {
    u8 unk0;    // 0x0
    u8 unk1;    // 0x1
    u8 unk2;    // 0x2
    u16 unk4;   // 0x4
};

#endif
