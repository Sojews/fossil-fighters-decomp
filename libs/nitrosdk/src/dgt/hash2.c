// NitroSDK DGT: SHA-1 (the blocks themselves are hashed by an ARM assembly routine).

#include "dgt_internal.h"

static int DGTi_OverlayTableMode = 0;
static void (*DGTi_Hash2ProcessMessageBlockFunc)(DGTHash2Context *ctx, u8 *source, u32 length) = DGTi_hash2_arm4_small;

void DGT_Hash2Reset(DGTHash2Context *ctx) {
    ctx->h0 = 0x67452301;
    ctx->h1 = 0xEFCDAB89;
    ctx->h2 = 0x98BADCFE;
    ctx->h3 = 0x10325476;
    ctx->h4 = 0xC3D2E1F0;
    ctx->Nl = 0;
    ctx->Nh = 0;
    ctx->num = 0;
}

void DGT_Hash2SetSource(DGTHash2Context *ctx, const u8 *input, u32 length) {
    u8 *buffer8 = (u8 *) ctx->data;
    if (length == 0) {
        return;
    }
    u32 new_low = ctx->Nl + (length << 3);
    if (new_low < ctx->Nl) {
        ctx->Nh++;
    }
    ctx->Nh += length >> 29;
    ctx->Nl = new_low;
    if (ctx->num != 0) {
        if (ctx->num + length >= 64) {
            u32 copy_bytes = 64 - ctx->num;
            MI_CpuCopy8(input, buffer8 + ctx->num, copy_bytes);
            length -= copy_bytes;
            input += copy_bytes;
            DGTi_Hash2ProcessMessageBlockFunc(ctx, buffer8, 64);
            ctx->num = 0;
        } else {
            MI_CpuCopy8(input, buffer8 + ctx->num, length);
            ctx->num += length;
            return;
        }
    }
    if (length >= 64) {
        int whole_blocks_len = length & ~63;
        length -= whole_blocks_len;
        if (((u32) input & 3) == 0) {
            DGTi_Hash2ProcessMessageBlockFunc(ctx, (u8 *) input, whole_blocks_len);
            input += whole_blocks_len;
        } else {
            do {
                MI_CpuCopy8(input, buffer8, 64);
                input += 64;
                DGTi_Hash2ProcessMessageBlockFunc(ctx, buffer8, 64);
                whole_blocks_len -= 64;
            } while (whole_blocks_len > 0);
        }
    }
    ctx->num = length;
    if (length > 0) {
        MI_CpuCopy8(input, buffer8, length);
    }
}

static inline void storeBE32(u8 *ptr, u32 val) {
    ptr[0] = (u8) (val >> 24);
    ptr[1] = (u8) (val >> 16);
    ptr[2] = (u8) (val >> 8);
    ptr[3] = (u8) val;
}

static inline void storeBE32_R(u8 *ptr, u32 val) {
    ptr[3] = (u8) val;
    ptr[2] = (u8) (val >> 8);
    ptr[1] = (u8) (val >> 16);
    ptr[0] = (u8) (val >> 24);
}

void DGT_Hash2GetDigest(DGTHash2Context *ctx, u8 *digest) {
    u32 *buffer32 = ctx->data;
    s32 idx32, len;
    len = ctx->num;
    idx32 = len >> 2;
    if ((len & 3) == 0) {
        buffer32[idx32] = 0;
    }
    u8 *buffer8 = (u8 *) ctx->data;
    buffer8[len] = 0x80;
    len++;
    while ((len & 3) != 0) {
        buffer8[len] = 0;
        len++;
    }
    idx32++;
    if (ctx->num >= 56) {
        while (idx32 < 16) {
            buffer32[idx32] = 0;
            idx32++;
        }
        DGTi_Hash2ProcessMessageBlockFunc(ctx, (u8 *) buffer32, 64);
        idx32 = 0;
    }
    while (idx32 < 14) {
        buffer32[idx32] = 0;
        idx32++;
    }
    storeBE32_R(buffer8 + 60, ctx->Nl);
    storeBE32_R(buffer8 + 56, ctx->Nh);
    DGTi_Hash2ProcessMessageBlockFunc(ctx, (u8 *) buffer32, 64);
    storeBE32(digest, ctx->h0);
    storeBE32(digest + 4, ctx->h1);
    storeBE32(digest + 8, ctx->h2);
    storeBE32(digest + 12, ctx->h3);
    storeBE32(digest + 16, ctx->h4);
    ctx->num = 0;
    // (Clears the pointer rather than the context.)
    MI_CpuFill32(&ctx, 0, sizeof(ctx));
}

void DGT_Hash2CalcHmac(void *digest, void *bin_ptr, int bin_len, void *key_ptr, int key_len) {
    DGTHash2Context sha1_ctx;
    u8 sha1_buf[DGT_HASH2_DIGEST_SIZE];
    DGTiHMACFuncs hash2func = {DGT_HASH2_DIGEST_SIZE, DGT_HASH_BLOCK_SIZE};
    hash2func.context = &sha1_ctx;
    hash2func.hash_buf = sha1_buf;
    hash2func.HashReset = (HashResetFunc) DGT_Hash2Reset;
    hash2func.HashSetSource = (HashSetSourceFunc) DGT_Hash2SetSource;
    hash2func.HashGetDigest = (HashGetDigestFunc) DGT_Hash2GetDigest;
    HmacCalc(digest, bin_ptr, bin_len, key_ptr, key_len, &hash2func);
}
