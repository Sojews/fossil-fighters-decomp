#ifndef GAME_DINO_FIELD_CHARA_MODEL_MOTION_ID_TASK_H
#define GAME_DINO_FIELD_CHARA_MODEL_MOTION_ID_TASK_H

#include "m/task.h"

class DinoFieldCharaModelMotionIdTask : public MTransfer {
public:
    virtual ~DinoFieldCharaModelMotionIdTask();
    virtual void Transfer();
};

#endif
