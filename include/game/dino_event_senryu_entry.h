#ifndef GAME_DINO_EVENT_SENRYU_ENTRY_H
#define GAME_DINO_EVENT_SENRYU_ENTRY_H

#include "game/dino_event_capture_fade_ui_task.h"

class DinoEventSenryuEntry : public DinoEventCaptureFadeUiTask {
public:
    virtual ~DinoEventSenryuEntry();
    virtual MTask *CreateUi();
};

#endif
