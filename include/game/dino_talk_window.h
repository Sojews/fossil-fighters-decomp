// The talk window and its name panel.
#ifndef GAME_DINO_TALK_WINDOW_H
#define GAME_DINO_TALK_WINDOW_H

#include <string>
#include "m/task.h"
#include "m/fx32.h"

class MConsole;

class DinoTalkWindow : public MProcess {
public:
    void SetUnkF4();
    void ClearUnkF4();
    void SetRate(MFx32 rate);
    void ResetRate();
    void Print(const std::string &text);
    BOOL IsEmpty();
    void NextPage();
    void SetUnkAC(u32 value);
    void SetUnkB0(u8 value);
    u8 GetUnkB0();
    virtual ~DinoTalkWindow();
    virtual void Idle();

    void (DinoTalkWindow::*mState)(); // 0x74
    std::wstring mText; // 0x7C
    u8 unk88[0xC];
    MConsole *mConsole; // 0x94
    u8 unk98[0x14];
    u32 mUnkAC;         // 0xAC
    u8 mUnkB0;          // 0xB0
    u8 unkB1[0xCC - 0xB1];
    MFx32 mRate;        // 0xCC
    u8 unkD0[0xF4 - 0xD0];
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
