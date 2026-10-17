#ifndef GAME_WAIT_KP_COUNT_TASK_H
#define GAME_WAIT_KP_COUNT_TASK_H

#include "m/task.h"

class WaitKPCountTask : public MTask {
public:
    virtual ~WaitKPCountTask();
    virtual void Execute();

    void *mOwner;   // 0x10
};

#endif
