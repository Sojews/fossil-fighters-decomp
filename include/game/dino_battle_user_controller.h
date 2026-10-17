#ifndef GAME_DINO_BATTLE_USER_CONTROLLER_H
#define GAME_DINO_BATTLE_USER_CONTROLLER_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_battle_controller.h"
#include "game/dino_field_camera_hook.h"

class DinoBattleUserController : public DinoBattleController {
public:
    virtual ~DinoBattleUserController();
};

class PlayerThinkTimeCountTask : public MTask {
public:
    virtual ~PlayerThinkTimeCountTask();
    virtual void Execute();
};

#endif
