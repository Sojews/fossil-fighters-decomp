// A fixed-point number with arithmetic operators, and a point made of two.
#ifndef M_FX32_H
#define M_FX32_H

#include "nitro/fx.h"

class MFx32 {
public:
    MFx32() {}
    MFx32(int i) { v = i << FX32_SHIFT; }
    static MFx32 FromRaw(fx32 raw) {
        MFx32 r;
        r.v = raw;
        return r;
    }
    MFx32 Mul(MFx32 b) const;
    // (As a member: the left operand stays in memory.)
    MFx32 Times(MFx32 b) const;
    MFx32 &operator+=(MFx32 b);
    MFx32 &operator-=(MFx32 b);
    MFx32 &operator*=(MFx32 b);
    MFx32 &operator/=(MFx32 b);

    fx32 v;
};

inline bool operator==(MFx32 a, MFx32 b) {
    return a.v == b.v;
}

inline bool operator<(MFx32 a, MFx32 b) {
    return a.v < b.v;
}

inline bool operator>(MFx32 a, MFx32 b) {
    return a.v > b.v;
}

inline bool operator>=(MFx32 a, MFx32 b) {
    return a.v >= b.v;
}

inline bool operator<=(MFx32 a, MFx32 b) {
    return a.v <= b.v;
}

inline MFx32 operator-(MFx32 a) {
    MFx32 r;
    r.v = -a.v;
    return r;
}

inline int MFx32ToInt(MFx32 x) {
    return x.v >> FX32_SHIFT;
}

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

inline MFx32 MFx32::Mul(MFx32 b) const {
    MFx32 r;
    r.v = FX_Mul(v, b.v);
    return r;
}

inline MFx32 MFx32::Times(MFx32 b) const {
    return *this * b;
}

inline MFx32 &MFx32::operator+=(MFx32 b) {
    *this = *this + b;
    return *this;
}

inline MFx32 &MFx32::operator-=(MFx32 b) {
    *this = *this - b;
    return *this;
}

inline MFx32 &MFx32::operator*=(MFx32 b) {
    *this = *this * b;
    return *this;
}

inline MFx32 operator/(MFx32 a, MFx32 b) {
    MFx32 r;
    r.v = FX_Div(a.v, b.v);
    return r;
}

inline MFx32 &MFx32::operator/=(MFx32 b) {
    *this = *this / b;
    return *this;
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

    MFx32Point &operator=(const MFx32Point &o) {
        x = o.x;
        y = o.y;
        return *this;
    }

    MFx32Point &operator+=(const MFx32Point &o) {
        x += o.x;
        y += o.y;
        return *this;
    }
    MFx32Point &operator-=(const MFx32Point &o) {
        x -= o.x;
        y -= o.y;
        return *this;
    }
    // (With the hardware square root.)
    MFx32 Length() const;

    MFx32 x;
    MFx32 y;
};

inline bool operator==(const MFx32Point &a, const MFx32Point &b) {
    return a.x == b.x && a.y == b.y;
}

inline bool operator!=(const MFx32Point &a, const MFx32Point &b) {
    return a.x != b.x || a.y != b.y;
}

inline MFx32Point operator+(const MFx32Point &a, const MFx32Point &b) {
    return MFx32Point(a.x + b.x, a.y + b.y);
}

inline MFx32Point operator-(const MFx32Point &a, const MFx32Point &b) {
    return MFx32Point(a.x - b.x, a.y - b.y);
}

inline MFx32Point operator*(const MFx32Point &a, MFx32 b) {
    return MFx32Point(a.x * b, a.y * b);
}

inline MFx32Point operator/(const MFx32Point &a, MFx32 b) {
    return MFx32Point(a.x / b, a.y / b);
}

inline MFx32 MFx32Point::Length() const {
    MFx32 r;
    s64 xx = x.v;
    s64 yy = y.v;
    *(vu16 *) 0x040002B0 = 1;
    *(u64 *) 0x040002B8 = yy * yy + 0x1000 + xx * xx;
    while (*(vu16 *) 0x040002B0 & 0x8000) {
    }
    r.v = *(vu32 *) 0x040002B4;
    return r;
}

struct MFx32Circle {
    MFx32Circle(MFx32Point center, MFx32 r_) {
        x = center.x;
        y = center.y;
        r = r_;
    }

    // Whether it overlaps the rectangle.
    bool Overlaps(const struct MFx32Rect &rect) const;

    MFx32 x;
    MFx32 y;
    MFx32 r;
};

struct MFx32Rect {
    MFx32 left;
    MFx32 top;
    MFx32 right;
    MFx32 bottom;

    // The shortest move that takes the circle out of the rectangle.
    MFx32Point PushOut(const MFx32Circle &c) const;
};

inline MFx32Rect operator+(const MFx32Rect &a, MFx32Point b) {
    MFx32Rect r = a;
    r.left = r.left + b.x;
    r.top = r.top + b.y;
    r.right = r.right + b.x;
    r.bottom = r.bottom + b.y;
    return r;
}

inline bool MFx32Circle::Overlaps(const MFx32Rect &rect) const {
    MFx32 d = 0;
    if (x < rect.left) {
        d += (rect.left - x) * (rect.left - x);
    }
    if (x > rect.right) {
        d += (x - rect.right) * (x - rect.right);
    }
    if (y < rect.top) {
        d += (rect.top - y) * (rect.top - y);
    }
    if (y > rect.bottom) {
        d += (y - rect.bottom) * (y - rect.bottom);
    }
    return d < r * r;
}

inline MFx32Point MFx32Rect::PushOut(const MFx32Circle &c) const {
    MFx32Point to(c.x, c.y);
    MFx32 best;
    best.v = 0x7FFFFFFF;
    MFx32 d = c.y + c.r - top;
    if (d > MFx32(0) && d < best) {
        best = d;
        to.Set(c.x, top - c.r);
    }
    d = bottom - (c.y - c.r);
    if (d > MFx32(0) && d < best) {
        best = d;
        to.Set(c.x, bottom + c.r);
    }
    d = c.x + c.r - left;
    if (d > MFx32(0) && d < best) {
        best = d;
        to.Set(left - c.r, c.y);
    }
    d = right - (c.x - c.r);
    if (d > MFx32(0) && d < best) {
        best = d;
        to.Set(right + c.r, c.y);
    }
    return to - MFx32Point(c.x, c.y);
}

// The angle (0-0xFFFF) of the direction (x, y).
inline u16 MFx32Atan2(MFx32 y, MFx32 x) {
    return FX_Atan2Idx(y.v, x.v);
}

// The angle of the vector.
inline u16 MFx32PointAngle(MFx32Point v) {
    return MFx32Atan2(v.y, v.x);
}

#endif
