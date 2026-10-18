#ifndef GAME_DINO_EVENT_REVIVE_FIELD_SUSPEND_TASK_H
#define GAME_DINO_EVENT_REVIVE_FIELD_SUSPEND_TASK_H

#include "m/task.h"

class DinoEventLoop;

class DinoEventReviveFieldSuspendTask : public MProcess {
public:
    DinoEventReviveFieldSuspendTask(DinoEventLoop *event) : MProcess(NULL), mEvent(event), mStep(0) {}
    virtual ~DinoEventReviveFieldSuspendTask();
    virtual void Idle();

    DinoEventLoop *mEvent;  // 0x74
    u32 mStep;              // 0x78
};

#endif
