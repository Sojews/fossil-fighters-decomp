// Sprites: OAM entries, the heaps that hand out OAM entries and affine parameter slots, and the OBJ system of a
// display (OBJ character memory, palettes and a shadow copy of OAM written to the real one each frame).
#ifndef M_OBJ_H
#define M_OBJ_H

#include "m/heap.h"
#include "m/pal.h"
#include "m/chr.h"
#include "m/geometry.h"

class MDisplay;

// OBJ dimensions by size and shape (the shape 3 entries are unused), in tiles and in pixels.
extern MPoint gObjTileSizes[4][4];
extern MPoint gObjSizes[4][4];

// One OAM entry (attributes 0-2 and the affine parameter word that shares its last halfword).
struct MOamEntry {
    union {
        u32 attr01;
        struct {
            u32 y : 8;
            u32 objMode : 2;    // 0: normal, 1: affine, 2: hidden, 3: affine, double size
            u32 gfxMode : 2;
            u32 mosaic : 1;
            u32 colors256 : 1;
            u32 shape : 2;
            u32 x : 9;
            u32 : 3;
            u32 hflip : 1;
            u32 vflip : 1;
            u32 size : 2;
        };
        struct {
            u32 : 25;
            u32 affineParam : 5;
            u32 : 2;
        };
    };
    union {
        u16 attr2;
        struct {
            u32 tile : 10;
            u32 priority : 2;
            u32 palette : 4;
        };
    };

    void Clear() {
        attr01 = 0;
        attr2 = 0;
        objMode = 2;
    }
    void SetCoord(MPoint pos) {
        x = pos.x;
        y = pos.y;
    }
    // shape: size * 4 + shape
    int GetShape() const { return shape | (size << 2); }
    void SetFlip(BOOL h, BOOL v) {
        hflip = h;
        vflip = v;
    }
    void SetShape(int shape) {
        this->shape = shape;
        size = shape >> 2;
    }
    void SetPalette(MPalBlock *pal);
};

// A run of OAM entries.
struct MOamUnit : public MHeapBlock {
    typedef MOamEntry Entry;

    u32 count;  // 0x0C

    MOamEntry *GetEntries() { return (MOamEntry *)addr; }
    void SetMode(int mode);
    void SetPriority(int priority);
    void Clear();
    void Hide();
    void Show();
};

class MOam : public MHeap {
public:
    MOam(u32 base, u32 size);
    virtual ~MOam();

    MOamUnit *Alloc(u32 count, BOOL fromEnd);
    void Free(MOamUnit *block);
};

// A run of affine parameter slots (32 bytes of OAM each).
struct MAfnBlock : public MHeapBlock {
    u32 index;  // 0x0C: the first slot
};

class MAfn : public MHeap {
public:
    MAfn(u32 base, u32 size);
    virtual ~MAfn();

    MAfnBlock *Alloc(u32 count, BOOL fromEnd);
    void Free(MAfnBlock *block);
};

class MObj {
public:
    MObj(MDisplay *display, u32 oam, u32 chrBase, u32 chrSize, u32 palBase, u32 extPalBase, void *extPalette);
    ~MObj();

    void Update();
    void Show();
    void Hide();

    MDisplay *mDisplay; // 0x00
    MChr *mChr;         // 0x04
    MPal *mPal;         // 0x08
    MPal *mExtPal;      // 0x0C
    MOam *mOam;         // 0x10
    MAfn *mAfn;         // 0x14
    u8 *mShadow;        // 0x18: OAM as it will be next frame
    u32 mOamAddr;       // 0x1C
};

#endif
