#ifndef GAME_DINO_FIELD_RASTER_H
#define GAME_DINO_FIELD_RASTER_H

#include "m/task.h"

class MRasterIO;

// The heat-haze wobble of the field background: a raster table of BG0 scroll offsets.
class DinoFieldRaster : public MProcess {
public:
    DinoFieldRaster(u32 arg);
    virtual ~DinoFieldRaster();
    virtual void Idle();

    MRasterIO *mIO;     // 0x74
    u32 mUnk78;         // 0x78
    u32 mFrame;         // 0x7C
};

#endif
