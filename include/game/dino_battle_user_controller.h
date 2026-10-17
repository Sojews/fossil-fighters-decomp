#ifndef GAME_DINO_BATTLE_USER_CONTROLLER_H
#define GAME_DINO_BATTLE_USER_CONTROLLER_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_battle_controller.h"
#include "game/dino_field_camera_hook.h"

class DinoBattleModelPlayer;

class DinoBattleUserController : public DinoBattleController {
public:
    virtual ~DinoBattleUserController();
    virtual BOOL Unk14();
    virtual void OnMessage(MMessage *msg);

    u32 GetCreatureMask(DinoBattleModelPlayer *player);
};

class PlayerThinkTimeCountTask : public MTask {
public:
    virtual ~PlayerThinkTimeCountTask();
    virtual void Execute();

    struct Player {
        u8 unk00[0xE0];
        struct Model {
            u8 unk00[0x150];
            int mThinkTime;     // 0x150
        } *mModel;              // 0xE0
    } *mPlayer;                 // 0x10
    u8 mActive;                 // 0x14
    int mCount;                 // 0x18
};

#endif
