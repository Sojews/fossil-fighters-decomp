// NitroSDK MI: CPU memory copy and fill. The 16- and 32-bit forms need aligned sizes; the 8-bit ones handle any
// alignment with halfword accesses (VRAM can't take byte writes).

#include "nitro/mi.h"

#pragma thumb off
asm void MIi_CpuClear16(register u16 data, register void *destp, register u32 size) {
    mov r3, #0
@loop:
    cmp r3, r2
    strlth r0, [r1, r3]
    addlt r3, r3, #2
    blt @loop
    bx lr
}

asm void MIi_CpuCopy16(register const void *srcp, register void *destp, register u32 size) {
    mov r12, #0
@loop:
    cmp r12, r2
    ldrlth r3, [r0, r12]
    strlth r3, [r1, r12]
    addlt r12, r12, #2
    blt @loop
    bx lr
}

asm void MIi_CpuClear32(register u32 data, register void *destp, register u32 size) {
    add r12, r1, r2
@loop:
    cmp r1, r12
    stmltia r1!, {r0}
    blt @loop
    bx lr
}

asm void MIi_CpuCopy32(register const void *srcp, register void *destp, register u32 size) {
    add r12, r1, r2
@loop:
    cmp r1, r12
    ldmltia r0!, {r2}
    stmltia r1!, {r2}
    blt @loop
    bx lr
}

asm void MIi_CpuClearFast(register u32 data, register void *destp, register u32 size) {
    stmfd sp!, {r4-r9}
    add r9, r1, r2
    mov r12, r2, lsr #5
    add r12, r1, r12, lsl #5
    mov r2, r0
    mov r3, r2
    mov r4, r2
    mov r5, r2
    mov r6, r2
    mov r7, r2
    mov r8, r2
@loop32:
    cmp r1, r12
    stmltia r1!, {r0, r2-r8}
    blt @loop32
@loop4:
    cmp r1, r9
    stmltia r1!, {r0}
    blt @loop4
    ldmfd sp!, {r4-r9}
    bx lr
}

asm void MIi_CpuCopyFast(register const void *srcp, register void *destp, register u32 size) {
    stmfd sp!, {r4-r10}
    add r10, r1, r2
    mov r12, r2, lsr #5
    add r12, r1, r12, lsl #5
@loop32:
    cmp r1, r12
    ldmltia r0!, {r2-r9}
    stmltia r1!, {r2-r9}
    blt @loop32
@loop4:
    cmp r1, r10
    ldmltia r0!, {r2}
    stmltia r1!, {r2}
    blt @loop4
    ldmfd sp!, {r4-r10}
    bx lr
}

asm void MI_Copy16B(register const void *pSrc, register void *pDest) {
    ldmia r0!, {r2, r3, r12}
    stmia r1!, {r2, r3, r12}
    ldmia r0!, {r2}
    stmia r1!, {r2}
    bx lr
}

asm void MI_Copy32B(register const void *pSrc, register void *pDest) {
    ldmia r0!, {r2, r3, r12}
    stmia r1!, {r2, r3, r12}
    ldmia r0!, {r2, r3, r12}
    stmia r1!, {r2, r3, r12}
    ldmia r0!, {r2, r3}
    stmia r1!, {r2, r3}
    bx lr
}

asm void MI_Copy36B(register const void *pSrc, register void *pDest) {
    ldmia r0!, {r2, r3, r12}
    stmia r1!, {r2, r3, r12}
    ldmia r0!, {r2, r3, r12}
    stmia r1!, {r2, r3, r12}
    ldmia r0!, {r2, r3, r12}
    stmia r1!, {r2, r3, r12}
    bx lr
}

asm void MI_Copy48B(register const void *pSrc, register void *pDest) {
    ldmia r0!, {r2, r3, r12}
    stmia r1!, {r2, r3, r12}
    ldmia r0!, {r2, r3, r12}
    stmia r1!, {r2, r3, r12}
    ldmia r0!, {r2, r3, r12}
    stmia r1!, {r2, r3, r12}
    ldmia r0!, {r2, r3, r12}
    stmia r1!, {r2, r3, r12}
    bx lr
}

asm void MI_Copy64B(register const void *pSrc, register void *pDest) {
    ldmia r0!, {r2, r3, r12}
    stmia r1!, {r2, r3, r12}
    ldmia r0!, {r2, r3, r12}
    stmia r1!, {r2, r3, r12}
    ldmia r0!, {r2, r3, r12}
    stmia r1!, {r2, r3, r12}
    ldmia r0!, {r2, r3, r12}
    stmia r1!, {r2, r3, r12}
    ldmia r0, {r0, r2, r3, r12}
    stmia r1!, {r0, r2, r3, r12}
    bx lr
}

asm void MI_CpuFill8(register void *dstp, register u8 data, register u32 size) {
    cmp r2, #0
    bxeq lr
    // An odd start: merge into the halfword below.
    tst r0, #1
    beq @aligned2
    ldrh r12, [r0, #-1]
    and r12, r12, #0xff
    orr r3, r12, r1, lsl #8
    strh r3, [r0, #-1]
    add r0, r0, #1
    subs r2, r2, #1
    bxeq lr
@aligned2:
    cmp r2, #2
    bcc @tail1
    orr r1, r1, r1, lsl #8
    tst r0, #2
    beq @aligned4
    strh r1, [r0], #2
    subs r2, r2, #2
    bxeq lr
@aligned4:
    orr r1, r1, r1, lsl #16
    bics r3, r2, #3
    beq @tail2
    sub r2, r2, r3
    add r12, r3, r0
@loop:
    str r1, [r0], #4
    cmp r0, r12
    bcc @loop
@tail2:
    tst r2, #2
    strneh r1, [r0], #2
@tail1:
    tst r2, #1
    bxeq lr
    ldrh r3, [r0]
    and r3, r3, #0xff00
    and r1, r1, #0xff
    orr r1, r1, r3
    strh r1, [r0]
    bx lr
}

asm void MI_CpuCopy8(register const void *srcp, register void *dstp, register u32 size) {
    cmp r2, #0
    bxeq lr
    // An odd destination: merge one byte into the halfword below it.
    tst r1, #1
    beq @dst_even
    ldrh r12, [r1, #-1]
    and r12, r12, #0xff
    tst r0, #1
    ldrneh r3, [r0, #-1]
    movne r3, r3, lsr #8
    ldreqh r3, [r0]
    orr r3, r12, r3, lsl #8
    strh r3, [r1, #-1]
    add r0, r0, #1
    add r1, r1, #1
    subs r2, r2, #1
    bxeq lr
@dst_even:
    eor r12, r1, r0
    tst r12, #1
    beq @same_parity
    // Source odd, destination even: shift bytes across halfwords.
    bic r0, r0, #1
    ldrh r12, [r0], #2
    mov r3, r12, lsr #8
    subs r2, r2, #2
    bcc @odd_tail
@odd_loop:
    ldrh r12, [r0], #2
    orr r12, r3, r12, lsl #8
    strh r12, [r1], #2
    mov r3, r12, lsr #16
    subs r2, r2, #2
    bcs @odd_loop
@odd_tail:
    tst r2, #1
    bxeq lr
    ldrh r12, [r1]
    and r12, r12, #0xff00
    orr r12, r12, r3
    strh r12, [r1]
    bx lr
@same_parity:
    tst r12, #2
    beq @same_word
    bics r3, r2, #1
    beq @tail1
    sub r2, r2, r3
    add r12, r3, r1
@half_loop:
    ldrh r3, [r0], #2
    strh r3, [r1], #2
    cmp r1, r12
    bcc @half_loop
    b @tail1
@same_word:
    cmp r2, #2
    bcc @tail1
    tst r1, #2
    beq @word
    ldrh r3, [r0], #2
    strh r3, [r1], #2
    subs r2, r2, #2
    bxeq lr
@word:
    bics r3, r2, #3
    beq @tail2
    sub r2, r2, r3
    add r12, r3, r1
@word_loop:
    ldr r3, [r0], #4
    str r3, [r1], #4
    cmp r1, r12
    bcc @word_loop
@tail2:
    tst r2, #2
    ldrneh r3, [r0], #2
    strneh r3, [r1], #2
@tail1:
    tst r2, #1
    bxeq lr
    ldrh r2, [r1]
    ldrh r0, [r0]
    and r2, r2, #0xff00
    and r0, r0, #0xff
    orr r0, r2, r0
    strh r0, [r1]
    bx lr
}

asm u32 MI_SwapWord(register u32 setData, register vu32 *destp) {
    swp r0, r0, [r1]
    bx lr
}
#pragma thumb reset
