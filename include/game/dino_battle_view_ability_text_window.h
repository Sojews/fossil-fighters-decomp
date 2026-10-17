#ifndef GAME_DINO_BATTLE_VIEW_ABILITY_TEXT_WINDOW_H
#define GAME_DINO_BATTLE_VIEW_ABILITY_TEXT_WINDOW_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoBattleViewAbilityTextWindow : public MTask {
public:
    virtual ~DinoBattleViewAbilityTextWindow();
    virtual void Execute();

    class ShowTask : public MTask {
    public:
        virtual ~ShowTask();
        virtual void Execute();
    };

    u32 mUnk10;     // 0x10
    MTask *mTask;   // 0x14
};

#endif
