// Memory copy and fill tasks.

#include "m/transfer.h"
#include "nitro/mi.h"

#define DMA_NO 3

extern u8 data_020c0af0;    // DMA transfers allowed
extern u8 data_0214d43c[];

void MTransfer::Execute() {
    Transfer();
    Exit();
}

void MSimpleTransfer::Transfer() {
    const void *src;
    void *dest;
    u32 size;

    size = mSize;
    dest = mDest;
    src = mSrc;

    if (!data_020c0af0 || data_0214d43c[3] || (size & 0x1F)) {
        if (((u32)src | (u32)dest | size) & 3) {
            MIi_CpuCopy16(src, dest, size);
        } else if (size & 0x1F) {
            MIi_CpuCopy32(src, dest, size);
        } else {
            MIi_CpuCopyFast(src, dest, size);
        }
    } else {
        DC_FlushRange(src, size);
        DC_FlushRange(dest, size);
        if (((u32)src | (u32)dest | size) & 3) {
            MI_DmaCopy16(DMA_NO, src, dest, size);
        } else {
            MI_DmaCopy32(DMA_NO, src, dest, size);
        }
    }
    if (mFreeSrc) {
        delete[] (u8 *)mSrc;
    }
}

void MSimpleFill::Transfer() {
    u32 value;
    void *dest;
    u32 size;

    size = mSize;
    dest = mDest;
    value = mValue;

    if (!data_020c0af0 || data_0214d43c[3]) {
        if (((u32)dest | size) & 3) {
            MIi_CpuClear16(value, dest, size);
        } else if (size & 0x1F) {
            MIi_CpuClear32(value, dest, size);
        } else {
            MIi_CpuClearFast(value, dest, size);
        }
    } else {
        DC_FlushRange(dest, size);
        if (((u32)dest | size) & 3) {
            MI_DmaFill16(DMA_NO, dest, value, size);
        } else {
            MI_DmaFill32(DMA_NO, dest, value, size);
        }
    }
}
