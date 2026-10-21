#ifndef GAME_DINO_COMMON_WINDOW_CONSOLE_FILTER_H
#define GAME_DINO_COMMON_WINDOW_CONSOLE_FILTER_H

#include "m/console_filter.h"

class MConsole;
class DinoCommonFont;

class DinoCommonWindowConsoleFilter : public MConsoleFilter {
public:
    virtual ~DinoCommonWindowConsoleFilter();
    // Handles a '$' escape (p/N: the player's or the rock's name, f: clear, n: font size, v/V: a local or system
    // variable, c: color).
    virtual BOOL Filter(MConsole *console, int c);

    DinoCommonFont *mFont; // 0x04
};

#endif
