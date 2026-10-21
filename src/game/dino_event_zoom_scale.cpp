// A file whose only code is its static initialization: the event picture zoom's scales (fx32), used by
// dino_event_exec.cpp (see dino_static_init.h).

#include "game/dino_static_init.h"

extern "C" u32 data_020b5ab0[];
static struct DinoEventZoomScaleStaticInit {
    DinoEventZoomScaleStaticInit() {
        DINO_POOL_WORD(data_020b5ab0, 0x4) = 0x2000;
        DINO_POOL_WORD(data_020b5ab0, 0x0) = 0x2000;
    }
} sDinoEventZoomScaleStaticInit;
