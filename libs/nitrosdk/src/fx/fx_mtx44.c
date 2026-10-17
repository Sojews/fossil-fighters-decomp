// NitroSDK FX: 4x4 matrices.

#include "nitro/fx.h"

void MI_Copy48B(const void *src, void *dest);

#pragma thumb off

asm void MTX_Identity44_(register MtxFx44 *pDst) {
    mov r2, #0x1000
    mov r3, #0
    stmia r0!, {r2, r3}
    mov r1, #0
    stmia r0!, {r1, r3}
    stmia r0!, {r1, r2, r3}
    stmia r0!, {r1, r3}
    stmia r0!, {r1, r2, r3}
    stmia r0!, {r1, r3}
    stmia r0!, {r1, r2}
    bx lr
}

asm void MTX_Copy44To43_(register const MtxFx44 *pSrc, register MtxFx43 *pDst) {
    ldmia r0!, {r2, r3, r12}
    add r0, r0, #4
    stmia r1!, {r2, r3, r12}
    ldmia r0!, {r2, r3, r12}
    add r0, r0, #4
    stmia r1!, {r2, r3, r12}
    ldmia r0!, {r2, r3, r12}
    add r0, r0, #4
    stmia r1!, {r2, r3, r12}
    ldmia r0!, {r2, r3, r12}
    add r0, r0, #4
    stmia r1!, {r2, r3, r12}
    bx lr
}

void MTX_TransApply44(const MtxFx44 *pSrc, MtxFx44 *pDst, fx32 x, fx32 y, fx32 z) {
    if (pSrc != pDst) {
        MI_Copy48B(pSrc, pDst);
    }
    pDst->_30 = pSrc->_30 + (fx32) (((fx64) x * pSrc->_00 + (fx64) y * pSrc->_10 + (fx64) z * pSrc->_20) >> FX32_SHIFT);
    pDst->_31 = pSrc->_31 + (fx32) (((fx64) x * pSrc->_01 + (fx64) y * pSrc->_11 + (fx64) z * pSrc->_21) >> FX32_SHIFT);
    pDst->_32 = pSrc->_32 + (fx32) (((fx64) x * pSrc->_02 + (fx64) y * pSrc->_12 + (fx64) z * pSrc->_22) >> FX32_SHIFT);
    pDst->_33 = pSrc->_33 + (fx32) (((fx64) x * pSrc->_03 + (fx64) y * pSrc->_13 + (fx64) z * pSrc->_23) >> FX32_SHIFT);
}

void MTX_ScaleApply44(const MtxFx44 *pSrc, MtxFx44 *pDst, fx32 x, fx32 y, fx32 z) {
    pDst->_00 = (fx32) (((fx64) x * pSrc->_00) >> FX32_SHIFT);
    pDst->_01 = (fx32) (((fx64) x * pSrc->_01) >> FX32_SHIFT);
    pDst->_02 = (fx32) (((fx64) x * pSrc->_02) >> FX32_SHIFT);
    pDst->_03 = (fx32) (((fx64) x * pSrc->_03) >> FX32_SHIFT);
    pDst->_10 = (fx32) (((fx64) y * pSrc->_10) >> FX32_SHIFT);
    pDst->_11 = (fx32) (((fx64) y * pSrc->_11) >> FX32_SHIFT);
    pDst->_12 = (fx32) (((fx64) y * pSrc->_12) >> FX32_SHIFT);
    pDst->_13 = (fx32) (((fx64) y * pSrc->_13) >> FX32_SHIFT);
    pDst->_20 = (fx32) (((fx64) z * pSrc->_20) >> FX32_SHIFT);
    pDst->_21 = (fx32) (((fx64) z * pSrc->_21) >> FX32_SHIFT);
    pDst->_22 = (fx32) (((fx64) z * pSrc->_22) >> FX32_SHIFT);
    pDst->_23 = (fx32) (((fx64) z * pSrc->_23) >> FX32_SHIFT);
    if (pSrc != pDst) {
        fx32 t1, t0;
        t1 = pSrc->_31;
        t0 = pSrc->_30;
        pDst->_30 = t0;
        pDst->_31 = t1;
        t1 = pSrc->_33;
        t0 = pSrc->_32;
        pDst->_32 = t0;
        pDst->_33 = t1;
    }
}

#pragma thumb reset

// The rotations are Thumb assembly.
asm void MTX_RotX44_(register MtxFx44 *pDst, register fx32 sinVal, register fx32 cosVal) {
    str r2, [r0, #0x14]
    str r2, [r0, #0x28]
    str r1, [r0, #0x18]
    neg r1, r1
    str r1, [r0, #0x24]
    mov r1, #1
    mov r2, #0
    lsl r1, r1, #0xc
    mov r3, #0
    stmia r0!, {r1, r2, r3}
    stmia r0!, {r2, r3}
    add r0, #8
    stmia r0!, {r2, r3}
    add r0, #8
    stmia r0!, {r2, r3}
    stmia r0!, {r2, r3}
    str r1, [r0]
    bx lr
}

asm void MTX_RotY44_(register MtxFx44 *pDst, register fx32 sinVal, register fx32 cosVal) {
    str r2, [r0]
    str r2, [r0, #0x28]
    str r1, [r0, #0x20]
    neg r1, r1
    str r1, [r0, #8]
    mov r3, #1
    mov r1, #0
    lsl r3, r3, #0xc
    mov r2, #0
    str r2, [r0, #4]
    add r0, #0xc
    stmia r0!, {r1, r2, r3}
    stmia r0!, {r1, r2}
    str r2, [r0, #4]
    add r0, #0xc
    stmia r0!, {r1, r2}
    stmia r0!, {r1, r2, r3}
    bx lr
}

asm void MTX_RotZ44_(register MtxFx44 *pDst, register fx32 sinVal, register fx32 cosVal) {
    str r2, [r0]
    str r2, [r0, #0x14]
    str r1, [r0, #4]
    neg r1, r1
    str r1, [r0, #0x10]
    mov r3, #1
    mov r1, #0
    lsl r3, r3, #0xc
    mov r2, #0
    add r0, #8
    stmia r0!, {r1, r2}
    add r0, #8
    stmia r0!, {r1, r2}
    stmia r0!, {r1, r2, r3}
    stmia r0!, {r1, r2}
    stmia r0!, {r1, r2, r3}
    bx lr
}
