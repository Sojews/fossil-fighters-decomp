#ifndef GAME_DINO_NET_BATTLE_SAMPLE_H
#define GAME_DINO_NET_BATTLE_SAMPLE_H

#include "m/task.h"

class DinoNetBattleSample : public MProcess {
public:
    virtual ~DinoNetBattleSample();
    virtual void Idle();

    void (DinoNetBattleSample::*mState)(); // 0x74
};

#endif
