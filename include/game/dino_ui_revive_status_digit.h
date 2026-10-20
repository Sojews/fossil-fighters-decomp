#ifndef GAME_DINO_UI_REVIVE_STATUS_DIGIT_H
#define GAME_DINO_UI_REVIVE_STATUS_DIGIT_H

#include "m/task.h"

class DinoUiReviveStatusDigit : public MProcess {
public:
    virtual ~DinoUiReviveStatusDigit();
    virtual void Idle();

    // Counts the number up to the value.
    MTask *CreateCountTask(u32 value, u8 unk88, u8 unk);
};

#endif
