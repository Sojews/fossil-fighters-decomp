#ifndef GAME_DINO_CLEANING_HAMMER_TASK_H
#define GAME_DINO_CLEANING_HAMMER_TASK_H

#include "m/task.h"

class DinoCleaningHammerTask : public MProcess {
public:
    virtual ~DinoCleaningHammerTask();
    virtual void Idle();

    void (DinoCleaningHammerTask::*mState)(); // 0x74
};

#endif
