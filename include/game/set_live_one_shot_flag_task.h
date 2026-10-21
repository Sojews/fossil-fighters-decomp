#ifndef GAME_SET_LIVE_ONE_SHOT_FLAG_TASK_H
#define GAME_SET_LIVE_ONE_SHOT_FLAG_TASK_H

#include "m/task.h"

class SetLiveOneShotFlagTask : public MTask {
public:
    SetLiveOneShotFlagTask(void *field, u32 flag) {
        mUnk10 = (u32)field;
        mUnk14 = flag;
    }
    virtual ~SetLiveOneShotFlagTask();
    virtual void Execute();

    u32 mUnk10;         // 0x10
    u32 mUnk14;         // 0x14
};

#endif
