#ifndef GAME_DINO_UI_REVIVE_EFFECT_LIGHT_H
#define GAME_DINO_UI_REVIVE_EFFECT_LIGHT_H

#include "m/task.h"

class MExtAffinePicture;

class DinoUiReviveEffectLight : public MProcess {
public:
    virtual ~DinoUiReviveEffectLight();
    virtual void Idle();

    MExtAffinePicture *mPicture;    // 0x74
    u16 mAngle;                     // 0x78: of the bobbing
    int mHeight;                    // 0x7C: risen so far
    int mTimer;                     // 0x80: frames left to rise
};

#endif
