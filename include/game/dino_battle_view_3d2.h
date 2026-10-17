#ifndef GAME_DINO_BATTLE_VIEW_3D2_H
#define GAME_DINO_BATTLE_VIEW_3D2_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoBattleView3DDamage : public MProcess {
public:
    virtual ~DinoBattleView3DDamage();
    virtual void Idle();
};

class DinoBattleView3DAction : public MProcess {
public:
    virtual ~DinoBattleView3DAction();
    virtual void Idle();
};

class DinoBattleView3DMask : public MTask {
public:
    virtual ~DinoBattleView3DMask();
    virtual void Execute();
};

class DinoBattleView3DGroupFade : public MProcess {
public:
    virtual ~DinoBattleView3DGroupFade();
    virtual void Idle();
};

#endif
