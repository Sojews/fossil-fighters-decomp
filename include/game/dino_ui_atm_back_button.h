#ifndef GAME_DINO_UI_ATM_BACK_BUTTON_H
#define GAME_DINO_UI_ATM_BACK_BUTTON_H

#include "game/dino_resource.h"
#include "m/motion.h"
#include "m/menu_item.h"
#include "game/help.h"

class DinoUiATMBackButton : public MMenuItem, public DinoHelpItem {
public:
    virtual ~DinoUiATMBackButton();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();

    u8 unk94[0x4];
    MMotion *mMotion;   // 0x98
    u8 unk9C[0x4];
    u32 mTextId;        // 0xA0
};

#endif
