#ifndef GAME_DINO_FIELD_CAPTURE_FADE_IN_TASK_H
#define GAME_DINO_FIELD_CAPTURE_FADE_IN_TASK_H

#include "m/task.h"
#include "game/help.h"

class DinoFieldCaptureFadeInTask : public MProcess, public DinoHelpDisabler {
public:
    virtual ~DinoFieldCaptureFadeInTask();
    virtual void Idle();
};

#endif
