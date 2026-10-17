#ifndef GAME_DINO_CLEANING_BLOWER_H
#define GAME_DINO_CLEANING_BLOWER_H

#include "m/task.h"

class DinoCleaningBlower : public MProcess {
public:
    virtual ~DinoCleaningBlower();
    virtual void Idle();
};

#endif
