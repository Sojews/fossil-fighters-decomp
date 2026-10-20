#ifndef GAME_DINO_UI_REVIVE_EFFECT_PARTICLE_H
#define GAME_DINO_UI_REVIVE_EFFECT_PARTICLE_H

#include "m/task.h"

class MMotion;
class MSmartOam;

class DinoUiReviveEffectParticle : public MProcess {
public:
    DinoUiReviveEffectParticle();
    virtual ~DinoUiReviveEffectParticle();
    virtual void Idle();

    MMotion *mMotion;           // 0x74
    MParallelTaskSet *mTasks;   // 0x78
    MSmartOam *mOam;            // 0x7C
    int mTimer;                 // 0x80
};

#endif
