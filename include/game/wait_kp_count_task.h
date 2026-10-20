#ifndef GAME_WAIT_KP_COUNT_TASK_H
#define GAME_WAIT_KP_COUNT_TASK_H

#include "m/task.h"

class WaitKPCountTask : public MTask {
public:
    // Waits for the count to stop; there's nothing to wait for without a counter.
    WaitKPCountTask(void *owner) {
        mOwner = owner;
        if (owner == NULL) {
            Exit();
        }
    }
    virtual ~WaitKPCountTask();
    virtual void Execute();

    void *mOwner;   // 0x10
};

#endif
