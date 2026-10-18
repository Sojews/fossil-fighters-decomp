#ifndef GAME_DINO_EVENT_REVIVE_FIELD_RESUME_TASK_H
#define GAME_DINO_EVENT_REVIVE_FIELD_RESUME_TASK_H

#include "game/dino_field.h"
#include "m/task.h"

class DinoEventReviveFieldResumeTask : public MProcess {
public:
    DinoEventReviveFieldResumeTask(DinoEventLoop *event) : MProcess(NULL), mEvent(event), mStep(0) {}
    virtual ~DinoEventReviveFieldResumeTask();
    virtual void Idle();

    DinoEventLoop *mEvent;  // 0x74
    u32 mStep;          // 0x78
};

#endif
