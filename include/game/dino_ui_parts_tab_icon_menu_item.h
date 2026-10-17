#ifndef GAME_DINO_UI_PARTS_TAB_ICON_MENU_ITEM_H
#define GAME_DINO_UI_PARTS_TAB_ICON_MENU_ITEM_H

#include "game/dino_resource.h"
#include "m/menu_item.h"
#include "game/help.h"

class DinoUiPartsTabIconMenuItem : public MMenuItem, public DinoHelpItem {
public:
    virtual ~DinoUiPartsTabIconMenuItem();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnSelectChanged(BOOL selected);
    virtual void OnDecide();
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();

    u32 mTextId;        // 0x94
};

#endif
