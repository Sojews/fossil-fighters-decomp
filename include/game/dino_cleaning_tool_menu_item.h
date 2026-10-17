#ifndef GAME_DINO_CLEANING_TOOL_MENU_ITEM_H
#define GAME_DINO_CLEANING_TOOL_MENU_ITEM_H

#include "game/dino_resource.h"
#include "game/help.h"
#include "m/menu_item.h"
#include "m/motion.h"

class DinoCleaningToolMenuItem : public DinoHelpItem, public MMenuItem {
public:
    virtual ~DinoCleaningToolMenuItem();
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();
    virtual MRect GetFrame();
    virtual void OnDecide();
    virtual void OnEnableChanged(BOOL enabled);
    virtual void OnFocusChanged(BOOL focused);

    u8 unk94[0x4];
    u32 mTextId;        // 0x98
    u32 mUnk9C[2];
    MMotion *mMotion;   // 0xA4
};

#endif
