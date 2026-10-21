#ifndef GAME_DINO_EVENT_ENDING_H
#define GAME_DINO_EVENT_ENDING_H

#include "game/dino_event_ui_task.h"

class DinoEventEnding : public DinoEventUiTask {
public:
    DinoEventEnding(DinoEventLoop *event) : DinoEventUiTask(event, 2, 1) {}
    virtual ~DinoEventEnding();
    virtual MTask *CreateUi();
};

#endif
