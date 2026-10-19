#ifndef GAME_COMMON_WINDOW_MESSAGE_H
#define GAME_COMMON_WINDOW_MESSAGE_H

#include <string>
#include "m/task.h"
#include "m/geometry.h"

class MView;

// A message window process.
class DinoCommonWindowMessage : public MProcess {
public:
    void SetUnkA8(u32 value);
    void SetUnkA4(u8 value);
    DinoCommonWindowMessage(MPoint pos, const std::string &text, int arg3, int arg4);
    DinoCommonWindowMessage(MDisplay *display, MPoint pos, const std::string &text, int arg3, int arg4);
    virtual ~DinoCommonWindowMessage();
    virtual void Idle();
    // Not decompiled yet: Load makes the window (in the background), Start shows it.
    void Load();
    void Start();

    MView *mUnk74;      // 0x74
    MView *mUnk78;      // 0x78: shown and hidden with mUnk74 to blink
    MPoint mPos;        // 0x7C
    std::string mText;  // 0x84
    int mUnk90;         // 0x90
    int mUnk94;         // 0x94
    u8 mUnk98;          // 0x98
    int mUnk9C;         // 0x9C
    u32 mUnkA0;         // 0xA0
    u8 mUnkA4;          // 0xA4
    u32 mUnkA8;         // 0xA8
};

#endif
