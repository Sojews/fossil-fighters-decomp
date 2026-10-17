// The talk window and its name panel.
#ifndef GAME_DINO_TALK_WINDOW_H
#define GAME_DINO_TALK_WINDOW_H

#include <string>
#include "m/task.h"

class DinoTalkWindow : public MProcess {
public:
    virtual ~DinoTalkWindow();
    virtual void Idle();

    void (DinoTalkWindow::*mState)(); // 0x74
    std::wstring mText; // 0x7C
};

class DinoTalkWindowNamePanel : public MProcess {
public:
    virtual ~DinoTalkWindowNamePanel();
    virtual void Idle();

    u32 mUnk74;         // 0x74
    std::wstring mText; // 0x78
};

#endif
