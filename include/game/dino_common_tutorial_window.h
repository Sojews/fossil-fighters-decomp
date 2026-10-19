// A window showing text on one screen (its text starts with "$upper" or "$lower").
#ifndef GAME_DINO_COMMON_TUTORIAL_WINDOW_H
#define GAME_DINO_COMMON_TUTORIAL_WINDOW_H

#include <string>
#include "m/geometry.h"
#include "m/task.h"

class MSmartOam;

class DinoCommonTutorialWindow : public MProcess {
public:
    DinoCommonTutorialWindow(MDisplay *display, std::string text);
    // The game's destructor is inline. Its vtable's owner (dino_common_tutorial_console_filter.cpp) defines DINO_COMMON_TUTORIAL_WINDOW_OWNER and emits it out of line
    // until the key function there is decompiled.
#ifdef DINO_COMMON_TUTORIAL_WINDOW_OWNER
    virtual ~DinoCommonTutorialWindow();
#else
    virtual ~DinoCommonTutorialWindow() {}
#endif
    virtual void Idle();

    void ShowLongArrow(MPoint pos, int dir);
    void ShowShortArrow(MPoint pos, int dir);
    void ShowCorners(MRect rect);

    void (DinoCommonTutorialWindow::*mState)(); // 0x74
    std::string mText;  // 0x7C
    u32 unk88[2];
    MTaskSet *mArrows;  // 0x90: the pointing arrows shown
    MSmartOam *mOam;    // 0x94
    u32 unk98;
};

#endif
