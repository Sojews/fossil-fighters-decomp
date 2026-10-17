// NitroSDK FX: 3D vectors.

#include "nitro/fx.h"

#define REG_DIVCNT (*(volatile u16 *) 0x04000280)
#define REG_DIV_NUMER (*(volatile s64 *) 0x04000290)
#define REG_DIV_DENOM (*(volatile s64 *) 0x04000298)
#define REG_DIV_RESULT (*(volatile s64 *) 0x040002a0)
#define REG_SQRTCNT (*(volatile u16 *) 0x040002b0)
#define REG_SQRT_RESULT (*(volatile u32 *) 0x040002b4)
#define REG_SQRT_PARAM (*(volatile s64 *) 0x040002b8)

#pragma thumb off

fx32 VEC_DotProduct(const VecFx32 *a, const VecFx32 *b) {
    return (fx32) (((fx64) a->x * b->x + (fx64) a->y * b->y + (fx64) a->z * b->z + (FX32_ONE >> 1)) >> FX32_SHIFT);
}

void VEC_CrossProduct(const VecFx32 *a, const VecFx32 *b, VecFx32 *axb) {
    fx32 x, y, z;
    x = (fx32) (((fx64) a->y * b->z - (fx64) a->z * b->y + (FX32_ONE >> 1)) >> FX32_SHIFT);
    y = (fx32) (((fx64) a->z * b->x - (fx64) a->x * b->z + (FX32_ONE >> 1)) >> FX32_SHIFT);
    z = (fx32) (((fx64) a->x * b->y - (fx64) a->y * b->x + (FX32_ONE >> 1)) >> FX32_SHIFT);
    axb->x = x;
    axb->y = y;
    axb->z = z;
}

// NONMATCHING: the game loads y then x separately; ours merges them into one ldm.
void VEC_Normalize(const VecFx32 *pSrc, VecFx32 *pDst) {
    fx64 t;
    s32 sqrt;

    t = (fx64) pSrc->x * pSrc->x + (fx64) pSrc->y * pSrc->y + (fx64) pSrc->z * pSrc->z;
    REG_DIVCNT = 2;
    REG_DIV_NUMER = (fx64) 1 << 56;
    REG_DIV_DENOM = t;
    REG_SQRTCNT = 1;
    REG_SQRT_PARAM = t << 2;
    while (REG_SQRTCNT & 0x8000) {
    }
    sqrt = (s32) REG_SQRT_RESULT;
    while (REG_DIVCNT & 0x8000) {
    }
    t = REG_DIV_RESULT * sqrt;
    pDst->x = (fx32) ((t * pSrc->x + ((fx64) 1 << 44)) >> 45);
    pDst->y = (fx32) ((t * pSrc->y + ((fx64) 1 << 44)) >> 45);
    pDst->z = (fx32) ((t * pSrc->z + ((fx64) 1 << 44)) >> 45);
}

#pragma thumb reset
