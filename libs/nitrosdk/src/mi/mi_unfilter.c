// NitroSDK MI: undoing the BIOS difference filter (8- or 16-bit deltas).

#include "nitro/mi.h"

#pragma thumb off
asm void MI_UnfilterDiff8(register const void *srcp, register void *destp) {
    stmfd sp!, {r4}
    ldmia r0, {r2}
    mov r3, #0
    and r4, r2, #0xf
    mov r2, r2, lsr #8
    cmp r4, #1
    bne @diff16
    add r0, r0, #3
    sub r1, r1, #1
@loop8:
    ldrb r4, [r0, #1]!
    subs r2, r2, #1
    add r3, r3, r4
    strb r3, [r1, #1]!
    bgt @loop8
    b @end
@diff16:
    add r0, r0, #2
    sub r1, r1, #2
@loop16:
    ldrh r4, [r0, #2]!
    subs r2, r2, #2
    add r3, r3, r4
    strh r3, [r1, #2]!
    bgt @loop16
@end:
    ldmfd sp!, {r4}
    bx lr
}

// The same, writing 16 bits at a time (for VRAM).
asm void MI_UnfilterDiff16(register const void *srcp, register void *destp) {
    stmfd sp!, {r4, r5}
    ldmia r0, {r2}
    mov r3, #0
    and r4, r2, #0xf
    mov r2, r2, lsr #8
    cmp r4, #1
    bne @diff16
    add r0, r0, #2
    sub r1, r1, #2
@loop8:
    ldrh r4, [r0, #2]!
    sub r2, r2, #2
    add r3, r3, r4
    and r5, r3, #0xff
    add r3, r3, r4, lsr #8
    add r5, r5, r3, lsl #8
    strh r5, [r1, #2]!
    cmp r2, #1
    bgt @loop8
    bne @end
    ldrh r4, [r0, #2]!
    add r3, r3, r4
    and r5, r3, #0xff
    strh r5, [r1, #2]!
    b @end
@diff16:
    add r0, r0, #2
    sub r1, r1, #2
@loop16:
    ldrh r4, [r0, #2]!
    subs r2, r2, #2
    add r3, r3, r4
    strh r3, [r1, #2]!
    bgt @loop16
@end:
    ldmfd sp!, {r4, r5}
    bx lr
}
#pragma thumb reset
