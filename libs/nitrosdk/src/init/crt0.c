// NitroSDK startup (crt0): the ARM entry point and its helpers, all assembly.

#include "types.h"

void init_cp15(void);
void INITi_CpuClear32(register u32 data, register void *dest, register u32 size);
void MIi_UncompressBackward(register void *bottom);
void do_autoload(void);
void AutoloadCallback(void);
void _fp_init(void);
void NitroStartUp(void);
void __call_static_initializers(void);

#pragma thumb off

asm void Entry(void) {
    mov ip, #0x4000000
    str ip, [ip, #0x208]
@2000808:
    ldrh r0, [ip, #6]
    cmp r0, #0
    bne @2000808
    bl init_cp15
    mov r0, #0x13
    msr cpsr_c, r0
    ldr r0, =0x27e0000
    add r0, r0, #0x3fc0
    mov sp, r0
    mov r0, #0x12
    msr cpsr_c, r0
    ldr r0, =0x27e0000
    add r0, r0, #0x3fc0
    sub r0, r0, #0x40
    sub sp, r0, #4
    tst sp, #4
    subeq sp, sp, #4
    ldr r1, =0x800
    sub r1, r0, r1
    mov r0, #0x1f
    msr cpsr_fsxc, r0
    sub sp, r1, #4
    mov r0, #0
    ldr r1, =0x27e0000
    mov r2, #0x4000
    bl INITi_CpuClear32
    mov r0, #0
    ldr r1, =0x5000000
    mov r2, #0x400
    bl INITi_CpuClear32
    mov r0, #0x200
    ldr r1, =0x7000000
    mov r2, #0x400
    bl INITi_CpuClear32
    ldr r1, =0x2000b64
    ldr r0, [r1, #0x14]
    bl MIi_UncompressBackward
    bl do_autoload
    ldr r0, =0x2000b64
    ldr r1, [r0, #0xc]
    ldr r2, [r0, #0x10]
    mov r3, r1
    mov r0, #0
@20008b4:
    cmp r1, r2
    strlo r0, [r1], #4
    blo @20008b4
    bic r1, r3, #0x1f
@20008c4:
    mcr p15, 0, r0, c7, c10, 4
    mcr p15, 0, r1, c7, c5, 1
    mcr p15, 0, r1, c7, c14, 1
    add r1, r1, #0x20
    cmp r1, r2
    blt @20008c4
    ldr r1, =0x27fff9c
    str r0, [r1]
    ldr r1, =0x27e0000
    add r1, r1, #0x3fc0
    add r1, r1, #0x3c
    ldr r0, =0x1ff94dc
    str r0, [r1]
    bl _fp_init
    blx NitroStartUp
    blx __call_static_initializers
    ldr r1, =0x2000ba1
    ldr lr, =0xffff0000
    tst sp, #4
    subne sp, sp, #4
    bx r1
}

asm void INITi_CpuClear32(register u32 data, register void *dest, register u32 size) {
    add ip, r1, r2
@2000940:
    cmp r1, ip
    stmltia r1!, {r0}
    blt @2000940
    bx lr
}

asm void MIi_UncompressBackward(register void *bottom) {
    cmp r0, #0
    beq @20009f8
    stmfd sp!, {r4, r5, r6, r7}
    ldmdb r0, {r1, r2}
    add r2, r0, r2
    sub r3, r0, r1, lsr #24
    bic r1, r1, #0xff000000
    sub r1, r0, r1
    mov r4, r2
@2000974:
    cmp r3, r1
    ble @20009d4
    ldrb r5, [r3, #-1]!
    mov r6, #8
@2000984:
    subs r6, r6, #1
    blt @2000974
    tst r5, #0x80
    bne @20009a0
    ldrb r0, [r3, #-1]!
    strb r0, [r2, #-1]!
    b @20009c8
@20009a0:
    ldrb ip, [r3, #-1]!
    ldrb r7, [r3, #-1]!
    orr r7, r7, ip, lsl #8
    bic r7, r7, #0xf000
    add r7, r7, #2
    add ip, ip, #0x20
@20009b8:
    ldrb r0, [r2, r7]
    strb r0, [r2, #-1]!
    subs ip, ip, #0x10
    bge @20009b8
@20009c8:
    cmp r3, r1
    mov r5, r5, lsl #1
    bgt @2000984
@20009d4:
    mov r0, #0
    bic r3, r1, #0x1f
@20009dc:
    mcr p15, 0, r0, c7, c10, 4
    mcr p15, 0, r3, c7, c5, 1
    mcr p15, 0, r3, c7, c14, 1
    add r3, r3, #0x20
    cmp r3, r4
    blt @20009dc
    ldmfd sp!, {r4, r5, r6, r7}
@20009f8:
    bx lr
}

asm void do_autoload(void) {
    ldr r0, =0x2000b64
    ldr r1, [r0]
    ldr r2, [r0, #4]
    ldr r3, [r0, #8]
@2000a0c:
    cmp r1, r2
    beq @2000a6c
    ldr r5, [r1], #4
    ldr r7, [r1], #4
    add r6, r5, r7
    mov r4, r5
@2000a24:
    cmp r4, r6
    ldrmi r7, [r3], #4
    strmi r7, [r4], #4
    bmi @2000a24
    ldr r7, [r1], #4
    add r6, r4, r7
    mov r7, #0
@2000a40:
    cmp r4, r6
    strlo r7, [r4], #4
    blo @2000a40
    bic r4, r5, #0x1f
@2000a50:
    mcr p15, 0, r7, c7, c10, 4
    mcr p15, 0, r4, c7, c5, 1
    mcr p15, 0, r4, c7, c14, 1
    add r4, r4, #0x20
    cmp r4, r6
    blt @2000a50
    b @2000a0c
@2000a6c:
    b AutoloadCallback
}

asm void AutoloadCallback(void) {
    bx lr
}

asm void init_cp15(void) {
    mrc p15, 0, r0, c1, c0, 0
    ldr r1, =0xf9005
    bic r0, r0, r1
    mcr p15, 0, r0, c1, c0, 0
    mov r0, #0
    mcr p15, 0, r0, c7, c5, 0
    mcr p15, 0, r0, c7, c6, 0
    mcr p15, 0, r0, c7, c10, 4
    ldr r0, =0x4000033
    mcr p15, 0, r0, c6, c0, 0
    ldr r0, =0x200002d
    mcr p15, 0, r0, c6, c1, 0
    ldr r0, =0x27e0021
    mcr p15, 0, r0, c6, c2, 0
    ldr r0, =0x8000035
    mcr p15, 0, r0, c6, c3, 0
    ldr r0, =0x27e0000
    orr r0, r0, #0x1a
    orr r0, r0, #1
    mcr p15, 0, r0, c6, c4, 0
    ldr r0, =0x100002f
    mcr p15, 0, r0, c6, c5, 0
    ldr r0, =0xffff001d
    mcr p15, 0, r0, c6, c6, 0
    ldr r0, =0x27ff017
    mcr p15, 0, r0, c6, c7, 0
    mov r0, #0x20
    mcr p15, 0, r0, c9, c1, 1
    ldr r0, =0x27e0000
    orr r0, r0, #0xa
    mcr p15, 0, r0, c9, c1, 0
    mov r0, #0x42
    mcr p15, 0, r0, c2, c0, 1
    mov r0, #0x42
    mcr p15, 0, r0, c2, c0, 0
    mov r0, #2
    mcr p15, 0, r0, c3, c0, 0
    ldr r0, =0x5100011
    mcr p15, 0, r0, c5, c0, 3
    ldr r0, =0x15111011
    mcr p15, 0, r0, c5, c0, 2
    mrc p15, 0, r0, c1, c0, 0
    ldr r1, =0x5707d
    orr r0, r0, r1
    mcr p15, 0, r0, c1, c0, 0
    bx lr
}

asm void OSi_ReferSymbol(void *symbol) {
    bx lr
}

#pragma thumb reset
