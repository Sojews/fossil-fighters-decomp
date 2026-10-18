// NitroSDK MATH: CRC-32 (bit-reversed) tables and updates.

#include "types.h"

typedef u32 MATHCRC32Context;

typedef struct {
    u32 table[256];
} MATHCRC32Table;

void MATHi_CRC32InitTableRev(MATHCRC32Table *table, u32 poly) {
    u32 r;
    u32 i, j;
    u32 *t = table->table;

    for (i = 0; i < 256; i++) {
        r = i;
        for (j = 0; j < 8; j++) {
            if (r & 1) {
                r = (r >> 1) ^ poly;
            } else {
                r >>= 1;
            }
        }
        t[i] = r;
    }
}

void MATHi_CRC32UpdateRev(const MATHCRC32Table *table, MATHCRC32Context *context, const void *input, u32 length) {
    u32 r;
    u32 i;
    const u32 *t = table->table;
    u8 *data = (u8 *) input;

    r = (u32) *context;
    for (i = 0; i < length; i++) {
        r = (r >> 8) ^ t[(r ^ *data) & 0xFF];
        data++;
    }
    *context = (MATHCRC32Context) r;
}
