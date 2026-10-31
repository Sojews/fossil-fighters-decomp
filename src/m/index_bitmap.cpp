#pragma ipa file
// MIndexBitmap and the indexed bitmap transfers.

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

// (The game makes the tasks through small inlines, which order their temporaries' stack slots.)
static inline MIndexBitmapLoadTransfer *MNewIndexBitmapLoadTransfer(MIndexBitmap *bitmap, const u8 *src, MPoint srcSize,
                                                                    MRect srcRect, MPoint dst, int palette, bool hflip,
                                                                    bool vflip) {
    return new MIndexBitmapLoadTransfer(bitmap, src, srcSize, srcRect, dst, palette, hflip, vflip);
}

static inline MIndexBitmapLoadTransfer *MNewIndexBitmapLoadTransfer(MIndexBitmap *bitmap, const u8 *src, MPoint srcSize,
                                                                    MPoint dst, int palette, bool hflip, bool vflip) {
    return new MIndexBitmapLoadTransfer(bitmap, src, srcSize, dst, palette, hflip, vflip);
}

void MIndexBitmap::Load(const u8 *src, MPoint srcSize, MRect srcRect, MPoint dst, int palette, bool hflip,
                        bool vflip) {
    gLooper->AddTransferTask(
        MNewIndexBitmapLoadTransfer(this, src, srcSize, srcRect, dst, palette, hflip, vflip));
}

void MIndexBitmap::Load(const u8 *src, MPoint srcSize, MPoint dst, int palette, bool hflip, bool vflip) {
    gLooper->AddTransferTask(MNewIndexBitmapLoadTransfer(this, src, srcSize, dst, palette, hflip, vflip));
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

// A pixel pair with the palette's offset added to the pixels that aren't 0.
#define MINDEX_BITMAP_PIXEL(px) \
    if ((u8)px != 0) { \
        px += lo; \
    } \
    if (px & 0xFF00) { \
        px += hi; \
    }

// Copies the source rect to the bitmap row by row, flipped as asked; with a palette, a pixel pair at a time (four to
// a loop), adding the palette's offset to each pixel that isn't 0.
// NONMATCHING: the game loads each rect width's left edge before its right one (and the first early return goes
// through a long branch).
void MIndexBitmapLoadTransfer::Transfer() {
    int srcSkip = mSrcSize.x;
    if (srcSkip != 0 && mSrcSize.y != 0) {
        u16 lo = mPalette << 4;
        u16 hi = lo << 8;
        const u16 *src;
        int width;
        if (!mVFlip) {
            src = (const u16 *) (mSrc + mSrcRect.left + srcSkip * mSrcRect.top);
            width = mSrcRect.GetWidth();
            srcSkip -= width;
        } else {
            src = (const u16 *) (mSrc + mSrcRect.left + srcSkip * (mSrcRect.bottom - 1));
            width = mSrcRect.GetWidth();
            srcSkip = -(srcSkip + width);
        }
        srcSkip >>= 1;
        if (!mHFlip) {
            u16 *dst = (u16 *) (mDstBase + mDst.x + mDstSize.x * mDst.y);
            int dstSkip = (mDstSize.x - width) >> 1;
            int pairs = width >> 1;
            u32 rows = mSrcRect.GetHeight();
            if (rows == 0) {
                return;
            }
            do {
                if (mPalette == 0) {
                    MCopy(src, dst, pairs * 2);
                    src += pairs;
                    dst += pairs;
                } else {
                    int n = (pairs + 3) >> 2;
                    switch (pairs & 3) {
                    case 0:
                        do {
                            u16 px;
                            px = *src;
                            MINDEX_BITMAP_PIXEL(px);
                            *dst = px;
                            src++;
                            dst++;
                    case 3:
                            px = *src;
                            MINDEX_BITMAP_PIXEL(px);
                            *dst = px;
                            src++;
                            dst++;
                    case 2:
                            px = *src;
                            MINDEX_BITMAP_PIXEL(px);
                            *dst = px;
                            src++;
                            dst++;
                    case 1:
                            px = *src;
                            MINDEX_BITMAP_PIXEL(px);
                            *dst = px;
                            src++;
                            dst++;
                        } while (--n != 0);
                    }
                }
                src += srcSkip;
                dst += dstSkip;
            } while (--rows != 0);
        } else {
            u16 *dst = (u16 *) (mDstBase + mDst.x + width - 1 + mDstSize.x * mDst.y);
            int dstSkip = (mDstSize.x + width) >> 1;
            int pairs = width >> 1;
            s32 rows = mSrcRect.GetHeight();
            if (rows == 0) {
                return;
            }
            do {
                int n = (pairs + 3) >> 2;
                switch (pairs & 3) {
                case 0:
                    do {
                        u16 px;
                        px = *src;
                        px = (px >> 8) | (u16)(px << 8);
                        MINDEX_BITMAP_PIXEL(px);
                        *dst = px;
                        src++;
                        dst--;
                case 3:
                        px = *src;
                        px = (px >> 8) | (u16)(px << 8);
                        MINDEX_BITMAP_PIXEL(px);
                        *dst = px;
                        src++;
                        dst--;
                case 2:
                        px = *src;
                        px = (px >> 8) | (u16)(px << 8);
                        MINDEX_BITMAP_PIXEL(px);
                        *dst = px;
                        src++;
                        dst--;
                case 1:
                        px = *src;
                        px = (px >> 8) | (u16)(px << 8);
                        MINDEX_BITMAP_PIXEL(px);
                        *dst = px;
                        src++;
                        dst--;
                    } while (--n != 0);
                }
                src += srcSkip;
                dst += dstSkip;
            } while (--rows != 0);
        }
    }
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
