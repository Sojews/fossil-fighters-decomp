#ifndef GAME_DINO_BATTLE_VIEW3_H
#define GAME_DINO_BATTLE_VIEW3_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoBattleViewLPCountPanel : public MProcess {
public:
    virtual ~DinoBattleViewLPCountPanel();
    virtual void Idle();
};

class DinoBattleViewDecideFirstAttackTeam : public MProcess {
public:
    virtual ~DinoBattleViewDecideFirstAttackTeam();
    virtual void Idle();
};

#endif
