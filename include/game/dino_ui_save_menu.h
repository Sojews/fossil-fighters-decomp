#ifndef GAME_DINO_UI_SAVE_MENU_H
#define GAME_DINO_UI_SAVE_MENU_H

#include "m/task.h"
#include "game/help.h"

class DinoUiSaveMenu : public MProcess, public DinoHelpBase {
public:
    virtual ~DinoUiSaveMenu();
    virtual void Idle();

    void (DinoUiSaveMenu::*mState)(); // 0x80
};

#endif
