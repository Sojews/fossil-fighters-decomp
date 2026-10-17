#ifndef GAME_DINO_BATTLE_VIEW_3D_MOVE_H
#define GAME_DINO_BATTLE_VIEW_3D_MOVE_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoBattleView3DMove : public MProcess {
public:
    virtual ~DinoBattleView3DMove();
    virtual void Idle();
};

#endif
