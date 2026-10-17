#ifndef GAME_DINO_CLEANING_STOP_CONFIRM_MENU_ITEM_H
#define GAME_DINO_CLEANING_STOP_CONFIRM_MENU_ITEM_H

#include "game/dino_resource.h"
#include "m/menu_item.h"
#include "game/help.h"

class DinoCleaningStopConfirmMenuItem : public MMenuItem, public DinoHelpItem {
public:
    virtual ~DinoCleaningStopConfirmMenuItem();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();

    u8 unk94[0x8];
    u32 mTextId;        // 0x9C
};

#endif
