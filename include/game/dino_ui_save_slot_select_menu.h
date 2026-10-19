#ifndef GAME_DINO_UI_SAVE_SLOT_SELECT_MENU_H
#define GAME_DINO_UI_SAVE_SLOT_SELECT_MENU_H

#include "m/menu.h"
#include "game/help.h"

class DinoUiSaveSlotSelectMenuItem;
class DinoUiSaveSlotSelectMenuCancelItem;
class DinoUiSaveSlotSelectMenuDeleteItem;
class DinoCommonWindowMessage;

class DinoUiSaveSlotSelectMenu : public MMenu, public DinoHelpBase {
public:
    DinoUiSaveSlotSelectMenu(MInput *input, u32 *result) : MMenu(input, 0) { mResult = result; }
    virtual ~DinoUiSaveSlotSelectMenu();
    virtual void OnStart();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);

    // Asks which file to delete: only the saved slots and cancel stay enabled.
    void StartDelete();

    u32 *mResult;   // 0xB4
    u32 mUnkB8;     // 0xB8
    DinoUiSaveSlotSelectMenuItem *mSlots[2];    // 0xBC
    DinoUiSaveSlotSelectMenuCancelItem *mCancel; // 0xC4
    DinoUiSaveSlotSelectMenuDeleteItem *mDelete; // 0xC8
    u8 mDeleting;   // 0xCC
    DinoCommonWindowMessage *mMessage;  // 0xD0: "which file?"
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
