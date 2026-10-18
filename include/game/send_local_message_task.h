#ifndef GAME_SEND_LOCAL_MESSAGE_TASK_H
#define GAME_SEND_LOCAL_MESSAGE_TASK_H

#include "m/task.h"

class MMessage;
class DinoBattleModel;

class SendLocalMessageTask : public MTask {
public:
    virtual ~SendLocalMessageTask();
    virtual void Execute();

    MMessage *mMessage;         // 0x10
    DinoBattleModel *mModel;    // 0x14
};

#endif
