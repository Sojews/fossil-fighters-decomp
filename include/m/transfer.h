// Tasks that copy or fill a block of memory once, by CPU or by DMA.
#ifndef M_TRANSFER_H
#define M_TRANSFER_H

#include "m/task.h"
#include "m/dma.h"
#include "nitro/mi.h"

// Copies by DMA when it's allowed and the size is a multiple of 32 bytes, otherwise by CPU.
inline void MCopy(const void *src, void *dest, u32 size) {
    if (!gDmaEnabled || gDmaChannelUsed[3] || (size & 0x1F)) {
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
            MI_DmaCopy16(M_DMA_NO, src, dest, size);
        } else {
            MI_DmaCopy32(M_DMA_NO, src, dest, size);
        }
    }
}

// Fills by DMA when it's allowed, otherwise by CPU.
inline void MFill(u32 value, void *dest, u32 size) {
    if (!gDmaEnabled || gDmaChannelUsed[3]) {
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
            MI_DmaFill16(M_DMA_NO, dest, value, size);
        } else {
            MI_DmaFill32(M_DMA_NO, dest, value, size);
        }
    }
}

class MSimpleTransfer : public MTransfer {
public:
    MSimpleTransfer(const void *src, void *dest, u32 size, BOOL freeSrc) {
        mSrc = src;
        mDest = dest;
        mSize = size;
        mFreeSrc = freeSrc;
    }
    virtual ~MSimpleTransfer() {}
    virtual void Transfer();

    const void *mSrc;   // 0x10
    void *mDest;        // 0x14
    u32 mSize;          // 0x18
    u8 mFreeSrc;        // 0x1C: delete[] mSrc afterwards
};

class MSimpleFill : public MTransfer {
public:
    MSimpleFill(u32 value, void *dest, u32 size) {
        mValue = value;
        mDest = dest;
        mSize = size;
    }
    virtual ~MSimpleFill() {}
    virtual void Transfer();

    u32 mValue;         // 0x10
    void *mDest;        // 0x14
    u32 mSize;          // 0x18
};

#endif
