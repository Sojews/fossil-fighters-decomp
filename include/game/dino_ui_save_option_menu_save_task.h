#ifndef GAME_DINO_UI_SAVE_OPTION_MENU_SAVE_TASK_H
#define GAME_DINO_UI_SAVE_OPTION_MENU_SAVE_TASK_H

#include "m/task.h"

class DinoUiSaveOptionMenu;

class DinoUiSaveOptionMenuSaveTask : public MProcess {
public:
    DinoUiSaveOptionMenuSaveTask(DinoUiSaveOptionMenu *menu, u32 arg) : MProcess(NULL) {
        mMenu = menu;
        mArg = arg;
        mUnk7C = 0;
    }
    virtual ~DinoUiSaveOptionMenuSaveTask();
    virtual void Idle();

    DinoUiSaveOptionMenu *mMenu;    // 0x74
    u32 mArg;                       // 0x78
    u32 mUnk7C;                     // 0x7C
    u32 mUnk80;                     // 0x80
};

#endif
