#ifndef GAME_DINO_UI_SAVE_OPTION_REMOVE_ITEM_TASK_H
#define GAME_DINO_UI_SAVE_OPTION_REMOVE_ITEM_TASK_H

#include "m/task.h"

class DinoUiSaveOptionRemoveItemTask : public MTask {
public:
    virtual ~DinoUiSaveOptionRemoveItemTask();
    virtual void Execute();
};

#endif
