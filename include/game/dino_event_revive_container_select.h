#ifndef GAME_DINO_EVENT_REVIVE_CONTAINER_SELECT_H
#define GAME_DINO_EVENT_REVIVE_CONTAINER_SELECT_H

#include "m/task.h"

class DinoEventLoop;

class DinoEventReviveContainerSelect : public MProcess {
public:
    DinoEventReviveContainerSelect(s32 *result) : MProcess(NULL), mResult(result), mStep(0) {}
    virtual ~DinoEventReviveContainerSelect();
    virtual void Idle();

    s32 *mResult;       // 0x74: the rock chosen, or -1
    u32 mStep;          // 0x78
};

#endif
