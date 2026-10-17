#ifndef GAME_DINO_UI_MULTIPLAY_MENU_ITEM2_H
#define GAME_DINO_UI_MULTIPLAY_MENU_ITEM2_H

#include "game/dino_resource.h"
#include "m/motion.h"
#include "m/menu_item.h"
#include "game/help.h"

class DinoUiMultiplayMenuItem2 : public MMenuItem, public DinoHelpItem {
public:
    virtual ~DinoUiMultiplayMenuItem2();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();

    u8 unk94[0x8];
    MMotion *mMotion;   // 0x9C
    u32 mTextId;        // 0xA0
};

#endif
