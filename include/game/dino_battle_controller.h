// What drives a side in battle (the player, the AI, ...).
#ifndef GAME_DINO_BATTLE_CONTROLLER_H
#define GAME_DINO_BATTLE_CONTROLLER_H

#include "m/task.h"
#include "m/observer.h"

class DinoBattleController : public MProcess, public MMessageObserver {
public:
    // The game's destructor is inline. Its vtable's owner (dino_battle_controller.cpp) defines
    // DINO_BATTLE_CONTROLLER_OWNER and emits it out of line until the key function there is decompiled.
#ifdef DINO_BATTLE_CONTROLLER_OWNER
    virtual ~DinoBattleController();
#else
    virtual ~DinoBattleController() {}
#endif
    virtual void Idle();
    virtual BOOL Unk10();
    virtual BOOL Unk14();
};

class DinoBattleDummyController : public DinoBattleController {
public:
    virtual ~DinoBattleDummyController();
    virtual BOOL Unk10();
    virtual void OnMessage(MMessage *msg);
};

#endif
