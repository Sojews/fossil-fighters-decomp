#ifndef GAME_DINO_BATTLE_LIVE_H
#define GAME_DINO_BATTLE_LIVE_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_common_live.h"
#include "game/dino_field_camera_hook.h"

class DinoBattleLive : public DinoCommonLive {
public:
    virtual ~DinoBattleLive();
    virtual void Idle();
};

#endif
