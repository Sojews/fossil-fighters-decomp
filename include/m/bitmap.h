// Bitmap BG screens: an affine screen whose map is a bitmap, either direct color (16 bits a pixel) or indexed
// (8 bits a pixel), and the transfers that draw into an indexed one.
#ifndef M_BITMAP_H
#define M_BITMAP_H

#include "m/affine_screen.h"

class MDirectBitmap : public MAffineScreen {
public:
    MDirectBitmap(MDisplay *display, u32 screenBase, u32 base, int size, int bg);
    virtual ~MDirectBitmap() {}

    static u32 GetVramSize(int size);

    void Capture(int srcA);

    int mSize;              // 0x34
    MPoint mBitmapSize;     // 0x38: pixels
};

class MIndexBitmap : public MAffineScreen {
public:
    MIndexBitmap(MDisplay *display, u32 screenBase, u32 base, int size, int bg);
    virtual ~MIndexBitmap() {}

    static u32 GetVramSize(int size);

    void Load(const u8 *src, MPoint srcSize, MRect srcRect, MPoint dst, int palette, bool hflip, bool vflip);
    void Load(const u8 *src, MPoint srcSize, MPoint dst, int palette, bool hflip, bool vflip);
    void Fill(MRect rect, int color);

    int mSize;              // 0x34
    MPoint mBitmapSize;     // 0x38: pixels
};

// Copies a rectangle of an indexed bitmap into an indexed bitmap screen, adding a 16-color palette's offset to
// every pixel and flipping it if asked.
class MIndexBitmapLoadTransfer : public MTransfer {
public:
    MIndexBitmapLoadTransfer(MIndexBitmap *bitmap, MPoint dst, const u8 *src, MPoint srcSize, MRect srcRect,
                             int palette, bool hflip, bool vflip) {
        mSrc = src;
        mSrcSize = srcSize;
        mSrcRect = srcRect;
        mDstBase = bitmap->mBase;
        mDstSize = bitmap->mBitmapSize;
        mDst = dst;
        mPalette = palette;
        mHFlip = hflip;
        mVFlip = vflip;
    }
    MIndexBitmapLoadTransfer(MIndexBitmap *bitmap, MPoint dst, const u8 *src, MPoint srcSize, int palette, bool hflip,
                             bool vflip) {
        mSrc = src;
        mSrcSize = srcSize;
        mSrcRect = MRect(MPoint(0, 0), srcSize);
        mDstBase = bitmap->mBase;
        mDstSize = bitmap->mBitmapSize;
        mDst = dst;
        mPalette = palette;
        mHFlip = hflip;
        mVFlip = vflip;
    }
    virtual ~MIndexBitmapLoadTransfer() {}
    virtual void Transfer();

    const u8 *mSrc;     // 0x10
    MPoint mSrcSize;    // 0x14
    MRect mSrcRect;     // 0x1C
    u32 mDstBase;       // 0x2C
    MPoint mDstSize;    // 0x30
    MPoint mDst;        // 0x38
    int mPalette;       // 0x40
    u8 mHFlip;          // 0x44
    u8 mVFlip;          // 0x45
};

// Fills a rectangle of an indexed bitmap screen with one color.
class MIndexBitmapFillTransfer : public MTransfer {
public:
    MIndexBitmapFillTransfer(MIndexBitmap *bitmap, MPoint dstSize, MRect rect, int color) {
        mDstBase = bitmap->mBase;
        mDstSize = dstSize;
        mRect = rect;
        mValue = (color << 8) | color;
    }
    virtual ~MIndexBitmapFillTransfer() {}
    virtual void Transfer();

    u32 mDstBase;       // 0x10
    MPoint mDstSize;    // 0x14
    MRect mRect;        // 0x1C
    u16 mValue;         // 0x2C: the color in both bytes
};

#endif
