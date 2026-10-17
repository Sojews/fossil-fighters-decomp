#ifndef GAME_DINO_UI_TITLE_H
#define GAME_DINO_UI_TITLE_H

#include "m/task.h"

class DinoUiTitle : public MProcess {
public:
    virtual ~DinoUiTitle();
    virtual void Idle();

    void (DinoUiTitle::*mState)(); // 0x74
};

#endif
