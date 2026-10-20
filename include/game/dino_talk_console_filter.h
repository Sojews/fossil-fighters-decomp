#ifndef GAME_DINO_TALK_CONSOLE_FILTER_H
#define GAME_DINO_TALK_CONSOLE_FILTER_H

#include "m/console_filter.h"

class DinoTalkConsoleFilter : public MConsoleFilter {
public:
    DinoTalkConsoleFilter(u32 unk) : mUnk04(0), mUnk08(unk), mUnk0C(0) {}
    virtual ~DinoTalkConsoleFilter();

    u32 mUnk04;     // 0x04
    u32 mUnk08;     // 0x08
    u32 mUnk0C;     // 0x0C
};

#endif
