#ifndef GAME_DINO_UI_ATM_H
#define GAME_DINO_UI_ATM_H

#include "m/task.h"

class DinoUiATM : public MProcess {
public:
    DinoUiATM(u8 arg);
    virtual ~DinoUiATM();
    virtual void Idle();

    void (DinoUiATM::*mState)();    // 0x74
    u8 mUnk7C;                      // 0x7C
    u8 unk7D[0xF];
};

#endif
