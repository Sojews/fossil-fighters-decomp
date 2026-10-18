// NitroSDK DGT: HMAC (RFC 2104) over a hash.

#include "dgt_internal.h"

void HmacCalc(void *digest, void *bin_ptr, int bin_len, void *key_ptr, int key_len, DGTiHMACFuncs *funcs) {
    if (digest == NULL || bin_ptr == NULL || bin_len == 0 || key_ptr == NULL || key_len == 0 || funcs == NULL) {
        return;
    }
    u8 *kp = (u8 *) key_ptr;
    u8 hashed_key[DGT_HASH_BLOCK_SIZE];
    if (key_len > (int) funcs->blength) {
        funcs->HashReset(funcs->context);
        funcs->HashSetSource(funcs->context, key_ptr, key_len);
        funcs->HashGetDigest(funcs->context, hashed_key);
        kp = hashed_key;
        key_len = funcs->dlength;
    }
    int i;
    u8 key_ipad[DGT_HASH_BLOCK_SIZE];
    for (i = 0; i < key_len; i++) {
        key_ipad[i] = kp[i] ^ 0x36;
    }
    for (; i < (int) funcs->blength; i++) {
        key_ipad[i] = 0x36;
    }
    funcs->HashReset(funcs->context);
    funcs->HashSetSource(funcs->context, key_ipad, funcs->blength);
    funcs->HashSetSource(funcs->context, bin_ptr, bin_len);
    funcs->HashGetDigest(funcs->context, funcs->hash_buf);
    u8 key_opad[DGT_HASH_BLOCK_SIZE];
    for (i = 0; i < key_len; i++) {
        key_opad[i] = kp[i] ^ 0x5C;
    }
    for (; i < (int) funcs->blength; i++) {
        key_opad[i] = 0x5C;
    }
    funcs->HashReset(funcs->context);
    funcs->HashSetSource(funcs->context, key_opad, funcs->blength);
    funcs->HashSetSource(funcs->context, funcs->hash_buf, funcs->dlength);
    funcs->HashGetDigest(funcs->context, digest);
}
