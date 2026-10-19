#ifndef GAME_DINO_UI_SAVE_SLOT_SELECT_MENU_CONFIRM_DELETE_TASK_H
#define GAME_DINO_UI_SAVE_SLOT_SELECT_MENU_CONFIRM_DELETE_TASK_H

#include "m/task.h"

class DinoUiSaveSlotSelectMenuConfirmDeleteTask : public MProcess {
public:
    virtual ~DinoUiSaveSlotSelectMenuConfirmDeleteTask();
    virtual void Idle();

    MTask *mDelete;     // 0x74: run if the player says yes
    BOOL mAnswer;       // 0x78
    u32 mStep;          // 0x7C
};

#endif
