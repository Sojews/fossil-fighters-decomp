#ifndef GAME_DINO_EVENT_REVIVE_CONTAINER_SELECT_H
#define GAME_DINO_EVENT_REVIVE_CONTAINER_SELECT_H

#include "m/task.h"

class DinoEventReviveContainerSelect : public MProcess {
public:
    virtual ~DinoEventReviveContainerSelect();
    virtual void Idle();
};

#endif
