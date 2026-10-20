// MIndexBitmap and the indexed bitmap transfers. MIndexBitmapLoadTransfer::Transfer isn't decompiled yet.

#include "m/bitmap.h"
#include "game/dino_static_init.h"
#include "m/lcd.h"
#include "m/looper.h"
#include "m/transfer.h"

// The sizes: (128, 128), (256, 256), (512, 256), (512, 512), set by the file's static initialization.
extern "C" MPoint sIndexBitmapSizes[4];

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

// The file's static initialization (see dino_static_init.h): element 0 through the pool, the others directly.
extern "C" u32 data_020c2620[];
static struct IndexBitmapStaticInit {
    IndexBitmapStaticInit() {
        DINO_POOL_WORD(data_020c2620, 0x58) = 128;
        DINO_POOL_WORD(data_020c2620, 0x5c) = 128;
        volatile MPoint *p = &sIndexBitmapSizes[1];
        p->x = 256;
        p->y = 256;
        p = &sIndexBitmapSizes[2];
        p->x = 512;
        p->y = 256;
        p = &sIndexBitmapSizes[3];
        p->x = 512;
        p->y = 512;
    }
} sIndexBitmapStaticInit;
