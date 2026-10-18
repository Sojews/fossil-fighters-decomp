// NitroSDK G3X: setting up and querying the 3D geometry engine.

#include "nitro/gx_internal.h"
#include "nitro/mi.h"

#define REG_G2_BG0CNT (*(volatile u16 *) 0x04000008)
#define REG_G2_BG0OFS (*(volatile u32 *) 0x04000010)
#define REG_G3X_DISP3DCNT (*(volatile u16 *) 0x04000060)
#define REG_G3X_EDGE_COLOR_ADDR 0x04000330
#define REG_G3X_CLEAR_COLOR (*(volatile u32 *) 0x04000350)
#define REG_G3X_CLEAR_DEPTH (*(volatile u16 *) 0x04000354)
#define REG_G3X_CLRIMAGE_OFFSET (*(volatile u16 *) 0x04000356)
#define REG_G3X_FOG_COLOR (*(volatile u32 *) 0x04000358)
#define REG_G3X_FOG_OFFSET (*(volatile u16 *) 0x0400035c)
#define REG_G3X_FOG_TABLE_ADDR 0x04000360
#define REG_G3X_GXFIFO_ADDR 0x04000400
#define REG_G3_MTX_MODE (*(volatile u32 *) 0x04000440)
#define REG_G3_MTX_POP (*(volatile u32 *) 0x04000448)
#define REG_G3_MTX_IDENTITY (*(volatile u32 *) 0x04000454)
#define REG_G3_POLYGON_ATTR (*(volatile u32 *) 0x040004a4)
#define REG_G3_TEXIMAGE_PARAM (*(volatile u32 *) 0x040004a8)
#define REG_G3_TEXPLTT_BASE (*(volatile u32 *) 0x040004ac)
#define REG_G3_SHININESS_ADDR 0x040004d0
#define REG_G3_END_VTXS (*(volatile u32 *) 0x04000504)
#define REG_G3X_GXSTAT (*(volatile u32 *) 0x04000600)
#define REG_G3X_VECMTX_RESULT ((volatile u32 *) 0x04000620)

#define GXSTAT_TB 0x00000001
#define GXSTAT_TR 0x00000002
#define GXSTAT_SB 0x00004000
#define GXSTAT_FI 0x08000000

typedef void (*MIDmaCallback)(void *arg);
void MI_DmaFill32Async(u32 dmaNo, void *dest, u32 data, u32 size, MIDmaCallback callback, void *arg);
void MI_Copy32B(const void *src, void *dest);
static asm void GXi_NopClearFifo128_(register void *pDest);
void G3X_ClearFifo(void);
void G3X_InitMtxStack(void);
void G3X_ResetMtxStack(void);
void G3X_InitTable(void);
s32 G3X_GetMtxStackLevelPV(s32 *level);
s32 G3X_GetMtxStackLevelPJ(s32 *level);

static inline void G3X_SetClearColor_(u16 rgb, int alpha, int depth, int polygonID, BOOL fog) {
    u32 value = (u32) (rgb | (alpha << 16) | (polygonID << 24));
    if (fog) {
        value |= 0x8000;
    }
    REG_G3X_CLEAR_COLOR = value;
    REG_G3X_CLEAR_DEPTH = (u16) depth;
}

void G3X_Init(void) {
    G3X_ClearFifo();
    REG_G3_END_VTXS = 0;
    while (REG_G3X_GXSTAT & GXSTAT_FI) {
    }
    REG_G3X_DISP3DCNT = 0;
    REG_G3X_GXSTAT = 0;
    REG_G2_BG0OFS = 0;
    REG_G3X_DISP3DCNT |= 0x2000;
    REG_G3X_DISP3DCNT |= 0x1000;
    REG_G3X_DISP3DCNT &= ~0x3002;
    REG_G3X_DISP3DCNT = (u16) ((REG_G3X_DISP3DCNT & ~0x3000) | 0x10);
    REG_G3X_DISP3DCNT &= (u16) ~0x3004;
    REG_G3X_GXSTAT |= 0x8000;
    REG_G3X_GXSTAT = (REG_G3X_GXSTAT & ~0xc0000000) | 0x80000000;
    G3X_InitMtxStack();
    G3X_SetClearColor_(0, 0, 0x7fff, 0, FALSE);
    REG_G3X_CLRIMAGE_OFFSET = 0;
    REG_G3X_FOG_COLOR = 0;
    REG_G3X_FOG_OFFSET = 0;
    REG_G2_BG0CNT = REG_G2_BG0CNT & ~3;
    G3X_InitTable();
    REG_G3_POLYGON_ATTR = 0x1f0080;
    REG_G3_TEXIMAGE_PARAM = 0;
    REG_G3_TEXPLTT_BASE = 0;
}

void G3X_Reset(void) {
    while (REG_G3X_GXSTAT & GXSTAT_FI) {
    }
    REG_G3X_GXSTAT |= 0x8000;
    REG_G3X_DISP3DCNT |= 0x2000;
    REG_G3X_DISP3DCNT |= 0x1000;
    G3X_ResetMtxStack();
    REG_G3_POLYGON_ATTR = 0x1f0080;
    REG_G3_TEXIMAGE_PARAM = 0;
    REG_G3_TEXPLTT_BASE = 0;
}

void G3X_ClearFifo(void) {
    GXi_NopClearFifo128_((void *) REG_G3X_GXFIFO_ADDR);
    while (REG_G3X_GXSTAT & GXSTAT_FI) {
    }
}

void G3X_InitMtxStack(void) {
    s32 pv;
    s32 pj;
    REG_G3X_GXSTAT |= 0x8000;
    while (G3X_GetMtxStackLevelPV(&pv)) {
    }
    while (G3X_GetMtxStackLevelPJ(&pj)) {
    }
    REG_G3_MTX_MODE = 3;
    REG_G3_MTX_IDENTITY = 0;
    REG_G3_MTX_MODE = 0;
    if (pj != 0) {
        REG_G3_MTX_POP = pj;
    }
    REG_G3_MTX_IDENTITY = 0;
    REG_G3_MTX_MODE = 2;
    REG_G3_MTX_POP = pv;
    REG_G3_MTX_IDENTITY = 0;
}

void G3X_ResetMtxStack(void) {
    s32 pv;
    s32 pj;
    REG_G3X_GXSTAT |= 0x8000;
    while (G3X_GetMtxStackLevelPV(&pv)) {
    }
    while (G3X_GetMtxStackLevelPJ(&pj)) {
    }
    REG_G3_MTX_MODE = 3;
    REG_G3_MTX_IDENTITY = 0;
    REG_G3_MTX_MODE = 0;
    if (pj != 0) {
        REG_G3_MTX_POP = pj;
    }
    REG_G3_MTX_MODE = 2;
    REG_G3_MTX_POP = pv;
    REG_G3_MTX_IDENTITY = 0;
}

void G3X_SetFog(BOOL enable, int fogMode, int fogSlope, int fogOffset) {
    if (enable) {
        REG_G3X_FOG_OFFSET = (u16) fogOffset;
        REG_G3X_DISP3DCNT = (u16) ((REG_G3X_DISP3DCNT & ~0x3f40) | ((fogSlope << 8) | (fogMode << 6) | 0x80));
    } else {
        REG_G3X_DISP3DCNT &= (u16) ~0x3080;
    }
}

void G3X_SetFogTable(const u32 *fogTable) {
    MI_Copy32B(fogTable, (void *) REG_G3X_FOG_TABLE_ADDR);
}

void G3X_SetClearColor(u16 rgb, int alpha, int depth, int polygonID, BOOL fog) {
    u32 value = (u32) (rgb | (alpha << 16) | (polygonID << 24));
    if (fog) {
        value |= 0x8000;
    }
    REG_G3X_CLEAR_COLOR = value;
    REG_G3X_CLEAR_DEPTH = (u16) depth;
}

void G3X_InitTable(void) {
    int i;
    if (GXi_DmaId != (u32) -1) {
        MI_DmaFill32Async(GXi_DmaId, (void *) REG_G3X_EDGE_COLOR_ADDR, 0, 0x10, NULL, NULL);
        MI_DmaFill32(GXi_DmaId, (void *) REG_G3X_FOG_TABLE_ADDR, 0, 0x60);
    } else {
        MIi_CpuClear32(0, (void *) REG_G3X_EDGE_COLOR_ADDR, 0x10);
        MIi_CpuClear32(0, (void *) REG_G3X_FOG_TABLE_ADDR, 0x60);
    }
    for (i = 0; i < 32; i++) {
        *(volatile u32 *) REG_G3_SHININESS_ADDR = 0;
    }
}

s32 G3X_GetMtxStackLevelPV(s32 *level) {
    if (REG_G3X_GXSTAT & GXSTAT_SB) {
        return -1;
    }
    *level = (s32) ((REG_G3X_GXSTAT & 0x1f00) >> 8);
    return 0;
}

s32 G3X_GetMtxStackLevelPJ(s32 *level) {
    if (REG_G3X_GXSTAT & GXSTAT_SB) {
        return -1;
    }
    *level = (s32) ((REG_G3X_GXSTAT & 0x2000) >> 13);
    return 0;
}

s32 G3X_GetBoxTestResult(s32 *in) {
    if (REG_G3X_GXSTAT & GXSTAT_TB) {
        return -1;
    }
    *in = (s32) (REG_G3X_GXSTAT & GXSTAT_TR);
    return 0;
}

s32 G3X_GetPositionTestResult(void *vec, s32 *w) {
    if (REG_G3X_GXSTAT & GXSTAT_TB) {
        return -1;
    }
    ((s32 *) vec)[0] = REG_G3X_VECMTX_RESULT[0];
    ((s32 *) vec)[1] = REG_G3X_VECMTX_RESULT[1];
    ((s32 *) vec)[2] = REG_G3X_VECMTX_RESULT[2];
    *w = REG_G3X_VECMTX_RESULT[3];
    return 0;
}

void G2_SetBG0Offset_(u32 offset) {
    REG_G2_BG0OFS = offset;
}

#pragma thumb off

// 128 words of zeros (32 NOPs) into the geometry FIFO.
static asm void GXi_NopClearFifo128_(register void *pDest) {
    mov r1, #0
    mov r2, #0
    mov r3, #0
    mov r12, #0
    stmia r0, {r1-r3, r12}
    stmia r0, {r1-r3, r12}
    stmia r0, {r1-r3, r12}
    stmia r0, {r1-r3, r12}
    stmia r0, {r1-r3, r12}
    stmia r0, {r1-r3, r12}
    stmia r0, {r1-r3, r12}
    stmia r0, {r1-r3, r12}
    stmia r0, {r1-r3, r12}
    stmia r0, {r1-r3, r12}
    stmia r0, {r1-r3, r12}
    stmia r0, {r1-r3, r12}
    stmia r0, {r1-r3, r12}
    stmia r0, {r1-r3, r12}
    stmia r0, {r1-r3, r12}
    stmia r0, {r1-r3, r12}
    stmia r0, {r1-r3, r12}
    stmia r0, {r1-r3, r12}
    stmia r0, {r1-r3, r12}
    stmia r0, {r1-r3, r12}
    stmia r0, {r1-r3, r12}
    stmia r0, {r1-r3, r12}
    stmia r0, {r1-r3, r12}
    stmia r0, {r1-r3, r12}
    stmia r0, {r1-r3, r12}
    stmia r0, {r1-r3, r12}
    stmia r0, {r1-r3, r12}
    stmia r0, {r1-r3, r12}
    stmia r0, {r1-r3, r12}
    stmia r0, {r1-r3, r12}
    stmia r0, {r1-r3, r12}
    stmia r0, {r1-r3, r12}
    bx lr
}

#pragma thumb reset
