#ifndef M_GEOMETRY_H
#define M_GEOMETRY_H

#include "types.h"

struct MPoint {
    MPoint() {}
    MPoint(int x_, int y_) {
        x = x_;
        y = y_;
    }

    void Set(int x_, int y_) {
        x = x_;
        y = y_;
    }

    int x;
    int y;
};

struct MRect {
    MRect() {}
    MRect(int left_, int top_, int right_, int bottom_) {
        left = left_;
        top = top_;
        right = right_;
        bottom = bottom_;
    }
    MRect(MPoint topLeft, MPoint bottomRight) {
        left = topLeft.x;
        top = topLeft.y;
        right = bottomRight.x;
        bottom = bottomRight.y;
    }

    int GetWidth() const { return right - left; }
    int GetHeight() const { return bottom - top; }
    MPoint GetCenter() const { return MPoint((left + right) >> 1, (top + bottom) >> 1); }
    bool Contains(MPoint p) const {
        int x = p.x;
        int y = p.y;
        return left <= x && x < right && top <= y && y < bottom;
    }

    int left;
    int top;
    int right;
    int bottom;
};

#endif
