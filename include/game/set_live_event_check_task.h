#ifndef GAME_SET_LIVE_EVENT_CHECK_TASK_H
#define GAME_SET_LIVE_EVENT_CHECK_TASK_H

#include "m/task.h"

class SetLiveEventCheckTask : public MTask {
public:
    virtual ~SetLiveEventCheckTask();
    virtual void Execute();
};

#endif
