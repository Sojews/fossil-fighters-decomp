// NitroSDK DGT: MD5.

#include "dgt_internal.h"

static void ProcessBlock(DGTHash1Context *ctx);

void DGT_Hash1Reset(DGTHash1Context *ctx) {
    ctx->a = 0x67452301;
    ctx->b = 0xEFCDAB89;
    ctx->c = 0x98BADCFE;
    ctx->d = 0x10325476;
    ctx->length = 0;
}

void DGT_Hash1SetSource(DGTHash1Context *ctx, const void *input, u32 length) {
    int buffer_len = ctx->length % 64;
    ctx->length += length;
    int free_bytes = 64 - buffer_len;
    if (free_bytes > length) {
        if (length > 0) {
            MI_CpuCopy8(input, ctx->buffer8 + buffer_len, length);
        }
    } else {
        MI_CpuCopy8(input, ctx->buffer8 + buffer_len, free_bytes);
        ProcessBlock(ctx);
        u8 *inputp = (u8 *) input + free_bytes;
        length -= free_bytes;
        for (int blocks = length / 64; blocks > 0; blocks--) {
            MI_CpuCopy8(inputp, ctx->buffer8, 64);
            inputp += 64;
            ProcessBlock(ctx);
        }
        length %= 64;
        if (length > 0) {
            MI_CpuCopy8(inputp, ctx->buffer8, length);
        }
    }
}

// Pads the message with its length, and clears the context.
void DGT_Hash1GetDigest_R(void *digest, DGTHash1Context *ctx) {
    u64 len_bits = ctx->length * 8;
    static u8 padding = 0x80;
    DGT_Hash1SetSource(ctx, &padding, 1);
    u32 buffer_len = ctx->length & 63;
    u32 free_bytes = 64 - buffer_len;
    if (free_bytes < 8) {
        MI_CpuFill8(ctx->buffer8 + buffer_len, 0, free_bytes);
        ProcessBlock(ctx);
        buffer_len = 0;
        free_bytes = 64;
    }
    if (free_bytes > 8) {
        MI_CpuFill8(ctx->buffer8 + buffer_len, 0, free_bytes - 8);
    }
    *(u64 *) (ctx->buffer8 + 56) = len_bits;
    ProcessBlock(ctx);
    MI_CpuCopy8(ctx->state, digest, 16);
    MI_CpuFill8(ctx, 0, sizeof(DGTHash1Context));
}

#define ROTL(x, amt) (((x) << (amt)) | ((x) >> (32 - (amt))))
#define FUNC_CH(b, c, d) ((b & c) | (~b & d))
#define FUNC_CH2(b, c, d) ((b & d) | (c & ~d))
#define FUNC_SUM(b, c, d) (b ^ c ^ d)
#define FUNC_R4(b, c, d) (c ^ (b | ~d))

static inline u32 CalcRound1(u32 a, u32 b, u32 c, u32 d, u32 x, u32 t, u32 s) {
    return b + ROTL(a + FUNC_CH(b, c, d) + x + t, s);
}

static inline u32 CalcRound2(u32 a, u32 b, u32 c, u32 d, u32 x, u32 t, u32 s) {
    return b + ROTL(a + FUNC_CH2(b, c, d) + x + t, s);
}

static inline u32 CalcRound3(u32 a, u32 b, u32 c, u32 d, u32 x, u32 t, u32 s) {
    return b + ROTL(a + FUNC_SUM(b, c, d) + x + t, s);
}

static inline u32 CalcRound4(u32 a, u32 b, u32 c, u32 d, u32 x, u32 t, u32 s) {
    return b + ROTL(a + FUNC_R4(b, c, d) + x + t, s);
}

static void ProcessBlock(DGTHash1Context *ctx) {
    // The sines, and the order the last three rounds take the words in.
    static u32 t[64] = {
        0xD76AA478, 0xE8C7B756, 0x242070DB, 0xC1BDCEEE, 0xF57C0FAF, 0x4787C62A, 0xA8304613, 0xFD469501,
        0x698098D8, 0x8B44F7AF, 0xFFFF5BB1, 0x895CD7BE, 0x6B901122, 0xFD987193, 0xA679438E, 0x49B40821,
        0xF61E2562, 0xC040B340, 0x265E5A51, 0xE9B6C7AA, 0xD62F105D, 0x02441453, 0xD8A1E681, 0xE7D3FBC8,
        0x21E1CDE6, 0xC33707D6, 0xF4D50D87, 0x455A14ED, 0xA9E3E905, 0xFCEFA3F8, 0x676F02D9, 0x8D2A4C8A,
        0xFFFA3942, 0x8771F681, 0x6D9D6122, 0xFDE5380C, 0xA4BEEA44, 0x4BDECFA9, 0xF6BB4B60, 0xBEBFBC70,
        0x289B7EC6, 0xEAA127FA, 0xD4EF3085, 0x04881D05, 0xD9D4D039, 0xE6DB99E5, 0x1FA27CF8, 0xC4AC5665,
        0xF4292244, 0x432AFF97, 0xAB9423A7, 0xFC93A039, 0x655B59C3, 0x8F0CCC92, 0xFFEFF47D, 0x85845DD1,
        0x6FA87E4F, 0xFE2CE6E0, 0xA3014314, 0x4E0811A1, 0xF7537E82, 0xBD3AF235, 0x2AD7D2BB, 0xEB86D391,
    };
    static u32 k[48] = {
        1, 6, 11, 0, 5, 10, 15, 4, 9, 14, 3, 8, 13, 2, 7, 12,
        5, 8, 11, 14, 1, 4, 7, 10, 13, 0, 3, 6, 9, 12, 15, 2,
        0, 7, 14, 5, 12, 3, 10, 1, 8, 15, 6, 13, 4, 11, 2, 9,
    };
    const u32 *kp;
    u32 a = ctx->state[0];
    u32 b = ctx->state[1];
    u32 c = ctx->state[2];
    u32 d = ctx->state[3];
    const u32 *x = ctx->buffer32;
    const u32 *xp = x;
    const u32 *tp = t;
    int i;

    for (i = 0; i < 4; i++) {
        a = CalcRound1(a, b, c, d, *xp++, *tp++, 7);
        d = CalcRound1(d, a, b, c, *xp++, *tp++, 12);
        c = CalcRound1(c, d, a, b, *xp++, *tp++, 17);
        b = CalcRound1(b, c, d, a, *xp++, *tp++, 22);
    }
    kp = &k[0];
    for (i = 0; i < 4; i++) {
        a = CalcRound2(a, b, c, d, x[*kp++], *tp++, 5);
        d = CalcRound2(d, a, b, c, x[*kp++], *tp++, 9);
        c = CalcRound2(c, d, a, b, x[*kp++], *tp++, 14);
        b = CalcRound2(b, c, d, a, x[*kp++], *tp++, 20);
    }
    for (i = 0; i < 4; i++) {
        a = CalcRound3(a, b, c, d, x[*kp++], *tp++, 4);
        d = CalcRound3(d, a, b, c, x[*kp++], *tp++, 11);
        c = CalcRound3(c, d, a, b, x[*kp++], *tp++, 16);
        b = CalcRound3(b, c, d, a, x[*kp++], *tp++, 23);
    }
    for (i = 0; i < 4; i++) {
        a = CalcRound4(a, b, c, d, x[*kp++], *tp++, 6);
        d = CalcRound4(d, a, b, c, x[*kp++], *tp++, 10);
        c = CalcRound4(c, d, a, b, x[*kp++], *tp++, 15);
        b = CalcRound4(b, c, d, a, x[*kp++], *tp++, 21);
    }
    ctx->state[0] += a;
    ctx->state[1] += b;
    ctx->state[2] += c;
    ctx->state[3] += d;
}
