// NitroSDK FX: 4x4 rotation matrices. (The rest of the 4x4 code is ARM assembly.)

#include "nitro/fx.h"

void MTX_RotX44_(MtxFx44 *pDst, fx32 sinVal, fx32 cosVal) {
    pDst->_11 = cosVal;
    pDst->_22 = cosVal;
    pDst->_12 = sinVal;
    pDst->_21 = -sinVal;
    pDst->_00 = FX32_ONE;
    pDst->_01 = 0;
    pDst->_02 = 0;
    pDst->_03 = 0;
    pDst->_10 = 0;
    pDst->_13 = 0;
    pDst->_20 = 0;
    pDst->_23 = 0;
    pDst->_30 = 0;
    pDst->_31 = 0;
    pDst->_32 = 0;
    pDst->_33 = FX32_ONE;
}

void MTX_RotY44_(MtxFx44 *pDst, fx32 sinVal, fx32 cosVal) {
    pDst->_00 = cosVal;
    pDst->_22 = cosVal;
    pDst->_20 = sinVal;
    pDst->_02 = -sinVal;
    pDst->_01 = 0;
    pDst->_03 = 0;
    pDst->_10 = 0;
    pDst->_11 = FX32_ONE;
    pDst->_12 = 0;
    pDst->_13 = 0;
    pDst->_21 = 0;
    pDst->_23 = 0;
    pDst->_30 = 0;
    pDst->_31 = 0;
    pDst->_32 = 0;
    pDst->_33 = FX32_ONE;
}

void MTX_RotZ44_(MtxFx44 *pDst, fx32 sinVal, fx32 cosVal) {
    pDst->_00 = cosVal;
    pDst->_11 = cosVal;
    pDst->_01 = sinVal;
    pDst->_10 = -sinVal;
    pDst->_02 = 0;
    pDst->_03 = 0;
    pDst->_12 = 0;
    pDst->_13 = 0;
    pDst->_20 = 0;
    pDst->_21 = 0;
    pDst->_22 = FX32_ONE;
    pDst->_23 = 0;
    pDst->_30 = 0;
    pDst->_31 = 0;
    pDst->_32 = 0;
    pDst->_33 = FX32_ONE;
}
