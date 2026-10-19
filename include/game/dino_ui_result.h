#ifndef GAME_DINO_UI_RESULT_H
#define GAME_DINO_UI_RESULT_H

#include <string>
#include "m/task.h"

class DinoUiResult : public MProcess {
public:
    DinoUiResult(u32 arg0, u8 arg1, u32 count, u32 count2, const std::string &text);
    virtual ~DinoUiResult();
    virtual void Idle();

    u32 mUnk74;         // 0x74
    u8 mUnk78;          // 0x78
    u32 mCount;         // 0x7C: at most 99
    u32 mCount2;        // 0x80: at most 99
    std::string mText;  // 0x84
};

#endif
