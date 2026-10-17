// NitroSDK OS: the ARM946E-S data and instruction caches (32-byte lines; the data cache is 4 segments of 1KB).

#include "nitro/os.h"

#define HW_CACHE_LINE_SIZE 32

#pragma thumb off
asm void DC_InvalidateAll(void) {
    mov r0, #0
    mcr p15, 0, r0, c7, c6, 0
    bx lr
}

asm void DC_StoreAll(void) {
    mov r1, #0
@loop_seg:
    mov r0, #0
@loop_line:
    orr r2, r1, r0
    mcr p15, 0, r2, c7, c10, 2
    add r0, r0, #HW_CACHE_LINE_SIZE
    cmp r0, #0x400
    blt @loop_line
    add r1, r1, #0x40000000
    cmp r1, #0
    bne @loop_seg
    bx lr
}

asm void DC_FlushAll(void) {
    mov r12, #0
    mov r1, #0
@loop_seg:
    mov r0, #0
@loop_line:
    orr r2, r1, r0
    mcr p15, 0, r12, c7, c10, 4
    mcr p15, 0, r2, c7, c14, 2
    add r0, r0, #HW_CACHE_LINE_SIZE
    cmp r0, #0x400
    blt @loop_line
    add r1, r1, #0x40000000
    cmp r1, #0
    bne @loop_seg
    bx lr
}

asm void DC_InvalidateRange(register void *startAddr, register u32 nBytes) {
    add r1, r1, r0
    bic r0, r0, #HW_CACHE_LINE_SIZE - 1
@loop:
    mcr p15, 0, r0, c7, c6, 1
    add r0, r0, #HW_CACHE_LINE_SIZE
    cmp r0, r1
    blt @loop
    bx lr
}

asm void DC_StoreRange(register const void *startAddr, register u32 nBytes) {
    add r1, r1, r0
    bic r0, r0, #HW_CACHE_LINE_SIZE - 1
@loop:
    mcr p15, 0, r0, c7, c10, 1
    add r0, r0, #HW_CACHE_LINE_SIZE
    cmp r0, r1
    blt @loop
    bx lr
}

asm void DC_FlushRange(register const void *startAddr, register u32 nBytes) {
    mov r12, #0
    add r1, r1, r0
    bic r0, r0, #HW_CACHE_LINE_SIZE - 1
@loop:
    mcr p15, 0, r12, c7, c10, 4
    mcr p15, 0, r0, c7, c14, 1
    add r0, r0, #HW_CACHE_LINE_SIZE
    cmp r0, r1
    blt @loop
    bx lr
}

asm void DC_WaitWriteBufferEmpty(void) {
    mov r0, #0
    mcr p15, 0, r0, c7, c10, 4
    bx lr
}

asm void IC_InvalidateAll(void) {
    mov r0, #0
    mcr p15, 0, r0, c7, c5, 0
    bx lr
}

asm void IC_InvalidateRange(register void *startAddr, register u32 nBytes) {
    add r1, r1, r0
    bic r0, r0, #HW_CACHE_LINE_SIZE - 1
@loop:
    mcr p15, 0, r0, c7, c5, 1
    add r0, r0, #HW_CACHE_LINE_SIZE
    cmp r0, r1
    blt @loop
    bx lr
}

// Waits for line 0 of the display with interrupts masked (IME off).
asm void OSi_WaitVCount0(void) {
    mov r12, #0x04000000
    ldr r1, [r12, #0x208]
    str r12, [r12, #0x208]
@loop:
    ldrh r0, [r12, #6]
    cmp r0, #0
    bne @loop
    str r1, [r12, #0x208]
    bx lr
}
#pragma thumb reset
