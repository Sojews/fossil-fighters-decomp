#ifndef GAME_DINO_UI_SAVE_OPTION_MENU_SAVE_TASK_H
#define GAME_DINO_UI_SAVE_OPTION_MENU_SAVE_TASK_H

#include "m/task.h"

class DinoUiSaveOptionMenu;

class DinoUiSaveOptionMenuSaveTask : public MProcess {
public:
    DinoUiSaveOptionMenuSaveTask(DinoUiSaveOptionMenu *menu, u32 arg) : MProcess(NULL) {
        mMenu = menu;
        mArg = arg;
        mStep = 0;
    }
    virtual ~DinoUiSaveOptionMenuSaveTask();
    virtual void Idle();

    DinoUiSaveOptionMenu *mMenu;    // 0x74
    u32 mArg;                       // 0x78: (a u32 *) set when the game was saved and the menu is left
    u32 mStep;                      // 0x7C
    u32 mResult;                    // 0x80: the save menu's
};

#endif
