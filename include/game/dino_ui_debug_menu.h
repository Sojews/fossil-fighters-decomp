#ifndef GAME_DINO_UI_DEBUG_MENU_H
#define GAME_DINO_UI_DEBUG_MENU_H

#include "m/task.h"

class DinoUiDebugMenu : public MProcess {
public:
    DinoUiDebugMenu();
    virtual ~DinoUiDebugMenu();
    virtual void Idle();

    void StateStart();
    void StateChosen();

    void (DinoUiDebugMenu::*mState)(); // 0x74
    u32 mUnk7C;                        // 0x7C
};

#endif
