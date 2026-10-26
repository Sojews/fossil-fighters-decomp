// Palette memory: hands out runs of 16-color palettes (or extended palettes) and loads them.
#ifndef M_PAL_H
#define M_PAL_H

#include <vector>
#include "m/heap.h"
#include "m/anime.h"

class MPal;

// An allocated palette run.
struct MPalUnit : public MHeapBlock {
    MPal *pal;          // 0x0C
    const void *data;   // 0x10: what was loaded into it; allocating the same data again shares the block
    u32 index;          // 0x14: the first palette number
    u32 unitSize;       // 0x18
    int refCount;       // 0x1C

    void Free();
    MTask *CreateLoadTask(const void *src, u32 start, u32 count);
    void Load(const void *src, u32 start, u32 count);
    MTask *CreateColorLoadTask(const void *src, u32 start, u32 count);
    void LoadColors(const void *src, u32 start, u32 count);
};

class MPal : public MHeap {
public:
    MPal(u32 base, void *extPalette);
    virtual ~MPal();

    MPalUnit *Alloc(u32 count, const void *data, BOOL fromEnd);
    void Free(MPalUnit *block);
    MTask *CreateLoadTask(const void *src, void *dest, u32 size, u8 unk);

    u32 mUnk38;         // 0x38
    void *mExtPalette;  // 0x3C
    u32 mExtended;      // 0x40
    u32 mUnitSize;      // 0x44: bytes per palette
};

// Animates a run of palettes through a list of frames (palettes or solid colors), cross-fading between them.
class MPalAnime : public MAnime {
public:
    struct Frame {
        Frame() {}
        Frame(const Frame &o) : data(o.data), owned(o.owned), color(o.color) {}

        const u16 *data;    // the colors, or NULL for a solid color
        s16 owned;          // data was made here (a blend) and is freed with the anime
        u16 color;
    };

    MPalAnime(MPalUnit *block, u32 start, u32 count);
    virtual ~MPalAnime();
    virtual void Apply(int step);

    void AddFrame(const u16 *data);
    void AddColor(u16 color);
    // A frame of the colors blended toward a color (weight 0-32).
    void AddBlend(const u16 *data, u16 color, int weight);
    // Loops back to the first frame.
    void SetLoop();
    // Loads only these colors each step.
    void SetLoadRange(u32 start, u32 count);

    std::vector<Frame> mFrames; // 0x30
    MPalUnit *mBlock;          // 0x3C
    u32 mStart;                 // 0x40: the first palette of the run
    u32 mCount;                 // 0x44: palettes
    u32 mColorCount;            // 0x48
    u8 mLoop;                   // 0x4C
    u16 *mColors;               // 0x50: the current step
    u32 mLoadStart;             // 0x54
    u32 mLoadCount;             // 0x58: 0: load the whole run
};

// (Copied member by member, but the game moves it with vector's trivial paths.)
namespace Metrowerks {
template <> struct is_trivial<MPalAnime::Frame> {
    static const bool value = true;
};
template <> struct has_trivial_copy_ctor<MPalAnime::Frame> {
    static const bool value = true;
};
} // namespace Metrowerks

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
    virtual ~MPalLoadTask();
    virtual void Transfer();

    void *mExtPalette;  // 0x10
    const void *mSrc;   // 0x14
    void *mDest;        // 0x18
    u32 mSize;          // 0x1C
    u8 mUnk20;          // 0x20
};

#endif
