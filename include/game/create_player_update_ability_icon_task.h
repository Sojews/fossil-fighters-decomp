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
};

class ResetStatusEffectSleepTask : public MTask {
public:
    virtual ~ResetStatusEffectSleepTask();
    virtual void Execute();
};

class AddLifePointTask : public MTask {
public:
    virtual ~AddLifePointTask();
    virtual void Execute();
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
};

class PlayerUpdateTask : public MTask {
public:
    virtual ~PlayerUpdateTask();
    virtual void Execute();
};

class DinoCreatureUpdateStateIconTask : public MTask {
public:
    virtual ~DinoCreatureUpdateStateIconTask();
    virtual void Execute();
};

class UpdateAbilityIconTask : public MTask {
public:
    virtual ~UpdateAbilityIconTask();
    virtual void Execute();
};

class SetKasekiPointTask : public MTask {
public:
    virtual ~SetKasekiPointTask();
    virtual void Execute();
};

class ExecCreatureEndTask : public MProcess {
public:
    virtual ~ExecCreatureEndTask();
    virtual void Idle();
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
