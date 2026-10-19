#ifndef GAME_DINO_COMMON_ITEM_H
#define GAME_DINO_COMMON_ITEM_H

#include "game/dino_common_label.h"
#include "m/motion.h"
#include "m/smart_oam.h"
#include "m/view.h"
#include "game/dino_resource.h"
#include "m/menu_item.h"
#include "game/help.h"

class DinoCommonItem : public MMenuItem, public DinoHelpItem {
public:
    // type: the look; label: a text shown on it (0: none); helpText: its help.
    DinoCommonItem(u32 type, MSmartOam *oam, MPoint pos, u32 unk, u32 label, u32 helpText);
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
    MMotion *mMotion;       // 0x98
    DinoCommonLabel *mView; // 0x9C
    MPoint mPos;            // 0xA0
    MRect mFrame;           // 0xA8
    u32 mTextId;            // 0xB8
};

#endif
