#ifndef GAME_DINO_BATTLE_VIEW_KP_UP_DOWN_H
#define GAME_DINO_BATTLE_VIEW_KP_UP_DOWN_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoBattleViewKPUpDown : public MProcess {
public:
    virtual ~DinoBattleViewKPUpDown();
    virtual void Idle();
};

class WaitPopNumberTask : public MTask {
public:
    WaitPopNumberTask() {}
    virtual ~WaitPopNumberTask();
    virtual void Execute();
};

class KPUpDownMotion : public MProcess {
public:
    virtual ~KPUpDownMotion();
    virtual void Idle();
};

#endif
