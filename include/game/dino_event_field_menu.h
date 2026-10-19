#ifndef GAME_DINO_EVENT_FIELD_MENU_H
#define GAME_DINO_EVENT_FIELD_MENU_H

#include "game/dino_event_capture_fade_ui_task.h"

class DinoEventFieldMenu : public DinoEventCaptureFadeUiTask {
public:
    DinoEventFieldMenu(DinoEventLoop *event, u32 arg) : DinoEventCaptureFadeUiTask(event, 4), mUnk84(arg) {}
    virtual ~DinoEventFieldMenu();
    virtual MTask *CreateUi();

    u32 mUnk84;     // 0x84
};

#endif
