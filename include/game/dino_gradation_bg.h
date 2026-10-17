// A gradient background drawn with a palette raster.
#ifndef GAME_DINO_GRADATION_BG_H
#define GAME_DINO_GRADATION_BG_H

#include "m/task.h"
#include "m/raster.h"

class DinoGradationBg : public MTask {
public:
    virtual ~DinoGradationBg();
    virtual void Execute();

    MDisplay *mDisplay;     // 0x10
    MRasterPal *mRaster;    // 0x14
};

#endif
