// Character (tile) memory: a heap over a display's BG or OBJ character VRAM.
#ifndef M_CHR_H
#define M_CHR_H

#include "m/heap.h"

class MDisplay;

class MChr : public MHeap {
public:
    MChr(MDisplay *display, int charBase, u32 unk44, u32 base, int mode, u32 units, int boundary);

    MDisplay *mDisplay; // 0x38
    int mMode;          // 0x3C: indexes the bytes-per-unit table
    u32 mCharBase;      // 0x40: the BGCNT character base block
    u32 mUnk44;         // 0x44
    int mBoundary;      // 0x48: the OBJ tile boundary setting
};

#endif
