#ifndef GAME_DINO_CLEANING_HAMMER_EFFECT_H
#define GAME_DINO_CLEANING_HAMMER_EFFECT_H

#include "m/task.h"

class DinoCleaningHammerEffect : public MProcess {
public:
    virtual ~DinoCleaningHammerEffect();
    virtual void Idle();
};

#endif
