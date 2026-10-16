// One of the DS's two 2D graphics engines (main or sub): its display control register and the hooks run on it.
#ifndef M_DISPLAY_H
#define M_DISPLAY_H

#include <vector>
#include "types.h"

// The fields of DISPCNT.
union MDispCnt {
    u32 raw;
    struct {
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
};
// The register at MDisplayRegs::unk88.
union MDisplayUnk88 {
    u32 raw;
    struct {
        u16 lo;
        u16 hi;
    };
};

// Where an engine's registers and VRAM areas are (one table per engine).
struct MDisplayRegs {
    u32 unk00;
    MDispCnt *dispcnt;          // 0x04
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
    volatile u8 *winin[4];      // 0x64: WININ/WINOUT bytes for windows 0, 1, OBJ and outside
    volatile u16 *winh[2];      // 0x74: WIN0H, WIN1H (WINxV follows each)
    volatile u16 *bldcnt;       // 0x7C
    volatile u16 *bldalpha;     // 0x80
    volatile u16 *bldy;         // 0x84
    MDisplayUnk88 *unk88;       // 0x88
    volatile u32 *masterBright; // 0x8C
};



class MDisplayHook;
class MLcd;
class MBg;
class MObj;
class MWin;
class MBlend;
class MBright;

// The VRAM banks given to one use.
struct MVramBanks {
    MVramBanks() {
        for (u32 i = 0; i < 4; i++) {
            banks[i] = 0;
        }
    }

    u32 count;
    u32 banks[4];
};

class MDisplay {
public:
    MDisplay(MLcd *lcd, MDisplayRegs *regs);
    virtual ~MDisplay();

    void Update();

    void AddHook(MDisplayHook *hook);
    void RemoveHook(MDisplayHook *hook);

    u32 GetRegs00();
    u32 GetRegs08(int i);
    u32 GetRegs18(int i);
    u32 GetRegs28(int i);
    volatile u8 *GetRegs64(int i);
    volatile u16 *GetRegs74(int i);
    volatile u16 *GetRegs7C();
    volatile u16 *GetRegs80();
    volatile u16 *GetRegs84();
    volatile u32 *GetRegs8C();
    void ShowPlanes(u32 mask);
    void HidePlanes(u32 mask);
    BOOL ArePlanesHidden(u32 mask);
    void ShowWindow(int window);
    void HideWindow(int window);
    void SetBackdrop(u16 color);
    volatile u16 *GetBackdropReg();
    void SetBg0Is3D(BOOL on);
    void SetObjTileBoundary(int boundary);

    MLcd *mLcd;             // 0x04
    MDisplayRegs *mRegs;    // 0x08
    MDispCnt mDispCnt;      // 0x0C
    MDisplayUnk88 mUnk10;   // 0x10
    MBg *mBg;               // 0x14
    MObj *mObj;             // 0x18
    MWin *mWin;             // 0x1C
    MBlend *mBlend;         // 0x20
    MBright *mBright;       // 0x24
    MVramBanks mBanks28;    // 0x28
    MVramBanks mBanks3C;    // 0x3C
    MVramBanks mBanks50;    // 0x50
    MVramBanks mBanks64;    // 0x64
    MVramBanks mBanks78;    // 0x78
    MVramBanks mBanks8C;    // 0x8C
    u32 mUnkA0;
    u16 mBackdrop;          // 0xA4
    std::vector<MDisplayHook *> mHooks; // 0xA8
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
