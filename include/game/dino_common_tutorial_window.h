// A window showing text on one screen (its text starts with "$upper" or "$lower").
#ifndef GAME_DINO_COMMON_TUTORIAL_WINDOW_H
#define GAME_DINO_COMMON_TUTORIAL_WINDOW_H

#include <string>
#include "m/task.h"

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

    u32 mUnk74;         // 0x74
    u32 mUnk78;         // 0x78
    std::string mText;  // 0x7C
    u8 unk88[0x14];
};

#endif
