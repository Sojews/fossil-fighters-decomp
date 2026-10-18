// The window event text is shown in, with optional choices.
#ifndef GAME_DINO_EVENT_TALK_WINDOW_H
#define GAME_DINO_EVENT_TALK_WINDOW_H

#include <string>
#include "m/task.h"
#include "game/dino_window_choice.h"

class MDisplay;

class DinoTalkWindow;

class DinoEventTalkWindow : public MProcess {
public:
    DinoEventTalkWindow(MDisplay *display, std::string text, int arg, u8 flag);
    virtual ~DinoEventTalkWindow();
    virtual void Idle();

    void AddEntry(u32 id, MRect rect, const std::string &text);
    void SetYesNo(u32 arg, MPoint pos, u32 unkB4, const std::string &a, const std::string &b, const std::string &c,
                  const std::string &d);
    void SetChoices(u32 arg, MPoint pos, const std::vector<DinoWindowChoice> &choices, u32 unkB4);

    void StatePage();

    void (DinoEventTalkWindow::*mState)();  // 0x74
    DinoTalkWindow *mWindow;                // 0x7C
    u32 unk80;
    std::string mText;                      // 0x84
    u32 mUnk90;                             // 0x90
    u32 unk94;
    u32 mUnk98;                             // 0x98
    u32 mUnk9C;                             // 0x9C
    MPoint mPos;                            // 0xA0
    std::vector<DinoWindowChoice> mChoices; // 0xA8
    u32 mUnkB4;                             // 0xB4
    u32 mUnkB8;                             // 0xB8
    std::string mUnkBC;                     // 0xBC
    std::string mUnkC8;                     // 0xC8
    std::string mUnkD4;                     // 0xD4
    std::string mUnkE0;                     // 0xE0
    std::vector<DinoCommonWindowMenuEntry> mEntries;    // 0xEC
};

#endif
