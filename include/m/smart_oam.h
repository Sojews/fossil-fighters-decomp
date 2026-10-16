// Smart OAM: a pool of sprite entries, each an OAM entry plus where and how big it is, that a task places into
// a block of real OAM entries every frame (offset, clipped to the screen and hidden as asked).
#ifndef M_SMART_OAM_H
#define M_SMART_OAM_H

#include "m/obj.h"
#include "m/anime.h"

class MDisplay;
class MSmartOam;
struct MSmartOamBlock;

// An allocated run of OBJ character memory.
struct MChrBlock : public MHeapBlock {
    u32 unk0C;
    u32 unk10;
    u32 unk14;
    u32 tile;   // 0x18: the first tile number
};

struct MSmartOamEntry {
    MSmartOamEntry();

    void SetChr(MChrBlock *chr);
    void SetPalette(MPalBlock *pal);

    void Clear() {
        oam.attr01 = 0;
        oam.attr2 = 0;
        oam.objMode = 2;
        unk08 = 0;
        unk0C = 0;
        unk10 = 0;
        unk14 = 0;
        width = 8;
        height = 8;
        flags = 0;
    }

    MOamEntry oam;          // 0x00
    int unk08;              // 0x08
    int unk0C;              // 0x0C
    int unk10;              // 0x10
    int unk14;              // 0x14
    int width;              // 0x18: pixels
    int height;             // 0x1C
    union {
        u32 links;          // 0x20
        struct {
            u32 next : 10;
            u32 flags : 22;
        };
        struct {
            u32 : 30;
            u32 anchor : 2;
        };
    };
    MSmartOamBlock *block;  // 0x24
};

// A run of entries.
struct MSmartOamBlock : public MHeapBlock {
    u32 count;          // 0x0C
    MSmartOam *owner;   // 0x10

    MSmartOamEntry *GetEntries() { return (MSmartOamEntry *)addr; }
    void SetMode(int mode);
    void SetObjMode(int mode);
    void SetAffineParam(int param);
    void SetFlip(BOOL hflip, BOOL vflip);
    void SetPriority(int priority);
    void SetAnchor(int anchor);
    void Clear();
    void Hide();
    void Show();
};

class MSmartOam : public MTask {
public:
    MSmartOam(int count, int oamCount, BOOL fromEnd);
    MSmartOam(MDisplay *display, int count, int oamCount, BOOL fromEnd);
    virtual ~MSmartOam();
    virtual void Execute();

    void Init(BOOL fromEnd);
    void SetUnk1C(int value);
    MSmartOamBlock *Alloc(int count, BOOL fromEnd);
    void Free(MSmartOamBlock *block);

    void SetOffset(MPoint offset) { mOffset = offset; }

    MTask *CreateOffsetTransfer(MPoint offset);
    MTask *CreateShowTransfer();
    MTask *CreateHideTransfer();
    MTask *CreateOffsetAnime(MPoint from, MPoint to, int duration, int ease);

    MDisplay *mDisplay;         // 0x10
    int mCount;                 // 0x14
    int mOamCount;              // 0x18: real OAM entries used
    int mUnk1C;                 // 0x1C
    MOamBlock *mOamBlock;       // 0x20
    MHeap *mHeap;               // 0x24: over mEntries
    MSmartOamEntry *mEntries;   // 0x28: mCount, then a terminator
    int *mOrder;                // 0x2C
    MPoint mOffset;             // 0x30
    u8 mHidden;                 // 0x38
    int mUnk3C;                 // 0x3C
    int mUnk40;                 // 0x40
    int mClipWidth;             // 0x44
    int mClipHeight;            // 0x48
};

class MSmartOamOffsetTransfer : public MTransfer {
public:
    MSmartOamOffsetTransfer(MSmartOam *oam, MPoint offset) {
        mOam = oam;
        mOffset = offset;
    }
    virtual ~MSmartOamOffsetTransfer() {}
    virtual void Transfer();

    MSmartOam *mOam;    // 0x10
    MPoint mOffset;     // 0x14
};

class MSmartOamShowTransfer : public MTransfer {
public:
    MSmartOamShowTransfer(MSmartOam *oam) { mOam = oam; }
    virtual ~MSmartOamShowTransfer() {}
    virtual void Transfer();

    MSmartOam *mOam;    // 0x10
};

class MSmartOamHideTransfer : public MTransfer {
public:
    MSmartOamHideTransfer(MSmartOam *oam) { mOam = oam; }
    virtual ~MSmartOamHideTransfer() {}
    virtual void Transfer();

    MSmartOam *mOam;    // 0x10
};

class MSmartOamOffsetAnime : public MPointAnime {
public:
    MSmartOamOffsetAnime(MSmartOam *oam, MPoint from, MPoint to, int duration, int ease) : MPointAnime(from, to) {
        mOam = oam;
        SetDuration(duration);
        mEase = ease;
    }
    virtual ~MSmartOamOffsetAnime() {}
    virtual void Apply(int step);

    MSmartOam *mOam;    // 0x50
};

#endif
