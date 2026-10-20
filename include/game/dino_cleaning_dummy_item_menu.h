#ifndef GAME_DINO_CLEANING_DUMMY_ITEM_MENU_H
#define GAME_DINO_CLEANING_DUMMY_ITEM_MENU_H

#include <string>
#include <vector>
#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"

class DinoCleaning;
class MSmartOam;

class DinoCleaningToolMenu : public MMenu {
public:
    DinoCleaningToolMenu(u32 arg0, u32 arg1, u32 arg2);
    void FocusFirst();
    void DisableAll();
    void EnableItem();
    void DisableItem();
    virtual ~DinoCleaningToolMenu();
    virtual void OnStart();

    DinoCleaning *mCleaning;    // 0xA8
    MSmartOam *mOam;            // 0xAC
    u32 *mSelected;             // 0xB0: the focused tool
    MMenuItem *mXrayItem;       // 0xB4
    std::vector<void *> mTools; // 0xB8
};

#endif
