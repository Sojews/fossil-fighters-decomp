#ifndef GAME_DINO_BATTLE_VIEW_EFFECT_GAME_BEGIN_H
#define GAME_DINO_BATTLE_VIEW_EFFECT_GAME_BEGIN_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoBattleViewEffectGameBeginPlayerNamePanel : public MMotion {
public:
    virtual ~DinoBattleViewEffectGameBeginPlayerNamePanel();
    virtual void Execute();
};

class DinoBattleViewEffectGameBegin : public MProcess {
public:
    virtual ~DinoBattleViewEffectGameBegin();
    virtual void Idle();

    void (DinoBattleViewEffectGameBegin::*mState)(); // 0x74
};

#endif
