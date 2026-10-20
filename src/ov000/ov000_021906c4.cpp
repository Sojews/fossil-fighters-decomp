// A file of which only the static initialization is left: a common header's three layout points and three colors.

#include "types.h"
#include "game/dino_static_init.h"

extern "C" u32 data_ov000_02193668[];
static struct Ov000021906c4StaticInit {
    Ov000021906c4StaticInit() {
        DINO_POOL_WORD(data_ov000_02193668, 0x30) = 128;
        DINO_POOL_WORD(data_ov000_02193668, 0x34) = 176;
        DINO_POOL_WORD(data_ov000_02193668, 0x18) = 8;
        DINO_POOL_WORD(data_ov000_02193668, 0x1c) = 150;
        DINO_POOL_WORD(data_ov000_02193668, 0x20) = 64;
        DINO_POOL_WORD(data_ov000_02193668, 0x24) = 112;
        ((volatile u16 *)data_ov000_02193668)[0x2a / 2] = 0x8000;
        ((volatile u16 *)data_ov000_02193668)[0x2c / 2] = 0xC210;
        ((volatile u16 *)data_ov000_02193668)[0x2e / 2] = 0xFFFF;
    }
} sOv000021906c4StaticInit;
