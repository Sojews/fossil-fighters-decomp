#ifndef GAME_DINO_TALK_CONSOLE_FILTER_H
#define GAME_DINO_TALK_CONSOLE_FILTER_H

#include "m/console_filter.h"

class MFont;
class DinoTalkWindow;
class MConsole;

class DinoTalkConsoleFilter : public MConsoleFilter {
public:
    DinoTalkConsoleFilter(DinoTalkWindow *window) : mFont(NULL), mWindow(window), mSe(0) {}
    DinoTalkConsoleFilter(MFont *font, DinoTalkWindow *window, u32 se) : mFont(font), mWindow(window), mSe(se) {}
    virtual ~DinoTalkConsoleFilter();
    // Handles a '$' escape (p/N: the player's or the rock's name, a: hold, e: next page, f: clear, t: pause, q: shake,
    // w: rate, n: font size, v/V: a local or system variable, c: color); other characters play the typing sound.
    virtual BOOL Filter(MConsole *console, int c);

    MFont *mFont;               // 0x04
    DinoTalkWindow *mWindow;    // 0x08
    u32 mSe;                    // 0x0C: the typing sound
};

#endif
