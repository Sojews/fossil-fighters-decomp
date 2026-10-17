// A fixed-point number with arithmetic operators, and a point made of two.
#ifndef M_FX32_H
#define M_FX32_H

#include "nitro/fx.h"

class MFx32 {
public:
    MFx32() {}
    MFx32(int i) { v = i << FX32_SHIFT; }

    bool operator==(MFx32 o) const { return v == o.v; }

    fx32 v;
};

inline bool operator!=(MFx32 a, MFx32 b) {
    return a.v != b.v;
}

inline MFx32 operator+(MFx32 a, MFx32 b) {
    MFx32 r;
    r.v = a.v + b.v;
    return r;
}

inline MFx32 operator-(MFx32 a, MFx32 b) {
    MFx32 r;
    r.v = a.v - b.v;
    return r;
}

inline MFx32 operator*(MFx32 a, MFx32 b) {
    MFx32 r;
    r.v = FX_Mul(a.v, b.v);
    return r;
}

inline MFx32 operator/(MFx32 a, MFx32 b) {
    MFx32 r;
    r.v = FX_Div(a.v, b.v);
    return r;
}

struct MFx32Point {
    MFx32Point() {}
    MFx32Point(MFx32 x_, MFx32 y_) {
        x = x_;
        y = y_;
    }

    void Set(MFx32 x_, MFx32 y_) {
        x = x_;
        y = y_;
    }

    MFx32 x;
    MFx32 y;
};

inline MFx32Point operator-(const MFx32Point &a, const MFx32Point &b) {
    return MFx32Point(a.x - b.x, a.y - b.y);
}

// The angle (0-0xFFFF) of the direction (x, y).
inline u16 MFx32Atan2(MFx32 y, MFx32 x) {
    return FX_Atan2Idx(y.v, x.v);
}

#endif
