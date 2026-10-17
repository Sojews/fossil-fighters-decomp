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

    u32 mUnkB4[8];
    u8 mSilentFocus; // 0xD4
};
class DinoUiSaveSlotConfirmMenu : public MMenu {
public:
    DinoUiSaveSlotConfirmMenu() : MMenu(0) {}
    virtual ~DinoUiSaveSlotConfirmMenu();
    virtual void OnStart();

    u32 mUnkA8;     // 0xA8
};
#endif
