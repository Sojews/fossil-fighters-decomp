// NitroSDK fixed-point math (declarations only).
#ifndef NITRO_FX_H
#define NITRO_FX_H

#include "types.h"

typedef s16 fx16;

#define FX32_ONE (1 << FX32_SHIFT)

typedef s64 fx64;
typedef s64 fx64c;

typedef struct {
    fx32 _00, _01, _10, _11;
} MtxFx22;

typedef struct {
    fx32 _00, _01, _02;
    fx32 _10, _11, _12;
    fx32 _20, _21, _22;
} MtxFx33;

typedef struct {
    fx32 _00, _01, _02;
    fx32 _10, _11, _12;
    fx32 _20, _21, _22;
    fx32 _30, _31, _32;
} MtxFx43;

typedef struct {
    fx32 x, y, z;
} VecFx32;

typedef struct {
    fx32 _00, _01, _02, _03;
    fx32 _10, _11, _12, _13;
    fx32 _20, _21, _22, _23;
    fx32 _30, _31, _32, _33;
} MtxFx44;

#ifdef __cplusplus
extern "C" {
#endif

extern const fx16 FX_SinCosTable_[];

fx32 FX_MulFunc(fx32 v1, fx32 v2);
fx32 FX_Div(fx32 numer, fx32 denom);
void FX_DivAsync(fx32 numer, fx32 denom);
void FX_InvAsync(fx32 denom);
fx32 FX_GetDivResult(void);
fx64c FX_GetDivResultFx64c(void);
fx32 FX_GetSqrtResult(void);
u16 FX_Atan2Idx(fx32 y, fx32 x);
void MTX_Rot22_(MtxFx22 *pDst, fx32 sinVal, fx32 cosVal);
void MTX_ScaleApply22(const MtxFx22 *pSrc, MtxFx22 *pDst, fx32 x, fx32 y);
void MTX_Identity33_(MtxFx33 *pDst);
void MTX_ScaleApply33(const MtxFx33 *pSrc, MtxFx33 *pDst, fx32 x, fx32 y, fx32 z);
void MTX_Concat33(const MtxFx33 *a, const MtxFx33 *b, MtxFx33 *ab);
fx32 FX_Mul32x64c(fx32 v32, fx64c v64c);
fx32 VEC_DotProduct(const VecFx32 *a, const VecFx32 *b);
void VEC_CrossProduct(const VecFx32 *a, const VecFx32 *b, VecFx32 *axb);
void VEC_Normalize(const VecFx32 *pSrc, VecFx32 *pDst);

#ifdef __cplusplus
}
#endif

#define FX_Mul(v1, v2) FX_MulFunc(v1, v2)
#define FX_SinIdx(idx) ((fx32)FX_SinCosTable_[((idx) >> 4) << 1])
#define FX_CosIdx(idx) ((fx32)FX_SinCosTable_[(((idx) >> 4) << 1) + 1])

#endif
