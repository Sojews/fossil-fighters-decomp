#ifndef GAME_ISLAND_MAP_KEY_CHECK_TASK_H
#define GAME_ISLAND_MAP_KEY_CHECK_TASK_H

#include "m/task.h"

class IslandMapKeyCheckTask : public MTask {
public:
    virtual ~IslandMapKeyCheckTask();
    virtual void Execute();
};

#endif
