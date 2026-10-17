#ifndef GAME_DINO_UI_DEBUG_REVIVER_MENU_MENU_H
#define GAME_DINO_UI_DEBUG_REVIVER_MENU_MENU_H

#include "m/font.h"
#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoUiDebugReviverMenu : public MMenu {
public:
    virtual ~DinoUiDebugReviverMenu();
    virtual void OnStart();
    virtual void OnIdle();
    virtual void OnFocus(MMenuItem *item);

    u32 mUnkA8[1];      // 0xA8
    MFont *mFont;       // 0xAC
};

class DinoUiDebugReviverMenuItem : public MMenuItem {
public:
    virtual ~DinoUiDebugReviverMenuItem();
    virtual MRect GetFrame();
    virtual void OnDecide();

    u32 mUnk88;         // 0x88
    MFont *mFont;       // 0x8C
    MPoint mPos;        // 0x90
};

#endif
