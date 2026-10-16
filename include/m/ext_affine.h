// Extended affine BG screens: affine layers with 16-bit tile map entries, which can use an extended palette.
#ifndef M_EXT_AFFINE_H
#define M_EXT_AFFINE_H

#include "m/affine_screen.h"

class MPal;

class MExtAffine : public MAffineScreen {
public:
    MExtAffine(MDisplay *display, u32 screenBase, u32 base, int size, int bg);
    virtual ~MExtAffine() {}

    static u32 GetVramSize(int size);

    int GetSize();
    void SetPalette(MPal *pal);
    MPal *GetPalette();

    int mSize;      // 0x34
    MPal *mPal;     // 0x38
};

#endif
