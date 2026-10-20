// NitroSDK 2D graphics: OAM affine parameters.
#ifndef NITRO_G2_H
#define NITRO_G2_H

#include "nitro/fx.h"

// One OAM affine parameter set, interleaved with the OAM attributes it shares memory with.
typedef struct {
    u16 _0[3];
    s16 PA;
    u16 _1[3];
    s16 PB;
    u16 _2[3];
    s16 PC;
    u16 _3[3];
    s16 PD;
} GXOamAffine;

// (Reads the whole matrix before writing: the game keeps the values in registers across the halfword stores.)
static inline void G2_SetOBJAffine(GXOamAffine *oam, const MtxFx22 *mtx) {
    fx32 a = mtx->_00;
    fx32 b = mtx->_01;
    fx32 c = mtx->_10;
    fx32 d = mtx->_11;
    oam->PA = (s16)(a >> (FX32_SHIFT - 8));
    oam->PB = (s16)(b >> (FX32_SHIFT - 8));
    oam->PC = (s16)(c >> (FX32_SHIFT - 8));
    oam->PD = (s16)(d >> (FX32_SHIFT - 8));
}

#endif
