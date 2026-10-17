// The talk window and its name panel.
#ifndef GAME_DINO_TALK_WINDOW_H
#define GAME_DINO_TALK_WINDOW_H

#include <string>
#include "m/task.h"

class DinoTalkWindow : public MProcess {
public:
    void ClearUnkF4();
    void SetUnkAC(u32 value);
    void SetUnkB0(u8 value);
    u8 GetUnkB0();
    virtual ~DinoTalkWindow();
    virtual void Idle();

    void (DinoTalkWindow::*mState)(); // 0x74
    std::wstring mText; // 0x7C
    u8 unk88[0x24];
    u32 mUnkAC;         // 0xAC
    u8 mUnkB0;          // 0xB0
    u8 unkB1[0x43];
    u8 mUnkF4;          // 0xF4
};

class DinoTalkWindowNamePanel : public MProcess {
public:
    virtual ~DinoTalkWindowNamePanel();
    virtual void Idle();

    u32 mUnk74;         // 0x74
    std::wstring mText; // 0x78
};

#endif
