#ifndef GAME_DINO_UI_SAVE_SLOT_SELECT_MENU_H
#define GAME_DINO_UI_SAVE_SLOT_SELECT_MENU_H

#include "m/menu.h"
#include "game/help.h"

class DinoUiSaveSlotSelectMenu : public MMenu, public DinoHelpBase {
public:
    virtual ~DinoUiSaveSlotSelectMenu();
    virtual void OnStart();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);
};
class DinoUiSaveSlotConfirmMenu : public MMenu {
public:
    virtual ~DinoUiSaveSlotConfirmMenu();
    virtual void OnStart();
};
#endif
