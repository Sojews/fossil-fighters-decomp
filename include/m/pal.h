// Palette memory: hands out runs of 16-color palettes (or extended palettes) and loads them.
#ifndef M_PAL_H
#define M_PAL_H

#include "m/heap.h"

class MPal;

// An allocated palette run.
struct MPalBlock : public MHeapBlock {
    MPal *pal;          // 0x0C
    const void *data;   // 0x10: what was loaded into it; allocating the same data again shares the block
    u32 index;          // 0x14: the first palette number
    u32 unitSize;       // 0x18
    int refCount;       // 0x1C

    MTask *CreateLoadTask(const void *src, u32 start, u32 count);
    void Load(const void *src, u32 start, u32 count);
    MTask *CreateColorLoadTask(const void *src, u32 start, u32 count);
    void LoadColors(const void *src, u32 start, u32 count);
};

class MPal : public MHeap {
public:
    MPal(u32 base, void *extPalette);
    virtual ~MPal();

    MPalBlock *Alloc(u32 count, const void *data, BOOL fromEnd);
    void Free(MPalBlock *block);
    MTask *CreateLoadTask(const void *src, void *dest, u32 size, u8 unk);

    u32 mUnk38;         // 0x38
    void *mExtPalette;  // 0x3C
    u32 mExtended;      // 0x40
    u32 mUnitSize;      // 0x44: bytes per palette
};

// Copies palette data, mapping the extended palette VRAM in first if needed.
class MPalLoadTask : public MTransfer {
public:
    MPalLoadTask(void *extPalette, const void *src, void *dest, u32 size, u8 unk) {
        mExtPalette = extPalette;
        mSrc = src;
        mDest = dest;
        mSize = size;
        mUnk20 = unk;
    }
    virtual ~MPalLoadTask() {}
    virtual void Transfer();

    void *mExtPalette;  // 0x10
    const void *mSrc;   // 0x14
    void *mDest;        // 0x18
    u32 mSize;          // 0x1C
    u8 mUnk20;          // 0x20
};

#endif
