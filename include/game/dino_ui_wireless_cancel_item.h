#ifndef GAME_DINO_UI_WIRELESS_CANCEL_ITEM_H
#define GAME_DINO_UI_WIRELESS_CANCEL_ITEM_H

#include "m/motion.h"
#include "m/menu_item.h"

class DinoUiWirelessCancelItem : public MMenuItem {
public:
    virtual ~DinoUiWirelessCancelItem();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();

    MMotion *mMotion;   // 0x88
};

#endif
