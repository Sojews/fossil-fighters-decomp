// The battle's model: the state both sides' views and controllers watch.
#ifndef GAME_DINO_BATTLE_MODEL_H
#define GAME_DINO_BATTLE_MODEL_H

#include "m/task.h"
#include "m/observer.h"
#include "game/dino_battle.h"

class DinoBattle;
class DinoBattleModelPlayer;

class DinoBattleModel : public MProcess, public MMessageObserver {
public:
    DinoBattleModelPlayer *GetPlayer(int index);
    u32 GetUnk10C();
    void SendLocalMessage(MMessage *msg);
    void SetBattle(DinoBattle *battle);
    virtual ~DinoBattleModel();
    virtual void Idle();
    virtual void OnMessage(MMessage *msg);

    u32 mUnkA8;                 // 0xA8
    MMessageSubject mSubject;   // 0xAC
    MMessageSubject mSubject2;  // 0xD4
    u32 mUnkFC;                 // 0xFC
    DinoBattle *mBattle;        // 0x100
    DinoBattleModelPlayer *mPlayers[2]; // 0x104
    u32 mUnk10C;                // 0x10C
    u32 mUnk110[13];            // 0x110
    DinoBattleSide mSides[2];   // 0x144
};

#endif
