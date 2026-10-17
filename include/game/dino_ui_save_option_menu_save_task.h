#ifndef GAME_DINO_UI_SAVE_OPTION_MENU_SAVE_TASK_H
#define GAME_DINO_UI_SAVE_OPTION_MENU_SAVE_TASK_H

#include "m/task.h"

class DinoUiSaveOptionMenuSaveTask : public MProcess {
public:
    virtual ~DinoUiSaveOptionMenuSaveTask();
    virtual void Idle();
};

#endif
