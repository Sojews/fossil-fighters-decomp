// NitroSDK GX: loading texture images and palettes into the VRAM banks set aside for them (the banks are
// mapped to the LCDC while loading).

#include "nitro/gx_internal.h"
#include "nitro/mi.h"

#define GX_DMA_NOT_USE ((u32) -1)

typedef void (*MIDmaCallback)(void *arg);
void MI_DmaCopy32Async(u32 dmaNo, const void *src, void *dest, u32 size, MIDmaCallback callback, void *arg);
void MI_WaitDma(u32 dmaNo);
int GX_ResetBankForTex(void);
int GX_ResetBankForTexPltt(void);
int GX_ResetBankForClearImage(void);
void GX_SetBankForTex(int tex);
void GX_SetBankForTexPltt(int texPltt);
void GX_SetBankForClearImage(int clrImg);

typedef struct {
    u16 blk1;
    u16 blk2;
    u16 szBlk1;
} TexStartAddr;

extern const TexStartAddr sTexStartAddrTable[16];
extern const u16 sTexPlttStartAddrTable[8];

// (With -ipa file a file's statics are laid out in a permutation of their declaration order.)
static u32 sSzTexBlk1;
static u32 sClrImgLCDCBlk;
static u32 sTexLCDCBlk1;
static u32 sTexPlttLCDCBlk;
static u32 sTexPltt;
static u32 sClrImg;
static u32 sTex;
static u32 sTexLCDCBlk2;

void GX_BeginLoadTex(void) {
    u32 tex = (u32) GX_ResetBankForTex();
    sTex = tex;
    sTexLCDCBlk1 = sTexStartAddrTable[tex].blk1 << 12;
    sTexLCDCBlk2 = sTexStartAddrTable[tex].blk2 << 12;
    sSzTexBlk1 = sTexStartAddrTable[tex].szBlk1 << 12;
}

void GX_LoadTex(const void *pSrc, u32 destSlotAddr, u32 szByte) {
    void *pLCDC;
    if (sTexLCDCBlk2 == 0) {
        pLCDC = (void *) (sTexLCDCBlk1 + destSlotAddr);
    } else if (destSlotAddr + szByte < sSzTexBlk1) {
        pLCDC = (void *) (sTexLCDCBlk1 + destSlotAddr);
    } else if (destSlotAddr >= sSzTexBlk1) {
        pLCDC = (void *) (sTexLCDCBlk2 + destSlotAddr - sSzTexBlk1);
    } else {
        void *pLCDC2 = (void *) sTexLCDCBlk2;
        u32 sz = sSzTexBlk1 - destSlotAddr;
        pLCDC = (void *) (sTexLCDCBlk1 + destSlotAddr);
        if (GXi_DmaId != GX_DMA_NOT_USE && sz > 0x30) {
            MI_DmaCopy32(GXi_DmaId, pSrc, pLCDC, sz);
        } else {
            MIi_CpuCopy32(pSrc, pLCDC, sz);
        }
        if (GXi_DmaId != GX_DMA_NOT_USE) {
            MI_DmaCopy32Async(GXi_DmaId, (const u8 *) pSrc + sz, pLCDC2, szByte - sz, NULL, NULL);
        } else {
            MIi_CpuCopy32((const u8 *) pSrc + sz, pLCDC2, szByte - sz);
        }
        return;
    }
    if (GXi_DmaId != GX_DMA_NOT_USE) {
        MI_DmaCopy32Async(GXi_DmaId, pSrc, pLCDC, szByte, NULL, NULL);
    } else {
        MIi_CpuCopy32(pSrc, pLCDC, szByte);
    }
}

void GX_EndLoadTex(void) {
    if (GXi_DmaId != GX_DMA_NOT_USE) {
        MI_WaitDma(GXi_DmaId);
    }
    GX_SetBankForTex((int) sTex);
    sTex = sTexLCDCBlk1 = sTexLCDCBlk2 = sSzTexBlk1 = 0;
}

void GX_BeginLoadTexPltt(void) {
    u32 texPltt = (u32) GX_ResetBankForTexPltt();
    sTexPltt = texPltt;
    sTexPlttLCDCBlk = sTexPlttStartAddrTable[(s32) texPltt >> 4] << 12;
}

static inline void GXi_DmaCopy32Async(u32 dmaNo, const void *src, void *dest, u32 size, MIDmaCallback callback,
                                      void *arg) {
    if (dmaNo != GX_DMA_NOT_USE) {
        MI_DmaCopy32Async(dmaNo, src, dest, size, callback, arg);
    } else {
        MIi_CpuCopy32(src, dest, size);
    }
}

void GX_LoadTexPltt(const void *pSrc, u32 destSlotAddr, u32 szByte) {
    GXi_DmaCopy32Async(GXi_DmaId, pSrc, (void *) (sTexPlttLCDCBlk + destSlotAddr), szByte, NULL, NULL);
}

void GX_EndLoadTexPltt(void) {
    if (GXi_DmaId != GX_DMA_NOT_USE) {
        MI_WaitDma(GXi_DmaId);
    }
    GX_SetBankForTexPltt((int) sTexPltt);
    sTexPlttLCDCBlk = sTexPltt = 0;
}

// (The clear-image loaders aren't linked into the game; they're here for the statics they use.)
void GX_BeginLoadClearImage(void) {
    u32 clrImg = (u32) GX_ResetBankForClearImage();
    sClrImg = clrImg;
    sClrImgLCDCBlk = clrImg << 12;
}

void GX_EndLoadClearImage(void) {
    GX_SetBankForClearImage((int) sClrImg);
    sClrImgLCDCBlk = sClrImg = 0;
}
