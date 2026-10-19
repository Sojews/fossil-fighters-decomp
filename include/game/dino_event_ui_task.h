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
    // Fades the field out and closes it, loads the UI's overlays, runs the UI, then brings the field back.
    virtual void Idle();

    DinoEventLoop *mEvent;  // 0x74
    u32 mArg;               // 0x78: which overlays the UI needs
    u32 mStep;              // 0x7C
    u8 mFlag;               // 0x80: fade the other way
};

#endif
