#ifndef GAME_DINO_UI_FIELD_MENU_FRACTION4_H
#define GAME_DINO_UI_FIELD_MENU_FRACTION4_H

#include "m/motion.h"
#include "m/task.h"

// A fraction of numbers of up to 3 digits each.
class DinoUiFieldMenuFraction4 : public MProcess {
public:
    DinoUiFieldMenuFraction4(MSmartOam *oam, MPoint pos, u8 digits);
    virtual ~DinoUiFieldMenuFraction4();
    virtual void Idle();
    void SetNumerator(u8 n);
    void SetDenominator(u8 n);
    void SetPriority(int priority);

    MMotion *mNumerator[3];   // 0x74: ones first, leftwards
    MMotion *mDenominator[3]; // 0x80: ones first, rightwards from the highest digit
    MMotion *mSlash;          // 0x8C
    u8 mDigits;               // 0x90
};

#endif
