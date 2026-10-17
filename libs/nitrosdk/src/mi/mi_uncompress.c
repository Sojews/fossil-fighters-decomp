// NitroSDK MI: decompressors for the BIOS formats (LZ77, Huffman, run-length), written in assembly. The 8-bit
// writers use swpb so they work on any memory; the LZ16 one writes halfwords (for VRAM).

#include "nitro/mi.h"

#pragma thumb off
asm void MI_UncompressLZ8(register const void *srcp, register void *destp) {
    stmfd sp!, {r4-r6, lr}
    ldr r5, [r0], #4
    mov r2, r5, lsr #8
@block:
    cmp r2, #0
    ble @end
    ldrb lr, [r0], #1
    mov r4, #8
@flag:
    subs r4, r4, #1
    blt @block
    tst lr, #0x80
    bne @copy
    ldrb r6, [r0], #1
    swpb r6, r6, [r1]
    add r1, r1, #1
    sub r2, r2, #1
    b @next
@copy:
    ldrb r5, [r0, #0]
    mov r6, #3
    add r3, r6, r5, asr #4
    ldrb r6, [r0], #1
    and r5, r6, #0xf
    mov r12, r5, lsl #8
    ldrb r6, [r0], #1
    orr r5, r6, r12
    add r12, r5, #1
    sub r2, r2, r3
@copy_loop:
    ldrb r5, [r1, -r12]
    swpb r5, r5, [r1]
    add r1, r1, #1
    subs r3, r3, #1
    bgt @copy_loop
@next:
    cmp r2, #0
    movgt lr, lr, lsl #1
    bgt @flag
    b @block
@end:
    ldmfd sp!, {r4-r6, lr}
    bx lr
}

asm void MI_UncompressLZ16(register const void *srcp, register void *destp) {
    stmfd sp!, {r4-r10, lr}
    mov r3, #0
    ldr r8, [r0], #4
    mov r10, r8, lsr #8
    mov r2, #0
@block:
    cmp r10, #0
    ble @end
    ldrb r6, [r0], #1
    mov r7, #8
@flag:
    subs r7, r7, #1
    blt @block
    tst r6, #0x80
    bne @copy
    ldrb r9, [r0], #1
    orr r3, r3, r9, lsl r2
    sub r10, r10, #1
    eors r2, r2, #8
    streqh r3, [r1], #2
    moveq r3, #0
    b @next
@copy:
    ldrb r9, [r0, #0]
    mov r8, #3
    add r5, r8, r9, asr #4
    ldrb r9, [r0], #1
    and r8, r9, #0xf
    mov r4, r8, lsl #8
    ldrb r9, [r0], #1
    orr r8, r9, r4
    add r4, r8, #1
    rsb r8, r2, #8
    and r9, r4, #1
    eor lr, r8, r9, lsl #3
    sub r10, r10, r5
@copy_loop:
    eor lr, lr, #8
    rsb r8, r2, #8
    add r8, r4, r8, lsr #3
    mov r8, r8, lsr #1
    mov r8, r8, lsl #1
    ldrh r9, [r1, -r8]
    mov r8, #0xff
    and r8, r9, r8, lsl lr
    mov r8, r8, asr lr
    orr r3, r3, r8, lsl r2
    eors r2, r2, #8
    streqh r3, [r1], #2
    moveq r3, #0
    subs r5, r5, #1
    bgt @copy_loop
@next:
    cmp r10, #0
    movgt r6, r6, lsl #1
    bgt @flag
    b @block
@end:
    ldmfd sp!, {r4-r10, lr}
    bx lr
}

asm void MI_UncompressHuffman(register const void *srcp, register void *destp) {
    stmfd sp!, {r4-r11, lr}
    sub sp, sp, #4
    add r2, r0, #4
    add r7, r2, #1
    ldrb r10, [r0, #0]
    and r4, r10, #0xf
    mov r3, #0
    mov lr, #0
    and r10, r4, #7
    add r11, r10, #4
    str r11, [sp, #0]
    ldr r10, [r0, #0]
    mov r12, r10, lsr #8
    ldrb r10, [r2, #0]
    add r10, r10, #1
    add r0, r2, r10, lsl #1
    mov r2, r7
@word:
    cmp r12, #0
    ble @end
    mov r8, #32
    ldr r5, [r0], #4
@bit:
    subs r8, r8, #1
    blt @word
    mov r10, #1
    and r9, r10, r5, lsr #31
    ldrb r6, [r2, #0]
    mov r6, r6, lsl r9
    mov r10, r2, lsr #1
    mov r10, r10, lsl #1
    ldrb r11, [r2, #0]
    and r11, r11, #0x3f
    add r11, r11, #1
    add r10, r10, r11, lsl #1
    add r2, r10, r9
    tst r6, #0x80
    beq @next
    mov r3, r3, lsr r4
    ldrb r10, [r2, #0]
    rsb r11, r4, #32
    orr r3, r3, r10, lsl r11
    mov r2, r7
    add lr, lr, #1
    ldr r11, [sp, #0]
    cmp lr, r11
    streq r3, [r1], #4
    subeq r12, r12, #4
    moveq lr, #0
@next:
    cmp r12, #0
    movgt r5, r5, lsl #1
    bgt @bit
    b @word
@end:
    add sp, sp, #4
    ldmfd sp!, {r4-r11, lr}
    bx lr
}

asm void MI_UncompressRL8(register const void *srcp, register void *destp) {
    stmfd sp!, {r4, r5, r7}
    ldmia r0!, {r3}
    mov r7, r3, lsr #8
@block:
    cmp r7, #0
    ble @end
    ldrb r4, [r0], #1
    ands r2, r4, #0x7f
    tst r4, #0x80
    bne @run
    add r2, r2, #1
    sub r7, r7, r2
@raw_loop:
    ldrb r3, [r0], #1
    swpb r3, r3, [r1]
    add r1, r1, #1
    subs r2, r2, #1
    bgt @raw_loop
    b @block
@run:
    add r2, r2, #3
    sub r7, r7, r2
    ldrb r5, [r0], #1
@run_loop:
    swpb r4, r5, [r1]
    add r1, r1, #1
    subs r2, r2, #1
    bgt @run_loop
    b @block
@end:
    ldmfd sp!, {r4, r5, r7}
    bx lr
}
#pragma thumb reset
