// The battle's model: the state both sides' views and controllers watch.
#ifndef GAME_DINO_BATTLE_MODEL_H
#define GAME_DINO_BATTLE_MODEL_H

#include "m/task.h"
#include "m/observer.h"
#include "game/dino_battle.h"

class DinoBattleModel : public MProcess, public MMessageObserver {
public:
    void SendLocalMessage(MMessage *msg);
    virtual ~DinoBattleModel();
    virtual void Idle();
    virtual void OnMessage(MMessage *msg);

    u32 mUnkA8;                 // 0xA8
    MMessageSubject mSubject;   // 0xAC
    MMessageSubject mSubject2;  // 0xD4
    u32 mUnkFC[18];             // 0xFC
    DinoBattleSide mSides[2];   // 0x144
};

#endif
