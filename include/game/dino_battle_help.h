#ifndef GAME_DINO_BATTLE_HELP_H
#define GAME_DINO_BATTLE_HELP_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoBattleHelpBase : public MProcess, public DinoHelpItem {
public:
    virtual ~DinoBattleHelpBase();
    virtual void Idle();
    virtual MRect GetRect();
    virtual MDisplay *GetDisplay();
    virtual const char *GetText();
};

class DinoBattleHelpSupport : public DinoBattleHelpBase {
public:
    virtual ~DinoBattleHelpSupport();
};

#endif
