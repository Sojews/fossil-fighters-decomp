// NitroSDK GX: setting up the 2D/3D graphics engines.

#include "nitro/gx_internal.h"
#include "nitro/mi.h"
#include "nitro/os.h"

#define REG_GX_DISPCNT (*(volatile u32 *) 0x04000000)
#define REG_GX_DISPSTAT (*(volatile u16 *) 0x04000004)
#define REG_GX_POWCNT (*(volatile u16 *) 0x04000304)
#define REG_GX_MASTER_BRIGHT (*(volatile u16 *) 0x0400006c)
#define REG_G2_BG2PA (*(volatile u16 *) 0x04000020)
#define REG_G2_BG2PD (*(volatile u16 *) 0x04000026)
#define REG_G2_BG3PA (*(volatile u16 *) 0x04000030)
#define REG_G2_BG3PD (*(volatile u16 *) 0x04000036)
#define REG_G2S_BG2PA (*(volatile u16 *) 0x04001020)
#define REG_G2S_BG2PD (*(volatile u16 *) 0x04001026)
#define REG_G2S_BG3PA (*(volatile u16 *) 0x04001030)
#define REG_G2S_BG3PD (*(volatile u16 *) 0x04001036)

#define GX_DMA_NOT_USE ((u32) -1)
#define OS_LOCK_ID_ERROR (-3)

s32 OS_GetLockID(void);
void MI_WaitDma(u32 dmaNo);

volatile u16 GXi_VRamLockId;
static u16 sDispMode;
static u16 sIsDispOn = TRUE;
u32 GXi_DmaId = 3;

void GX_Init(void) {
    REG_GX_POWCNT |= 0x8000;
    REG_GX_POWCNT = (u16) ((REG_GX_POWCNT & ~0x20e) | 0x20e);
    REG_GX_POWCNT |= 1;
    GX_InitGXState();
    while (GXi_VRamLockId == 0) {
        s32 lockResult = OS_GetLockID();
        if (lockResult == OS_LOCK_ID_ERROR) {
            OS_Terminate();
        }
        GXi_VRamLockId = (u16) lockResult;
    }
    REG_GX_DISPSTAT = 0;
    REG_GX_DISPCNT = 0;
    if (GXi_DmaId != GX_DMA_NOT_USE) {
        MI_DmaFill32(GXi_DmaId, (void *) 0x04000008, 0, 0x60);
        REG_GX_MASTER_BRIGHT = 0;
        MI_DmaFill32(GXi_DmaId, (void *) 0x04001000, 0, 0x70);
    } else {
        MIi_CpuClear32(0, (void *) 0x04000008, 0x60);
        REG_GX_MASTER_BRIGHT = 0;
        MIi_CpuClear32(0, (void *) 0x04001000, 0x70);
    }
    REG_G2_BG2PA = 0x100;
    REG_G2_BG2PD = 0x100;
    REG_G2_BG3PA = 0x100;
    REG_G2_BG3PD = 0x100;
    REG_G2S_BG2PA = 0x100;
    REG_G2S_BG2PD = 0x100;
    REG_G2S_BG3PA = 0x100;
    REG_G2S_BG3PD = 0x100;
}

s32 GX_HBlankIntr(BOOL enable) {
    s32 prev = REG_GX_DISPSTAT & 0x10;
    if (enable) {
        REG_GX_DISPSTAT |= 0x10;
    } else {
        REG_GX_DISPSTAT &= ~0x10;
    }
    return prev;
}

s32 GX_VBlankIntr(BOOL enable) {
    s32 prev = REG_GX_DISPSTAT & 0x8;
    if (enable) {
        REG_GX_DISPSTAT |= 0x8;
    } else {
        REG_GX_DISPSTAT &= ~0x8;
    }
    return prev;
}

void GX_DispOff(void) {
    u32 dispcnt = REG_GX_DISPCNT;
    sIsDispOn = FALSE;
    sDispMode = (u16) ((dispcnt & 0x30000) >> 16);
    REG_GX_DISPCNT = dispcnt & ~0x30000;
}

void GX_DispOn(void) {
    sIsDispOn = TRUE;
    REG_GX_DISPCNT = (REG_GX_DISPCNT & ~0x30000) | (sDispMode << 16);
}

u32 GX_SetDefaultDMA(u32 dmaNo) {
    u32 previous = GXi_DmaId;
    OSIntrMode enabled;
    if (previous != GX_DMA_NOT_USE) {
        MI_WaitDma(previous);
    }
    enabled = OS_DisableInterrupts();
    GXi_DmaId = dmaNo;
    OS_RestoreInterrupts(enabled);
    return previous;
}
