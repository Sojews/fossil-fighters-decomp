#ifndef GAME_DINO_UI_REVIVE_STATUS_DIGIT_H
#define GAME_DINO_UI_REVIVE_STATUS_DIGIT_H

#include "m/task.h"
#include "m/motion.h"

class DinoUiReviveStatusDigit : public MProcess {
public:
    virtual ~DinoUiReviveStatusDigit();
    virtual void Idle();

    // Counts the number up to the value.
    MTask *CreateCountTask(u32 value, u8 unk88, u8 unk);

    u32 unk74;
    MMotion *mMotion;       // 0x78: shown in place of the digits at the top value
    MMotion *mDigits[3];    // 0x7C
    u8 mUnk88;              // 0x88
    u32 mValue;             // 0x8C
    u8 mUnk90;              // 0x90
    u8 mUnk91;              // 0x91
    u8 mUnk92;              // 0x92
};

#endif
