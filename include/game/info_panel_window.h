#ifndef GAME_INFO_PANEL_WINDOW_H
#define GAME_INFO_PANEL_WINDOW_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class InfoPanelWindow : public MProcess {
public:
    virtual ~InfoPanelWindow();
    virtual void Idle();
};

class DinoBattleViewPopNumber : public MProcess {
public:
    virtual ~DinoBattleViewPopNumber();
    virtual void Idle();
};

#endif
