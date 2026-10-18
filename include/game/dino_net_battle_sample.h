#ifndef GAME_DINO_NET_BATTLE_SAMPLE_H
#define GAME_DINO_NET_BATTLE_SAMPLE_H

#include "m/task.h"

class DinoNetBattleSample : public MProcess {
public:
    DinoNetBattleSample();
    virtual ~DinoNetBattleSample();
    virtual void Idle();

    void StateSetup();
    void StateTeamSelect();

    void (DinoNetBattleSample::*mState)(); // 0x74
    u32 mTeam;                             // 0x7C
};

#endif
