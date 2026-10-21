#ifndef GAME_DINO_COMMON_TUTORIAL_CONSOLE_FILTER_H
#define GAME_DINO_COMMON_TUTORIAL_CONSOLE_FILTER_H

#include "m/console_filter.h"

class DinoCommonTutorialWindow;
class DinoCommonFont;

class DinoCommonTutorialConsoleFilter : public MConsoleFilter {
public:
    DinoCommonTutorialConsoleFilter(DinoCommonTutorialWindow *window, DinoCommonFont *font)
        : mWindow(window), mFont(font), mUnk0C(1), mUnk0D(0) {}
    virtual ~DinoCommonTutorialConsoleFilter();

    DinoCommonTutorialWindow *mWindow; // 0x04
    DinoCommonFont *mFont;             // 0x08
    u8 mUnk0C;
    u8 mUnk0D;
};

#endif
