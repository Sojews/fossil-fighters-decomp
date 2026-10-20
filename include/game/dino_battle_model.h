// The battle's model: the state both sides' views and controllers watch.
#ifndef GAME_DINO_BATTLE_MODEL_H
#define GAME_DINO_BATTLE_MODEL_H

#include "m/task.h"
#include "m/observer.h"
#include "game/dino_battle.h"

class DinoBattle;
class DinoBattleModelPlayer;

class DinoBattleModelCreature;

class DinoBattleTutorial;

class DinoBattleModel : public MProcess, public MMessageObserver {
public:
    DinoBattleModel(const DinoBattleParams *params, DinoBattleTutorial *tutorial);
    DinoBattleModelPlayer *GetPlayer(int index);
    DinoBattleModelCreature *FindCreature(u32 id);
    u32 GetUnk10C();
    void SendLocalMessage(MMessage *msg);
    void SetBattle(DinoBattle *battle);
    virtual ~DinoBattleModel();
    virtual void Idle();
    virtual void OnMessage(MMessage *msg);

    u8 mUnkA8;                  // 0xA8
    MMessageSubject mSubject;   // 0xAC
    MMessageSubject mSubject2;  // 0xD4
    u32 mUnkFC;                 // 0xFC
    DinoBattle *mBattle;        // 0x100
    DinoBattleModelPlayer *mPlayers[2]; // 0x104
    class DinoBattleModelField *mField; // 0x10C
    DinoBattleTutorial *mTutorial;      // 0x110
    DinoBattleParams mParams;           // 0x114
};

#endif
