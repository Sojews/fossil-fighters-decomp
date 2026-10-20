#ifndef GAME_DINO_CLEANING_TALK_FILTER_H
#define GAME_DINO_CLEANING_TALK_FILTER_H

#include "m/console_filter.h"

class MConsole;
class DinoCleaningTalkWindow;
class DinoCommonFont;

// The cleaning talk window's console escapes ($p player name, $t/$q window values, $f clear, $w rate, $n font,
// $c shadow color).
class DinoCleaningTalkFilter : public MConsoleFilter {
public:
    virtual ~DinoCleaningTalkFilter();
    virtual BOOL Filter(MConsole *console, int c);

    DinoCleaningTalkWindow *mWindow;    // 0x04
    DinoCommonFont *mFont;              // 0x08
};

#endif
