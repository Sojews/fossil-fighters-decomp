#ifndef GAME_DINO_UI_SAVE_OPTION_REMOVE_ITEM_TASK_H
#define GAME_DINO_UI_SAVE_OPTION_REMOVE_ITEM_TASK_H

#include "m/task.h"

class DinoUiSaveOptionMenu;

class DinoUiSaveOptionRemoveItemTask : public MTask {
public:
    DinoUiSaveOptionRemoveItemTask(DinoUiSaveOptionMenu *owner) { mOwner = owner; }
    virtual ~DinoUiSaveOptionRemoveItemTask();
    virtual void Execute();

    DinoUiSaveOptionMenu *mOwner;  // 0x10
};

#endif
