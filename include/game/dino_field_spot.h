#ifndef GAME_DINO_FIELD_SPOT_H
#define GAME_DINO_FIELD_SPOT_H

#include "m/task.h"

// Fades both screens out to (or in from) a spot.
class DinoFieldSpotOut : public MProcess {
public:
    virtual ~DinoFieldSpotOut();
    virtual void Idle();
};

class DinoFieldSpotIn : public MProcess {
public:
    virtual ~DinoFieldSpotIn();
    virtual void Idle();
};

#endif
