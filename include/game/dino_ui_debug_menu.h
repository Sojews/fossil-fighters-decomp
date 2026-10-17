#ifndef GAME_DINO_UI_DEBUG_MENU_H
#define GAME_DINO_UI_DEBUG_MENU_H

#include "m/task.h"

class DinoUiDebugMenu : public MProcess {
public:
    virtual ~DinoUiDebugMenu();
    virtual void Idle();

    void (DinoUiDebugMenu::*mState)(); // 0x74
};

#endif
