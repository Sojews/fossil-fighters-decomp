#ifndef GAME_DINO_CLEANING_DUMMY_ITEM_MENU_H
#define GAME_DINO_CLEANING_DUMMY_ITEM_MENU_H

#include <string>
#include <vector>
#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"

class DinoCleaningToolMenu : public MMenu {
public:
    void FocusFirst();
    void DisableAll();
    void EnableItem();
    void DisableItem();
    virtual ~DinoCleaningToolMenu();
    virtual void OnStart();

    u32 mUnkA8[3];          // 0xA8
    MMenuItem *mItemB4;     // 0xB4
    std::vector<void *> mUnkB8; // 0xB8
};

#endif
