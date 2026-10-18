#ifndef GAME_DINO_BATTLE_VIEW3_H
#define GAME_DINO_BATTLE_VIEW3_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoBattleViewLPCountPanel : public MProcess {
public:
    virtual ~DinoBattleViewLPCountPanel();
    virtual void Idle();
};

class DinoBattleViewDecideFirstAttackTeam : public MProcess {
public:
    DinoBattleViewDecideFirstAttackTeam(u32 unk88, u32 unk8C, u32 unk90, u32 unk94);
    virtual ~DinoBattleViewDecideFirstAttackTeam();
    virtual void Idle();

    void (DinoBattleViewDecideFirstAttackTeam::*mState)(); // 0x74
    u32 unk7C[3];
    u32 mUnk88;     // 0x88
    u32 mUnk8C;     // 0x8C
    u32 mUnk90;     // 0x90
    u32 mUnk94;     // 0x94
};

#endif
