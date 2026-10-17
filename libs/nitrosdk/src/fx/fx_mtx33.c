// NitroSDK FX: 3x3 matrices.

#include "nitro/fx.h"

#pragma thumb off

asm void MTX_Identity33_(register MtxFx33 *pDst) {
    mov r2, #0x1000
    str r2, [r0, #0x20]
    mov r3, #0
    stmia r0!, {r2, r3}
    mov r1, #0
    stmia r0!, {r1, r3}
    stmia r0!, {r2, r3}
    stmia r0!, {r1, r3}
    bx lr
}

void MTX_ScaleApply33(const MtxFx33 *pSrc, MtxFx33 *pDst, fx32 x, fx32 y, fx32 z) {
    pDst->_00 = (fx32) (((fx64) x * pSrc->_00) >> FX32_SHIFT);
    pDst->_01 = (fx32) (((fx64) x * pSrc->_01) >> FX32_SHIFT);
    pDst->_02 = (fx32) (((fx64) x * pSrc->_02) >> FX32_SHIFT);
    pDst->_10 = (fx32) (((fx64) y * pSrc->_10) >> FX32_SHIFT);
    pDst->_11 = (fx32) (((fx64) y * pSrc->_11) >> FX32_SHIFT);
    pDst->_12 = (fx32) (((fx64) y * pSrc->_12) >> FX32_SHIFT);
    pDst->_20 = (fx32) (((fx64) z * pSrc->_20) >> FX32_SHIFT);
    pDst->_21 = (fx32) (((fx64) z * pSrc->_21) >> FX32_SHIFT);
    pDst->_22 = (fx32) (((fx64) z * pSrc->_22) >> FX32_SHIFT);
}

void MTX_Concat33(const MtxFx33 *a, const MtxFx33 *b, MtxFx33 *ab) {
    MtxFx33 tmp;
    MtxFx33 *p;
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
    p->_22 = (fx32) (((fx64) x * b->_02 + (fx64) y * b->_12 + (fx64) z * b->_22) >> FX32_SHIFT);

    if (p == &tmp) {
        *ab = tmp;
    }
}

#pragma thumb reset
