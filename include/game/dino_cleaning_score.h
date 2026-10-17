#ifndef GAME_DINO_CLEANING_SCORE_H
#define GAME_DINO_CLEANING_SCORE_H

#include "m/task.h"

class DinoCleaningScore : public MProcess {
public:
    virtual ~DinoCleaningScore();
    virtual void Idle();
};

#endif
