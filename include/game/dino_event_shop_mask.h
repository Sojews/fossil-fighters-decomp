#ifndef GAME_DINO_EVENT_SHOP_MASK_H
#define GAME_DINO_EVENT_SHOP_MASK_H

#include "game/dino_event_capture_fade_ui_task.h"

class DinoEventShopMask : public DinoEventCaptureFadeUiTask {
public:
    virtual ~DinoEventShopMask();
    virtual MTask *CreateUi();
};

#endif
