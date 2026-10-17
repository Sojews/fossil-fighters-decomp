#ifndef GAME_CREATE_PLAYER_UPDATE_ABILITY_ICON_TASK_H
#define GAME_CREATE_PLAYER_UPDATE_ABILITY_ICON_TASK_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class CreatePlayerUpdateAbilityIconTask : public MTask {
public:
    virtual ~CreatePlayerUpdateAbilityIconTask();
    virtual void Execute();

    u8 mUnk10;          // 0x10
    u32 mUnk14;         // 0x14
    u32 mUnk18;         // 0x18
};

class UpdateAllCreatureTask : public MProcess {
public:
    virtual ~UpdateAllCreatureTask();
    virtual void Idle();
};

class MoveCreatureFromEscape : public MProcess {
public:
    virtual ~MoveCreatureFromEscape();
    virtual void Idle();
};

class DinoBattleModelCreature : public MProcess, public MMessageObserver {
public:
    virtual ~DinoBattleModelCreature();
    virtual void Idle();
    virtual void OnMessage(MMessage *msg);

    class AddLifePointTask : public MTask {
    public:
        virtual ~AddLifePointTask();
        virtual void Execute();
    };
};

class ModelCretureAddLifeTask : public MTask {
public:
    virtual ~ModelCretureAddLifeTask();
    virtual void Execute();

    u32 mUnk10;         // 0x10
    u32 mUnk14;         // 0x14
};

class ResetStatusEffectSleepTask : public MTask {
public:
    virtual ~ResetStatusEffectSleepTask();
    virtual void Execute();

    u32 mUnk10;         // 0x10
};

class AddLifePointTask : public MTask {
public:
    virtual ~AddLifePointTask();
    virtual void Execute();

    u32 mUnk10;         // 0x10
    u32 mUnk14;         // 0x14
    u8 mUnk18;          // 0x18
};

class ExecSpecialAttackTask : public MProcess {
public:
    virtual ~ExecSpecialAttackTask();
    virtual void Idle();
};

class SendModelCreatureAbilityResultTask : public MTask {
public:
    virtual ~SendModelCreatureAbilityResultTask();
    virtual void Execute();
};

class SetLifePointTask : public MTask {
public:
    virtual ~SetLifePointTask();
    virtual void Execute();

    u32 mUnk10;         // 0x10
    u32 mUnk14;         // 0x14
};

class PlayerUpdateTask : public MTask {
public:
    virtual ~PlayerUpdateTask();
    virtual void Execute();

    u32 mUnk10;         // 0x10
    u32 mUnk14;         // 0x14
};

class DinoCreatureUpdateStateIconTask : public MTask {
public:
    virtual ~DinoCreatureUpdateStateIconTask();
    virtual void Execute();

    u32 mUnk10;         // 0x10
    u32 mUnk14;         // 0x14
    u32 mUnk18;         // 0x18
};

class UpdateAbilityIconTask : public MTask {
public:
    virtual ~UpdateAbilityIconTask();
    virtual void Execute();

    u32 mUnk10;         // 0x10
    u32 mUnk14;         // 0x14
    u8 mUnk18;          // 0x18
};

class SetKasekiPointTask : public MTask {
public:
    virtual ~SetKasekiPointTask();
    virtual void Execute();

    u32 mUnk10;         // 0x10
    u32 mUnk14;         // 0x14
};

class ExecCreatureEndTask : public MProcess {
public:
    virtual ~ExecCreatureEndTask();
    virtual void Idle();

    u32 mUnk74;     // 0x74
    u32 mUnk78;     // 0x78
};

class MoveToHeatAreaAnyCreature : public MProcess {
public:
    virtual ~MoveToHeatAreaAnyCreature();
    virtual void Idle();
};

class AddKasekiPointCreatureDie : public MProcess {
public:
    virtual ~AddKasekiPointCreatureDie();
    virtual void Idle();
};

#endif
