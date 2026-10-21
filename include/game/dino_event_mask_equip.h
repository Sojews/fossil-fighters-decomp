#ifndef GAME_DINO_EVENT_MASK_EQUIP_H
#define GAME_DINO_EVENT_MASK_EQUIP_H

#include "game/dino_event_capture_fade_ui_task.h"

class DinoEventMaskEquip : public DinoEventCaptureFadeUiTask {
public:
    DinoEventMaskEquip(DinoEventLoop *event) : DinoEventCaptureFadeUiTask(event, 3) {}
    virtual ~DinoEventMaskEquip();
    virtual MTask *CreateUi();
};

#endif
