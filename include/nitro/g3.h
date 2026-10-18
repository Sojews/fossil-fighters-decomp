// NitroSDK geometry engine commands used by the game (declarations and the SDK's register inlines).
#ifndef NITRO_G3_H
#define NITRO_G3_H

#include "nitro/fx.h"

#define REG_G3_MTX_MODE (*(volatile u32 *) 0x04000440)
#define REG_G3_MTX_PUSH (*(volatile u32 *) 0x04000444)
#define REG_G3_MTX_POP (*(volatile u32 *) 0x04000448)
#define REG_G3_MTX_TRANS (*(volatile u32 *) 0x04000470)
#define REG_G3_COLOR (*(volatile u32 *) 0x04000480)
#define REG_G3_VTX_16 (*(volatile u32 *) 0x0400048C)
#define REG_G3_POLYGON_ATTR (*(volatile u32 *) 0x040004A4)
#define REG_G3_TEXIMAGE_PARAM (*(volatile u32 *) 0x040004A8)
#define REG_G3_BEGIN_VTXS (*(volatile u32 *) 0x04000500)
#define REG_G3_END_VTXS (*(volatile u32 *) 0x04000504)
#define REG_G3_POS_TEST (*(volatile u32 *) 0x040005C4)

#define GX_MTXMODE_POSITION_VECTOR 2
#define GX_BEGIN_TRIANGLE_STRIP 2
#define GX_CULL_NONE 3

#ifdef __cplusplus
extern "C" {
#endif

void G3_LoadMtx43(const MtxFx43 *m);
void G3_MultMtx44(const MtxFx44 *m);
int G3X_GetPositionTestResult(VecFx32 *vec, fx32 *w);

#ifdef __cplusplus
}
#endif

static inline void G3_MtxMode(int mode) {
    REG_G3_MTX_MODE = (u32) mode;
}
static inline void G3_PushMtx(void) {
    REG_G3_MTX_PUSH = 0;
}
static inline void G3_PopMtx(int num) {
    REG_G3_MTX_POP = (u32) num;
}
static inline void G3_Translate(fx32 x, fx32 y, fx32 z) {
    REG_G3_MTX_TRANS = (u32) x;
    REG_G3_MTX_TRANS = (u32) y;
    REG_G3_MTX_TRANS = (u32) z;
}
// Lighting mask, polygon mode, culling, polygon id, alpha and the other attribute bits, as the register packs them.
static inline void G3_PolygonAttr(int light, int mode, int cull, int id, int alpha, int misc) {
    REG_G3_POLYGON_ATTR = (u32) (light | (mode << 4) | (cull << 6) | misc | (id << 24) | (alpha << 16));
}
static inline void G3_TexImageParam(u32 param) {
    REG_G3_TEXIMAGE_PARAM = param;
}
static inline void G3_Color(u16 rgb) {
    REG_G3_COLOR = (u32) rgb;
}
static inline void G3_Begin(int primitive) {
    REG_G3_BEGIN_VTXS = (u32) primitive;
}
static inline void G3_End(void) {
    REG_G3_END_VTXS = 0;
}
static inline void G3_Vtx(fx16 x, fx16 y, fx16 z) {
    REG_G3_VTX_16 = (u32) (u16) x | ((u32) (u16) y << 16);
    REG_G3_VTX_16 = (u32) (u16) z;
}
static inline void G3_PositionTest(fx16 x, fx16 y, fx16 z) {
    REG_G3_POS_TEST = (u32) (u16) x | ((u32) (u16) y << 16);
    REG_G3_POS_TEST = (u32) (u16) z;
}

#endif
