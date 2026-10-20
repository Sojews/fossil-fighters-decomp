// A file of which only the static initialization is left: a common header's three layout points.

#include "types.h"
#include "game/dino_static_init.h"

extern "C" u32 data_ov000_02192dfc[];
static struct Ov00002190674StaticInit {
    Ov00002190674StaticInit() {
        DINO_POOL_WORD(data_ov000_02192dfc, 0xc) = 128;
        DINO_POOL_WORD(data_ov000_02192dfc, 0x10) = 176;
        DINO_POOL_WORD(data_ov000_02192dfc, 0x14) = 8;
        DINO_POOL_WORD(data_ov000_02192dfc, 0x18) = 150;
        DINO_POOL_WORD(data_ov000_02192dfc, 0x4) = 64;
        DINO_POOL_WORD(data_ov000_02192dfc, 0x8) = 112;
    }
} sOv00002190674StaticInit;
