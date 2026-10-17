// NitroSDK MI: run-length decompression written 16 bits at a time (for VRAM).

#include "nitro/mi.h"

void MI_UncompressRL16(const void *srcp, void *destp) {
    const u8 *pSrc = srcp;
    u16 *pDst = destp;
    u16 destTmp = 0;
    s32 destCount = *(const u32 *) pSrc >> 8;
    u32 shift = 0;
    pSrc += 4;
    while (destCount > 0) {
        u32 flags = *pSrc++;
        s32 length = flags & 0x7f;
        if (!(flags & 0x80)) {
            length++;
            destCount -= length;
            do {
                destTmp |= *pSrc++ << shift;
                shift ^= 8;
                if (shift == 0) {
                    *pDst++ = destTmp;
                    destTmp = 0;
                }
            } while (--length > 0);
        } else {
            u32 srcTmp;
            length += 3;
            destCount -= length;
            srcTmp = *pSrc++;
            do {
                destTmp |= srcTmp << shift;
                shift ^= 8;
                if (shift == 0) {
                    *pDst++ = destTmp;
                    destTmp = 0;
                }
            } while (--length > 0);
        }
    }
}
