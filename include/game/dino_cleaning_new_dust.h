#ifndef GAME_DINO_CLEANING_NEW_DUST_H
#define GAME_DINO_CLEANING_NEW_DUST_H

#include "m/task.h"

class DinoCleaningNewDust : public MProcess {
public:
    virtual ~DinoCleaningNewDust();
    virtual void Idle();
};

#endif
