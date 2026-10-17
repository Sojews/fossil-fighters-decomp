#ifndef GAME_DINO_CLEANING_ROCK_H
#define GAME_DINO_CLEANING_ROCK_H

#include "m/task.h"

class DinoCleaningRock : public MProcess {
public:
    virtual ~DinoCleaningRock();
    virtual void Idle();
};

#endif
