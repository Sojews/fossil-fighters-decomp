// The window event text is shown in, with optional choices.
#ifndef GAME_DINO_EVENT_TALK_WINDOW_H
#define GAME_DINO_EVENT_TALK_WINDOW_H

#include <string>
#include "m/task.h"

class MDisplay;

class DinoEventTalkWindow : public MProcess {
public:
    DinoEventTalkWindow(MDisplay *display, std::string text, int arg, u8 flag);
    virtual void Idle();

    u8 unk74[0x84];
};

#endif
