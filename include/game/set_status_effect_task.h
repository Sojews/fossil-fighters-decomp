#ifndef GAME_SET_STATUS_EFFECT_TASK_H
#define GAME_SET_STATUS_EFFECT_TASK_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class DinoBattleModelCreature;

class SetStatusEffectTask : public MTask {
public:
    virtual ~SetStatusEffectTask();
    virtual void Execute();

    DinoBattleModelCreature *mCreature; // 0x10
    u32 mUnk14;                         // 0x14
    u32 mUnk18;                         // 0x18
    u32 mUnk1C;                         // 0x1C
    u32 mUnk20;                         // 0x20
    u8 mUnk24;                          // 0x24
    MMessageObserver *mView;            // 0x28: told when the effect is set
};

class DinoCreatureUpdateTask : public MTask {
public:
    virtual ~DinoCreatureUpdateTask();
    virtual void Execute();

    DinoBattleModelCreature *mCreature; // 0x10
    u8 mUnk14;          // 0x14
    u32 mUnk18;         // 0x18
};

class DamagedUpdate : public MTask {
public:
    virtual ~DamagedUpdate();
    virtual void Execute();

    DinoBattleModelCreature *mCreature; // 0x10
    MMessageObserver *mView;            // 0x14
    u32 mUnk18;                         // 0x18
    u32 mUnk1C;                         // 0x1C
    u32 mUnk20;                         // 0x20: from the creature, if there is one
    u32 mUnk24;                         // 0x24
    u32 mUnk28;                         // 0x28
    u8 mUnk2C;                          // 0x2C
    u8 mUnk2D;                          // 0x2D
};

#endif
