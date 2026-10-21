// A file whose only code is its static initialization: the field characters' move speeds and the like (fx32),
// used by ov001's chara angle anime and raster (see dino_static_init.h).

#include "game/dino_static_init.h"

extern "C" u32 data_020b5ab8[];
static struct DinoFieldCharaSpeedsStaticInit {
    DinoFieldCharaSpeedsStaticInit() {
        DINO_POOL_WORD(data_020b5ab8, 0xc) = 0x99a;
        DINO_POOL_WORD(data_020b5ab8, 0x4) = 0x1800;
        DINO_POOL_WORD(data_020b5ab8, 0x0) = 0x3000;
        DINO_POOL_WORD(data_020b5ab8, 0x8) = 0x5000;
        DINO_POOL_WORD(data_020b5ab8, 0x10) = 0x14cd;
    }
} sDinoFieldCharaSpeedsStaticInit;
