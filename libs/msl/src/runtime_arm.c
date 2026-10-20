// Metrowerks runtime: software floating point and 64-bit integer helpers.
// Hand-written ARM assembly in the original library. The reciprocal tables and the
// branches between entry points (which mwcc inline asm cannot name) are raw words.

#include "types.h"
#include <errno.h>

void _dadd(void);
void func_020847c8(void);
void _dfix(void);
void _ll_ufrom_d(void);
void _dflt(void);
void _dfltu(void);
void _dmul(void);
void sqrt(void);
void func_02084f20(void);
void _dsub(void);
void func_020852ec(void);
void func_02085508(void);
void _dgr(void);
void _dleq(void);
void _dls(void);
void _deq(void);
void _dneq(void);
void func_02085800(void);
void func_0208585c(void);
void func_02085868(void);
void _f2d(void);
void func_02085ca4(void);
void func_02085cd8(void);
void func_02085d20(void);
void func_02085f00(void);
void func_02085f0c(void);
void func_02086184(void);
void _ll_sdiv(void);
void _ll_udiv(void);
void _ull_mod(void);
void _s32_div_f(void);
void _u32_div_f(void);
void func_020865c0(void);
void func_0208679c(void);
void _ddiv(void);
void _fp_init(void);

// Some entry points are only reached by falling through from the function before.
#pragma force_active on
#pragma thumb off

asm void _dadd(void) {
    stmfd sp!, {r4, lr}
    eors ip, r1, r3
    eormi r3, r3, #0x80000000
    dcd 0x4a0002a1 // bmi 0x2084f48
    subs ip, r0, r2
    sbcs lr, r1, r3
    bhs @20844dc
    adds r2, r2, ip
    adc r3, r3, lr
    subs r0, r0, ip
    sbc r1, r1, lr
@20844dc:
    mov lr, #0x80000000
    mov ip, r1, lsr #0x14
    orr r1, lr, r1, lsl #11
    orr r1, r1, r0, lsr #21
    mov r0, r0, lsl #0xb
    movs r4, ip, lsl #0x15
    cmnne r4, #0x200000
    beq @20845d8
    mov r4, r3, lsr #0x14
    orr r3, lr, r3, lsl #11
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #0xb
    movs lr, r4, lsl #0x15
    beq @2084620
@2084514:
    subs r4, ip, r4
    beq @208456c
    cmp r4, #0x20
    ble @2084550
    cmp r4, #0x38
    movge r4, #0x3f
    sub r4, r4, #0x20
    rsb lr, r4, #0x20
    orrs lr, r2, r3, lsl lr
    mov r2, r3, lsr r4
    orrne r2, r2, #1
    adds r0, r0, r2
    adcs r1, r1, #0
    blo @2084594
    b @2084578
@2084550:
    rsb lr, r4, #0x20
    movs lr, r2, lsl lr
    rsb lr, r4, #0x20
    mov r2, r2, lsr r4
    orr r2, r2, r3, lsl lr
    mov r3, r3, lsr r4
    orrne r2, r2, #1
@208456c:
    adds r0, r0, r2
    adcs r1, r1, r3
    blo @2084594
@2084578:
    add ip, ip, #1
    and r4, r0, #1
    movs r1, r1, rrx
    orr r0, r4, r0, rrx
    mov lr, ip, lsl #0x15
    cmn lr, #0x200000
    beq @20847a4
@2084594:
    movs r2, r0, lsl #0x15
    mov r0, r0, lsr #0xb
    orr r0, r0, r1, lsl #21
    add r1, r1, r1
    mov r1, r1, lsr #0xc
    orr r1, r1, ip, lsl #20
    tst r2, #0x80000000
    ldmeqfd sp!, {r4, lr}
    bxeq lr
    movs r2, r2, lsl #1
    andeqs r2, r0, #1
    ldmeqfd sp!, {r4, lr}
    bxeq lr
    adds r0, r0, #1
    adc r1, r1, #0
    ldmfd sp!, {r4, lr}
    bx lr
@20845d8:
    cmp ip, #0x800
    movge lr, #0x80000000
    movlt lr, #0
    bics ip, ip, #0x800
    beq @2084644
    orrs r4, r0, r1, lsl #1
    bne @2084780
    mov r4, r3, lsr #0x14
    mov r3, r3, lsl #0xb
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #0xb
    movs r4, r4, lsl #0x15
    beq @208476c
    cmn r4, #0x200000
    bne @208476c
    orrs r4, r2, r3, lsl #1
    beq @208476c
    b @2084780
@2084620:
    cmp r4, #0x800
    movge lr, #0x80000000
    movlt lr, #0
    bic ip, ip, #0x800
    bics r4, r4, #0x800
    beq @20846b0
    orrs r4, r2, r3, lsl #1
    bne @2084780
    b @208476c
@2084644:
    orrs r4, r0, r1, lsl #1
    beq @2084684
    mov ip, #1
    bic r1, r1, #0x80000000
    mov r4, r3, lsr #0x14
    mov r3, r3, lsl #0xb
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #0xb
    movs r4, r4, lsl #0x15
    cmnne r4, #0x200000
    mov r4, r4, lsr #0x15
    orr r4, r4, lr, lsr #20
    beq @2084620
    orr r3, r3, #0x80000000
    orr ip, ip, lr, lsr #20
    b @2084514
@2084684:
    mov ip, r3, lsr #0x14
    mov r1, r3, lsl #0xb
    orr r1, r1, r2, lsr #21
    mov r0, r2, lsl #0xb
    movs r4, ip, lsl #0x15
    beq @2084738
    cmn r4, #0x200000
    bne @2084738
    orrs r4, r0, r1, lsl #1
    beq @208476c
    b @2084784
@20846b0:
    orrs r4, r2, r3, lsl #1
    beq @2084748
    mov r4, #1
    bic r3, r3, #0x80000000
    cmp r1, #0
    bpl @20846d4
    orr ip, ip, lr, lsr #20
    orr r4, r4, lr, lsr #20
    b @2084514
@20846d4:
    adds r0, r0, r2
    adcs r1, r1, r3
    blo @20846f4
    add ip, ip, #1
    and r4, r0, #1
    movs r1, r1, rrx
    mov r0, r0, rrx
    orr r0, r0, r4
@20846f4:
    cmp r1, #0
    subges ip, ip, #1
    movs r2, r0, lsl #0x15
    mov r0, r0, lsr #0xb
    orr r0, r0, r1, lsl #21
    add r1, r1, r1
    orr r1, lr, r1, lsr #12
    orr r1, r1, ip, lsl #20
    ldmeqfd sp!, {r4, lr}
    bxeq lr
    tst r2, #0x80000000
    ldmeqfd sp!, {r4, lr}
    bxeq lr
    movs r2, r2, lsl #1
    andeqs r2, r0, #1
    ldmeqfd sp!, {r4, lr}
    bxeq lr
@2084738:
    mov r1, r3
    mov r0, r2
    ldmfd sp!, {r4, lr}
    bx lr
@2084748:
    cmp r1, #0
    subges ip, ip, #1
    mov r0, r0, lsr #0xb
    orr r0, r0, r1, lsl #21
    add r1, r1, r1
    orr r1, lr, r1, lsr #12
    orr r1, r1, ip, lsl #20
    ldmfd sp!, {r4, lr}
    bx lr
@208476c:
    ldr r1, =0x7ff00000
    orr r1, lr, r1
    mov r0, #0
    ldmfd sp!, {r4, lr}
    bx lr
@2084780:
    mov r1, r3
@2084784:
    mvn r0, #0
    bic r1, r0, #0x80000000
    ldmfd sp!, {r4, lr}
    bx lr
    mvn r0, #0
    bic r1, r0, #0x80000000
    ldmfd sp!, {r4, lr}
    bx lr
@20847a4:
    cmp ip, #0x800
    movge lr, #0x80000000
    movlt lr, #0
    ldr r1, =0x7ff00000
    orr r1, lr, r1
    mov r0, #0
    ldmfd sp!, {r4, lr}
    bx lr
}

asm void func_020847c8(void) {
    and r2, r1, #0x80000000
    mov ip, r1, lsr #0x14
    bics ip, ip, #0x800
    beq @2084840
    mov r3, ip, lsl #0x15
    cmn r3, #0x200000
    bhs @2084824
    subs ip, ip, #0x380
    bls @2084850
    cmp ip, #0xff
    bge @20848c0
    mov r1, r1, lsl #0xc
    orr r3, r2, r1, lsr #9
    orr r3, r3, r0, lsr #29
    movs r1, r0, lsl #3
    orr r0, r3, ip, lsl #23
    bxeq lr
    tst r1, #0x80000000
    bxeq lr
    movs r1, r1, lsl #1
    andeqs r1, r0, #1
    addne r0, r0, #1
    bx lr
@2084824:
    orrs r3, r0, r1, lsl #12
    bne @2084838
    mov r0, #0xff000000
    orr r0, r2, r0, lsr #1
    bx lr
@2084838:
    mvn r0, #0x80000000
    bx lr
@2084840:
    orrs r3, r0, r1, lsl #12
    bne @20848b8
    mov r0, r2
    bx lr
@2084850:
    cmn ip, #0x17
    beq @20848a4
    bmi @20848b8
    mov r1, r1, lsl #0xb
    orr r1, r1, #0x80000000
    mov r3, r1, lsr #8
    orr r3, r3, r0, lsr #29
    rsb ip, ip, #1
    movs r1, r0, lsl #3
    orr r0, r2, r3, lsr ip
    rsb ip, ip, #0x20
    mov r3, r3, lsl ip
    orrne r3, r3, #1
    movs r1, r3
    bxeq lr
    tst r1, #0x80000000
    bxeq lr
    movs r1, r1, lsl #1
    andeqs r1, r0, #1
    addne r0, r0, #1
    bx lr
@20848a4:
    orr r0, r0, r1, lsl #12
    movs r1, r0
    mov r0, r2
    addne r0, r0, #1
    bx lr
@20848b8:
    mov r0, r2
    bx lr
@20848c0:
    mov r0, #0xff000000
    orr r0, r2, r0, lsr #1
    bx lr
}

asm void _dfix(void) {
    bic r3, r1, #0x80000000
    ldr r2, =0x41e
    subs r2, r2, r3, lsr #20
    ble @2084908
    cmp r2, #0x20
    bge @2084900
    mov r3, r1, lsl #0xb
    orr r3, r3, #0x80000000
    orr r3, r3, r0, lsr #21
    cmp r1, #0
    mov r0, r3, lsr r2
    rsbmi r0, r0, #0
    bx lr
@2084900:
    mov r0, #0
    bx lr
@2084908:
    mvn r0, r1, asr #31
    add r0, r0, #0x80000000
    bx lr
}

asm void _ll_ufrom_d(void) {
    tst r1, #0x80000000
    bne @208497c
    ldr r2, =0x43e
    subs r2, r2, r1, lsr #20
    blt @2084994
    cmp r2, #0x40
    bge @2084970
    mov ip, r1, lsl #0xb
    orr ip, ip, #0x80000000
    orr ip, ip, r0, lsr #21
    cmp r2, #0x20
    ble @2084958
    sub r2, r2, #0x20
    mov r1, #0
    mov r0, ip, lsr r2
    bx lr
@2084958:
    mov r3, r0, lsl #0xb
    mov r1, ip, lsr r2
    mov r0, r3, lsr r2
    rsb r2, r2, #0x20
    orr r0, r0, ip, lsl r2
    bx lr
@2084970:
    mov r1, #0
    mov r0, #0
    bx lr
@208497c:
    cmn r1, #0x100000
    cmpeq r0, #0
    bhi @2084994
    mov r1, #0
    mov r0, #0
    bx lr
@2084994:
    mvn r1, #0
    mvn r0, #0
    bx lr
}

asm void _dflt(void) {
    ands r2, r0, #0x80000000
    rsbmi r0, r0, #0
    cmp r0, #0
    mov r1, #0
    bxeq lr
    mov r3, #0x400
    add r3, r3, #0x1e
    clz ip, r0
    movs r0, r0, lsl ip
    sub r3, r3, ip
    movs r1, r0
    mov r0, r1, lsl #0x15
    add r1, r1, r1
    orr r1, r2, r1, lsr #12
    orr r1, r1, r3, lsl #20
    bx lr
}

asm void _dfltu(void) {
    cmp r0, #0
    mov r1, #0
    bxeq lr
    mov r3, #0x400
    add r3, r3, #0x1e
    bmi @2084a08
    clz ip, r0
    movs r0, r0, lsl ip
    sub r3, r3, ip
@2084a08:
    mov r1, r0
    mov r0, r1, lsl #0x15
    add r1, r1, r1
    mov r1, r1, lsr #0xc
    orr r1, r1, r3, lsl #20
    bx lr
}

asm void _dmul(void) {
    stmfd sp!, {r4, r5, r6, r7, lr}
    eor lr, r1, r3
    and lr, lr, #0x80000000
    mov ip, r1, lsr #0x14
    mov r1, r1, lsl #0xb
    orr r1, r1, r0, lsr #21
    mov r0, r0, lsl #0xb
    movs r6, ip, lsl #0x15
    cmnne r6, #0x200000
    beq @2084b28
    orr r1, r1, #0x80000000
    bic ip, ip, #0x800
    mov r4, r3, lsr #0x14
    mov r3, r3, lsl #0xb
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #0xb
    movs r5, r4, lsl #0x15
    cmnne r5, #0x200000
    beq @2084b70
    orr r3, r3, #0x80000000
    bic r4, r4, #0x800
@2084a74:
    add ip, r4, ip
    umull r5, r4, r0, r2
    umull r7, r6, r0, r3
    adds r4, r7, r4
    adc r6, r6, #0
    umull r7, r0, r1, r2
    adds r4, r7, r4
    adcs r0, r0, r6
    umull r7, r2, r1, r3
    adc r1, r2, #0
    adds r0, r0, r7
    adc r1, r1, #0
    orrs r4, r4, r5
    orrne r0, r0, #1
    cmp r1, #0
    blt @2084ac0
    sub ip, ip, #1
    adds r0, r0, r0
    adc r1, r1, r1
@2084ac0:
    add ip, ip, #2
    subs ip, ip, #0x400
    bmi @2084c5c
    beq @2084c5c
    mov r6, ip, lsl #0x14
    cmn r6, #0x100000
    bmi @2084d5c
    movs r2, r0, lsl #0x15
    mov r0, r0, lsr #0xb
    orr r0, r0, r1, lsl #21
    add r1, r1, r1
    orr r1, lr, r1, lsr #12
    orr r1, r1, ip, lsl #20
    ldmeqfd sp!, {r4, r5, r6, r7, lr}
    bxeq lr
    tst r2, #0x80000000
    ldmeqfd sp!, {r4, r5, r6, r7, lr}
    bxeq lr
    movs r2, r2, lsl #1
    andeqs r2, r0, #1
    ldmeqfd sp!, {r4, r5, r6, r7, lr}
    bxeq lr
    adds r0, r0, #1
    adc r1, r1, #0
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
@2084b28:
    bics ip, ip, #0x800
    beq @2084b84
    orrs r6, r0, r1, lsl #1
    bne @2084d10
    mov r4, r3, lsr #0x14
    mov r3, r3, lsl #0xb
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #0xb
    movs r5, r4, lsl #0x15
    beq @2084b64
    cmn r5, #0x200000
    bne @2084cfc
    orrs r5, r2, r3, lsl #1
    beq @2084cfc
    b @2084d10
@2084b64:
    orrs r5, r3, r2
    beq @2084d24
    b @2084cfc
@2084b70:
    bics r4, r4, #0x800
    beq @2084c18
    orrs r6, r2, r3, lsl #1
    bne @2084d10
    b @2084cfc
@2084b84:
    orrs r6, r0, r1, lsl #1
    beq @2084bec
    mov ip, #1
    cmp r1, #0
    bne @2084ba8
    sub ip, ip, #0x20
    movs r1, r0
    mov r0, #0
    bmi @2084bc4
@2084ba8:
    clz r6, r1
    movs r1, r1, lsl r6
    rsb r6, r6, #0x20
    orr r1, r1, r0, lsr r6
    rsb r6, r6, #0x20
    mov r0, r0, lsl r6
    sub ip, ip, r6
@2084bc4:
    mov r4, r3, lsr #0x14
    mov r3, r3, lsl #0xb
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #0xb
    movs r5, r4, lsl #0x15
    cmnne r5, #0x200000
    beq @2084b70
    orr r3, r3, #0x80000000
    bic r4, r4, #0x800
    b @2084a74
@2084bec:
    mov r4, r3, lsr #0x14
    mov r3, r3, lsl #0xb
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #0xb
    movs r5, r4, lsl #0x15
    beq @2084d70
    cmn r5, #0x200000
    bne @2084d70
    orrs r6, r2, r3, lsl #1
    beq @2084d24
    b @2084d10
@2084c18:
    orrs r5, r2, r3, lsl #1
    beq @2084d70
    mov r4, #1
    cmp r3, #0
    bne @2084c3c
    sub r4, r4, #0x20
    movs r3, r2
    mov r2, #0
    bmi @2084a74
@2084c3c:
    clz r6, r3
    movs r3, r3, lsl r6
    rsb r6, r6, #0x20
    orr r3, r3, r2, lsr r6
    rsb r6, r6, #0x20
    mov r2, r2, lsl r6
    sub r4, r4, r6
    b @2084a74
@2084c5c:
    cmn ip, #0x34
    beq @2084cf4
    bmi @2084d4c
    mov r2, r1
    mov r3, r0
    add r4, ip, #0x34
    cmp r4, #0x20
    movge r2, r3
    movge r3, #0
    subge r4, r4, #0x20
    rsb r5, r4, #0x20
    mov r2, r2, lsl r4
    orr r2, r2, r3, lsr r5
    movs r3, r3, lsl r4
    orrne r2, r2, #1
    rsb ip, ip, #0xc
    cmp ip, #0x20
    movge r0, r1
    movge r1, #0
    subge ip, ip, #0x20
    rsb r4, ip, #0x20
    mov r0, r0, lsr ip
    orr r0, r0, r1, lsl r4
    orr r1, lr, r1, lsr ip
    cmp r2, #0
    ldmeqfd sp!, {r4, r5, r6, r7, lr}
    bxeq lr
    tst r2, #0x80000000
    ldmeqfd sp!, {r4, r5, r6, r7, lr}
    bxeq lr
    movs r2, r2, lsl #1
    andeqs r2, r0, #1
    ldmeqfd sp!, {r4, r5, r6, r7, lr}
    bxeq lr
    adds r0, r0, #1
    adc r1, r1, #0
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
@2084cf4:
    orr r0, r0, r1, lsl #1
    b @2084d34
@2084cfc:
    ldr r1, =0x7ff00000
    orr r1, lr, r1
    mov r0, #0
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
@2084d10:
    mov r1, r3
    mvn r0, #0
    bic r1, r0, #0x80000000
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
@2084d24:
    mvn r0, #0
    bic r1, r0, #0x80000000
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
@2084d34:
    movs r2, r0
    mov r1, lr
    mov r0, #0
    addne r0, r0, #1
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
@2084d4c:
    mov r1, lr
    mov r0, #0
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
@2084d5c:
    ldr r1, =0x7ff00000
    orr r1, lr, r1
    mov r0, #0
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
@2084d70:
    mov r1, lr
    mov r0, #0
    ldmfd sp!, {r4, r5, r6, r7, lr}
    bx lr
}

asm void sqrt(void) {
    stmfd sp!, {r4, r5, r6, lr}
    ldr r2, =0x7ff00000
    cmp r1, r2
    bhs @2084ed0
    movs ip, r1, lsr #0x14
    beq @2084e7c
    bic r1, r1, r2
    orr r1, r1, #0x100000
@2084da4:
    movs ip, ip, asr #1
    bhs @2084db8
    sub ip, ip, #1
    movs r0, r0, lsl #1
    adc r1, r1, r1
@2084db8:
    movs r3, r0, lsl #1
    adc r1, r1, r1
    mov r2, #0
    mov r4, #0
    mov lr, #0x200000
@2084dcc:
    add r6, r4, lr
    cmp r6, r1
    addle r4, r6, lr
    suble r1, r1, r6
    addle r2, r2, lr
    movs r3, r3, lsl #1
    adc r1, r1, r1
    movs lr, lr, lsr #1
    bne @2084dcc
    mov r0, #0
    mov r5, #0
    cmp r1, r4
    cmpeq r3, #0x80000000
    blo @2084e14
    subs r3, r3, #0x80000000
    sbc r1, r1, r4
    add r4, r4, #1
    mov r0, #0x80000000
@2084e14:
    movs r3, r3, lsl #1
    adc r1, r1, r1
    mov lr, #0x40000000
@2084e20:
    add r6, r5, lr
    cmp r4, r1
    cmpeq r6, r3
    bhi @2084e40
    add r5, r6, lr
    subs r3, r3, r6
    sbc r1, r1, r4
    add r0, r0, lr
@2084e40:
    movs r3, r3, lsl #1
    adc r1, r1, r1
    movs lr, lr, lsr #1
    bne @2084e20
    orrs r1, r1, r3
    biceq r0, r0, #1
    movs r1, r2, lsr #1
    movs r0, r0, rrx
    adcs r0, r0, #0
    adc r1, r1, #0
    add r1, r1, #0x20000000
    sub r1, r1, #0x100000
    add r1, r1, ip, lsl #20
    ldmfd sp!, {r4, r5, r6, lr}
    bx lr
@2084e7c:
    cmp r1, #0
    bne @2084eac
    cmp r0, #0
    ldmeqfd sp!, {r4, r5, r6, lr}
    bxeq lr
    mvn ip, #0x13
    clz r5, r0
    movs r0, r0, lsl r5
    sub ip, ip, r5
    mov r1, r0, lsr #0xb
    mov r0, r0, lsl #0x15
    b @2084da4
@2084eac:
    clz r2, r1
    movs r1, r1, lsl r2
    rsb r2, r2, #0x2b
    mov r1, r1, lsr #0xb
    orr r1, r1, r0, lsr r2
    rsb r2, r2, #0x20
    mov r0, r0, lsl r2
    rsb ip, r2, #1
    b @2084da4
@2084ed0:
    tst r1, #0x80000000
    beq @2084eec
    bics r3, r1, #0x80000000
    cmpeq r0, #0
    ldmeqfd sp!, {r4, r5, r6, lr}
    bxeq lr
    b @2084ef8
@2084eec:
    orrs r2, r0, r1, lsl #12
    ldmeqfd sp!, {r4, r5, r6, lr}
    bxeq lr
@2084ef8:
    ldr r2, =0x7ff80000
    orr r1, r1, r2
    ldr r3, =errno
    mov r4, #0x21
    str r4, [r3]
    ldmfd sp!, {r4, r5, r6, lr}
    bx lr
}

asm void func_02084f20(void) {
    eor r1, r1, r3
    eor r3, r1, r3
    eor r1, r1, r3
    eor r0, r0, r2
    eor r2, r0, r2
    eor r0, r0, r2
}

asm void _dsub(void) {
    stmfd sp!, {r4, lr}
    eors ip, r1, r3
    eormi r3, r3, #0x80000000
    dcd 0x4afffd5d // bmi 0x20844c0
    subs ip, r0, r2
    sbcs lr, r1, r3
    bhs @2084f68
    eor lr, lr, #0x80000000
    adds r2, r2, ip
    adc r3, r3, lr
    subs r0, r0, ip
    sbc r1, r1, lr
@2084f68:
    mov lr, #0x80000000
    mov ip, r1, lsr #0x14
    orr r1, lr, r1, lsl #11
    orr r1, r1, r0, lsr #21
    mov r0, r0, lsl #0xb
    movs r4, ip, lsl #0x15
    cmnne r4, #0x200000
    beq @208516c
    mov r4, r3, lsr #0x14
    orr r3, lr, r3, lsl #11
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #0xb
    movs lr, r4, lsl #0x15
    beq @20851b4
@2084fa0:
    subs r4, ip, r4
    beq @2085048
    cmp r4, #0x20
    ble @2084fdc
    cmp r4, #0x38
    movge r4, #0x3f
    sub r4, r4, #0x20
    rsb lr, r4, #0x20
    orrs lr, r2, r3, lsl lr
    mov r2, r3, lsr r4
    orrne r2, r2, #1
    subs r0, r0, r2
    sbcs r1, r1, #0
    bmi @2085004
    b @20850f4
@2084fdc:
    rsb lr, r4, #0x20
    movs lr, r2, lsl lr
    rsb lr, r4, #0x20
    mov r2, r2, lsr r4
    orr r2, r2, r3, lsl lr
    mov r3, r3, lsr r4
    orrne r2, r2, #1
    subs r0, r0, r2
    sbcs r1, r1, r3
    bpl @20850f4
@2085004:
    movs r2, r0, lsl #0x15
    mov r0, r0, lsr #0xb
    orr r0, r0, r1, lsl #21
    add r1, r1, r1
    mov r1, r1, lsr #0xc
    orr r1, r1, ip, lsl #20
    tst r2, #0x80000000
    ldmeqfd sp!, {r4, lr}
    bxeq lr
    movs r2, r2, lsl #1
    andeqs r2, r0, #1
    ldmeqfd sp!, {r4, lr}
    bxeq lr
    adds r0, r0, #1
    adc r1, r1, #0
    ldmfd sp!, {r4, lr}
    bx lr
@2085048:
    subs r0, r0, r2
    sbc r1, r1, r3
    orrs lr, r1, r0
    beq @20852d8
    mov lr, ip, lsl #0x14
    and lr, lr, #0x80000000
    bic ip, ip, #0x800
    cmp r1, #0
    bmi @20850d0
    bne @2085080
    sub ip, ip, #0x20
    movs r1, r0
    mov r0, #0
    bmi @208509c
@2085080:
    clz r4, r1
    movs r1, r1, lsl r4
    rsb r4, r4, #0x20
    orr r1, r1, r0, lsr r4
    rsb r4, r4, #0x20
    mov r0, r0, lsl r4
    sub ip, ip, r4
@208509c:
    cmp ip, #0
    bgt @20850d8
    rsb ip, ip, #0xc
    cmp ip, #0x20
    movge r0, r1
    movge r1, #0
    subge ip, ip, #0x20
    rsb r4, ip, #0x20
    mov r0, r0, lsr ip
    orr r0, r0, r1, lsl r4
    orr r1, lr, r1, lsr ip
    ldmfd sp!, {r4, lr}
    bx lr
@20850d0:
    cmp r1, #0
    subges ip, ip, #1
@20850d8:
    mov r0, r0, lsr #0xb
    orr r0, r0, r1, lsl #21
    add r1, r1, r1
    orr r1, lr, r1, lsr #12
    orr r1, r1, ip, lsl #20
    ldmfd sp!, {r4, lr}
    bx lr
@20850f4:
    mov lr, ip, lsl #0x14
    and lr, lr, #0x80000000
    bic ip, ip, #0x800
    cmp r1, #0
    bne @2085118
    sub ip, ip, #0x20
    movs r1, r0
    mov r0, #0
    bmi @2085134
@2085118:
    clz r4, r1
    movs r1, r1, lsl r4
    rsb r4, r4, #0x20
    orr r1, r1, r0, lsr r4
    rsb r4, r4, #0x20
    mov r0, r0, lsl r4
    sub ip, ip, r4
@2085134:
    cmp ip, #0
    orrgt ip, ip, lr, lsr #20
    bgt @2085004
    rsb ip, ip, #0xc
    cmp ip, #0x20
    movge r0, r1
    movge r1, #0
    subge ip, ip, #0x20
    rsb r4, ip, #0x20
    mov r0, r0, lsr ip
    orr r0, r0, r1, lsl r4
    orr r1, lr, r1, lsr ip
    ldmfd sp!, {r4, lr}
    bx lr
@208516c:
    cmp ip, #0x800
    movge lr, #0x80000000
    movlt lr, #0
    bics ip, ip, #0x800
    beq @20851d8
    orrs r4, r0, r1, lsl #1
    bne @20852b4
    mov r4, r3, lsr #0x14
    mov r3, r3, lsl #0xb
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #0xb
    movs r4, r4, lsl #0x15
    beq @20852a0
    cmn r4, #0x200000
    bne @20852a0
    orrs r4, r2, r3, lsl #1
    beq @20852c8
    b @20852b4
@20851b4:
    cmp r4, #0x800
    movge lr, #0x80000000
    movlt lr, #0
    bic ip, ip, #0x800
    bics r4, r4, #0x800
    beq @2085250
    orrs r4, r2, r3, lsl #1
    bne @20852b4
    b @20852a0
@20851d8:
    orrs r4, r0, r1, lsl #1
    beq @2085218
    mov ip, #1
    bic r1, r1, #0x80000000
    mov r4, r3, lsr #0x14
    mov r3, r3, lsl #0xb
    orr r3, r3, r2, lsr #21
    mov r2, r2, lsl #0xb
    movs r4, r4, lsl #0x15
    cmnne r4, #0x200000
    mov r4, r4, lsr #0x15
    orr r4, r4, lr, lsr #20
    beq @20851b4
    orr r3, r3, #0x80000000
    orr ip, ip, lr, lsr #20
    b @2084fa0
@2085218:
    mov ip, r3, lsr #0x14
    mov r1, r3, lsl #0xb
    orr r1, r1, r2, lsr #21
    mov r0, r2, lsl #0xb
    movs r4, ip, lsl #0x15
    beq @2085244
    cmn r4, #0x200000
    bne @208526c
    orrs r4, r0, r1, lsl #1
    bne @20852b8
    b @20852a0
@2085244:
    orrs r4, r0, r1, lsl #1
    beq @20852d8
    b @208526c
@2085250:
    orrs r4, r2, r3, lsl #1
    beq @208527c
    mov r4, #1
    bic r3, r3, #0x80000000
    orr ip, ip, lr, lsr #20
    orr r4, r4, lr, lsr #20
    b @2084fa0
@208526c:
    mov r1, r3
    mov r0, r2
    ldmfd sp!, {r4, lr}
    bx lr
@208527c:
    cmp r1, #0
    subges ip, ip, #1
    mov r0, r0, lsr #0xb
    orr r0, r0, r1, lsl #21
    add r1, r1, r1
    orr r1, lr, r1, lsr #12
    orr r1, r1, ip, lsl #20
    ldmfd sp!, {r4, lr}
    bx lr
@20852a0:
    ldr r1, =0x7ff00000
    orr r1, lr, r1
    mov r0, #0
    ldmfd sp!, {r4, lr}
    bx lr
@20852b4:
    mov r1, r3
@20852b8:
    mvn r0, #0
    bic r1, r0, #0x80000000
    ldmfd sp!, {r4, lr}
    bx lr
@20852c8:
    mvn r0, #0
    bic r1, r0, #0x80000000
    ldmfd sp!, {r4, lr}
    bx lr
@20852d8:
    mov r1, #0
    mov r0, #0
    ldmfd sp!, {r4, lr}
    bx lr
}

asm void func_020852ec(void) {
    eors r2, r0, r1
    eormi r1, r1, #0x80000000
    dcd 0x4a000307 // bmi 0x2085f18
    subs ip, r0, r1
    sublo r0, r0, ip
    addlo r1, r1, ip
    mov r2, #0x80000000
    mov r3, r0, lsr #0x17
    orr r0, r2, r0, lsl #8
    ands ip, r3, #0xff
    cmpne ip, #0xff
    beq @208538c
    mov ip, r1, lsr #0x17
    orr r1, r2, r1, lsl #8
    ands r2, ip, #0xff
    beq @20853cc
@208532c:
    subs ip, r3, ip
    beq @2085344
    rsb r2, ip, #0x20
    movs r2, r1, lsl r2
    mov r1, r1, lsr ip
    orrne r1, r1, #1
@2085344:
    adds r0, r0, r1
    blo @2085364
    and r1, r0, #1
    orr r0, r1, r0, rrx
    add r3, r3, #1
    and r2, r3, #0xff
    cmp r2, #0xff
    beq @20854d4
@2085364:
    ands r1, r0, #0xff
    add r0, r0, r0
    mov r0, r0, lsr #9
    orr r0, r0, r3, lsl #23
    tst r1, #0x80
    bxeq lr
    ands r1, r1, #0x7f
    andeqs r1, r0, #1
    addne r0, r0, #1
    bx lr
@208538c:
    cmp r3, #0x100
    movge r2, #0x80000000
    movlt r2, #0
    ands r3, r3, #0xff
    beq @20853f0
    movs r0, r0, lsl #1
    bne @2085500
    mov ip, r1, lsr #0x17
    mov r1, r1, lsl #9
    ands ip, ip, #0xff
    beq @20854f4
    cmp ip, #0xff
    blt @20854f4
    cmp r1, #0
    beq @20854f4
    b @2085500
@20853cc:
    cmp r3, #0x100
    movge r2, #0x80000000
    movlt r2, #0
    and r3, r3, #0xff
    ands ip, ip, #0xff
    beq @208544c
@20853e4:
    movs r1, r1, lsl #1
    bne @2085500
    b @20854f4
@20853f0:
    movs r0, r0, lsl #1
    beq @2085428
    mov r3, #1
    mov r0, r0, lsr #1
    mov ip, r1, lsr #0x17
    mov r1, r1, lsl #8
    ands ip, ip, #0xff
    beq @208544c
    cmp ip, #0xff
    beq @20853e4
    orr r1, r1, #0x80000000
    orr r3, r3, r2, lsr #23
    orr ip, ip, r2, lsr #23
    b @208532c
@2085428:
    mov r3, r1, lsr #0x17
    mov r0, r1, lsl #9
    ands r3, r3, #0xff
    beq @20854b4
    cmp r3, #0xff
    blt @20854b4
    cmp r0, #0
    beq @20854f4
    b @20854ec
@208544c:
    movs r1, r1, lsl #1
    beq @20854bc
    mov r1, r1, lsr #1
    mov ip, #1
    orr r3, r3, r2, lsr #23
    orr ip, ip, r2, lsr #23
    cmp r0, #0
    bmi @208532c
    adds r0, r0, r1
    blo @2085480
    and r1, r0, #1
    orr r0, r1, r0, rrx
    add ip, ip, #1
@2085480:
    cmp r0, #0
    subge ip, ip, #1
    ands r1, r0, #0xff
    add r0, r0, r0
    mov r0, r0, lsr #9
    orr r0, r0, ip, lsl #23
    bxeq lr
    tst r1, #0x80
    bxeq lr
    ands r1, r1, #0x7f
    andeqs r1, r0, #1
    addne r0, r0, #1
    bx lr
@20854b4:
    mov r0, r1
    bx lr
@20854bc:
    cmp r0, #0
    subges r3, r3, #1
    add r0, r0, r0
    orr r0, r2, r0, lsr #9
    orr r0, r0, r3, lsl #23
    bx lr
@20854d4:
    cmp r3, #0x100
    movge r2, #0x80000000
    movlt r2, #0
    mov r0, #0xff000000
    orr r0, r2, r0, lsr #1
    bx lr
@20854ec:
    mvn r0, #0x80000000
    bx lr
@20854f4:
    mov r0, #0xff000000
    orr r0, r2, r0, lsr #1
    bx lr
@2085500:
    mvn r0, #0x80000000
    bx lr
}

asm void func_02085508(void) {
    mvn r0, #0x80000000
    bx lr
}

asm void _dgr(void) {
    mov ip, #0x200000
    cmn ip, r1, lsl #1
    bhs @2085584
    cmn ip, r3, lsl #1
    bhs @2085598
@2085524:
    orrs ip, r3, r1
    bmi @2085554
    cmp r1, r3
    cmpeq r0, r2
    movhi r0, #1
    movls r0, #0
    bx lr
@2085540:
    mov r0, #0
    mrs ip, cpsr
    bic ip, ip, #0x20000000
    msr cpsr_f, ip
    bx lr
@2085554:
    orr ip, r0, ip, lsl #1
    orrs ip, ip, r2
    moveq r0, #0
    mrs ip, cpsr
    bic ip, ip, #0x20000000
    msr cpsr_f, ip
    bxeq lr
    cmp r3, r1
    cmpeq r2, r0
    movhi r0, #1
    movls r0, #0
    bx lr
@2085584:
    bne @2085540
    cmp r0, #0
    bhi @2085540
    cmn ip, r3, lsl #1
    blo @2085524
@2085598:
    bne @2085540
    cmp r2, #0
    bhi @2085540
    b @2085524
}

asm void _dleq(void) {
    mov ip, #0x200000
    cmn ip, r1, lsl #1
    bhs @2085628
    cmn ip, r3, lsl #1
    bhs @208563c
@20855bc:
    orrs ip, r3, r1
    bmi @20855f0
    cmp r1, r3
    cmpeq r0, r2
    movls r0, #1
    movhi r0, #0
    bx lr
@20855d8:
    mov r0, #0
    mrs ip, cpsr
    bic ip, ip, #0x40000000
    orr ip, ip, #0x20000000
    msr cpsr_f, ip
    bx lr
@20855f0:
    orr ip, r0, ip, lsl #1
    orrs ip, ip, r2
    moveq r0, #1
    bne @2085614
    mrs ip, cpsr
    bic ip, ip, #0x20000000
    orr ip, ip, #0x40000000
    msr cpsr_f, ip
    bxeq lr
@2085614:
    cmp r3, r1
    cmpeq r2, r0
    movls r0, #1
    movhi r0, #0
    bx lr
@2085628:
    bne @20855d8
    cmp r0, #0
    bhi @20855d8
    cmn ip, r3, lsl #1
    blo @20855bc
@208563c:
    bne @20855d8
    cmp r2, #0
    bhi @20855d8
    b @20855bc
}

asm void _dls(void) {
    mov ip, #0x200000
    cmn ip, r1, lsl #1
    bhs @20856c4
    cmn ip, r3, lsl #1
    bhs @20856d8
@2085660:
    orrs ip, r3, r1
    bmi @2085690
    cmp r1, r3
    cmpeq r0, r2
    movlo r0, #1
    movhs r0, #0
    bx lr
@208567c:
    mov r0, #0
    mrs ip, cpsr
    orr ip, ip, #0x20000000
    msr cpsr_f, ip
    bx lr
@2085690:
    orr ip, r0, ip, lsl #1
    orrs ip, ip, r2
    moveq r0, #0
    bne @20856b0
    mrs ip, cpsr
    orr ip, ip, #0x20000000
    msr cpsr_f, ip
    bxeq lr
@20856b0:
    cmp r3, r1
    cmpeq r2, r0
    movlo r0, #1
    movhs r0, #0
    bx lr
@20856c4:
    bne @208567c
    cmp r0, #0
    bhi @208567c
    cmn ip, r3, lsl #1
    blo @2085660
@20856d8:
    bne @208567c
    cmp r2, #0
    bhi @208567c
    b @2085660
}

asm void _deq(void) {
    mov ip, #0x200000
    cmn ip, r1, lsl #1
    bhs @2085750
    cmn ip, r3, lsl #1
    bhs @2085764
@20856fc:
    orrs ip, r3, r1
    bmi @208572c
    cmp r1, r3
    cmpeq r0, r2
    moveq r0, #1
    movne r0, #0
    bx lr
@2085718:
    mov r0, #0
    mrs ip, cpsr
    bic ip, ip, #0x40000000
    msr cpsr_f, ip
    bx lr
@208572c:
    orr ip, r0, ip, lsl #1
    orrs ip, ip, r2
    moveq r0, #1
    bxeq lr
    cmp r3, r1
    cmpeq r2, r0
    moveq r0, #1
    movne r0, #0
    bx lr
@2085750:
    bne @2085718
    cmp r0, #0
    bhi @2085718
    cmn ip, r3, lsl #1
    blo @20856fc
@2085764:
    bne @2085718
    cmp r2, #0
    bhi @2085718
    b @20856fc
}

asm void _dneq(void) {
    mov ip, #0x200000
    cmn ip, r1, lsl #1
    bhs @20857dc
    cmn ip, r3, lsl #1
    bhs @20857f0
@2085788:
    orrs ip, r3, r1
    bmi @20857b8
    cmp r1, r3
    cmpeq r0, r2
    movne r0, #1
    moveq r0, #0
    bx lr
@20857a4:
    mov r0, #1
    mrs ip, cpsr
    bic ip, ip, #0x40000000
    msr cpsr_f, ip
    bx lr
@20857b8:
    orr ip, r0, ip, lsl #1
    orrs ip, ip, r2
    moveq r0, #0
    bxeq lr
    cmp r3, r1
    cmpeq r2, r0
    movne r0, #1
    moveq r0, #0
    bx lr
@20857dc:
    bne @20857a4
    cmp r0, #0
    bhi @20857a4
    cmn ip, r3, lsl #1
    blo @2085788
@20857f0:
    bne @20857a4
    cmp r2, #0
    bhi @20857a4
    b @2085788
}

asm void func_02085800(void) {
    mov r3, #0xff000000
    cmp r3, r0, lsl #1
    cmphs r3, r1, lsl #1
    blo @2085848
    cmp r0, #0
    bicmi r0, r0, #0x80000000
    rsbmi r0, r0, #0
    cmp r1, #0
    bicmi r1, r1, #0x80000000
    rsbmi r1, r1, #0
    cmp r0, r1
    movgt r0, #1
    movle r0, #0
    mrs ip, cpsr
    bicle ip, ip, #0x20000000
    orrgt ip, ip, #0x20000000
    msr cpsr_f, ip
    bx lr
@2085848:
    mov r0, #0
    mrs ip, cpsr
    bic ip, ip, #0x20000000
    msr cpsr_f, ip
    bx lr
}

asm void func_0208585c(void) {
    eor r0, r0, r1
    eor r1, r0, r1
    eor r0, r0, r1
}

asm void func_02085868(void) {
    stmdb sp!, {lr}
    mov ip, #0xff
    ands r3, ip, r0, lsr #23
    cmpne r3, #0xff
    beq @2085a3c
    ands ip, ip, r1, lsr #23
    cmpne ip, #0xff
    beq @2085a78
    orr r1, r1, #0x800000
    orr r0, r0, #0x800000
    bic r2, r0, #0xff000000
    bic lr, r1, #0xff000000
@2085898:
    cmp r2, lr
    movlo r2, r2, lsl #1
    sublo r3, r3, #1
    teq r0, r1
    sub r0, pc, #0x94
    ldrb r1, [r0, lr, lsr #15]
    rsb lr, lr, #0
    mov r0, lr, asr #1
    mul r0, r1, r0
    add r0, r0, #0x80000000
    mov r0, r0, lsr #6
    mul r0, r1, r0
    mov r0, r0, lsr #0xe
    mul r1, lr, r0
    sub ip, r3, ip
    mov r1, r1, lsr #0xc
    mul r1, r0, r1
    mov r0, r0, lsl #0xe
    add r0, r0, r1, lsr #15
    umull r1, r0, r2, r0
    mov r3, r0
    orrmi r0, r0, #0x80000000
    adds ip, ip, #0x7e
    bmi @2085b40
    cmp ip, #0xfe
    bge @2085bf4
    add r0, r0, ip, lsl #23
    mov ip, r1, lsr #0x1c
    cmp ip, #7
    beq @2085a1c
    add r0, r0, r1, lsr #31
    ldmia sp!, {lr}
    bx lr
    dcd 0xfdfeffff
    dcd 0xf9fafbfc
    dcd 0xf5f6f7f8
    dcd 0xf1f2f3f4
    dcd 0xeeeff0f0
    dcd 0xeaebeced
    dcd 0xe7e8e9ea
    dcd 0xe4e5e6e6
    dcd 0xe1e2e2e3
    dcd 0xdedfdfe0
    dcd 0xdbdcdcdd
    dcd 0xd8d9d9da
    dcd 0xd5d6d7d7
    dcd 0xd2d3d4d4
    dcd 0xd0d0d1d2
    dcd 0xcdcececf
    dcd 0xcbcbcccc
    dcd 0xc8c9c9ca
    dcd 0xc6c6c7c8
    dcd 0xc3c4c5c5
    dcd 0xc1c2c2c3
    dcd 0xbfbfc0c0
    dcd 0xbdbdbebe
    dcd 0xbabbbcbc
    dcd 0xb8b9b9ba
    dcd 0xb6b7b7b8
    dcd 0xb4b5b5b6
    dcd 0xb2b3b3b4
    dcd 0xb0b1b1b2
    dcd 0xafafafb0
    dcd 0xadadaeae
    dcd 0xababacac
    dcd 0xa9aaaaaa
    dcd 0xa7a8a8a9
    dcd 0xa6a6a7a7
    dcd 0xa4a4a5a5
    dcd 0xa2a3a3a4
    dcd 0xa1a1a2a2
    dcd 0x9fa0a0a0
    dcd 0x9e9e9e9f
    dcd 0x9c9d9d9d
    dcd 0x9b9b9b9c
    dcd 0x999a9a9a
    dcd 0x98989999
    dcd 0x96979798
    dcd 0x95959696
    dcd 0x94949495
    dcd 0x92939393
    dcd 0x91919292
    dcd 0x90909191
    dcd 0x8f8f8f90
    dcd 0x8d8e8e8e
    dcd 0x8c8c8d8d
    dcd 0x8b8b8c8c
    dcd 0x8a8a8a8b
    dcd 0x8989898a
    dcd 0x88888888
    dcd 0x86878787
    dcd 0x85868686
    dcd 0x84858585
    dcd 0x83838484
    dcd 0x82828383
    dcd 0x81818282
    dcd 0x80808181
@2085a1c:
    mov r1, r3, lsl #1
    add r1, r1, #1
    rsb lr, lr, #0
    mul r1, lr, r1
    cmp r1, r2, lsl #24
    addmi r0, r0, #1
    ldmia sp!, {lr}
    bx lr
@2085a3c:
    eor lr, r0, r1
    and lr, lr, #0x80000000
    cmp r3, #0
    beq @2085a94
    movs r0, r0, lsl #9
    bne @2085bdc
    mov ip, r1, lsr #0x17
    mov r1, r1, lsl #9
    ands ip, ip, #0xff
    beq @2085bcc
    cmp ip, #0xff
    blt @2085bcc
    cmp r1, #0
    beq @2085be8
    b @2085bc4
@2085a78:
    eor lr, r0, r1
    and lr, lr, #0x80000000
    cmp ip, #0
    beq @2085af8
@2085a88:
    movs r1, r1, lsl #9
    bne @2085bc4
    b @2085c14
@2085a94:
    movs r2, r0, lsl #9
    beq @2085ac8
    clz r3, r2
    movs r2, r2, lsl r3
    rsb r3, r3, #0
    mov r2, r2, lsr #8
    ands ip, ip, r1, lsr #23
    beq @2085b20
    cmp ip, #0xff
    beq @2085a88
    orr r1, r1, #0x800000
    bic lr, r1, #0xff000000
    b @2085898
@2085ac8:
    mov ip, r1, lsr #0x17
    mov r1, r1, lsl #9
    ands ip, ip, #0xff
    beq @2085aec
    cmp ip, #0xff
    blt @2085c14
    cmp r1, #0
    beq @2085c14
    b @2085bc4
@2085aec:
    cmp r1, #0
    beq @2085be8
    b @2085c14
@2085af8:
    movs ip, r1, lsl #9
    beq @2085bcc
    mov lr, ip
    clz ip, lr
    movs lr, lr, lsl ip
    rsb ip, ip, #0
    mov lr, lr, lsr #8
    orr r0, r0, #0x800000
    bic r2, r0, #0xff000000
    b @2085898
@2085b20:
    movs ip, r1, lsl #9
    beq @2085bcc
    mov lr, ip
    clz ip, lr
    movs lr, lr, lsl ip
    rsb ip, ip, #0
    mov lr, lr, lsr #8
    b @2085898
@2085b40:
    and r0, r0, #0x80000000
    cmn ip, #0x18
    beq @2085bb4
    bmi @2085c0c
    add r1, ip, #0x17
    mov r2, r2, lsl r1
    rsb ip, ip, #0
    mov r3, r3, lsr ip
    orr r0, r0, r3
    rsb lr, lr, #0
    mul r1, lr, r3
    cmp r1, r2
    ldmeqia sp!, {lr}
    bxeq lr
    add r1, r1, lr
    cmp r1, r2
    beq @2085ba8
    addmi r0, r0, #1
    subpl r1, r1, lr
    add r1, lr, r1, lsl #1
    cmp r1, r2, lsl #1
    and r3, r0, #1
    addmi r0, r0, #1
    addeq r0, r0, r3
    ldmia sp!, {lr}
    bx lr
@2085ba8:
    add r0, r0, #1
    ldmia sp!, {lr}
    bx lr
@2085bb4:
    cmn r2, lr
    addne r0, r0, #1
    ldmia sp!, {lr}
    bx lr
@2085bc4:
    mov r0, r1
    b @2085bdc
@2085bcc:
    mov r0, #0xff000000
    orr r0, lr, r0, lsr #1
    ldmia sp!, {lr}
    bx lr
@2085bdc:
    mvn r0, #0x80000000
    ldmia sp!, {lr}
    bx lr
@2085be8:
    mvn r0, #0x80000000
    ldmia sp!, {lr}
    bx lr
@2085bf4:
    tst r0, #0x80000000
    mov r0, #0xff000000
    movne r0, r0, asr #1
    moveq r0, r0, lsr #1
    ldmia sp!, {lr}
    bx lr
@2085c0c:
    ldmia sp!, {lr}
    bx lr
@2085c14:
    mov r0, lr
    ldmia sp!, {lr}
    bx lr
}

asm void _f2d(void) {
    and r2, r0, #0x80000000
    mov ip, r0, lsr #0x17
    mov r3, r0, lsl #9
    ands ip, ip, #0xff
    beq @2085c50
    cmp ip, #0xff
    beq @2085c7c
@2085c3c:
    add ip, ip, #0x380
    mov r0, r3, lsl #0x14
    orr r1, r2, r3, lsr #12
    orr r1, r1, ip, lsl #20
    bx lr
@2085c50:
    cmp r3, #0
    bne @2085c64
    mov r1, r2
    mov r0, #0
    bx lr
@2085c64:
    mov r3, r3, lsr #1
    clz ip, r3
    movs r3, r3, lsl ip
    rsb ip, ip, #1
    add r3, r3, r3
    b @2085c3c
@2085c7c:
    cmp r3, #0
    bhi @2085c94
    ldr r1, =0x7ff00000
    orr r1, r1, r2
    mov r0, #0
    bx lr
@2085c94:
    mvn r0, #0
    bic r1, r0, #0x80000000
    bx lr
}

asm void func_02085ca4(void) {
    bic r1, r0, #0x80000000
    mov r2, #0x9e
    subs r2, r2, r1, lsr #23
    ble @2085ccc
    mov r1, r1, lsl #8
    orr r1, r1, #0x80000000
    cmp r0, #0
    mov r0, r1, lsr r2
    rsbmi r0, r0, #0
    bx lr
@2085ccc:
    mvn r0, r0, asr #31
    add r0, r0, #0x80000000
    bx lr
}

asm void func_02085cd8(void) {
    ands r2, r0, #0x80000000
    rsbmi r0, r0, #0
    cmp r0, #0
    bxeq lr
    clz r3, r0
    movs r0, r0, lsl r3
    rsb r3, r3, #0x9e
    ands r1, r0, #0xff
    add r0, r0, r0
    orr r0, r2, r0, lsr #9
    orr r0, r0, r3, lsl #23
    bxeq lr
    tst r1, #0x80
    bxeq lr
    ands r3, r1, #0x7f
    andeqs r3, r0, #1
    addne r0, r0, #1
    bx lr
}

asm void func_02085d20(void) {
    eor r2, r0, r1
    and r2, r2, #0x80000000
    mov ip, #0xff
    ands r3, ip, r0, lsr #23
    mov r0, r0, lsl #8
    cmpne r3, #0xff
    beq @2085d9c
    orr r0, r0, #0x80000000
    ands ip, ip, r1, lsr #23
    mov r1, r1, lsl #8
    cmpne ip, #0xff
    beq @2085ddc
    orr r1, r1, #0x80000000
@2085d54:
    add ip, r3, ip
    umull r1, r3, r0, r1
    movs r0, r3
    addpl r0, r0, r0
    subpl ip, ip, #1
    subs ip, ip, #0x7f
    bmi @2085e68
    cmp ip, #0xfe
    bge @2085ed4
    ands r3, r0, #0xff
    orr r0, r2, r0, lsr #8
    add r0, r0, ip, lsl #23
    tst r3, #0x80
    bxeq lr
    orrs r1, r1, r3, lsl #25
    andeqs r3, r0, #1
    addne r0, r0, #1
    bx lr
@2085d9c:
    cmp r3, #0
    beq @2085df0
    movs r0, r0, lsl #1
    bne @2085ec4
    mov ip, r1, lsr #0x17
    mov r1, r1, lsl #9
    ands ip, ip, #0xff
    beq @2085dd0
    cmp ip, #0xff
    blt @2085eb8
    cmp r1, #0
    beq @2085eb8
    b @2085ec4
@2085dd0:
    cmp r1, #0
    beq @2085ecc
    b @2085eb8
@2085ddc:
    cmp ip, #0
    beq @2085e4c
@2085de4:
    movs r1, r1, lsl #1
    bne @2085ec4
    b @2085eb8
@2085df0:
    movs r0, r0, lsl #1
    beq @2085e28
    mov r0, r0, lsr #1
    clz r3, r0
    movs r0, r0, lsl r3
    rsb r3, r3, #1
    mov ip, r1, lsr #0x17
    mov r1, r1, lsl #8
    ands ip, ip, #0xff
    beq @2085e4c
    cmp ip, #0xff
    beq @2085de4
    orr r1, r1, #0x80000000
    b @2085d54
@2085e28:
    mov ip, r1, lsr #0x17
    mov r1, r1, lsl #9
    ands ip, ip, #0xff
    beq @2085ef8
    cmp ip, #0xff
    blt @2085ef8
    cmp r1, #0
    beq @2085ecc
    b @2085ec4
@2085e4c:
    movs r1, r1, lsl #1
    beq @2085ef8
    mov r1, r1, lsr #1
    clz ip, r1
    movs r1, r1, lsl ip
    rsb ip, ip, #1
    b @2085d54
@2085e68:
    cmn ip, #0x18
    beq @2085eb0
    bmi @2085ef0
    cmp r1, #0
    orrne r0, r0, #1
    mov r3, r0
    mov r0, r0, lsr #8
    rsb ip, ip, #0
    orr r0, r2, r0, lsr ip
    rsb ip, ip, #0x18
    movs r1, r3, lsl ip
    bxeq lr
    tst r1, #0x80000000
    bxeq lr
    movs r1, r1, lsl #1
    andeqs r1, r0, #1
    addne r0, r0, #1
    bx lr
@2085eb0:
    mov r0, r0, lsl #1
    b @2085ee0
@2085eb8:
    mov r0, #0xff000000
    orr r0, r2, r0, lsr #1
    bx lr
@2085ec4:
    mvn r0, #0x80000000
    bx lr
@2085ecc:
    mvn r0, #0x80000000
    bx lr
@2085ed4:
    mov r0, #0xff000000
    orr r0, r2, r0, lsr #1
    bx lr
@2085ee0:
    movs r1, r0
    mov r0, r2
    addne r0, r0, #1
    bx lr
@2085ef0:
    mov r0, r2
    bx lr
@2085ef8:
    mov r0, r2
    bx lr
}

asm void func_02085f00(void) {
    eor r0, r0, r1
    eor r1, r0, r1
    eor r0, r0, r1
}

asm void func_02085f0c(void) {
    eors r2, r0, r1
    eormi r1, r1, #0x80000000
    dcd 0x4afffcf7 // bmi 0x20852f8
    subs ip, r0, r1
    eorlo ip, ip, #0x80000000
    sublo r0, r0, ip
    addlo r1, r1, ip
    mov r2, #0x80000000
    mov r3, r0, lsr #0x17
    orr r0, r2, r0, lsl #8
    ands ip, r3, #0xff
    cmpne ip, #0xff
    beq @2086034
    mov ip, r1, lsr #0x17
    orr r1, r2, r1, lsl #8
    ands r2, ip, #0xff
    beq @2086074
@2085f50:
    subs ip, r3, ip
    beq @2085f98
    rsb r2, ip, #0x20
    movs r2, r1, lsl r2
    mov r1, r1, lsr ip
    orrne r1, r1, #1
    subs r0, r0, r1
    bpl @2085fdc
    ands r1, r0, #0xff
    add r0, r0, r0
    mov r0, r0, lsr #9
    orr r0, r0, r3, lsl #23
    tst r1, #0x80
    bxeq lr
    ands r1, r1, #0x7f
    andeqs r1, r0, #1
    addne r0, r0, #1
    bx lr
@2085f98:
    subs r0, r0, r1
    beq @2086140
    mov r2, r3, lsl #0x17
    and r2, r2, #0x80000000
    bic r3, r3, #0x100
    clz ip, r0
    movs r0, r0, lsl ip
    sub r3, r3, ip
    cmp r3, #0
    bgt @2085fcc
    rsb r3, r3, #9
    orr r0, r2, r0, lsr r3
    bx lr
@2085fcc:
    add r0, r0, r0
    orr r0, r2, r0, lsr #9
    orr r0, r0, r3, lsl #23
    bx lr
@2085fdc:
    mov r2, r3, lsl #0x17
    and r2, r2, #0x80000000
    bic r3, r3, #0x100
    clz ip, r0
    movs r0, r0, lsl ip
    sub r3, r3, ip
    cmp r3, #0
    bgt @2086008
    rsb r3, r3, #9
    orr r0, r2, r0, lsr r3
    bx lr
@2086008:
    ands r1, r0, #0xff
    add r0, r0, r0
    orr r0, r2, r0, lsr #9
    orr r0, r0, r3, lsl #23
    bxeq lr
    tst r1, #0x80
    bxeq lr
    ands r1, r1, #0x7f
    andeqs r1, r0, #1
    addne r0, r0, #1
    bx lr
@2086034:
    cmp r3, #0x100
    movge r2, #0x80000000
    movlt r2, #0
    ands r3, r3, #0xff
    beq @208609c
    movs r0, r0, lsl #1
    bne @2086174
    mov ip, r1, lsr #0x17
    mov r1, r1, lsl #9
    ands ip, ip, #0xff
    beq @2086168
    cmp ip, #0xff
    blt @2086168
    cmp r1, #0
    beq @208617c
    b @2086174
@2086074:
    cmp ip, #0x100
    movge r2, #0x80000000
    movlt r2, #0
    and r3, r3, #0xff
    ands ip, ip, #0xff
    beq @2086104
@208608c:
    eor r2, r2, #0x80000000
    movs r1, r1, lsl #1
    bne @2086174
    b @2086168
@208609c:
    movs r0, r0, lsl #1
    beq @20860d4
    mov r0, r0, lsr #1
    mov r3, #1
    mov ip, r1, lsr #0x17
    mov r1, r1, lsl #8
    ands ip, ip, #0xff
    beq @2086104
    cmp ip, #0xff
    beq @208608c
    orr r1, r1, #0x80000000
    orr r3, r3, r2, lsr #23
    orr ip, ip, r2, lsr #23
    b @2085f50
@20860d4:
    mov r3, r1, lsr #0x17
    mov r0, r1, lsl #9
    ands r2, r3, #0xff
    beq @20860f8
    cmp r2, #0xff
    blt @2086120
    cmp r0, #0
    bne @2086160
    b @2086168
@20860f8:
    cmp r0, #0
    beq @2086140
    b @2086120
@2086104:
    movs r1, r1, lsl #1
    beq @2086128
    mov r1, r1, lsr #1
    mov ip, #1
    orr ip, ip, r2, lsr #23
    orr r3, r3, r2, lsr #23
    b @2085f50
@2086120:
    mov r0, r1
    bx lr
@2086128:
    cmp r0, #0
    subges r3, r3, #1
    add r0, r0, r0
    orr r0, r2, r0, lsr #9
    orr r0, r0, r3, lsl #23
    bx lr
@2086140:
    mov r0, #0
    bx lr
    cmp r0, #0
    subges r3, r3, #1
    add r0, r0, r0
    mov r0, r0, lsr #9
    orr r0, r0, r3, lsl #23
    bx lr
@2086160:
    mvn r0, #0x80000000
    bx lr
@2086168:
    mov r0, #0xff000000
    orr r0, r2, r0, lsr #1
    bx lr
@2086174:
    mvn r0, #0x80000000
    bx lr
@208617c:
    mvn r0, #0x80000000
    bx lr
}

asm void func_02086184(void) {
    stmfd sp!, {r4, r5, r6, r7, r11, ip, lr}
    mov r4, r1
    orr r4, r4, #1
    dcd 0xea000003 // b 0x20861a4
}

asm void _ll_sdiv(void) {
    stmfd sp!, {r4, r5, r6, r7, r11, ip, lr}
    eor r4, r1, r3
    mov r4, r4, asr #1
    mov r4, r4, lsl #1
    orrs r5, r3, r2
    bne @20861b4
    ldmfd sp!, {r4, r5, r6, r7, r11, ip, lr}
    bx lr
@20861b4:
    mov r5, r0, lsr #0x1f
    add r5, r5, r1
    mov r6, r2, lsr #0x1f
    add r6, r6, r3
    orrs r6, r5, r6
    bne @20861e8
    mov r1, r2
    bl _s32_div_f
    ands r4, r4, #1
    movne r0, r1
    mov r1, r0, asr #0x1f
    ldmfd sp!, {r4, r5, r6, r7, r11, ip, lr}
    bx lr
@20861e8:
    cmp r1, #0
    bge @20861f8
    rsbs r0, r0, #0
    rsc r1, r1, #0
@20861f8:
    cmp r3, #0
    bge @2086208
    rsbs r2, r2, #0
    rsc r3, r3, #0
@2086208:
    orrs r5, r1, r0
    beq @208632c
    mov r5, #0
    mov r6, #1
    cmp r3, #0
    bmi @2086234
@2086220:
    add r5, r5, #1
    adds r2, r2, r2
    adcs r3, r3, r3
    bpl @2086220
    add r6, r6, r5
@2086234:
    cmp r1, #0
    blt @2086254
@208623c:
    cmp r6, #1
    beq @2086254
    sub r6, r6, #1
    adds r0, r0, r0
    adcs r1, r1, r1
    bpl @208623c
@2086254:
    mov r7, #0
    mov ip, #0
    mov r11, #0
    b @208627c
@2086264:
    orr ip, ip, #1
    subs r6, r6, #1
    beq @20862d4
    adds r0, r0, r0
    adcs r1, r1, r1
    adcs r7, r7, r7
@208627c:
    subs r0, r0, r2
    sbcs r1, r1, r3
    sbcs r7, r7, #0
    adds ip, ip, ip
    adc r11, r11, r11
    cmp r7, #0
    bge @2086264
@2086298:
    subs r6, r6, #1
    beq @20862cc
    adds r0, r0, r0
    adcs r1, r1, r1
    adc r7, r7, r7
    adds r0, r0, r2
    adcs r1, r1, r3
    adc r7, r7, #0
    adds ip, ip, ip
    adc r11, r11, r11
    cmp r7, #0
    bge @2086264
    b @2086298
@20862cc:
    adds r0, r0, r2
    adc r1, r1, r3
@20862d4:
    ands r7, r4, #1
    moveq r0, ip
    moveq r1, r11
    beq @208630c
    subs r7, r5, #0x20
    movge r0, r1, lsr r7
    bge @2086330
    rsb r7, r5, #0x20
    mov r0, r0, lsr r5
    orr r0, r0, r1, lsl r7
    mov r1, r1, lsr r5
    b @208630c
    mov r0, r1, lsr r7
    mov r1, #0
@208630c:
    cmp r4, #0
    blt @208631c
    ldmfd sp!, {r4, r5, r6, r7, r11, ip, lr}
    bx lr
@208631c:
    rsbs r0, r0, #0
    rsc r1, r1, #0
    ldmfd sp!, {r4, r5, r6, r7, r11, ip, lr}
    bx lr
@208632c:
    mov r0, #0
@2086330:
    mov r1, #0
    cmp r4, #0
    blt @208631c
    ldmfd sp!, {r4, r5, r6, r7, r11, ip, lr}
    bx lr
}

asm void _ll_udiv(void) {
    stmfd sp!, {r4, r5, r6, r7, r11, ip, lr}
    mov r4, #0
    dcd 0xea000001 // b 0x2086358
}

asm void _ull_mod(void) {
    stmfd sp!, {r4, r5, r6, r7, r11, ip, lr}
    mov r4, #1
    orrs r5, r3, r2
    bne @2086368
    ldmfd sp!, {r4, r5, r6, r7, r11, ip, lr}
    bx lr
@2086368:
    orrs r5, r1, r3
    dcd 0x1affffa5 // bne 0x2086208
    mov r1, r2
    bl func_020865c0
    cmp r4, #0
    movne r0, r1
    mov r1, #0
    ldmfd sp!, {r4, r5, r6, r7, r11, ip, lr}
    bx lr
}

#pragma thumb reset
#pragma force_active reset
