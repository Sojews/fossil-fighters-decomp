#ifndef GAME_DINO_UI_ORDER_SELECT_H
#define GAME_DINO_UI_ORDER_SELECT_H

#include <string>

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoUiOrderSelect : public MProcess {
public:
    virtual ~DinoUiOrderSelect();
    virtual void Idle();

    u32 mUnk74;         // 0x74
    std::string mUnk78; // 0x78
};

class DinoUiOrderSelectMenuItem : public MMenuItem, public DinoHelpItem {
public:
    virtual ~DinoUiOrderSelectMenuItem();
    virtual MRect GetFrame();
    virtual void OnFocusChanged(BOOL focused);
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();
};

class DinoUiOrderSelectBackButton : public DinoUiOrderSelectMenuItem {
public:
    virtual ~DinoUiOrderSelectBackButton();
    virtual void OnFocusChanged(BOOL focused);
    virtual void OnDecide();
};

class DinoUiOrderSelectDecideButton : public DinoUiOrderSelectMenuItem {
public:
    virtual ~DinoUiOrderSelectDecideButton();
    virtual void OnDecide();
};

#endif
