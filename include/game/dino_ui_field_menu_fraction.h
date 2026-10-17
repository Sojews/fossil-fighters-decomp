#ifndef GAME_DINO_UI_FIELD_MENU_FRACTION_H
#define GAME_DINO_UI_FIELD_MENU_FRACTION_H

#include "m/task.h"

class DinoUiFieldMenuFraction : public MProcess {
public:
    virtual ~DinoUiFieldMenuFraction();
    virtual void Idle();
    void SetNumerator(u8 n);
    void SetDenominator(u8 n);
};

#endif
