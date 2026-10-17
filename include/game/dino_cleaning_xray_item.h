#ifndef GAME_DINO_CLEANING_XRAY_ITEM_H
#define GAME_DINO_CLEANING_XRAY_ITEM_H

#include "game/help.h"
#include "m/menu_item.h"

class DinoCleaningXrayItem : public DinoHelpItem, public MMenuItem {
public:
    virtual ~DinoCleaningXrayItem();
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();
    virtual MRect GetFrame();
    virtual void OnEnableChanged(BOOL enabled);
    virtual void OnDecide();
};

#endif
