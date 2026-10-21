#ifndef GAME_ISLAND_MAP_KEY_CHECK_TASK_H
#define GAME_ISLAND_MAP_KEY_CHECK_TASK_H

#include "m/task.h"

class DinoUiIslandMapMenu;

class IslandMapKeyCheckTask : public MTask {
public:
    IslandMapKeyCheckTask(DinoUiIslandMapMenu *menu) { mMenu = menu; }
    virtual ~IslandMapKeyCheckTask();
    virtual void Execute();

    DinoUiIslandMapMenu *mMenu; // 0x10
};

#endif
