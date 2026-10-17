#ifndef GAME_DINO_UI_DEBUG_FLAG_MENU_MENU_H
#define GAME_DINO_UI_DEBUG_FLAG_MENU_MENU_H

#include "m/font.h"
#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoUiDebugFlagMenu : public MMenu {
public:
    virtual ~DinoUiDebugFlagMenu();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);

    u32 mUnkA8[2];      // 0xA8
    MFont *mFont;       // 0xB0
};

class DinoUiDebugFlagMenuItem : public MMenuItem {
public:
    virtual ~DinoUiDebugFlagMenuItem();
    virtual MRect GetFrame();
    virtual void OnDecide();

    u32 mUnk88;         // 0x88
    MFont *mFont;       // 0x8C
    MPoint mPos;        // 0x90
};

#endif
