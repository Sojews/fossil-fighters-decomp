// NitroSDK G3: camera and texture matrix helpers.

#include "nitro/fx.h"

#define REG_G3_MTX_MODE (*(volatile u32 *) 0x04000440)
#define REG_G3_MTX_LOAD_4x4 (*(volatile u32 *) 0x04000458)
#define REG_G3_MTX_LOAD_4x3 (*(volatile u32 *) 0x0400045c)

#define REG_CP_DIV_NUMER (*(volatile s64 *) 0x04000290)
#define REG_CP_DIV_DENOM (*(volatile u64 *) 0x04000298)


// (The divider is already in 64/32-bit mode.)
static inline void FX_DivAsyncImm(fx32 numer, fx32 denom) {
    REG_CP_DIV_NUMER = (s64) numer << 32;
    REG_CP_DIV_DENOM = (u32) denom;
}

static inline void FX_InvAsyncImm(fx32 denom) {
    REG_CP_DIV_NUMER = (s64) FX32_ONE << 32;
    REG_CP_DIV_DENOM = (u32) denom;
}


void G3i_PerspectiveW_(fx32 fovySin, fx32 fovyCos, fx32 aspect, fx32 n, fx32 f, fx32 scaleW, BOOL draw,
                       MtxFx44 *mtx) {
    fx32 t;
    fx32 a00;
    fx32 a22;
    fx32 a32;
    fx64c dvd;
    volatile fx32 *fifo;

    t = FX_Div(fovyCos, fovySin);
    if (scaleW != FX32_ONE) {
        t = t * scaleW / FX32_ONE;
    }
    FX_DivAsyncImm(t, aspect);
    if (draw) {
        REG_G3_MTX_MODE = 0;
        fifo = (volatile fx32 *) &REG_G3_MTX_LOAD_4x4;
    }
    if (mtx != NULL) {
        mtx->_01 = 0;
        mtx->_02 = 0;
        mtx->_03 = 0;
        mtx->_10 = 0;
        mtx->_12 = 0;
        mtx->_13 = 0;
        mtx->_20 = 0;
        mtx->_21 = 0;
        mtx->_23 = -scaleW;
        mtx->_30 = 0;
        mtx->_31 = 0;
        mtx->_33 = 0;
    }
    a00 = FX_GetDivResult();
    FX_InvAsyncImm(n - f);
    if (draw) {
        *fifo = a00;
        *fifo = 0;
        *fifo = 0;
        *fifo = 0;
        *fifo = 0;
        *fifo = t;
        *fifo = 0;
        *fifo = 0;
        *fifo = 0;
        *fifo = 0;
    }
    if (mtx != NULL) {
        mtx->_00 = a00;
        mtx->_11 = t;
    }
    dvd = FX_GetDivResultFx64c();
    if (scaleW != FX32_ONE) {
        dvd = dvd * scaleW / FX32_ONE;
    }
    a22 = FX_Mul32x64c(f + n, dvd);
    a32 = FX_Mul32x64c(FX_Mul(n << 1, f), dvd);
    if (draw) {
        *fifo = a22;
        *fifo = -scaleW;
        *fifo = 0;
        *fifo = 0;
        *fifo = a32;
        *fifo = 0;
    }
    if (mtx != NULL) {
        mtx->_22 = a22;
        mtx->_32 = a32;
    }
}

void G3i_LookAt_(const VecFx32 *camPos, const VecFx32 *camUp, const VecFx32 *target, BOOL draw, MtxFx43 *mtx) {
    VecFx32 vLook;
    VecFx32 vRight;
    VecFx32 vUp;
    fx32 x;
    fx32 y;
    fx32 z;
    volatile fx32 *fifo;

    vLook.x = camPos->x - target->x;
    vLook.y = camPos->y - target->y;
    vLook.z = camPos->z - target->z;
    VEC_Normalize(&vLook, &vLook);
    VEC_CrossProduct(camUp, &vLook, &vRight);
    VEC_Normalize(&vRight, &vRight);
    VEC_CrossProduct(&vLook, &vRight, &vUp);
    if (draw) {
        REG_G3_MTX_MODE = 2;
        fifo = (volatile fx32 *) &REG_G3_MTX_LOAD_4x3;
        *fifo = vRight.x;
        *fifo = vUp.x;
        *fifo = vLook.x;
        *fifo = vRight.y;
        *fifo = vUp.y;
        *fifo = vLook.y;
        *fifo = vRight.z;
        *fifo = vUp.z;
        *fifo = vLook.z;
    }
    x = -VEC_DotProduct(camPos, &vRight);
    y = -VEC_DotProduct(camPos, &vUp);
    z = -VEC_DotProduct(camPos, &vLook);
    if (draw) {
        *fifo = x;
        *fifo = y;
        *fifo = z;
    }
    if (mtx != NULL) {
        mtx->_00 = vRight.x;
        mtx->_01 = vUp.x;
        mtx->_02 = vLook.x;
        mtx->_10 = vRight.y;
        mtx->_11 = vUp.y;
        mtx->_12 = vLook.y;
        mtx->_20 = vRight.z;
        mtx->_21 = vUp.z;
        mtx->_22 = vLook.z;
        mtx->_30 = x;
        mtx->_31 = y;
        mtx->_32 = z;
    }
}

void G3_LoadTexMtxTexCoord(const MtxFx44 *mtx) {
    REG_G3_MTX_MODE = 3;
    REG_G3_MTX_LOAD_4x4 = mtx->_00;
    REG_G3_MTX_LOAD_4x4 = mtx->_01;
    REG_G3_MTX_LOAD_4x4 = mtx->_02;
    REG_G3_MTX_LOAD_4x4 = mtx->_03;
    REG_G3_MTX_LOAD_4x4 = mtx->_10;
    REG_G3_MTX_LOAD_4x4 = mtx->_11;
    REG_G3_MTX_LOAD_4x4 = mtx->_12;
    REG_G3_MTX_LOAD_4x4 = mtx->_13;
    REG_G3_MTX_LOAD_4x4 = mtx->_20 << 4;
    REG_G3_MTX_LOAD_4x4 = mtx->_21 << 4;
    REG_G3_MTX_LOAD_4x4 = mtx->_22 << 4;
    REG_G3_MTX_LOAD_4x4 = mtx->_23 << 4;
    REG_G3_MTX_LOAD_4x4 = mtx->_30 << 4;
    REG_G3_MTX_LOAD_4x4 = mtx->_31 << 4;
    REG_G3_MTX_LOAD_4x4 = mtx->_32 << 4;
    REG_G3_MTX_LOAD_4x4 = mtx->_33 << 4;
}
