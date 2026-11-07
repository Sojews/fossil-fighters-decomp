// A file whose only code is its static initialization: main's shared layout points and rects at data_020bafbc
// (one pool; its words past 0x100 go through the same anchor, see dino_static_init.h).
// The centred icon rect reads its size back from the pool (one stack copy, not two), and its offset is a named
// local computed before the uncentred rect is stored (that orders the temp slots and the rect's zero-half stores).

#include "game/dino_static_init.h"

extern "C" u32 data_020bafbc[];
extern "C" u32 data_020bb0bc[];
#pragma always_inline on
static struct DinoMainLayoutStaticInit {
    DinoMainLayoutStaticInit() {
        DINO_POOL_NV(MRect, data_020bafbc, 0x110) = MRect(16, 0, 240, 192);
        DINO_POOL_NV(MRect, data_020bafbc, 0x120) = MRect(24, 8, 232, 24);
        DINO_POOL_NV(MPoint, data_020bafbc, 0x2c) = MPoint(128, 16);
        DINO_POOL_NV(MPoint, data_020bafbc, 0x54) = MPoint(24, 32);
        DINO_POOL_NV(MRect, data_020bafbc, 0x1c) = MRect(208, 24, 24, 32);
        DINO_POOL_NV(MPoint, data_020bafbc, 0xc) = MPoint(32, 24);
        DINO_POOL_NV(MPoint, data_020bafbc, 0x5c) = MPoint(56, 32);
        DINO_POOL_NV(MPoint, data_020bafbc, 0x14) = MPoint(176, 24);
        DINO_POOL_NV(MPoint, data_020bafbc, 0x34) = MPoint(160, 10);
        MPoint pos(224, 16);
        DINO_POOL_NV(MPoint, data_020bafbc, 0x3c) = pos;
        DINO_POOL_NV(MPoint, data_020bafbc, 0x4c) = MPoint(16, 16);
        MRect rect = MRect(MPoint(0, 0), DINO_POOL_NV(MPoint, data_020bafbc, 0x4c));
        MPoint off = pos - DINO_POOL_NV(MPoint, data_020bafbc, 0x4c) / 2;
        DINO_POOL_NV(MRect, data_020bafbc, 0x130) = rect;
        DINO_POOL_NV(MRect, data_020bafbc, 0x130) = rect + off;
    }
} sDinoMainLayoutStaticInit;
