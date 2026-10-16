// Tasks that copy or fill a block of memory once, by CPU or by DMA.
#ifndef M_TRANSFER_H
#define M_TRANSFER_H

#include "m/task.h"

class MSimpleTransfer : public MTransfer {
public:
    virtual ~MSimpleTransfer() {}
    virtual void Transfer();

    const void *mSrc;   // 0x10
    void *mDest;        // 0x14
    u32 mSize;          // 0x18
    u8 mFreeSrc;        // 0x1C: delete[] mSrc afterwards
};

class MSimpleFill : public MTransfer {
public:
    virtual ~MSimpleFill() {}
    virtual void Transfer();

    u32 mValue;         // 0x10
    void *mDest;        // 0x14
    u32 mSize;          // 0x18
};

#endif
