#ifndef GAME_DINO_EVENT_REVIVE_FIELD_SUSPEND_TASK_H
#define GAME_DINO_EVENT_REVIVE_FIELD_SUSPEND_TASK_H

#include "m/task.h"

class DinoEventReviveFieldSuspendTask : public MProcess {
public:
    virtual ~DinoEventReviveFieldSuspendTask();
    virtual void Idle();
};

#endif
