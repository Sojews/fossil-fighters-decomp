// NitroSDK FX: 2x2 matrices.

#include "nitro/fx.h"

void MI_Copy16B(const void *src, void *dest);

void MTX_Rot22_(MtxFx22 *pDst, fx32 sinVal, fx32 cosVal) {
    pDst->_00 = cosVal;
    pDst->_01 = sinVal;
    pDst->_10 = -sinVal;
    pDst->_11 = cosVal;
}

void MTX_ScaleApply22(const MtxFx22 *pSrc, MtxFx22 *pDst, fx32 x, fx32 y) {
    pDst->_00 = (fx32) (((fx64) x * pSrc->_00) >> FX32_SHIFT);
    pDst->_01 = (fx32) (((fx64) x * pSrc->_01) >> FX32_SHIFT);
    pDst->_10 = (fx32) (((fx64) y * pSrc->_10) >> FX32_SHIFT);
    pDst->_11 = (fx32) (((fx64) y * pSrc->_11) >> FX32_SHIFT);
}

int MTX_Inverse22(const MtxFx22 *pSrc, MtxFx22 *pDst) {
    MtxFx22 tmp;
    MtxFx22 *p;
    fx32 det;
    if (pSrc == pDst) {
        p = &tmp;
    } else {
        p = pDst;
    }
    det = (fx32) (((fx64) pSrc->_00 * pSrc->_11 - (fx64) pSrc->_01 * pSrc->_10 + (FX32_ONE >> 1)) >> FX32_SHIFT);
    if (det == 0) {
        return -1;
    }
    FX_InvAsync(det);
    {
        fx32 invDet = FX_GetDivResult();
        p->_00 = (fx32) (((fx64) invDet * pSrc->_11) >> FX32_SHIFT);
        p->_01 = -(fx32) (((fx64) invDet * pSrc->_01) >> FX32_SHIFT);
        p->_10 = -(fx32) (((fx64) invDet * pSrc->_10) >> FX32_SHIFT);
        p->_11 = (fx32) (((fx64) invDet * pSrc->_00) >> FX32_SHIFT);
    }
    if (p == &tmp) {
        MI_Copy16B(&tmp, pDst);
    }
    return 0;
}
