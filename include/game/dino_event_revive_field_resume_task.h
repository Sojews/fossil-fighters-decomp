#ifndef GAME_DINO_EVENT_REVIVE_FIELD_RESUME_TASK_H
#define GAME_DINO_EVENT_REVIVE_FIELD_RESUME_TASK_H

#include "m/task.h"

class DinoEventReviveFieldResumeTask : public MProcess {
public:
    virtual ~DinoEventReviveFieldResumeTask();
    virtual void Idle();
};

#endif
