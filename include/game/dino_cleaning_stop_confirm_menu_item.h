#ifndef GAME_DINO_CLEANING_STOP_CONFIRM_MENU_ITEM_H
#define GAME_DINO_CLEANING_STOP_CONFIRM_MENU_ITEM_H

#include "game/dino_resource.h"
#include "m/menu_item.h"
#include "game/help.h"

class DinoCleaningStopConfirmMenuItem : public MMenuItem, public DinoHelpItem {
public:
    DinoCleaningStopConfirmMenuItem(MMotion *motion, u32 cell, u32 textId) {
        mMotion = motion;
        mCell = cell;
        mTextId = textId;
    }
    virtual ~DinoCleaningStopConfirmMenuItem();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();

    MMotion *mMotion;   // 0x94
    u32 mCell;          // 0x98: the cell of mMotion this item covers
    u32 mTextId;        // 0x9C
};

#endif
