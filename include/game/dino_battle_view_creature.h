#ifndef GAME_DINO_BATTLE_VIEW_CREATURE_H
#define GAME_DINO_BATTLE_VIEW_CREATURE_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoBattleViewCreature : public MProcess, public MMessageObserver {
public:
    virtual ~DinoBattleViewCreature();
    virtual void Idle();
    virtual void OnMessage(MMessage *msg);

    class SetCoordTask : public MTask {
    public:
        virtual ~SetCoordTask();
        virtual void Execute();
    };
};

#endif
