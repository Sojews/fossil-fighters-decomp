#ifndef GAME_DINO_COMMON_TUTORIAL_CONSOLE_FILTER_H
#define GAME_DINO_COMMON_TUTORIAL_CONSOLE_FILTER_H

#include "m/console_filter.h"

class DinoCommonTutorialWindow;
class DinoCommonFont;
class MConsole;

class DinoCommonTutorialConsoleFilter : public MConsoleFilter {
public:
    DinoCommonTutorialConsoleFilter(DinoCommonTutorialWindow *window, DinoCommonFont *font)
        : mWindow(window), mFont(font), mUnk0C(1), mUnk0D(0) {}
    virtual ~DinoCommonTutorialConsoleFilter();
    // Handles a '$' escape (B: the tutorial bgm, W: quiet, s: speed sound, f: clear, w: rate, n: font size, p: the
    // player's name, c: color, S/L: point an arrow, F: mark a rect, H: clear them); other characters play the typing
    // sounds.
    virtual BOOL Filter(MConsole *console, int c);

    DinoCommonTutorialWindow *mWindow; // 0x04
    DinoCommonFont *mFont;             // 0x08
    u8 mUnk0C;
    u8 mUnk0D;
};

#endif
