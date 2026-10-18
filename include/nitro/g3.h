// NitroSDK geometry engine commands used by the game (declarations and the SDK's register inlines).
#ifndef NITRO_G3_H
#define NITRO_G3_H

#include "nitro/fx.h"

#define REG_G3_MTX_MODE (*(volatile u32 *) 0x04000440)
#define REG_G3_MTX_PUSH (*(volatile u32 *) 0x04000444)
#define REG_G3_MTX_POP (*(volatile u32 *) 0x04000448)
#define REG_G3_POS_TEST (*(volatile u32 *) 0x040005C4)

#define GX_MTXMODE_POSITION_VECTOR 2

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
static inline void G3_PositionTest(fx16 x, fx16 y, fx16 z) {
    REG_G3_POS_TEST = (u32) (u16) x | ((u32) (u16) y << 16);
    REG_G3_POS_TEST = (u32) (u16) z;
}

#endif
