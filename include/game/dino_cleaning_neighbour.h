#ifndef GAME_DINO_CLEANING_NEIGHBOUR_H
#define GAME_DINO_CLEANING_NEIGHBOUR_H

#include "m/geometry.h"

// The cell next to pos in a direction (0 right, then counterclockwise), for even and odd columns.
// (Not static: the game has one table and init guard shared by every file that uses it.)
inline MPoint GetNeighbour(MPoint pos, u32 dir) {
    static const MPoint sOffsets[2][6] = {
        {MPoint(1, 0), MPoint(0, 1), MPoint(-1, 1), MPoint(-1, 0), MPoint(-1, -1), MPoint(0, -1)},
        {MPoint(1, 0), MPoint(1, 1), MPoint(0, 1), MPoint(-1, 0), MPoint(0, -1), MPoint(1, -1)},
    };
    const MPoint &d = sOffsets[pos.y & 1][dir];
    return MPoint(pos.x + d.x, pos.y + d.y);
}

#endif
