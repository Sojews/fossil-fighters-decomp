// NitroSDK FX: division and square roots on the hardware divider.

#include "nitro/fx.h"

#define REG_CP_DIVCNT (*(volatile u16 *) 0x04000280)
#define REG_CP_DIV_NUMER (*(volatile u64 *) 0x04000290)
#define REG_CP_DIV_DENOM (*(volatile u64 *) 0x04000298)
#define REG_CP_DIV_RESULT (*(volatile u64 *) 0x040002a0)
#define REG_CP_DIV_RESULT_64 (*(fx64c *) 0x040002a0)    // (Read as one 64-bit value)
#define REG_CP_SQRTCNT (*(volatile u16 *) 0x040002b0)
#define REG_CP_SQRT_RESULT (*(volatile u32 *) 0x040002b4)
#define REG_CP_SQRT_PARAM (*(volatile u64 *) 0x040002b8)

#define CP_DIV_64_32BIT_MODE 1
#define CP_SQRT_64BIT_MODE 1
#define CP_BUSY 0x8000

fx32 FX_Div(fx32 numer, fx32 denom) {
    FX_DivAsync(numer, denom);
    return FX_GetDivResult();
}

fx64c FX_InvFx64c(fx32 denom) {
    FX_InvAsync(denom);
    while (REG_CP_DIVCNT & CP_BUSY) {
    }
    return REG_CP_DIV_RESULT_64;
}

fx32 FX_Sqrt(fx32 x) {
    if (x > 0) {
        REG_CP_SQRTCNT = CP_SQRT_64BIT_MODE;
        REG_CP_SQRT_PARAM = (u64) (u32) x << 32;
        return FX_GetSqrtResult();
    }
    return 0;
}

fx64c FX_GetDivResultFx64c(void) {
    while (REG_CP_DIVCNT & CP_BUSY) {
    }
    return REG_CP_DIV_RESULT_64;
}

fx32 FX_GetDivResult(void) {
    while (REG_CP_DIVCNT & CP_BUSY) {
    }
    return (fx32) ((REG_CP_DIV_RESULT + (1 << 19)) >> 20);
}

void FX_InvAsync(fx32 denom) {
    REG_CP_DIVCNT = CP_DIV_64_32BIT_MODE;
    REG_CP_DIV_NUMER = (u64) FX32_ONE << 32;
    REG_CP_DIV_DENOM = (u32) denom;
}

fx32 FX_GetSqrtResult(void) {
    while (REG_CP_SQRTCNT & CP_BUSY) {
    }
    return (fx32) ((REG_CP_SQRT_RESULT + (1 << 9)) >> 10);
}

void FX_DivAsync(fx32 numer, fx32 denom) {
    REG_CP_DIVCNT = CP_DIV_64_32BIT_MODE;
    REG_CP_DIV_NUMER = (s64) numer << 32;
    REG_CP_DIV_DENOM = (u32) denom;
}
