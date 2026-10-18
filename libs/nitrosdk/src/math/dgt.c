// NitroSDK MATH: MD5 of a buffer, through DGT.

#include "../dgt/dgt_internal.h"

void MATH_CalcMD5(void *digest, const void *data, u32 dataLength) {
    DGTHash1Context context;

    DGT_Hash1Reset(&context);
    DGT_Hash1SetSource(&context, (u8 *) data, dataLength);
    DGT_Hash1GetDigest_R((u8 *) digest, &context);
}
