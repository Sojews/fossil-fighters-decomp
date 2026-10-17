#ifndef GAME_DINO_UI_REVIVE_EFFECT_H
#define GAME_DINO_UI_REVIVE_EFFECT_H

#include "m/task.h"

// The reviving effect (lights and particles).
class DinoUiReviveEffect : public MProcess {
public:
    virtual ~DinoUiReviveEffect();
    virtual void Idle();

    void (DinoUiReviveEffect::*mState)(); // 0x74
};

#endif
