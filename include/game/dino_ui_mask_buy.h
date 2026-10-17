#ifndef GAME_DINO_UI_MASK_BUY_H
#define GAME_DINO_UI_MASK_BUY_H

#include "m/task.h"

class DinoUiMaskBuy : public MProcess {
public:
    DinoUiMaskBuy();
    virtual ~DinoUiMaskBuy();
    virtual void Idle();
};

#endif
