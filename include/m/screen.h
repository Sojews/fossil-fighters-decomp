// Screens: the tile map of one BG layer, kept in a block of screen VRAM, with the layer's control register.
#ifndef M_SCREEN_H
#define M_SCREEN_H

#include "m/heap.h"
#include "m/anime.h"
#include "m/display.h"
#include "m/chr.h"

class MScreen : public MMemoryBlock {
public:
    MScreen(MDisplay *display, u32 screenBase, u32 base, u32 size, u32 screenSize, int bg);
    virtual ~MScreen();
    virtual void Update();

    void Show();
    void Hide();
    BOOL IsHidden();
    void SetChr(MChr *chr);
    void SetBg(int bg);

    void SetOffset(MPoint offset) { mOffset = offset; }

    MTask *CreateShowTransfer();
    MTask *CreateHideTransfer();
    MTask *CreateOffsetAnime(MPoint from, MPoint to, int duration, int ease);

    union {
        u16 mBgCnt;             // 0x0C: the BGxCNT value
        struct {
            u32 priority : 2;
            u32 charBase : 4;
            u32 : 2;
            u32 screenBase : 5;
            u32 extPaletteSlot : 1;
            u32 screenSize : 2;
        } mBgCntBits;
        struct {
            u32 : 7;
            u32 colors256 : 1;      // text screens: 256-color tiles
        } mTextCntBits;
        struct {
            u32 : 2;
            u32 directColor : 1;    // with bitmap set: a direct color bitmap
            u32 : 4;
            u32 bitmap : 1;
        } mBmpCntBits;
    };
    MDisplay *mDisplay;         // 0x10
    int mBg;                    // 0x14
    MPoint mOffset;             // 0x18
};

class MScreenShowTransfer : public MTransfer {
public:
    MScreenShowTransfer(MScreen *screen) { mScreen = screen; }
    virtual ~MScreenShowTransfer() {}
    virtual void Transfer();

    MScreen *mScreen;   // 0x10
};

class MScreenHideTransfer : public MTransfer {
public:
    MScreenHideTransfer(MScreen *screen) { mScreen = screen; }
    virtual ~MScreenHideTransfer() {}
    virtual void Transfer();

    MScreen *mScreen;   // 0x10
};

// Scrolls a screen.
class MScreenOffsetAnime : public MPointAnime {
public:
    MScreenOffsetAnime(MScreen *screen, MPoint from, MPoint to, int duration, int ease) : MPointAnime(from, to) {
        mScreen = screen;
        SetDuration(duration);
        mEase = ease;
    }
    virtual ~MScreenOffsetAnime() {}
    virtual void Apply(int step);

    MScreen *mScreen;   // 0x50
};

#endif
