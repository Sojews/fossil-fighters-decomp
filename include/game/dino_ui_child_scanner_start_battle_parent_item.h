#ifndef GAME_DINO_UI_CHILD_SCANNER_START_BATTLE_PARENT_ITEM_H
#define GAME_DINO_UI_CHILD_SCANNER_START_BATTLE_PARENT_ITEM_H

#include "m/menu_item.h"
#include "game/help.h"

class DinoUiChildScannerStartBattleParentItem : public MMenuItem, public DinoHelpItem {
public:
    virtual ~DinoUiChildScannerStartBattleParentItem();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
    virtual MDisplay *GetDisplay();
    virtual MRect GetRect();
    virtual const char *GetText();
};

#endif
