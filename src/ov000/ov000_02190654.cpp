// A file of which only the static initialization is left: a common header's three layout points.

#include "types.h"
#include "game/dino_static_init.h"

extern "C" u32 data_ov000_02192c24[];
static struct Ov00002190654StaticInit {
    Ov00002190654StaticInit() {
        DINO_POOL_WORD(data_ov000_02192c24, 0xc) = 128;
        DINO_POOL_WORD(data_ov000_02192c24, 0x10) = 176;
        DINO_POOL_WORD(data_ov000_02192c24, 0x2c) = 8;
        DINO_POOL_WORD(data_ov000_02192c24, 0x30) = 150;
        DINO_POOL_WORD(data_ov000_02192c24, 0x1c) = 64;
        DINO_POOL_WORD(data_ov000_02192c24, 0x20) = 112;
    }
} sOv00002190654StaticInit;
