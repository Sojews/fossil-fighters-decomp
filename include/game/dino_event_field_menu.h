#ifndef GAME_DINO_EVENT_FIELD_MENU_H
#define GAME_DINO_EVENT_FIELD_MENU_H

#include "game/dino_event_capture_fade_ui_task.h"

class DinoEventFieldMenu : public DinoEventCaptureFadeUiTask {
public:
    virtual ~DinoEventFieldMenu();
    virtual MTask *CreateUi();

    u32 mUnk80;     // 0x80
    u32 mUnk84;     // 0x84
};

#endif
