#ifndef GAME_DINO_UI_STATUS_CHANGE_MODE_HELP_TASK_H
#define GAME_DINO_UI_STATUS_CHANGE_MODE_HELP_TASK_H

#include "game/help.h"
#include "m/task.h"

class DinoUiStatusChangeModeHelpTask : public DinoHelpBase, public MProcess {
public:
    virtual ~DinoUiStatusChangeModeHelpTask();
    virtual void Idle();
};

#endif
