#ifndef GAME_DINO_UI_SAVE_OPTION_APPEND_ITEM_TASK_H
#define GAME_DINO_UI_SAVE_OPTION_APPEND_ITEM_TASK_H

#include "m/task.h"

class DinoUiSaveOptionAppendItemTask : public MTask {
public:
    virtual ~DinoUiSaveOptionAppendItemTask();
    virtual void Execute();
};

#endif
