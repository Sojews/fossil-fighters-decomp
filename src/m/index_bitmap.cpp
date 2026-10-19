// MIndexBitmap and the indexed bitmap transfers. MIndexBitmapLoadTransfer::Transfer isn't decompiled yet.
// NONMATCHING: the static initializer (the game pools the size table after the type infos, at +0x58).

#include "m/bitmap.h"
#include "m/lcd.h"
#include "m/looper.h"
#include "m/transfer.h"

static MPoint sIndexBitmapSizes[4] = {
    MPoint(128, 128),
    MPoint(256, 256),
    MPoint(512, 256),
    MPoint(512, 512),
};

u32 MIndexBitmap::GetVramSize(int size) {
    return sIndexBitmapSizes[size].x * sIndexBitmapSizes[size].y;
}

MIndexBitmap::MIndexBitmap(MDisplay *display, u32 screenBase, u32 base, int size, int bg)
    : MAffineScreen(display, screenBase, base, sIndexBitmapSizes[size].x * sIndexBitmapSizes[size].y, size, bg) {
    mSize = size;
    mBitmapSize.x = sIndexBitmapSizes[size].x;
    mBitmapSize.y = sIndexBitmapSizes[size].y;
    mBmpCntBits.directColor = FALSE;
    mBmpCntBits.bitmap = TRUE;
}

// NONMATCHING: both Loads differ in register allocation only, like MText's transfer factories.
void MIndexBitmap::Load(const u8 *src, MPoint srcSize, MRect srcRect, MPoint dst, int palette, bool hflip,
                        bool vflip) {
    gLooper->AddTransferTask(
        new MIndexBitmapLoadTransfer(this, dst, src, srcSize, srcRect, palette, hflip, vflip));
}

void MIndexBitmap::Load(const u8 *src, MPoint srcSize, MPoint dst, int palette, bool hflip, bool vflip) {
    gLooper->AddTransferTask(new MIndexBitmapLoadTransfer(this, dst, src, srcSize, palette, hflip, vflip));
}

void MIndexBitmap::Fill(MRect rect, int color) {
    gLooper->AddTransferTask(new MIndexBitmapFillTransfer(this, mBitmapSize, rect, color));
}

void MIndexBitmapFillTransfer::Transfer() {
    if (mRect.GetWidth() == 0 || mRect.GetHeight() == 0) {
        return;
    }
    for (int y = mRect.top; y < mRect.bottom; y++) {
        MFill(mValue | (mValue << 16), (u8 *)mDstBase + mRect.left + mDstSize.x * y, mRect.GetWidth());
    }
}

MIndexBitmapLoadTransfer::~MIndexBitmapLoadTransfer() {
}
