// MChr16Fill and MChr256Transfer. MChr16Fill::Transfer isn't decompiled yet.

#include "m/canvas.h"
#include "m/chr.h"
#include "m/transfer.h"

void MChr256Transfer::Transfer() {
    if (mPalette != 0) {
        MAddPaletteOffset((const u16 *)mSrc, (u16 *)mDest, mSize, mPalette);
    } else {
        MCopy(mSrc, mDest, mSize);
    }
}

MChr16Fill::~MChr16Fill() {
}
