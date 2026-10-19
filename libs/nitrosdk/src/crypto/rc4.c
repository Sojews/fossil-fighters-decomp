// RC4, as the save data's blocks are encrypted with. The state keeps each byte in a word's top byte, for the ARM
// cipher loop (func_0204b15c, not decompiled yet).

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
