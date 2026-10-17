// A gradient background drawn with a palette raster.
#ifndef GAME_DINO_GRADATION_BG_H
#define GAME_DINO_GRADATION_BG_H

#include "m/task.h"
#include "m/raster.h"

class DinoGradationBg : public MTask {
public:
    DinoGradationBg(MDisplay *display);
    virtual ~DinoGradationBg();
    virtual void Execute();

    // A vertical gradient from (r0, g0, b0) at the top to (r1, g1, b1) at the bottom; the second table is a
    // little brighter.
    void SetGradation(int r0, int g0, int b0, int r1, int g1, int b1);

    MDisplay *mDisplay;     // 0x10
    MRasterPal *mRaster;    // 0x14
    u16 *mFront;            // 0x18
    u16 *mBack;             // 0x1C
};

#endif
