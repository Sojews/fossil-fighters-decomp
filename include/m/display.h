// One of the DS's two 2D graphics engines (main or sub): its display control register and the hooks run on it.
#ifndef M_DISPLAY_H
#define M_DISPLAY_H

#include "types.h"

// Where an engine's registers and VRAM areas are (one table per engine).
struct MDisplayRegs {
    u32 unk00;
    volatile u32 *dispcnt;      // 0x04
    u32 unk08[4];               // 0x08
    u32 unk18[4];               // 0x18
    u32 unk28[4];               // 0x28
    volatile u16 *backdrop;     // 0x38
    u32 unk3C;
    u32 unk40;
    u32 unk44;
    u32 unk48;
    u8 *unk4C;                  // 0x4C: VRAM bank configurations, 0x1C bytes each
    u8 *unk50;
    u8 *unk54;
    u8 *unk58;
    u8 *unk5C;
    u8 *unk60;
    u32 unk64[4];               // 0x64
    u32 unk74[2];               // 0x74
    u32 unk7C;
    u32 unk80;
    u32 unk84;
    volatile u32 *unk88;
    u32 unk8C;
};

// The fields of DISPCNT.
struct MDispCnt {
    u32 bgMode : 3;
    u32 bg0Is3D : 1;
    u32 objTileMapping : 1;
    u32 objBmpMapping : 2;
    u32 forceBlank : 1;
    u32 visiblePlanes : 5;  // BG0-3, OBJ
    u32 visibleWindows : 3;
    u32 displayMode : 4;
    u32 objTileBoundary : 2;
    u32 objBmpBoundary : 1;
    u32 hblankObj : 1;
    u32 bgCharBase : 3;
    u32 bgScreenBase : 3;
    u32 bgExtPalette : 1;
    u32 objExtPalette : 1;
};

class MDisplayHook;

class MDisplay {
public:
    virtual ~MDisplay();

    void AddHook(MDisplayHook *hook);
    void RemoveHook(MDisplayHook *hook);

    u32 GetRegs00();
    u32 GetRegs08(int i);
    u32 GetRegs18(int i);
    u32 GetRegs28(int i);
    u32 GetRegs64(int i);
    u32 GetRegs74(int i);
    u32 GetRegs7C();
    u32 GetRegs80();
    u32 GetRegs84();
    u32 GetRegs8C();
    void ShowPlanes(u32 mask);
    void HidePlanes(u32 mask);
    BOOL ArePlanesHidden(u32 mask);
    void ShowWindow(int window);
    void HideWindow(int window);
    void SetBackdrop(u16 color);
    volatile u16 *GetBackdropReg();
    void SetBg0Is3D(BOOL on);
    void SetObjTileBoundary(int boundary);

    int mEngine;            // 0x04
    MDisplayRegs *mRegs;    // 0x08
    MDispCnt mDispCnt;      // 0x0C
    u32 mUnk10;
    void *mUnk14;
    void *mUnk18;
    void *mUnk1C;
    void *mUnk20;
    void *mUnk24;
    u32 mUnk28[(0xA4 - 0x28) / 4];
    u16 mBackdrop;          // 0xA4
};

class MDisplayHook {
public:
    MDisplayHook(MDisplay *display);
    virtual ~MDisplayHook();
    virtual void Unk08() = 0;
    virtual void Unk0C() = 0;

    MDisplay *mDisplay;     // 0x04
};

#endif
