// A bit set stored in caller-provided memory. Built as ARM code.

#pragma thumb off

#include "sound.h"

BitSet::BitSet() {
    ClearFields();
}

BitSet::~BitSet() {
    if (initialized) {
        ClearFields();
    }
}

void BitSet::ClearFields() {
    bits = NULL;
    count = 0;
    bytes = 0;
    initialized = FALSE;
}

void BitSet::Init(u16 *storage, int n) {
    if (initialized) {
        return;
    }
    bits = (s8 *)storage;
    count = n;
    bytes = n / 8;
    initialized = TRUE;
}

#ifdef NONMATCHING
// Equivalent, but register allocation differs from the original.
void BitSet::Set(int bit) {
    if (!initialized) {
        return;
    }
    if (bit >= count) {
        return;
    }
    bits[bit / 8] |= 1 << (bit % 8);
}
#else
asm void BitSet::Set(int bit) {
    nofralloc
    stmdb sp!, {r3, lr}
    ldrb r2, [r0, #0xc]
    cmp r2, #0x0
    ldmeqia sp!, {r3, pc}
    ldr r2, [r0, #0x4]
    cmp r1, r2
    ldmgeia sp!, {r3, pc}
    mov r2, r1, asr #0x2
    add r2, r1, r2, lsr #0x1d
    ldr ip, [r0, #0x0]
    mov lr, r2, asr #0x3
    mov r3, r1, lsr #0x1f
    rsb r0, r3, r1, lsl #0x1d
    ldrsb r1, [ip, lr]
    add r3, r3, r0, ror #0x1d
    mov r0, #0x1
    orr r0, r1, r0, lsl r3
    strb r0, [ip, r2, asr #0x3]
    ldmia sp!, {r3, pc}
}
#endif

#ifdef NONMATCHING
// Equivalent, but register allocation differs from the original.
void BitSet::Clear(int bit) {
    if (!initialized) {
        return;
    }
    if (bit >= count) {
        return;
    }
    bits[bit / 8] &= ~(1 << (bit % 8));
}
#else
asm void BitSet::Clear(int bit) {
    nofralloc
    ldrb r2, [r0, #0xc]
    cmp r2, #0x0
    bxeq lr
    ldr r2, [r0, #0x4]
    cmp r1, r2
    bxge lr
    mov r2, r1, asr #0x2
    add r2, r1, r2, lsr #0x1d
    mov r3, r1, lsr #0x1f
    rsb r1, r3, r1, lsl #0x1d
    ldr ip, [r0, #0x0]
    mov r0, r2, asr #0x3
    add r3, r3, r1, ror #0x1d
    mov r1, #0x1
    ldrsb r0, [ip, r0]
    mvn r1, r1, lsl r3
    and r0, r1, r0
    strb r0, [ip, r2, asr #0x3]
    bx lr
}
#endif

#ifdef NONMATCHING
// Equivalent, but register allocation differs from the original.
BOOL BitSet::Test(int bit) {
    if (!initialized) {
        return FALSE;
    }
    if (bit >= count) {
        return FALSE;
    }
    return (bits[bit / 8] & (1 << (bit % 8))) ? TRUE : FALSE;
}
#else
asm BOOL BitSet::Test(int bit) {
    nofralloc
    ldrb r2, [r0, #0xc]
    cmp r2, #0x0
    moveq r0, #0x0
    bxeq lr
    ldr r2, [r0, #0x4]
    cmp r1, r2
    movge r0, #0x0
    bxge lr
    mov r2, r1, asr #0x2
    add r2, r1, r2, lsr #0x1d
    mov ip, r2, asr #0x3
    ldr r3, [r0, #0x0]
    mov r2, r1, lsr #0x1f
    rsb r0, r2, r1, lsl #0x1d
    add r2, r2, r0, ror #0x1d
    ldrsb r1, [r3, ip]
    mov r0, #0x1
    tst r1, r0, lsl r2
    moveq r0, #0x0
    bx lr
}
#endif

#ifdef NONMATCHING
// Equivalent, but register allocation differs from the original.
void BitSet::SetAll(s8 value) {
    int i;

    if (!initialized) {
        return;
    }
    for (i = 0; i < bytes; i++) {
        bits[i] = -value;
    }
}
#else
asm void BitSet::SetAll(s8 value) {
    nofralloc
    ldrb r2, [r0, #0xc]
    cmp r2, #0x0
    bxeq lr
    mov r1, r1, lsl #0x18
    ldr r2, [r0, #0x8]
    mov r1, r1, asr #0x18
    cmp r2, #0x0
    rsb r2, r1, #0x0
    mov r3, #0x0
    bxle lr
L_02004fe8:
    ldr r1, [r0, #0x0]
    strb r2, [r1, r3]
    ldr r1, [r0, #0x8]
    add r3, r3, #0x1
    cmp r3, r1
    blt L_02004fe8
    bx lr
}
#endif
