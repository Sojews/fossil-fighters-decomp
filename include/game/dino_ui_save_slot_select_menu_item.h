#ifndef GAME_DINO_UI_SAVE_SLOT_SELECT_MENU_ITEM_H
#define GAME_DINO_UI_SAVE_SLOT_SELECT_MENU_ITEM_H

#include "m/motion.h"
#include "m/menu_item.h"
#include "game/help.h"

class DinoUiSaveSlotSelectMenuItem : public MMenuItem, public DinoHelpItem {
public:
    virtual ~DinoUiSaveSlotSelectMenuItem();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
    virtual MDisplay *GetDisplay();
    virtual MRect GetRect();
    virtual const char *GetText();

    u32 mUnk94;
    u32 mSlot;          // 0x98
    MPoint mPos;        // 0x9C
    u8 mUnkA4;          // 0xA4
    MMotion *mMotion;   // 0xA8
    MMotion *mMotion2;  // 0xAC
    MMotion *mMotion3;  // 0xB0
    MView *mLabel;      // 0xB4: centred on the item
};

#endif
