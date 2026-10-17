// Text BG screens: tile maps made of 32x32-entry blocks (one to four of them, by the screen size), and the
// transfers that copy tile map rectangles into them during VBlank.
#ifndef M_TEXT_H
#define M_TEXT_H

#include "m/screen.h"

// A screen size's dimensions in blocks.
struct MTextSize {
    int width;
    int height;
    int blocks;
};

// The blocks to the right of and below each block of a screen size (the block itself at the edge).
struct MTextBlockLinks {
    int right;
    int down;
};

class MText : public MScreen {
public:
    MText(MDisplay *display, u32 screenBase, u32 base, int size, int bg);
    virtual ~MText() {}
    virtual void Update();

    static u32 GetVramSize(int size);
    static int GetRightBlock(int size, int block);
    static int GetDownBlock(int size, int block);

    void SetUnk30(u32 value, u32 extPaletteSlot);

    void Transfer(const u16 *src, MPoint srcSize, MRect srcRect, MPoint dst, int tile, int palette, bool hflip,
                  bool vflip);
    void Transfer(const u16 *src, MPoint srcSize, MPoint dst, int tile, int palette, bool hflip, bool vflip);
    void TransferSequence(MPoint srcSize, MRect srcRect, MPoint dst, int tile, int palette, bool hflip, bool vflip);
    void Fill(MRect rect, int tile, int palette, bool hflip, bool vflip);

    int mSize;      // 0x20
    int mWidth;     // 0x24: blocks
    int mHeight;    // 0x28: blocks
    int mBlocks;    // 0x2C
    u32 mUnk30;     // 0x30
};

// Copies a rectangle of a tile map into a text screen, adding a tile number and palette to every entry.
class MTextTransfer : public MTransfer {
public:
    MTextTransfer(MText *text, MPoint dst, const u16 *src, MPoint srcSize, MRect srcRect, int tile, int palette,
                  bool hflip, bool vflip) {
        mSrc = src;
        mSrcSize = srcSize;
        mSrcRect = srcRect;
        mDstBase = text->mBase;
        mDstSize = text->mSize;
        mDst = dst;
        mAttr = tile | (palette << 12);
        mFlip = (hflip ? 0x400 : 0) | (vflip ? 0x800 : 0);
    }
    MTextTransfer(MText *text, MPoint dst, const u16 *src, MPoint srcSize, int tile, int palette, bool hflip,
                  bool vflip) {
        mSrc = src;
        mSrcSize = srcSize;
        mSrcRect = MRect(MPoint(0, 0), srcSize);
        mDstBase = text->mBase;
        mDstSize = text->mSize;
        mDst = dst;
        mAttr = tile | (palette << 12);
        mFlip = (hflip ? 0x400 : 0) | (vflip ? 0x800 : 0);
    }
    virtual ~MTextTransfer();
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

// Fills a rectangle of a text screen with one entry.
class MTextFill : public MTransfer {
public:
    MTextFill(MText *text, MRect rect, int tile, int palette, bool hflip, bool vflip) {
        mDstBase = text->mBase;
        mRect = rect;
        mDstSize = text->mSize;
        mEntry = (palette << 12) | tile | (hflip ? 0x400 : 0) | (vflip ? 0x800 : 0);
    }
    virtual ~MTextFill();
    virtual void Transfer();

    u32 mDstBase;   // 0x10
    MRect mRect;    // 0x14
    int mDstSize;   // 0x24
    u16 mEntry;     // 0x28
};

#endif
