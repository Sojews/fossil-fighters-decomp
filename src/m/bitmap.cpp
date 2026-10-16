// MDirectBitmap, MIndexBitmap and the indexed bitmap transfers. MIndexBitmapLoadTransfer::Transfer isn't
// decompiled yet.

#include "m/bitmap.h"
#include "m/lcd.h"
#include "m/looper.h"
#include "m/transfer.h"

static MPoint sDirectBitmapSizes[4] = {
    MPoint(128, 128),
    MPoint(256, 256),
    MPoint(512, 256),
    MPoint(512, 512),
};

u32 MDirectBitmap::GetVramSize(int size) {
    return sDirectBitmapSizes[size].x * 2 * sDirectBitmapSizes[size].y;
}

MDirectBitmap::MDirectBitmap(MDisplay *display, u32 screenBase, u32 base, int size, int bg)
    : MAffineScreen(display, screenBase, base, sDirectBitmapSizes[size].x * 2 * sDirectBitmapSizes[size].y, size,
                    bg) {
    mSize = size;
    mBitmapSize.x = sDirectBitmapSizes[size].x;
    mBitmapSize.y = sDirectBitmapSizes[size].y;
    mBmpCntBits.directColor = TRUE;
    mBmpCntBits.bitmap = TRUE;
}

// NONMATCHING: the bank compare's operands are swapped.
// Captures the screen into this bitmap, which must be 256x256 and in one of VRAM banks A-D.
void MDirectBitmap::Capture(int srcA) {
    u32 bank;

    if (mSize != 1) {
        OS_Terminate();
    }
    for (bank = 1; bank <= 4; bank++) {
        u32 base = mBase;
        if (base == gLcd->GetBankAddress(bank)) {
            goto found;
        }
    }
    OS_Terminate();
found:
    gLcd->StartCapture(0, srcA, 0, 3, 0, bank, 0, 16, 0);
}

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
