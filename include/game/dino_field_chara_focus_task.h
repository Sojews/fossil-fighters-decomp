#ifndef GAME_DINO_FIELD_CHARA_FOCUS_TASK_H
#define GAME_DINO_FIELD_CHARA_FOCUS_TASK_H

#include "m/task.h"

class DinoFieldCharaFocusTask : public MTransfer {
public:
    virtual ~DinoFieldCharaFocusTask();
    virtual void Transfer();
};

#endif
