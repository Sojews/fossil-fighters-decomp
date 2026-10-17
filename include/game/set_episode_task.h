#ifndef GAME_SET_EPISODE_TASK_H
#define GAME_SET_EPISODE_TASK_H

#include "m/task.h"
#include "game/dino_field.h"

class SetEpisodeTask : public MTask {
public:
    virtual ~SetEpisodeTask();
    virtual void Execute();

    DinoEventLoop *mEvent;  // 0x10
    u32 mId;            // 0x14
    u8 mSeen;           // 0x18
};

#endif
