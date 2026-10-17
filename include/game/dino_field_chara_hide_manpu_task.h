#ifndef GAME_DINO_FIELD_CHARA_HIDE_MANPU_TASK_H
#define GAME_DINO_FIELD_CHARA_HIDE_MANPU_TASK_H

#include "m/task.h"

class DinoFieldCharaHideManpuTask : public MTransfer {
public:
    virtual ~DinoFieldCharaHideManpuTask();
    virtual void Transfer();
};

#endif
