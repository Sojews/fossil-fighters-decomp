#ifndef GAME_DINO_UI2_PARTS_KASEKI_BUTTON_H
#define GAME_DINO_UI2_PARTS_KASEKI_BUTTON_H

#include "game/dino_resource.h"
#include "m/menu_item.h"
#include "game/help.h"

class DinoUi2PartsKasekiButton : public MMenuItem, public DinoHelpItem {
public:
    virtual ~DinoUi2PartsKasekiButton();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();

    u8 unk94[0x28];
    u32 mTextId;        // 0xBC
};

#endif
