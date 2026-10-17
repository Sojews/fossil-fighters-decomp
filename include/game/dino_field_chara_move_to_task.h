#ifndef GAME_DINO_FIELD_CHARA_MOVE_TO_TASK_H
#define GAME_DINO_FIELD_CHARA_MOVE_TO_TASK_H

#include "m/task.h"

class DinoFieldCharaMoveToTask : public MTransfer {
public:
    virtual ~DinoFieldCharaMoveToTask();
    virtual void Transfer();
};

#endif
