// The character (tile) memory of a display: a heap over its BG or OBJ character VRAM.
#ifndef M_CHR_H
#define M_CHR_H

#include "m/heap.h"

class MChr : public MHeap {
public:
    void Update();

    u32 mUnk38;
    u32 mUnk3C;
    u32 mCharBase;  // 0x40: the BGCNT character base block
};

#endif
