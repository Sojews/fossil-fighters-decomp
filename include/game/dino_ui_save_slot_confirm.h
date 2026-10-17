#ifndef GAME_DINO_UI_SAVE_SLOT_CONFIRM_H
#define GAME_DINO_UI_SAVE_SLOT_CONFIRM_H

#include "game/dino_ui_save_slot_select_menu.h"
#include "m/task.h"

class DinoUiSaveSlotConfirm : public MProcess {
public:
    virtual ~DinoUiSaveSlotConfirm();
    virtual void Idle();

    DinoUiSaveSlotConfirmMenu *mMenu;   // 0x74
};

#endif
