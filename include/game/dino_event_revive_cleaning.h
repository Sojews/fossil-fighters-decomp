#ifndef GAME_DINO_EVENT_REVIVE_CLEANING_H
#define GAME_DINO_EVENT_REVIVE_CLEANING_H

#include "m/task.h"

class DinoEventReviveCleaning : public MProcess {
public:
    virtual ~DinoEventReviveCleaning();
    virtual void Idle();
};

#endif
