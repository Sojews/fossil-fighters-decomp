#ifndef GAME_DINO_EVENT_UI_TASK_H
#define GAME_DINO_EVENT_UI_TASK_H

#include "m/task.h"

class DinoEventLoop;

class DinoEventUiTask : public MProcess {
public:
    DinoEventUiTask(DinoEventLoop *event, u32 arg, u8 flag);
    // The game's destructor is inline. Its vtable's owner (dino_event_ui_task.cpp) defines DINO_EVENT_UI_TASK_OWNER and emits it out of line
    // until the key function there is decompiled.
#ifdef DINO_EVENT_UI_TASK_OWNER
    virtual ~DinoEventUiTask();
#else
    virtual ~DinoEventUiTask() {}
#endif
    // Makes the UI this event shows.
    virtual MTask *CreateUi() = 0;

    DinoEventLoop *mEvent;  // 0x74
    u32 mArg;               // 0x78
    MTask *mUi;             // 0x7C
    u8 mFlag;               // 0x80
};

#endif
