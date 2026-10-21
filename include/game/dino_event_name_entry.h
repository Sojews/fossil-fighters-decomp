#ifndef GAME_DINO_EVENT_NAME_ENTRY_H
#define GAME_DINO_EVENT_NAME_ENTRY_H

#include "game/dino_event_ui_task.h"

class DinoEventNameEntry : public DinoEventUiTask {
public:
    DinoEventNameEntry(DinoEventLoop *event, u8 unk81) : DinoEventUiTask(event, 5, 1) {
        mUnk81 = unk81;
    }
    virtual ~DinoEventNameEntry();
    virtual MTask *CreateUi();

    u8 mUnk81;      // 0x81
};

#endif
