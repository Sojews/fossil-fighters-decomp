#ifndef GAME_ENABLE_ROOT_SELECT_TASK_H
#define GAME_ENABLE_ROOT_SELECT_TASK_H

#include "m/task.h"

class EnableRootSelectTask : public MTask {
public:
    virtual ~EnableRootSelectTask();
    virtual void Execute();
};

#endif
