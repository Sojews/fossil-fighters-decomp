#ifndef GAME_DINO_EVENT_SAVE_H
#define GAME_DINO_EVENT_SAVE_H

#include "game/dino_event_ui_task.h"

class DinoEventSave : public DinoEventUiTask {
public:
    virtual ~DinoEventSave();
    virtual MTask *CreateUi();

    u32 mUnk74[4];  // 0x74
    u32 mResult;    // 0x84
};

#endif
