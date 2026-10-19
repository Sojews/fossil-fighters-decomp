// RC4, as the save data's blocks are encrypted with. The state keeps each byte in a word's top byte, for the ARM
// cipher loop.

#include "types.h"

typedef struct {
    u32 i, j;
    u32 state[256];
} RC4Context;

void func_0204b0ec(RC4Context *context, const void *key, u32 keyLength) {
    u32 *state;
    int i, k, j;

    if (keyLength > 16) {
        keyLength = 16;
    }
    context->i = 0;
    context->j = 0;
    state = context->state;
    for (i = 0; i < 256; i++) {
        state[i] = i;
    }
    j = 0;
    k = 0;
    for (i = 0; i < 256; i++) {
        u32 t = state[i];
        j = (int) (j + (t + ((const u8 *) key)[k])) & 0xFF;
        if (++k == keyLength) {
            k = 0;
        }
        state[i] = state[j];
        state[j] = t;
    }
    for (i = 0; i < 256; i++) {
        state[i] <<= 24;
    }
}

// Encrypts (or decrypts) size bytes from src into dest, eight at a time. The indices count in the words' top bytes,
// so `lsr #22` turns them straight into word offsets.
#pragma thumb off
asm void func_0204b15c(RC4Context *context, const void *src, u32 size, void *dest) {
    stmfd sp!, {r4, r5, r6, r7, r8, r9, lr}
    ldr r7, [r0]
    ldr r6, [r0, #4]
    add r0, r0, #8
    mov lr, #0x1000000
    add r7, r7, lr
    subs r2, r2, #8
    ldr r9, [r0, r7, lsr #22]
    bmi @tail
@loop:
    add r6, r6, r9
    ldr r8, [r0, r6, lsr #22]
    ldrb r5, [r1], #1
    str r8, [r0, r7, lsr #22]
    str r9, [r0, r6, lsr #22]
    add r4, r9, r8
    add r7, r7, lr
    ldr r4, [r0, r4, lsr #22]
    ldr r9, [r0, r7, lsr #22]
    eor r5, r5, r4, lsr #24
    strb r5, [r3], #1
    add r6, r6, r9
    ldr r8, [r0, r6, lsr #22]
    ldrb r5, [r1], #1
    str r8, [r0, r7, lsr #22]
    str r9, [r0, r6, lsr #22]
    add r4, r9, r8
    add r7, r7, lr
    ldr r4, [r0, r4, lsr #22]
    ldr r9, [r0, r7, lsr #22]
    eor r5, r5, r4, lsr #24
    strb r5, [r3], #1
    add r6, r6, r9
    ldr r8, [r0, r6, lsr #22]
    ldrb r5, [r1], #1
    str r8, [r0, r7, lsr #22]
    str r9, [r0, r6, lsr #22]
    add r4, r9, r8
    add r7, r7, lr
    ldr r4, [r0, r4, lsr #22]
    ldr r9, [r0, r7, lsr #22]
    eor r5, r5, r4, lsr #24
    strb r5, [r3], #1
    add r6, r6, r9
    ldr r8, [r0, r6, lsr #22]
    ldrb r5, [r1], #1
    str r8, [r0, r7, lsr #22]
    str r9, [r0, r6, lsr #22]
    add r4, r9, r8
    add r7, r7, lr
    ldr r4, [r0, r4, lsr #22]
    ldr r9, [r0, r7, lsr #22]
    eor r5, r5, r4, lsr #24
    strb r5, [r3], #1
    add r6, r6, r9
    ldr r8, [r0, r6, lsr #22]
    ldrb r5, [r1], #1
    str r8, [r0, r7, lsr #22]
    str r9, [r0, r6, lsr #22]
    add r4, r9, r8
    add r7, r7, lr
    ldr r4, [r0, r4, lsr #22]
    ldr r9, [r0, r7, lsr #22]
    eor r5, r5, r4, lsr #24
    strb r5, [r3], #1
    add r6, r6, r9
    ldr r8, [r0, r6, lsr #22]
    ldrb r5, [r1], #1
    str r8, [r0, r7, lsr #22]
    str r9, [r0, r6, lsr #22]
    add r4, r9, r8
    add r7, r7, lr
    ldr r4, [r0, r4, lsr #22]
    ldr r9, [r0, r7, lsr #22]
    eor r5, r5, r4, lsr #24
    strb r5, [r3], #1
    add r6, r6, r9
    ldr r8, [r0, r6, lsr #22]
    ldrb r5, [r1], #1
    str r8, [r0, r7, lsr #22]
    str r9, [r0, r6, lsr #22]
    add r4, r9, r8
    add r7, r7, lr
    ldr r4, [r0, r4, lsr #22]
    ldr r9, [r0, r7, lsr #22]
    eor r5, r5, r4, lsr #24
    strb r5, [r3], #1
    add r6, r6, r9
    subs r2, r2, #8
    ldr r8, [r0, r6, lsr #22]
    ldrb r5, [r1], #1
    str r8, [r0, r7, lsr #22]
    str r9, [r0, r6, lsr #22]
    add r4, r9, r8
    add r7, r7, lr
    ldr r4, [r0, r4, lsr #22]
    ldr r9, [r0, r7, lsr #22]
    eor r5, r5, r4, lsr #24
    strb r5, [r3], #1
    bpl @loop
@tail:
    adds r2, r2, #8
    ble @done
@one:
    add r6, r6, r9
    subs r2, r2, #1
    ldr r8, [r0, r6, lsr #22]
    ldrb r5, [r1], #1
    str r8, [r0, r7, lsr #22]
    str r9, [r0, r6, lsr #22]
    add r4, r9, r8
    add r7, r7, lr
    ldr r4, [r0, r4, lsr #22]
    ldr r9, [r0, r7, lsr #22]
    eor r5, r5, r4, lsr #24
    strb r5, [r3], #1
    bgt @one
@done:
    sub r7, r7, lr
    and r7, r7, #0xff000000
    str r6, [r0, #-4]
    str r7, [r0, #-8]
    ldmfd sp!, {r4, r5, r6, r7, r8, r9, pc}
}
#pragma thumb reset
