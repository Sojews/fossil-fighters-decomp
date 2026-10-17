// The BG layers of a display: hands out the screen and character VRAM, the screens (text, affine and bitmap
// layers) by BG mode, and the palettes.
#ifndef M_BG_H
#define M_BG_H

#include "m/display.h"
#include "m/heap.h"

class MScreen;
class MText;
class MDirectBitmap;
class MIndexBitmap;

// A BG layer's mode (text, affine, ...); the game names the type bg_mode.
enum bg_mode {
    BG_MODE_TEXT,
};
class MExtAffine;
class MChr;
class MPal;
class M3d;

class MBg {
public:
    // The BG numbers each kind of layer can use in a BG mode.
    struct Mode {
        struct Range {
            u32 start;
            u32 count;
        };
        Range text;     // 0x00
        Range affine;   // 0x08
        Range ext;      // 0x10: extended affine and bitmap layers
        Range unk18;
    };

    // Extended palettes, until the first without one.
    struct ExtPalettes {
        u32 unk00;
        struct {
            void *palette;
            u32 base;
        } entries[4];
    };

    MBg(MDisplay *display, u32 vramBase, u32 vramSize, u32 paletteBase, ExtPalettes ext, u32 unk44, u32 unk48);
    virtual ~MBg();

    void SetMode(int mode);
    void Update();
    void SetBg0Reserved(BOOL reserved);
    MText *CreateText(int size, BOOL fromEnd);
    void DeleteText(MText *text);
    MDirectBitmap *CreateDirectBitmap(int size, BOOL fromEnd);
    void DeleteDirectBitmap(MDirectBitmap *bitmap);
    MIndexBitmap *CreateIndexBitmap(int size, BOOL fromEnd);
    void DeleteIndexBitmap(MIndexBitmap *bitmap);
    MExtAffine *CreateExtAffine(int size, BOOL fromEnd);
    void DeleteExtAffine(MExtAffine *screen);
    MChr *CreateChr(int mode, u32 units, BOOL fromEnd);
    void DeleteChr(MChr *chr);
    M3d *Create3d();
    void Delete3d(M3d *screen);

    const Mode *mMode;          // 0x04
    MDisplay *mDisplay;         // 0x08
    MHeap *mVram;               // 0x0C
    MScreen *mScreens[4];       // 0x10: by BG number
    MChr *mChrs[4];             // 0x20
    MPal *mPal;                 // 0x30
    MPal *mExtPals[4];          // 0x34
    u32 mUnk44;                 // 0x44
    u32 mUnk48;                 // 0x48
    u32 mExtPalCount;           // 0x4C
    u8 mBg0Reserved;            // 0x50: BG 0 is kept for 3D
};

#endif
