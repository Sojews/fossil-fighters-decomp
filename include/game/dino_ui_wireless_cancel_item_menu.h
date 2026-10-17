#ifndef GAME_DINO_UI_WIRELESS_CANCEL_ITEM_MENU_H
#define GAME_DINO_UI_WIRELESS_CANCEL_ITEM_MENU_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "game/help.h"

class DinoUiWirelessCancel : public MMenu {
public:
    virtual ~DinoUiWirelessCancel();
    virtual void OnStart();
    virtual void OnDecide(MMenuItem *item);
};

class DinoUiChildScannerParentItem : public MMenuItem {
public:
    virtual ~DinoUiChildScannerParentItem();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

#endif
