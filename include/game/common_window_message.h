#ifndef GAME_COMMON_WINDOW_MESSAGE_H
#define GAME_COMMON_WINDOW_MESSAGE_H

#include <string>
#include "m/task.h"
#include "m/geometry.h"

// A message window process.
class DinoCommonWindowMessage : public MProcess {
public:
    void SetUnkA8(u32 value);
    void SetUnkA4(u8 value);
    DinoCommonWindowMessage(MPoint pos, const std::string &text, int arg3, int arg4);
    DinoCommonWindowMessage(MDisplay *display, MPoint pos, const std::string &text, int arg3, int arg4);
    virtual ~DinoCommonWindowMessage();
    virtual void Idle();

    u32 mUnk74[2];      // 0x74
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
