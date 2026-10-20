#ifndef GAME_DINO_COMMON_YES_NO_MENU_ITEM_MENU_H
#define GAME_DINO_COMMON_YES_NO_MENU_ITEM_MENU_H

#include <string>
#include <vector>
#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"

class DinoCommonYesNoMenuItem;
class DinoCommonWindow;

class DinoCommonYesNoMenu : public MMenu, public DinoHelpBase {
public:
    DinoCommonYesNoMenu(const std::string &text, BOOL *result, u32 unkD0, u32 unkD4, u32 unkD8);
    // The game's destructor is inline; its vtable's owner (dino_common_yes_no_menu_item.cpp) emits it out of line.
#ifdef DINO_COMMON_YES_NO_MENU_OWNER
    virtual ~DinoCommonYesNoMenu();
#else
    virtual ~DinoCommonYesNoMenu() {}
#endif
    virtual void OnStart();
    virtual void OnFocus(MMenuItem *item);
    virtual void OnDecide(MMenuItem *item);

    DinoCommonWindow *mWindow;      // 0xB4
    DinoCommonYesNoMenuItem *mItems[2]; // 0xB8: yes, no (decided: *mResult = the index)
    std::string mUnkC0;             // 0xC0: the question
    BOOL *mResult;                  // 0xCC
    u32 mUnkD0;                     // 0xD0
    u32 mUnkD4;                     // 0xD4
    u32 mUnkD8;                     // 0xD8
};

#endif
