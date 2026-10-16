// Character (tile) memory: a heap over a display's BG or OBJ character VRAM, handing out runs of tiles.
#ifndef M_CHR_H
#define M_CHR_H

#include "m/heap.h"
#include "m/archive.h"

class MDisplay;

// An allocated run of tiles.
struct MChrBlock : public MHeapBlock {
    const void *key;    // 0x0C: the data loaded into it, when it's shared
    u32 unitSize;       // 0x10: bytes per tile
    u32 index;          // 0x14: the first tile, counted from the character base
    u32 tile;           // 0x18: the first tile number, in units of the tile boundary
    int refCount;       // 0x1C

    MTask *CreateLoadTask(const void *src, u32 start, u32 count, int palette);
    void Load(const void *src, u32 start, u32 count, int palette);
    void Load(MFileRef *file, u32 offset, u32 start, u32 unused, u32 count, int palette);
    MTask *CreateFillTask(int color, u32 start, u32 count);
    void Fill(int color, u32 start, u32 count);
};

class MChr : public MHeap {
public:
    MChr(MDisplay *display, int charBase, u32 unk44, u32 base, int mode, u32 units, int boundary);
    virtual ~MChr() {}

    static u32 GetVramSize(int mode, u32 units);

    MChrBlock *Alloc(u32 count, BOOL fromEnd);
    MChrBlock *Alloc(int mode, u32 count, BOOL fromEnd);
    MChrBlock *AllocShared(const void *data, u32 count, int palette, BOOL fromEnd);
    MChrBlock *AllocShared(int mode, const void *data, u32 count, int palette, BOOL fromEnd);
    void FreeShared(MChrBlock *block);

    MDisplay *mDisplay; // 0x38
    int mMode;          // 0x3C: 0: 16 colors, 1: 256 colors
    u32 mCharBase;      // 0x40: the BGCNT character base block
    u32 mUnk44;         // 0x44
    int mBoundary;      // 0x48: tiles per tile number step
};

// Adds a 16-color palette's offset to each nonzero pixel of 256-color tiles.
inline void MAddPaletteOffset(const u16 *src, u16 *dest, u32 size, int palette) {
    u16 lo = palette << 4;
    u16 hi = lo << 8;
    for (u32 n = size >> 1; n != 0; n--) {
        u16 px = *src++;
        if ((u8)px != 0) {
            px += lo;
        }
        if (px & 0xFF00) {
            px += hi;
        }
        *dest++ = px;
    }
}

class MChr256Filter : public MArchiveFilter {
public:
    MChr256Filter(int palette) { mPalette = palette; }
    virtual ~MChr256Filter() {}
    virtual void Filter(const void *src, void *dest, u32 size);

    int mPalette;   // 0x04
};

// Copies 256-color tiles, adding a 16-color palette's offset to them.
class MChr256Transfer : public MTransfer {
public:
    MChr256Transfer(const void *src, void *dest, u32 size, int palette) {
        mSrc = src;
        mDest = dest;
        mSize = size;
        mPalette = palette;
    }
    virtual ~MChr256Transfer() {}
    virtual void Transfer();

    const void *mSrc;   // 0x10
    void *mDest;        // 0x14
    u32 mSize;          // 0x18
    int mPalette;       // 0x1C
};

#endif
