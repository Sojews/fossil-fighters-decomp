#ifndef GAME_DINO_EVENT_SENRYU_LIST_ENTRY_H
#define GAME_DINO_EVENT_SENRYU_LIST_ENTRY_H

#include "game/dino_event_capture_fade_ui_task.h"

class DinoEventSenryuListEntry : public DinoEventCaptureFadeUiTask {
public:
    DinoEventSenryuListEntry(DinoEventLoop *event) : DinoEventCaptureFadeUiTask(event, 5) {}
    virtual ~DinoEventSenryuListEntry();
    virtual MTask *CreateUi();
};

#endif
