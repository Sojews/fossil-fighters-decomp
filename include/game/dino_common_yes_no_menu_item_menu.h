#ifndef GAME_DINO_COMMON_YES_NO_MENU_ITEM_MENU_H
#define GAME_DINO_COMMON_YES_NO_MENU_ITEM_MENU_H

#include <string>
#include <vector>
#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"

class DinoCommonYesNoMenuItem;

class DinoCommonYesNoMenu : public MMenu, public DinoHelpBase {
public:
    // The game's destructor is inline; its vtable's owner (dino_common_yes_no_menu_item.cpp) emits it out of line.
#ifdef DINO_COMMON_YES_NO_MENU_OWNER
    virtual ~DinoCommonYesNoMenu();
#else
    virtual ~DinoCommonYesNoMenu() {}
#endif
    virtual void OnStart();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);

    u32 mUnkB4[2];                  // 0xB4
    DinoCommonYesNoMenuItem *mYes;  // 0xBC
    std::string mUnkC0;             // 0xC0
    BOOL *mResult;                  // 0xCC
};

#endif
