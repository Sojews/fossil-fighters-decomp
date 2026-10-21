#ifndef GAME_DINO_EVENT_MUSEUM_H
#define GAME_DINO_EVENT_MUSEUM_H

#include "game/dino_event_ui_task.h"

class DinoEventMuseum : public DinoEventUiTask {
public:
    DinoEventMuseum(DinoEventLoop *event) : DinoEventUiTask(event, 4, 0) {}
    virtual ~DinoEventMuseum();
    virtual MTask *CreateUi();
};

#endif
