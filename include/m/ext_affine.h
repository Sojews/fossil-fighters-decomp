// Extended affine BG screens: affine layers with 16-bit tile map entries, which can use an extended palette.
#ifndef M_EXT_AFFINE_H
#define M_EXT_AFFINE_H

#include "m/affine_screen.h"
#include "m/looper.h"

class MPal;

class MExtAffine : public MAffineScreen {
public:
    MExtAffine(MDisplay *display, u32 screenBase, u32 base, int size, int bg);
    virtual ~MExtAffine() {}

    static u32 GetVramSize(int size);
    static u16 *GetEntry(u16 *base, int size, int x, int y);
    static MPoint GetDimensions(int size);

    int GetSize();
    void SetPalette(MPal *pal);
    MPal *GetPalette();

    void Transfer(const u16 *src, MPoint srcSize, MPoint dst, int tile, int palette, bool hflip, bool vflip);
    void Fill(MRect rect, int tile, int palette, bool hflip, bool vflip);

    int mSize;      // 0x34
    MPal *mPal;     // 0x38
};

// Copies a rectangle of a tile map into an extended affine screen, adding a tile number and palette to every entry.
class MExtAffineTransfer : public MTransfer {
public:
    MExtAffineTransfer(MExtAffine *screen, const u16 *src, MPoint srcSize, MPoint dst, int tile, int palette, bool hflip,
                       bool vflip)
        : mSrc(src), mSrcSize(srcSize), mSrcRect(MPoint(0, 0), srcSize) {
        mDstBase = screen->mBase;
        mDstSize = screen->GetSize();
        mDst = dst;
        mAttr = tile | (palette << 12);
        mFlip = (hflip ? 0x400 : 0) | (vflip ? 0x800 : 0);
    }
    virtual ~MExtAffineTransfer();
    virtual void Transfer();

    const u16 *mSrc;    // 0x10
    MPoint mSrcSize;    // 0x14
    MRect mSrcRect;     // 0x1C
    u32 mDstBase;       // 0x2C
    int mDstSize;       // 0x30
    MPoint mDst;        // 0x34
    u16 mAttr;          // 0x3C: tile number and palette
    u16 mFlip;          // 0x3E: the flip bits
};

// Fills a rectangle of an extended affine screen with one entry.
class MExtAffineFill : public MTransfer {
public:
    MExtAffineFill(MExtAffine *screen, MRect rect, int tile, int palette, bool hflip, bool vflip) {
        mDstBase = screen->mBase;
        mRect = rect;
        mDstSize = screen->GetSize();
        mEntry = (hflip ? 0x400 : 0) | ((palette << 12) | tile) | (vflip ? 0x800 : 0);
    }
    virtual ~MExtAffineFill();
    virtual void Transfer();

    u32 mDstBase;   // 0x10
    MRect mRect;    // 0x14
    int mDstSize;   // 0x24
    u16 mEntry;     // 0x28
};

#endif
