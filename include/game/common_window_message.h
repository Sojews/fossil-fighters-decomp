#ifndef GAME_COMMON_WINDOW_MESSAGE_H
#define GAME_COMMON_WINDOW_MESSAGE_H

#include <string>
#include "m/task.h"
#include "m/geometry.h"

// A message window process.
class DinoCommonWindowMessage : public MProcess {
public:
    DinoCommonWindowMessage(MDisplay *display, MPoint pos, const std::string &text, int arg3, int arg4);
    virtual ~DinoCommonWindowMessage();
    virtual void Idle();

    u32 mUnk74[(0xAC - 0x74) / 4];
};

#endif
