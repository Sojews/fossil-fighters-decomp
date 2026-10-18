#ifndef GAME_DINO_UI_PARTS_BLACK_BELT_H
#define GAME_DINO_UI_PARTS_BLACK_BELT_H

#include "m/task.h"

// A black band across the top screen.
class DinoUiPartsBlackBelt : public MProcess {
public:
    DinoUiPartsBlackBelt();
    virtual ~DinoUiPartsBlackBelt();
    virtual void Idle();
};

#endif
