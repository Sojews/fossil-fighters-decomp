#ifndef GAME_DINO_COMMON_ITEM_H
#define GAME_DINO_COMMON_ITEM_H

#include "game/dino_resource.h"
#include "m/menu_item.h"
#include "game/help.h"

class DinoCommonItem : public MMenuItem, public DinoHelpItem {
public:
    virtual ~DinoCommonItem();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnEnableChanged(BOOL enabled);
    virtual void OnSelectChanged(BOOL selected);
    virtual void OnDecide();
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();

    u8 unk94[0x24];
    u32 mTextId;        // 0xB8
};

#endif
