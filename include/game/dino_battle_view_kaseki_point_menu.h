#ifndef GAME_DINO_BATTLE_VIEW_KASEKI_POINT_MENU_H
#define GAME_DINO_BATTLE_VIEW_KASEKI_POINT_MENU_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"

class DinoBattleViewPlayer : public MProcess, public MMessageObserver {
public:
    virtual ~DinoBattleViewPlayer();
    virtual void Idle();
    virtual void OnMessage(MMessage *msg);
};

#endif
