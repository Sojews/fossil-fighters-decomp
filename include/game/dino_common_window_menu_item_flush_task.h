#ifndef GAME_DINO_COMMON_WINDOW_MENU_ITEM_FLUSH_TASK_H
#define GAME_DINO_COMMON_WINDOW_MENU_ITEM_FLUSH_TASK_H

#include "m/task.h"

class DinoCommonWindowMenu;

// Flashes a decided item's row of the menu's window, then exits.
class DinoCommonWindowMenuItemFlushTask : public MTask {
public:
    DinoCommonWindowMenuItemFlushTask(DinoCommonWindowMenu *menu, int index) : mMenu(menu), mIndex(index), mFrame(0) {}
    virtual ~DinoCommonWindowMenuItemFlushTask();
    virtual void Execute();

    DinoCommonWindowMenu *mMenu;    // 0x10
    int mIndex;                     // 0x14: the item's row
    int mFrame;                     // 0x18
};

#endif
