// NitroSDK MI: run-length decompression written 16 bits at a time (for VRAM), in assembly.

#include "nitro/mi.h"

asm void MI_UncompressRL16(register const void *srcp, register void *destp) {
    push {r4, r5, r6, r7}
    sub sp, #0xc
    mov r7, #0
    ldmia r0!, {r3}
    lsr r5, r3, #8
    mov r4, #0
@block:
    cmp r5, #0
    ble @end
    ldrb r3, [r0, #0]
    str r3, [sp, #4]
    add r0, #1
    ldr r3, [sp, #4]
    lsl r2, r3, #0x19
    lsr r2, r2, #0x19
    ldr r6, [sp, #4]
    lsr r3, r6, #8
    bcs @run
    add r2, #1
    sub r5, r5, r2
@copy:
    ldrb r6, [r0, #0]
    lsl r6, r4
    orr r7, r6
    add r0, #1
    mov r3, #8
    eor r4, r3
    bne @copy_next
    strh r7, [r1, #0]
    add r1, #2
    mov r7, #0
@copy_next:
    sub r2, r2, #1
    bgt @copy
    b @block
@run:
    add r2, #3
    sub r5, r5, r2
    ldrb r6, [r0, #0]
    str r6, [sp, #8]
    add r0, #1
@run_loop:
    ldr r6, [sp, #8]
    lsl r6, r4
    orr r7, r6
    mov r3, #8
    eor r4, r3
    bne @run_next
    strh r7, [r1, #0]
    add r1, #2
    mov r7, #0
@run_next:
    sub r2, r2, #1
    bgt @run_loop
    b @block
@end:
    add sp, #0xc
    pop {r4, r5, r6, r7}
    bx lr
}
