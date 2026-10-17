#ifndef GAME_DINO_UI_MULTIPLAY_MENU_ITEM_H
#define GAME_DINO_UI_MULTIPLAY_MENU_ITEM_H

#include "game/dino_resource.h"
#include "m/motion.h"
#include "m/menu_item.h"
#include "game/help.h"

class DinoUiMultiplayMenu;

class DinoUiMultiplayMenuItem : public MMenuItem, public DinoHelpItem {
public:
    virtual ~DinoUiMultiplayMenuItem();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();

    DinoUiMultiplayMenu *mOwner; // 0x94
    MMotion *mMotion;   // 0x98
    u32 mTextId;        // 0x9C
};

#endif
