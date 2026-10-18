#ifndef GAME_ENABLE_ROOT_SELECT_TASK_H
#define GAME_ENABLE_ROOT_SELECT_TASK_H

#include "m/task.h"

class DinoBattleView;

class EnableRootSelectTask : public MTask {
public:
    EnableRootSelectTask(DinoBattleView *view) { mUnk10 = (u32) view; }
    virtual ~EnableRootSelectTask();
    virtual void Execute();

    u32 mUnk10;         // 0x10
};

#endif
