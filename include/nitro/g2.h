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

static inline void G2_SetOBJAffine(GXOamAffine *oam, const MtxFx22 *mtx) {
    oam->PA = (s16)(mtx->_00 >> (FX32_SHIFT - 8));
    oam->PB = (s16)(mtx->_01 >> (FX32_SHIFT - 8));
    oam->PC = (s16)(mtx->_10 >> (FX32_SHIFT - 8));
    oam->PD = (s16)(mtx->_11 >> (FX32_SHIFT - 8));
}

#endif
