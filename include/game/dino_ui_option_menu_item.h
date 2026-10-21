#ifndef GAME_DINO_UI_OPTION_MENU_ITEM_H
#define GAME_DINO_UI_OPTION_MENU_ITEM_H

#include "game/dino_resource.h"
#include "m/motion.h"
#include "m/menu_item.h"
#include "game/help.h"

class DinoUiOptionMenu;

class DinoUiOptionMenuItem : public MMenuItem, public DinoHelpItem {
public:
    DinoUiOptionMenuItem(DinoUiOptionMenu *owner, const std::string &path, MPoint pos, u32 textId);
    DinoUiOptionMenuItem(DinoUiOptionMenu *owner, u32 textId);
    virtual ~DinoUiOptionMenuItem();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();

    DinoUiOptionMenu *mOwner; // 0x94
    MMotion *mMotion;   // 0x98
    u32 mTextId;        // 0x9C
};

#endif
