#ifndef GAME_DINO_COMMON_ITEM_H
#define GAME_DINO_COMMON_ITEM_H

#include "m/smart_oam.h"
#include "m/view.h"
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
    void UpdateLook();

    MSmartOam *mOam;        // 0x94
    u32 mUnk98;             // 0x98
    MView *mView;           // 0x9C
    MPoint mPos;            // 0xA0
    MRect mFrame;           // 0xA8
    u32 mTextId;            // 0xB8
};

#endif
