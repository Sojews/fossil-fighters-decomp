#ifndef GAME_DINO_CLEANING_DUMMY_ITEM_MENU_H
#define GAME_DINO_CLEANING_DUMMY_ITEM_MENU_H

#include <string>
#include <vector>
#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"

class DinoCleaningToolMenu : public MMenu {
public:
    DinoCleaningToolMenu(u32 arg0, u32 arg1, u32 arg2);
    void FocusFirst();
    void DisableAll();
    void EnableItem();
    void DisableItem();
    virtual ~DinoCleaningToolMenu();
    virtual void OnStart();

    u32 mUnkA8;             // 0xA8
    u32 mUnkAC;             // 0xAC
    u32 mUnkB0;             // 0xB0
    MMenuItem *mItemB4;     // 0xB4
    std::vector<void *> mUnkB8; // 0xB8
};

#endif
