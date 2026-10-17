#ifndef GAME_DINO_BATTLE_VIEW_EFFECT_GAME_BEGIN_H
#define GAME_DINO_BATTLE_VIEW_EFFECT_GAME_BEGIN_H

#include <string>
#include "m/font.h"
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
    virtual MRect GetFrame();
    virtual MPoint GetPosition();

    MFont *mFont;       // 0x7C
    MTask *mUnk80;      // 0x80
    MTask *mUnk84;      // 0x84
};

class DinoBattleViewEffectGameBegin : public MProcess {
public:
    virtual ~DinoBattleViewEffectGameBegin();
    virtual void Idle();

    void (DinoBattleViewEffectGameBegin::*mState)(); // 0x74
    u32 unk7C;
    std::string mNames[2];  // 0x80
};

#endif
