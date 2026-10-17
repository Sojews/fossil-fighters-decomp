#ifndef GAME_OPTION_KEY_CHECK_TASK_H
#define GAME_OPTION_KEY_CHECK_TASK_H

#include "m/task.h"

class DinoUiOptionMenu;

class OptionKeyCheckTask : public MTask {
public:
    virtual ~OptionKeyCheckTask();
    virtual void Execute();

    DinoUiOptionMenu *mMenu;    // 0x10
};

#endif
