#ifndef GAME_DINO_EVENT_ENDING_H
#define GAME_DINO_EVENT_ENDING_H

#include "game/dino_event_ui_task.h"

class DinoEventEnding : public DinoEventUiTask {
public:
    virtual ~DinoEventEnding();
    virtual MTask *CreateUi();
};

#endif
