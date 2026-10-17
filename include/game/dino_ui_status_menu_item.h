#ifndef GAME_DINO_UI_STATUS_MENU_ITEM_H
#define GAME_DINO_UI_STATUS_MENU_ITEM_H

#include "m/motion.h"
#include "m/menu_item.h"
#include "game/help.h"

class DinoUiStatusMenuItem : public MMenuItem, public DinoHelpItem {
public:
    virtual ~DinoUiStatusMenuItem();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();

    u8 unk94[0x4];
    MMotion *mMotion;   // 0x98
};

#endif
