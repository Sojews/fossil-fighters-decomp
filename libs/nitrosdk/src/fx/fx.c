// NitroSDK FX: multiplication.

#include "nitro/fx.h"

#pragma thumb off

fx32 FX_MulFunc(fx32 v1, fx32 v2) {
    return (fx32) (((fx64) v1 * v2 + (FX32_ONE >> 1)) >> FX32_SHIFT);
}

fx32 FX_Mul32x64c(fx32 v32, fx64c v64c) {
    return (fx32) ((v64c * v32 + 0x80000000LL) >> 32);
}

#pragma thumb reset
