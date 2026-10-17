#ifndef GAME_DINO_UI_ATM_H
#define GAME_DINO_UI_ATM_H

#include "m/task.h"

class DinoUiATM : public MProcess {
public:
    virtual ~DinoUiATM();
    virtual void Idle();

    void (DinoUiATM::*mState)(); // 0x74
};

#endif
