#ifndef GAME_BATTLE_SELECT_H
#define GAME_BATTLE_SELECT_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class BattleSelect : public MProcess {
public:
    virtual ~BattleSelect();
    virtual void Idle();
};

class UseCreatureSelect : public MProcess {
public:
    virtual ~UseCreatureSelect();
    virtual void Idle();
};

class PlayerLevelSelect : public MProcess {
public:
    virtual ~PlayerLevelSelect();
    virtual void Idle();
};

class IncValTask : public MTask {
public:
    virtual ~IncValTask();
    virtual void Execute();
};

class TimeCountTask : public MProcess {
public:
    virtual ~TimeCountTask();
    virtual void Idle();
};

#endif
