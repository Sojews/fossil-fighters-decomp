#ifndef GAME_DINO_EVENT_LICENSE_CARD_H
#define GAME_DINO_EVENT_LICENSE_CARD_H

#include "game/dino_event_ui_task.h"

class DinoEventLicenseCard : public DinoEventUiTask {
public:
    DinoEventLicenseCard(DinoEventLoop *event) : DinoEventUiTask(event, 3, 1) {}
    virtual ~DinoEventLicenseCard();
    virtual MTask *CreateUi();
};

#endif
