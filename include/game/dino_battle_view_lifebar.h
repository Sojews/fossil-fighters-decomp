#ifndef GAME_DINO_BATTLE_VIEW_LIFEBAR_H
#define GAME_DINO_BATTLE_VIEW_LIFEBAR_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoBattleViewLifebar : public MProcess {
public:
    virtual ~DinoBattleViewLifebar();
    virtual void Idle();
};

class LifeMotion : public MProcess {
public:
    virtual ~LifeMotion();
    virtual void Idle();
};

class LifebarCoordTask : public MTask {
public:
    virtual ~LifebarCoordTask();
    virtual void Execute();

    MPoint mPos;        // 0x10
    void *mLifebar;     // 0x18
};

class LifebarShowTask : public MTask {
public:
    virtual ~LifebarShowTask();
    virtual void Execute();

    u8 mShow;           // 0x10
    void *mLifebar;     // 0x14
};

class LifebarCoordAnime : public MPointAnime {
public:
    virtual ~LifebarCoordAnime();
    virtual void Apply(int step);

    void *mLifebar;     // 0x50
};

#endif
