#ifndef GAME_DINO_COMMON_YES_NO_MENU_ITEM_H
#define GAME_DINO_COMMON_YES_NO_MENU_ITEM_H

#include "game/dino_resource.h"
#include "m/motion.h"
#include "game/help.h"
#include "m/menu_item.h"

class DinoCommonYesNoMenuItem : public DinoHelpItem, public MMenuItem {
public:
    virtual ~DinoCommonYesNoMenuItem();
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
    MMotion *GetMotion();
    u8 IsYes();

    MMotion *mMotion;   // 0x94
    u8 mIsYes;          // 0x98
    u8 unk99[3];
    u32 mTextId;        // 0x9C
};

#endif
