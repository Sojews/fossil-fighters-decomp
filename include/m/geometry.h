#ifndef M_GEOMETRY_H
#define M_GEOMETRY_H

#include "types.h"

struct MPoint {
    MPoint(int x_, int y_) {
        x = x_;
        y = y_;
    }

    int x;
    int y;
};

#endif
