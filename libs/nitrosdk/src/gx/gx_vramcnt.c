// NitroSDK GX: giving VRAM banks to texture image and palette slots, and taking them back.

#include "nitro/gx_internal.h"

#define REG_G3X_DISP3DCNT (*(volatile u16 *) 0x04000060)

#define GX_VRAM_A 0x001
#define GX_VRAM_B 0x002
#define GX_VRAM_C 0x004
#define GX_VRAM_D 0x008
#define GX_VRAM_E 0x010
#define GX_VRAM_F 0x020
#define GX_VRAM_G 0x040
#define GX_VRAM_H 0x080
#define GX_VRAM_I 0x100

void OSi_UnlockVram(u16 bank, u16 lockId);

void GX_VRAMCNT_SetLCDC_(int lcdc) {
    if (lcdc & GX_VRAM_A) {
        REG_VRAMCNT_A = 0x80;
    }
    if (lcdc & GX_VRAM_B) {
        REG_VRAMCNT_B = 0x80;
    }
    if (lcdc & GX_VRAM_C) {
        REG_VRAMCNT_C = 0x80;
    }
    if (lcdc & GX_VRAM_D) {
        REG_VRAMCNT_D = 0x80;
    }
    if (lcdc & GX_VRAM_E) {
        REG_VRAMCNT_E = 0x80;
    }
    if (lcdc & GX_VRAM_F) {
        REG_VRAMCNT_F = 0x80;
    }
    if (lcdc & GX_VRAM_G) {
        REG_VRAMCNT_G = 0x80;
    }
    if (lcdc & GX_VRAM_H) {
        REG_VRAMCNT_H = 0x80;
    }
    if (lcdc & GX_VRAM_I) {
        REG_VRAMCNT_I = 0x80;
    }
}

void GX_SetBankForTex(int tex) {
    gGXState.lcdc = (u16) (~tex & (gGXState.lcdc | gGXState.tex));
    gGXState.tex = (u16) tex;
    if (tex == 0) {
        REG_G3X_DISP3DCNT &= (u16) ~0x3001;
    } else {
        REG_G3X_DISP3DCNT = (u16) ((REG_G3X_DISP3DCNT & ~0x3000) | 1);
        switch (tex) {
        case GX_VRAM_A | GX_VRAM_C:
            REG_VRAMCNT_A = 0x83;
            REG_VRAMCNT_C = 0x8b;
            break;
        case GX_VRAM_A | GX_VRAM_D:
            REG_VRAMCNT_A = 0x83;
            REG_VRAMCNT_D = 0x8b;
            break;
        case GX_VRAM_B | GX_VRAM_D:
            REG_VRAMCNT_B = 0x83;
            REG_VRAMCNT_D = 0x8b;
            break;
        case GX_VRAM_A | GX_VRAM_B | GX_VRAM_D:
            REG_VRAMCNT_A = 0x83;
            REG_VRAMCNT_B = 0x8b;
            REG_VRAMCNT_D = 0x93;
            break;
        case GX_VRAM_A | GX_VRAM_C | GX_VRAM_D:
            REG_VRAMCNT_A = 0x83;
            REG_VRAMCNT_C = 0x8b;
            REG_VRAMCNT_D = 0x93;
            break;
        case GX_VRAM_D:
            REG_VRAMCNT_D = 0x83;
            break;
        case GX_VRAM_C | GX_VRAM_D:
            REG_VRAMCNT_D = 0x8b;
        case GX_VRAM_C:
            REG_VRAMCNT_C = 0x83;
            break;
        case GX_VRAM_B | GX_VRAM_C | GX_VRAM_D:
            REG_VRAMCNT_D = 0x93;
        case GX_VRAM_B | GX_VRAM_C:
            REG_VRAMCNT_C = 0x8b;
        case GX_VRAM_B:
            REG_VRAMCNT_B = 0x83;
            break;
        case GX_VRAM_A | GX_VRAM_B | GX_VRAM_C | GX_VRAM_D:
            REG_VRAMCNT_D = 0x9b;
        case GX_VRAM_A | GX_VRAM_B | GX_VRAM_C:
            REG_VRAMCNT_C = 0x93;
        case GX_VRAM_A | GX_VRAM_B:
            REG_VRAMCNT_B = 0x8b;
        case GX_VRAM_A:
            REG_VRAMCNT_A = 0x83;
            break;
        }
    }
    GX_VRAMCNT_SetLCDC_(gGXState.lcdc);
}

void GX_SetBankForTexPltt(int texPltt) {
    gGXState.lcdc = (u16) (~texPltt & (gGXState.lcdc | gGXState.texPltt));
    gGXState.texPltt = (u16) texPltt;
    switch (texPltt) {
    case 0:
        break;
    case GX_VRAM_F | GX_VRAM_G:
        REG_VRAMCNT_G = 0x8b;
    case GX_VRAM_F:
        REG_VRAMCNT_F = 0x83;
        break;
    case GX_VRAM_G:
        REG_VRAMCNT_G = 0x83;
        break;
    case GX_VRAM_E | GX_VRAM_F | GX_VRAM_G:
        REG_VRAMCNT_G = 0x9b;
    case GX_VRAM_E | GX_VRAM_F:
        REG_VRAMCNT_F = 0x93;
    case GX_VRAM_E:
        REG_VRAMCNT_E = 0x83;
        break;
    }
    GX_VRAMCNT_SetLCDC_(gGXState.lcdc);
}

static int resetBankForX_(u16 *pBank) {
    u16 bank = *pBank;
    *pBank = 0;
    gGXState.lcdc |= bank;
    GX_VRAMCNT_SetLCDC_(bank);
    return bank;
}

int GX_ResetBankForTex(void) {
    return resetBankForX_(&gGXState.tex);
}

int GX_ResetBankForTexPltt(void) {
    return resetBankForX_(&gGXState.texPltt);
}

static int disableBankForX_(u16 *pBank) {
    u32 bank = *pBank;
    *pBank = 0;
    if (bank & GX_VRAM_A) {
        REG_VRAMCNT_A = 0;
    }
    if (bank & GX_VRAM_B) {
        REG_VRAMCNT_B = 0;
    }
    if (bank & GX_VRAM_C) {
        REG_VRAMCNT_C = 0;
    }
    if (bank & GX_VRAM_D) {
        REG_VRAMCNT_D = 0;
    }
    if (bank & GX_VRAM_E) {
        REG_VRAMCNT_E = 0;
    }
    if (bank & GX_VRAM_F) {
        REG_VRAMCNT_F = 0;
    }
    if (bank & GX_VRAM_G) {
        REG_VRAMCNT_G = 0;
    }
    if (bank & GX_VRAM_H) {
        REG_VRAMCNT_H = 0;
    }
    if (bank & GX_VRAM_I) {
        REG_VRAMCNT_I = 0;
    }
    OSi_UnlockVram((u16) bank, GXi_VRamLockId);
    return (int) bank;
}

int GX_DisableBankForTex(void) {
    return disableBankForX_(&gGXState.tex);
}

int GX_DisableBankForTexPltt(void) {
    return disableBankForX_(&gGXState.texPltt);
}
