#ifndef GAME_DINO_TALK_CONSOLE_FILTER_H
#define GAME_DINO_TALK_CONSOLE_FILTER_H

#include "m/console_filter.h"

class MFont;
class DinoTalkWindow;

class DinoTalkConsoleFilter : public MConsoleFilter {
public:
    DinoTalkConsoleFilter(DinoTalkWindow *window) : mFont(NULL), mWindow(window), mSe(0) {}
    DinoTalkConsoleFilter(MFont *font, DinoTalkWindow *window, u32 se) : mFont(font), mWindow(window), mSe(se) {}
    virtual ~DinoTalkConsoleFilter();

    MFont *mFont;               // 0x04
    DinoTalkWindow *mWindow;    // 0x08
    u32 mSe;                    // 0x0C: the typing sound
};

#endif
