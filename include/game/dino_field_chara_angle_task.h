#ifndef GAME_DINO_FIELD_CHARA_ANGLE_TASK_H
#define GAME_DINO_FIELD_CHARA_ANGLE_TASK_H

#include "m/task.h"

class DinoFieldCharaAngleTask : public MTransfer {
public:
    virtual ~DinoFieldCharaAngleTask();
    virtual void Transfer();
};

#endif
