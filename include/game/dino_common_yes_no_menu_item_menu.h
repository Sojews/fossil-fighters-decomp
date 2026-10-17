#ifndef GAME_DINO_COMMON_YES_NO_MENU_ITEM_MENU_H
#define GAME_DINO_COMMON_YES_NO_MENU_ITEM_MENU_H

#include <string>
#include <vector>
#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"

class DinoCommonYesNoMenu : public MMenu, public DinoHelpBase {
public:
    virtual ~DinoCommonYesNoMenu();
    virtual void OnStart();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);

    u32 mUnkB4[3];          // 0xB4
    std::string mUnkC0;     // 0xC0
};

#endif
