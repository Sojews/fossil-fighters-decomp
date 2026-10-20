#ifndef GAME_DINO_UI_SAVE_SLOT_SELECT_MENU_ITEM_H
#define GAME_DINO_UI_SAVE_SLOT_SELECT_MENU_ITEM_H

#include "m/motion.h"
#include "m/menu_item.h"
#include "game/help.h"
#include "game/dino_backup.h"

class MSmartOam;
class MSmartObjCanvas;

class DinoUiSaveSlotSelectMenuItem : public MMenuItem, public DinoHelpItem {
public:
    // An empty slot's frame, or a saved one's: its rank and horider level badges, and the player's name, title and
    // play time.
    DinoUiSaveSlotSelectMenuItem(MSmartOam *oam, u32 slot, DinoSaveSummary summary, MPoint pos);
    virtual ~DinoUiSaveSlotSelectMenuItem();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
    virtual MDisplay *GetDisplay();
    virtual MRect GetRect();
    virtual const char *GetText();

    MSmartOam *mOam;    // 0x94
    u32 mSlot;          // 0x98
    MPoint mPos;        // 0x9C
    u8 mUnkA4;          // 0xA4: the slot is empty
    MMotion *mMotion;   // 0xA8
    MMotion *mMotion2;  // 0xAC: the rank badge
    MMotion *mMotion3;  // 0xB0: the horider level badge
    MSmartObjCanvas *mLabel;    // 0xB4: centred on the item
};

#endif
