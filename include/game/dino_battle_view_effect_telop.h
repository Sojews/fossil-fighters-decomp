#ifndef GAME_DINO_BATTLE_VIEW_EFFECT_TELOP_H
#define GAME_DINO_BATTLE_VIEW_EFFECT_TELOP_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoBattleViewEffectTelop : public MProcess {
public:
    virtual ~DinoBattleViewEffectTelop();
    virtual void Idle();
};

class SetBGModeTask : public MTask {
public:
    virtual ~SetBGModeTask();
    virtual void Execute();
};

class SkipTelopTask : public MProcess {
public:
    virtual ~SkipTelopTask();
    virtual void Idle();
};

class SkipWinTelopTask : public SkipTelopTask {
public:
    virtual ~SkipWinTelopTask();
};

class SkipLoseTelopTask : public SkipTelopTask {
public:
    virtual ~SkipLoseTelopTask();
};

class NopWaitTask : public MTask {
public:
    virtual ~NopWaitTask();
    virtual void Execute();
};

#endif
