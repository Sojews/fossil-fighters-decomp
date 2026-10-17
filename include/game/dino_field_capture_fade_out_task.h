#ifndef GAME_DINO_FIELD_CAPTURE_FADE_OUT_TASK_H
#define GAME_DINO_FIELD_CAPTURE_FADE_OUT_TASK_H

#include "m/task.h"
#include "game/help.h"

class DinoFieldCaptureFadeOutTask : public MProcess, public DinoHelpDisabler {
public:
    virtual ~DinoFieldCaptureFadeOutTask();
    virtual void Idle();
};

#endif
