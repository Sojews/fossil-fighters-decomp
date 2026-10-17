// NitroSDK GX: which VRAM banks are given to what.

#include "nitro/gx_internal.h"

#define REG_GX_VRAMCNT (*(volatile u32 *) 0x04000240)
#define REG_GX_VRAM_HI_CNT (*(volatile u16 *) 0x04000248)

GX_VRAMCnt_ gGXState;

void GX_InitGXState(void) {
    gGXState.lcdc = 0;
    gGXState.bg = 0;
    gGXState.obj = 0;
    gGXState.arm7 = 0;
    gGXState.tex = 0;
    gGXState.texPltt = 0;
    gGXState.clrImg = 0;
    gGXState.bgExtPltt = 0;
    gGXState.objExtPltt = 0;
    gGXState.subBg = 0;
    gGXState.subObj = 0;
    gGXState.subBgExtPltt = 0;
    gGXState.subObjExtPltt = 0;
    REG_GX_VRAMCNT = 0;
    REG_VRAMCNT_E = 0;
    REG_VRAMCNT_F = 0;
    REG_VRAMCNT_G = 0;
    REG_GX_VRAM_HI_CNT = 0;
}
