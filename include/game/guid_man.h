#ifndef GAME_GUID_MAN_H
#define GAME_GUID_MAN_H

#include "game/help.h"
#include "m/task.h"

class GuidMan : public MTask {
public:
    GuidMan(MTaskSet *tasks, DinoHelpBase *help) {
        mTasks = tasks;
        mHelp = help;
    }
    virtual ~GuidMan();
    virtual void Execute();

    MTaskSet *mTasks;       // 0x10: help is on while this runs out of tasks
    DinoHelpBase *mHelp;    // 0x14
};

#endif
