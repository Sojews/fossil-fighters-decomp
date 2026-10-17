#ifndef GAME_DINO_BATTLE_VIEW_ABILITY_MENU_BG_H
#define GAME_DINO_BATTLE_VIEW_ABILITY_MENU_BG_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class MScreen;

class DinoBattleViewAbilityMenuBG : public MProcess {
public:
    virtual ~DinoBattleViewAbilityMenuBG();
    virtual void Idle();

    void SetPriority(int priority);

    u32 unk74[2];
    MScreen *mScreen;   // 0x7C
    u8 unk80[0x40];
    void (DinoBattleViewAbilityMenuBG::*mState)(); // 0xC0
    u8 unkC8[0x100C];
    u8 mUnk10D4;        // 0x10D4

    class SetPriorityTask : public MTask {
    public:
        virtual ~SetPriorityTask();
        virtual void Execute();

        DinoBattleViewAbilityMenuBG *mBG;   // 0x10
        int mPriority;                      // 0x14
    };
};

#endif
