// The 3D engine's layer (BG 0 drawn by the 3D engine).
#ifndef M_3D_H
#define M_3D_H

#include "m/screen.h"

class M3d : public MScreen {
public:
    M3d(MDisplay *display, u32 unk44, u32 unk48);
    virtual ~M3d();

    u8 mUnk20[0xE4];
};

#endif
