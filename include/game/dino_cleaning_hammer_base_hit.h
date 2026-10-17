#ifndef GAME_DINO_CLEANING_HAMMER_BASE_HIT_H
#define GAME_DINO_CLEANING_HAMMER_BASE_HIT_H

#include "m/task.h"

class DinoCleaningHammerBaseHit : public MProcess {
public:
    virtual ~DinoCleaningHammerBaseHit();
    virtual void Idle();

    void (DinoCleaningHammerBaseHit::*mState)(); // 0x74
};

#endif
