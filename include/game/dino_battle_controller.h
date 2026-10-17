// What drives a side in battle (the player, the AI, ...).
#ifndef GAME_DINO_BATTLE_CONTROLLER_H
#define GAME_DINO_BATTLE_CONTROLLER_H

#include "m/task.h"
#include "m/observer.h"

class DinoBattleController : public MProcess, public MMessageObserver {
public:
    virtual ~DinoBattleController();
    virtual void Idle();
    virtual void Unk10();
    virtual void Unk14();
};

class DinoBattleDummyController : public DinoBattleController {
public:
    virtual ~DinoBattleDummyController();
    virtual void Unk10();
    virtual void OnMessage(MMessage *msg);
};

#endif
