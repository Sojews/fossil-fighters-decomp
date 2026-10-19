#ifndef GAME_DINO_EVENT_REVIVE_DEPOSIT_SELECT_H
#define GAME_DINO_EVENT_REVIVE_DEPOSIT_SELECT_H

#include <vector>
#include "game/dino_event_capture_fade_ui_task.h"

class DinoEventReviveDepositSelect : public DinoEventCaptureFadeUiTask {
public:
    DinoEventReviveDepositSelect(DinoEventLoop *event, std::vector<void *> *list)
        : DinoEventCaptureFadeUiTask(event, 3), mList(list) {}
    virtual ~DinoEventReviveDepositSelect();
    virtual MTask *CreateUi();

    std::vector<void *> *mList; // 0x84: gets what's chosen
};

#endif
