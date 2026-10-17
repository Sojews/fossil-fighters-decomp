// NitroSDK FX: 4x3 matrices.

#include "nitro/fx.h"

void MI_Copy36B(const void *src, void *dest);

#pragma thumb off

asm void MTX_Identity43_(register MtxFx43 *pDst) {
    mov r2, #0x1000
    mov r3, #0
    stmia r0!, {r2, r3}
    mov r1, #0
    stmia r0!, {r1, r3}
    stmia r0!, {r2, r3}
    stmia r0!, {r1, r3}
    stmia r0!, {r2, r3}
    stmia r0!, {r1, r3}
    bx lr
}

asm void MTX_Copy43To44_(register const MtxFx43 *pSrc, register MtxFx44 *pDst) {
    stmfd sp!, {r4}
    mov r12, #0
    ldmia r0!, {r2, r3, r4}
    stmia r1!, {r2, r3, r4, r12}
    ldmia r0!, {r2, r3, r4}
    stmia r1!, {r2, r3, r4, r12}
    ldmia r0!, {r2, r3, r4}
    stmia r1!, {r2, r3, r4, r12}
    mov r12, #0x1000
    ldmia r0!, {r2, r3, r4}
    stmia r1!, {r2, r3, r4, r12}
    ldmfd sp!, {r4}
    bx lr
}

void MTX_TransApply43(const MtxFx43 *pSrc, MtxFx43 *pDst, fx32 x, fx32 y, fx32 z) {
    if (pSrc != pDst) {
        MI_Copy36B(pSrc, pDst);
    }
    pDst->_30 = pSrc->_30 + (fx32) (((fx64) x * pSrc->_00 + (fx64) y * pSrc->_10 + (fx64) z * pSrc->_20) >> FX32_SHIFT);
    pDst->_31 = pSrc->_31 + (fx32) (((fx64) x * pSrc->_01 + (fx64) y * pSrc->_11 + (fx64) z * pSrc->_21) >> FX32_SHIFT);
    pDst->_32 = pSrc->_32 + (fx32) (((fx64) x * pSrc->_02 + (fx64) y * pSrc->_12 + (fx64) z * pSrc->_22) >> FX32_SHIFT);
}

void MTX_ScaleApply43(const MtxFx43 *pSrc, MtxFx43 *pDst, fx32 x, fx32 y, fx32 z) {
    MTX_ScaleApply33((const MtxFx33 *) pSrc, (MtxFx33 *) pDst, x, y, z);
    pDst->_30 = pSrc->_30;
    pDst->_31 = pSrc->_31;
    pDst->_32 = pSrc->_32;
}

void MTX_Concat43(const MtxFx43 *a, const MtxFx43 *b, MtxFx43 *ab) {
    MtxFx43 tmp;
    MtxFx43 *p;
    fx32 x, y, z;
    fx32 xx, yy, zz;

    if (ab == b) {
        p = &tmp;
    } else {
        p = ab;
    }

    x = a->_00;
    y = a->_01;
    z = a->_02;
    p->_00 = (fx32) (((fx64) x * b->_00 + (fx64) y * b->_10 + (fx64) z * b->_20) >> FX32_SHIFT);
    p->_01 = (fx32) (((fx64) x * b->_01 + (fx64) y * b->_11 + (fx64) z * b->_21) >> FX32_SHIFT);
    xx = b->_02;
    yy = b->_12;
    zz = b->_22;
    p->_02 = (fx32) (((fx64) x * xx + (fx64) y * yy + (fx64) z * zz) >> FX32_SHIFT);

    x = a->_10;
    y = a->_11;
    z = a->_12;
    p->_12 = (fx32) (((fx64) x * xx + (fx64) y * yy + (fx64) z * zz) >> FX32_SHIFT);
    p->_11 = (fx32) (((fx64) x * b->_01 + (fx64) y * b->_11 + (fx64) z * b->_21) >> FX32_SHIFT);
    xx = b->_00;
    yy = b->_10;
    zz = b->_20;
    p->_10 = (fx32) (((fx64) x * xx + (fx64) y * yy + (fx64) z * zz) >> FX32_SHIFT);

    x = a->_20;
    y = a->_21;
    z = a->_22;
    p->_20 = (fx32) (((fx64) x * xx + (fx64) y * yy + (fx64) z * zz) >> FX32_SHIFT);
    p->_21 = (fx32) (((fx64) x * b->_01 + (fx64) y * b->_11 + (fx64) z * b->_21) >> FX32_SHIFT);
    xx = b->_02;
    yy = b->_12;
    zz = b->_22;
    p->_22 = (fx32) (((fx64) x * xx + (fx64) y * yy + (fx64) z * zz) >> FX32_SHIFT);

    x = a->_30;
    y = a->_31;
    z = a->_32;
    p->_32 = (fx32) ((((fx64) x * xx + (fx64) y * yy + (fx64) z * zz) >> FX32_SHIFT) + b->_32);
    p->_31 = (fx32) ((((fx64) x * b->_01 + (fx64) y * b->_11 + (fx64) z * b->_21) >> FX32_SHIFT) + b->_31);
    p->_30 = (fx32) ((((fx64) x * b->_00 + (fx64) y * b->_10 + (fx64) z * b->_20) >> FX32_SHIFT) + b->_30);

    if (p == &tmp) {
        *ab = tmp;
    }
}

void MTX_MultVec43(const VecFx32 *vec, const MtxFx43 *m, VecFx32 *dst) {
    fx32 x = vec->x;
    fx32 y = vec->y;
    fx32 z = vec->z;
    dst->x = (fx32) (((fx64) x * m->_00 + (fx64) y * m->_10 + (fx64) z * m->_20) >> FX32_SHIFT);
    dst->x += m->_30;
    dst->y = (fx32) (((fx64) x * m->_01 + (fx64) y * m->_11 + (fx64) z * m->_21) >> FX32_SHIFT);
    dst->y += m->_31;
    dst->z = (fx32) (((fx64) x * m->_02 + (fx64) y * m->_12 + (fx64) z * m->_22) >> FX32_SHIFT);
    dst->z += m->_32;
}

#pragma thumb reset
