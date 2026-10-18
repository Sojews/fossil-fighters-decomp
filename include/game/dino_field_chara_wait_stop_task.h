#ifndef GAME_DINO_FIELD_CHARA_WAIT_STOP_TASK_H
#define GAME_DINO_FIELD_CHARA_WAIT_STOP_TASK_H

#include "m/task.h"

class DinoFieldChara;

class DinoFieldCharaWaitStopTask : public MTask {
public:
    DinoFieldCharaWaitStopTask(DinoFieldChara *chara) { mChara = chara; }
    virtual ~DinoFieldCharaWaitStopTask();
    virtual void Execute();

    DinoFieldChara *mChara; // 0x10
};

#endif
