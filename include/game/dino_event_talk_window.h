// The window event text is shown in, with optional choices.
#ifndef GAME_DINO_EVENT_TALK_WINDOW_H
#define GAME_DINO_EVENT_TALK_WINDOW_H

#include <string>
#include "m/task.h"
#include "game/dino_window_choice.h"

class MDisplay;

class DinoEventTalkWindow : public MProcess {
public:
    DinoEventTalkWindow(MDisplay *display, std::string text, int arg, u8 flag);
    virtual ~DinoEventTalkWindow();
    virtual void Idle();

    void AddEntry(u32 id, MRect rect, const std::string &text);

    void (DinoEventTalkWindow::*mState)();  // 0x74
    u32 mUnk7C[2];                          // 0x7C
    std::string mText;                      // 0x84
    u32 mUnk90[6];                          // 0x90
    std::vector<DinoWindowChoice> mChoices; // 0xA8
    u32 mUnkB4[2];                          // 0xB4
    std::string mUnkBC;                     // 0xBC
    std::string mUnkC8;                     // 0xC8
    std::string mUnkD4;                     // 0xD4
    std::string mUnkE0;                     // 0xE0
    std::vector<DinoCommonWindowMenuEntry> mEntries;    // 0xEC
};

#endif
