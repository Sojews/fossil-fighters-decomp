#ifndef GAME_DINO_UI_REVIVE_EFFECT_H
#define GAME_DINO_UI_REVIVE_EFFECT_H

#include "m/picture.h"
#include "m/resource.h"
#include "m/task.h"

// The reviving effect (lights and particles).
// (The data's layout isn't known yet.)
struct DinoUiReviveEffectData;

class DinoUiReviveEffect : public MProcess {
public:
    virtual ~DinoUiReviveEffect();
    virtual void Idle();

    void (DinoUiReviveEffect::*mState)(); // 0x74
    u32 unk7C[20];
    MPictureFile mPicture;                  // 0xCC
    u32 unk100[10];
    MResource<DinoUiReviveEffectData> mUnk128; // 0x128
};

#endif
