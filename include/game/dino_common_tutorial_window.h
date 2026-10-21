// A window showing text on one screen (its text starts with "$upper" or "$lower").
#ifndef GAME_DINO_COMMON_TUTORIAL_WINDOW_H
#define GAME_DINO_COMMON_TUTORIAL_WINDOW_H

#include <string>
#include "m/geometry.h"
#include "m/task.h"

class MSmartOam;
class MInput;
class MConsole;
class MView;

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
    virtual void OnSetActive(BOOL active);

    void SetActive(BOOL active);
    void ClearArrows();
    void ShowLongArrow(MPoint pos, int dir);
    void ShowShortArrow(MPoint pos, int dir);
    void ShowCorners(MRect rect);
    // Opens on the screen the current display isn't: its frame, the console printing the text and the page mark.
    void StateStart();
    // Pages through the text: A, B or the debug buttons hurry it up, then turn the page (or close at the end).
    void StateRead();
    void StateClose();

    void (DinoCommonTutorialWindow::*mState)(); // 0x74
    std::string mText;  // 0x7C
    MInput *mInput;     // 0x88
    MConsole *mConsole; // 0x8C
    MTaskSet *mArrows;  // 0x90: the pointing arrows shown
    MSmartOam *mOam;    // 0x94
    MView *mNextMark;   // 0x98: shown while a page waits
};

#endif
