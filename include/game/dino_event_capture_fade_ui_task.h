#ifndef GAME_DINO_EVENT_CAPTURE_FADE_UI_TASK_H
#define GAME_DINO_EVENT_CAPTURE_FADE_UI_TASK_H

#include "m/task.h"

class DinoEventCaptureFadeUiTask : public MProcess {
public:
    // The game's destructor is inline. Its vtable's owner (dino_event_ui_task.cpp) defines DINO_EVENT_CAPTURE_FADE_UI_TASK_OWNER and emits it out of line
    // until the key function there is decompiled.
#ifdef DINO_EVENT_CAPTURE_FADE_UI_TASK_OWNER
    virtual ~DinoEventCaptureFadeUiTask();
#else
    virtual ~DinoEventCaptureFadeUiTask() {}
#endif
};

#endif
