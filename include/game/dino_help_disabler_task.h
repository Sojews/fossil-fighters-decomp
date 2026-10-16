#ifndef GAME_DINO_HELP_DISABLER_TASK_H
#define GAME_DINO_HELP_DISABLER_TASK_H

#include "m/task.h"
#include "game/help.h"

class DinoHelpDisablerTask : public MTask, public DinoHelpDisabler {
public:
    virtual ~DinoHelpDisablerTask();
};

#endif
