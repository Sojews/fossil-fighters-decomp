#ifndef M_GEOMETRY_H
#define M_GEOMETRY_H

#include "types.h"

struct MPoint {
    MPoint() {}
    MPoint(int x_, int y_) {
        x = x_;
        y = y_;
    }

    MPoint operator+(MPoint o) const { return MPoint(x + o.x, y + o.y); }
    bool operator==(const MPoint &o) const { return x == o.x && y == o.y; }

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

    void Offset(MPoint p) {
        left += p.x;
        top += p.y;
        right += p.x;
        bottom += p.y;
    }

    void MoveTo(MPoint p) {
        right += p.x - left;
        bottom += p.y - top;
        left = p.x;
        top = p.y;
    }

    int GetWidth() const { return right - left; }
    int GetHeight() const { return bottom - top; }
    MPoint GetSize() const { return MPoint(GetWidth(), GetHeight()); }
    MPoint GetCenter() const { return MPoint((left + right) >> 1, (top + bottom) >> 1); }
    bool Intersects(MRect o) const { return o.right > left && right > o.left && o.bottom > top && bottom > o.top; }
    MRect Intersection(MRect o) const {
        return MRect(left > o.left ? left : o.left, top > o.top ? top : o.top, right < o.right ? right : o.right,
                     bottom < o.bottom ? bottom : o.bottom);
    }
    bool operator==(const MRect &o) const {
        return left == o.left && top == o.top && right == o.right && bottom == o.bottom;
    }
    bool Contains(MRect o) const { return left <= o.left && o.right <= right && top <= o.top && o.bottom <= bottom; }
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
