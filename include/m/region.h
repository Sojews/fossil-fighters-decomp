// A region: a set of non-overlapping rectangles, with union, subtraction and intersection.
#ifndef M_REGION_H
#define M_REGION_H

#include <vector>
#include "m/geometry.h"

class MRegion {
public:
    MRegion();
    MRegion(MRect rect);
    MRegion(const MRegion &other);
    MRegion &operator=(const MRegion &other);

    u32 GetCount();
    MRect GetRect(u32 i);
    void Set(MRect rect);
    void Add(MRect rect);
    void Add(const MRegion &other);
    void Subtract(MRect rect);
    void Subtract(const MRegion &other);
    void Intersect(MRect rect);
    bool IsEmpty() const;

    std::vector<MRect> mRects;  // 0x00
    std::vector<MRect> mTemp;   // 0x0C: the result while one is worked out
};

#endif
