#ifndef GAME_DINO_EVENT_CAPTURE_FADE_UI_TASK_H
#define GAME_DINO_EVENT_CAPTURE_FADE_UI_TASK_H

#include "m/task.h"

class DinoEventLoop;

class DinoEventCaptureFadeUiTask : public MProcess {
public:
    DinoEventCaptureFadeUiTask(DinoEventLoop *event, u32 arg);
    // The game's destructor is inline. Its vtable's owner (dino_event_ui_task.cpp) defines DINO_EVENT_CAPTURE_FADE_UI_TASK_OWNER and emits it out of line
    // until the key function there is decompiled.
#ifdef DINO_EVENT_CAPTURE_FADE_UI_TASK_OWNER
    virtual ~DinoEventCaptureFadeUiTask();
#else
    virtual ~DinoEventCaptureFadeUiTask() {}
#endif
    virtual void Idle();
    // Makes the UI this event shows.
    virtual MTask *CreateUi() = 0;
    // Closes the field, loads the UI's overlays and makes the UI (in the background).
    void LoadUi();

    DinoEventLoop *mEvent;  // 0x74
    u32 mArg;               // 0x78: which overlays the UI needs
    u32 mStep;              // 0x7C
    MTask *mUi;             // 0x80
};

#endif
