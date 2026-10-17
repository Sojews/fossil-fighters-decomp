#ifndef GAME_DINO_CLEANING_SAMPLE_H
#define GAME_DINO_CLEANING_SAMPLE_H

#include "m/task.h"

class DinoCleaningSample : public MProcess {
public:
    virtual ~DinoCleaningSample();
    virtual void Idle();

    void (DinoCleaningSample::*mState)(); // 0x74
};

#endif
