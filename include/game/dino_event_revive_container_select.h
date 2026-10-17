#ifndef GAME_DINO_EVENT_REVIVE_CONTAINER_SELECT_H
#define GAME_DINO_EVENT_REVIVE_CONTAINER_SELECT_H

#include "m/task.h"

class DinoEvent;

class DinoEventReviveContainerSelect : public MProcess {
public:
    virtual ~DinoEventReviveContainerSelect();
    virtual void Idle();

    DinoEvent *mEvent;  // 0x74
    u32 mStep;          // 0x78
};

#endif
