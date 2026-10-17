#ifndef GAME_SEND_LOCAL_MESSAGE_TASK_H
#define GAME_SEND_LOCAL_MESSAGE_TASK_H

#include "m/task.h"

class SendLocalMessageTask : public MTask {
public:
    virtual ~SendLocalMessageTask();
    virtual void Execute();
};

#endif
