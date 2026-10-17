#ifndef GAME_DINO_UI_TOP_MENU_LOOP_H
#define GAME_DINO_UI_TOP_MENU_LOOP_H

#include "m/task.h"

class DinoUiTopMenuLoop : public MProcess {
public:
    virtual ~DinoUiTopMenuLoop();
    virtual void Idle();
};

#endif
