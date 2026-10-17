#ifndef GAME_DINO_FIELD_RASTER_H
#define GAME_DINO_FIELD_RASTER_H

#include "m/task.h"

class DinoFieldRaster : public MProcess {
public:
    virtual ~DinoFieldRaster();
    virtual void Idle();
};

#endif
