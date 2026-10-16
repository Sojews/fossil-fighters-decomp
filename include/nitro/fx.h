// NitroSDK fixed-point math (declarations only).
#ifndef NITRO_FX_H
#define NITRO_FX_H

#include "types.h"

typedef s16 fx16;

#define FX32_ONE (1 << FX32_SHIFT)

typedef struct {
    fx32 _00, _01, _10, _11;
} MtxFx22;

#ifdef __cplusplus
extern "C" {
#endif

extern const fx16 FX_SinCosTable_[];

fx32 FX_MulFunc(fx32 v1, fx32 v2);
fx32 FX_Div(fx32 numer, fx32 denom);
void MTX_Rot22_(MtxFx22 *pDst, fx32 sinVal, fx32 cosVal);
void MTX_ScaleApply22(const MtxFx22 *pSrc, MtxFx22 *pDst, fx32 x, fx32 y);

#ifdef __cplusplus
}
#endif

#define FX_Mul(v1, v2) FX_MulFunc(v1, v2)
#define FX_SinIdx(idx) ((fx32)FX_SinCosTable_[((idx) >> 4) << 1])
#define FX_CosIdx(idx) ((fx32)FX_SinCosTable_[(((idx) >> 4) << 1) + 1])

#endif
