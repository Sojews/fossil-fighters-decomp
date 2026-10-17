#ifndef GAME_DINO_EVENT_NAME_ENTRY_H
#define GAME_DINO_EVENT_NAME_ENTRY_H

#include "game/dino_event_ui_task.h"

class DinoEventNameEntry : public DinoEventUiTask {
public:
    virtual ~DinoEventNameEntry();
    virtual MTask *CreateUi();

    u8 unk74[0xD];
    u8 mUnk81;      // 0x81
};

#endif
