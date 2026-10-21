// A file whose only code is its static initialization: colors (15-bit, bit 15 set) and a rect of the battle's
// shared pool at data_020b5a90 (see dino_static_init.h).

#include "game/dino_static_init.h"

extern "C" u32 data_020b5a90[];
#define DINO_POOL_U16(sym, off) (((volatile u16 *) &(sym))[(off) / 2])
static struct DinoBattleColorsStaticInit {
    DinoBattleColorsStaticInit() {
        DINO_POOL_U16(data_020b5a90, 0xa) = 0xfeed;
        DINO_POOL_U16(data_020b5a90, 0x6) = 0x98df;
        DINO_POOL_U16(data_020b5a90, 0x4) = 0xffff;
        DINO_POOL_U16(data_020b5a90, 0x8) = 0xffff;
        DINO_POOL_U16(data_020b5a90, 0x2) = 0x8000;
        DINO_POOL_U16(data_020b5a90, 0x0) = 0x825f;
        DINO_POOL_WORD(data_020b5a90, 0x10) = -32;
        DINO_POOL_WORD(data_020b5a90, 0x14) = -32;
        DINO_POOL_WORD(data_020b5a90, 0x18) = 32;
        DINO_POOL_WORD(data_020b5a90, 0x1c) = 32;
        DINO_POOL_U16(data_020b5a90, 0xc) = 0xffff;
    }
} sDinoBattleColorsStaticInit;
