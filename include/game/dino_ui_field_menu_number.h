#ifndef GAME_DINO_UI_FIELD_MENU_NUMBER_H
#define GAME_DINO_UI_FIELD_MENU_NUMBER_H

#include "m/motion.h"
#include "m/task.h"

// A number of up to 5 digits, without leading zeros.
class DinoUiFieldMenuNumber : public MProcess {
public:
    DinoUiFieldMenuNumber(MSmartOam *oam, MPoint pos);
    virtual ~DinoUiFieldMenuNumber();
    virtual void Idle();
    void SetValue(int value);

    MMotion *mDigits[5]; // 0x74: highest first
};

#endif
