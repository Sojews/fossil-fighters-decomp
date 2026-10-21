#ifndef GAME_DINO_EVENT_ATM_H
#define GAME_DINO_EVENT_ATM_H

#include "game/dino_event_ui_task.h"

class DinoEventATM : public DinoEventUiTask {
public:
    DinoEventATM(DinoEventLoop *event) : DinoEventUiTask(event, 5, 1) {}
    virtual ~DinoEventATM();
    virtual MTask *CreateUi();
};

#endif
