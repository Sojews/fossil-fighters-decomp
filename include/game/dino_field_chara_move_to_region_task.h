#ifndef GAME_DINO_FIELD_CHARA_MOVE_TO_REGION_TASK_H
#define GAME_DINO_FIELD_CHARA_MOVE_TO_REGION_TASK_H

#include "m/task.h"

class DinoFieldCharaMoveToRegionTask : public MTransfer {
public:
    virtual ~DinoFieldCharaMoveToRegionTask();
    virtual void Transfer();
};

#endif
