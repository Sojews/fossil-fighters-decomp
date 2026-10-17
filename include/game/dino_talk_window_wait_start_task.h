#ifndef GAME_DINO_TALK_WINDOW_WAIT_START_TASK_H
#define GAME_DINO_TALK_WINDOW_WAIT_START_TASK_H

#include "m/task.h"

class DinoTalkWindowWaitStartTask : public MTask {
public:
    virtual ~DinoTalkWindowWaitStartTask();
    virtual void Execute();

    u8 *mStarted;   // 0x10: exits once it is set
};

#endif
