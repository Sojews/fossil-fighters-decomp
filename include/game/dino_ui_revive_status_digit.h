#ifndef GAME_DINO_UI_REVIVE_STATUS_DIGIT_H
#define GAME_DINO_UI_REVIVE_STATUS_DIGIT_H

#include "m/task.h"
#include "m/motion.h"

class MSmartOam;

class DinoUiReviveStatusDigit : public MProcess {
public:
    DinoUiReviveStatusDigit(MSmartOam *oam, MPoint pos, u8 unk88, u32 value, u8 unk90, u8 unk91, u8 unk92);
    virtual ~DinoUiReviveStatusDigit();
    virtual void Idle();

    void SetValue(u32 value, u8 unk88);
    // Counts the number up to the value.
    MTask *CreateCountTask(u32 value, u8 unk88, u8 unk);

    MSmartOam *mOam;        // 0x74
    MMotion *mMotion;       // 0x78: shown in place of the digits at the top value
    MMotion *mDigits[3];    // 0x7C
    u8 mUnk88;              // 0x88
    u32 mValue;             // 0x8C
    u8 mUnk90;              // 0x90
    u8 mUnk91;              // 0x91
    u8 mUnk92;              // 0x92
};

#endif
