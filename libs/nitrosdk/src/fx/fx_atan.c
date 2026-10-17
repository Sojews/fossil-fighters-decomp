// NitroSDK FX: arctangent.

#include "nitro/fx.h"

extern const s16 FX_AtanIdxTable_[];

u16 FX_Atan2Idx(fx32 y, fx32 x) {
    fx32 a, b;
    int c;
    int sgn;

    if (y > 0) {
        if (x > 0) {
            if (x > y) {
                a = y;
                b = x;
                c = 0;
                sgn = 1;
            } else if (x < y) {
                a = x;
                b = y;
                c = 0x4000;
                sgn = 0;
            } else {
                return 0x2000;
            }
        } else if (x < 0) {
            x = -x;
            if (x < y) {
                a = x;
                b = y;
                c = 0x4000;
                sgn = 1;
            } else if (x > y) {
                a = y;
                b = x;
                c = 0x8000;
                sgn = 0;
            } else {
                return 0x6000;
            }
        } else {
            return 0x4000;
        }
    } else if (y < 0) {
        y = -y;
        if (x < 0) {
            x = -x;
            if (x > y) {
                a = y;
                b = x;
                c = -0x8000;
                sgn = 1;
            } else if (x < y) {
                a = x;
                b = y;
                c = -0x4000;
                sgn = 0;
            } else {
                return 0xA000;
            }
        } else if (x > 0) {
            if (x < y) {
                a = x;
                b = y;
                c = -0x4000;
                sgn = 1;
            } else if (x > y) {
                a = y;
                b = x;
                c = 0;
                sgn = 0;
            } else {
                return 0xE000;
            }
        } else {
            return 0xC000;
        }
    } else {
        if (x >= 0) {
            return 0;
        }
        return 0x8000;
    }

    if (b == 0) {
        return 0;
    }
    if (sgn) {
        return (u16) (c + FX_AtanIdxTable_[FX_Div(a, b) >> 5]);
    }
    return (u16) (c - FX_AtanIdxTable_[FX_Div(a, b) >> 5]);
}
