#ifndef GAME_DINO_UI_SAVE_SLOT_CONFIRM_H
#define GAME_DINO_UI_SAVE_SLOT_CONFIRM_H

#include "game/dino_ui_save_slot_select_menu.h"
#include "m/task.h"

class DinoUiSaveSlotConfirm : public MProcess {
public:
    DinoUiSaveSlotConfirm();
    virtual ~DinoUiSaveSlotConfirm();
    virtual void Idle();
    // Shows the game about to be saved in the slot and presses it.
    void Unk02159590();

    DinoUiSaveSlotConfirmMenu *mMenu;   // 0x74
};

#endif
