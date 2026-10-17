#ifndef GAME_DINO_UI_ORDER_SELECT_H
#define GAME_DINO_UI_ORDER_SELECT_H

#include "game/dino_resource.h"
#include "m/motion.h"
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

    u8 unk94[0x4];
    MMotion *mMotion;   // 0x98
    u32 mTextId;        // 0x9C
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
