// Memory copy and fill tasks.

#include "m/transfer.h"
#include "nitro/mi.h"

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

    MCopy(src, dest, size);
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

    MFill(value, dest, size);
}
