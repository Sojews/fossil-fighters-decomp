// NitroSDK DGT: MD5 ("hash 1"), SHA-1 ("hash 2") and HMAC over either.
#ifndef DGT_INTERNAL_H
#define DGT_INTERNAL_H

#include "nitro/mi.h"
#include "types.h"

#define DGT_HASH1_DIGEST_SIZE (128 / 8)
#define DGT_HASH2_DIGEST_SIZE (160 / 8)
#define DGT_HASH_BLOCK_SIZE (512 / 8)

typedef struct {
    union {
        struct {
            u32 a, b, c, d;
        };
        u32 state[4];
    };
    u64 length;
    union {
        u32 buffer32[16];
        u8 buffer8[64];
    };
} DGTHash1Context;

typedef struct {
    u32 h0, h1, h2, h3, h4;
    u32 Nl, Nh;
    int num;
    u32 data[64 / 4];
    int dummy[2];
} DGTHash2Context;

typedef void (*HashResetFunc)(void *ctx);
typedef void (*HashSetSourceFunc)(void *ctx, const void *input, u32 length);
typedef void (*HashGetDigestFunc)(void *ctx, void *digest);

// One hash for HMAC: its digest and block lengths, a context and digest buffer, and its functions.
typedef struct {
    const u32 dlength;
    const u32 blength;
    void *context;
    u8 *hash_buf;
    HashResetFunc HashReset;
    HashSetSourceFunc HashSetSource;
    HashGetDigestFunc HashGetDigest;
} DGTiHMACFuncs;

static inline void MI_CpuFill32(void *dest, u32 data, u32 size) {
    MIi_CpuClear32(data, dest, size);
}

void DGT_Hash1Reset(DGTHash1Context *ctx);
void DGT_Hash1SetSource(DGTHash1Context *ctx, const void *input, u32 length);
void DGT_Hash1GetDigest_R(void *digest, DGTHash1Context *ctx);
void DGT_Hash2Reset(DGTHash2Context *ctx);
void DGT_Hash2SetSource(DGTHash2Context *ctx, const u8 *input, u32 length);
void DGT_Hash2GetDigest(DGTHash2Context *ctx, u8 *digest);
void DGTi_hash2_arm4_small(DGTHash2Context *ctx, u8 *source, u32 length);
void HmacCalc(void *digest, void *bin_ptr, int bin_len, void *key_ptr, int key_len, DGTiHMACFuncs *funcs);

#endif
