#ifndef GAME_SET_LIVE_EVENT_CHECK_TASK_H
#define GAME_SET_LIVE_EVENT_CHECK_TASK_H

#include "m/task.h"

class DinoCommonLive;

class SetLiveEventCheckTask : public MTask {
public:
    virtual ~SetLiveEventCheckTask();
    virtual void Execute();

    DinoCommonLive *mLive;  // 0x10
    u8 mCheck;      // 0x14
};

#endif
