#ifndef GAME_DINO_UI_REVIVE_EFFECT_PARTICLE_H
#define GAME_DINO_UI_REVIVE_EFFECT_PARTICLE_H

#include "m/task.h"

class DinoUiReviveEffectParticle : public MProcess {
public:
    virtual ~DinoUiReviveEffectParticle();
    virtual void Idle();
};

#endif
