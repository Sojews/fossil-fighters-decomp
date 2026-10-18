// NitroSDK GX: copying commands into the geometry FIFO in 48-, 64- and 128-byte blocks.

#include "types.h"

#pragma thumb off

asm void GX_SendFifo48B(register const void *pSrc, register void *pDest) {
    ldmia r0!, {r2, r3, r12}
    stmia r1, {r2, r3, r12}
    ldmia r0!, {r2, r3, r12}
    stmia r1, {r2, r3, r12}
    ldmia r0!, {r2, r3, r12}
    stmia r1, {r2, r3, r12}
    ldmia r0!, {r2, r3, r12}
    stmia r1, {r2, r3, r12}
    bx lr
}

asm void GX_SendFifo64B(register const void *pSrc, register void *pDest) {
    stmfd sp!, {r4-r8}
    ldmia r0!, {r2-r8, r12}
    stmia r1, {r2-r8, r12}
    ldmia r0!, {r2-r8, r12}
    stmia r1, {r2-r8, r12}
    ldmfd sp!, {r4-r8}
    bx lr
}

asm void GX_SendFifo128B(register const void *pSrc, register void *pDest) {
    stmfd sp!, {r4-r8}
    ldmia r0!, {r2-r8, r12}
    stmia r1, {r2-r8, r12}
    ldmia r0!, {r2-r8, r12}
    stmia r1, {r2-r8, r12}
    ldmia r0!, {r2-r8, r12}
    stmia r1, {r2-r8, r12}
    ldmia r0!, {r2-r8, r12}
    stmia r1, {r2-r8, r12}
    ldmfd sp!, {r4-r8}
    bx lr
}

#pragma thumb reset
