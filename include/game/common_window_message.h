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
    DinoCommonWindowMessage(MDisplay *display, MPoint pos, const std::string &text, int arg3, int arg4);
    virtual ~DinoCommonWindowMessage();
    virtual void Idle();

    u32 mUnk74[4];      // 0x74
    std::string mText;  // 0x84
    u32 mUnk90[5];      // 0x90
    u8 mUnkA4;          // 0xA4
    u32 mUnkA8;         // 0xA8
};

#endif
