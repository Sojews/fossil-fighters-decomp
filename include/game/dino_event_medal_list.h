#ifndef GAME_DINO_EVENT_MEDAL_LIST_H
#define GAME_DINO_EVENT_MEDAL_LIST_H

#include "game/dino_event_capture_fade_ui_task.h"

class DinoEventMedalList : public DinoEventCaptureFadeUiTask {
public:
    virtual ~DinoEventMedalList();
    virtual MTask *CreateUi();
};

#endif
