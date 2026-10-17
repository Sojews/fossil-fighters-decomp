// NitroSDK GX: the VRAM bank bookkeeping shared by the GX files (declarations only).
#ifndef NITRO_GX_INTERNAL_H
#define NITRO_GX_INTERNAL_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    u16 lcdc;
    u16 bg;
    u16 obj;
    u16 arm7;
    u16 tex;
    u16 texPltt;
    u16 clrImg;
    u16 bgExtPltt;
    u16 objExtPltt;
    u16 subBg;
    u16 subObj;
    u16 subBgExtPltt;
    u16 subObjExtPltt;
} GX_VRAMCnt_;

extern GX_VRAMCnt_ gGXState;
extern u32 GXi_DmaId;
extern volatile u16 GXi_VRamLockId;

#define REG_VRAMCNT_A (*(volatile u8 *) 0x04000240)
#define REG_VRAMCNT_B (*(volatile u8 *) 0x04000241)
#define REG_VRAMCNT_C (*(volatile u8 *) 0x04000242)
#define REG_VRAMCNT_D (*(volatile u8 *) 0x04000243)
#define REG_VRAMCNT_E (*(volatile u8 *) 0x04000244)
#define REG_VRAMCNT_F (*(volatile u8 *) 0x04000245)
#define REG_VRAMCNT_G (*(volatile u8 *) 0x04000246)
#define REG_VRAMCNT_H (*(volatile u8 *) 0x04000248)
#define REG_VRAMCNT_I (*(volatile u8 *) 0x04000249)

void GX_VRAMCNT_SetLCDC_(int lcdc);
void GX_InitGXState(void);

#ifdef __cplusplus
}
#endif

#endif
