#ifndef GAME_DINO_UI_REVIVE_EFFECT_H
#define GAME_DINO_UI_REVIVE_EFFECT_H

#include "m/picture.h"
#include "m/resource.h"
#include "m/task.h"

class MIndexBitmap;
class MInput;
struct MPalBlock;

// The reviving effect (lights and particles).
// (The data's layout isn't known yet.)
struct DinoUiReviveEffectData;

class DinoUiReviveEffect : public MProcess {
public:
    virtual ~DinoUiReviveEffect();
    virtual void Idle();

    void State0215e220();
    void State0215e274();
    void State0215ee24();
    void StartWait();
    void StateWait();
    void State0215fab4();

    void (DinoUiReviveEffect::*mState)(); // 0x74
    u32 unk7C[19];
    u32 mUnkC8;                             // 0xC8
    MPictureFile mPicture;                  // 0xCC
    MIndexBitmap *mBitmap;                  // 0x100
    MPalBlock *mPalBlock;                   // 0x104
    u32 unk108[3];
    u8 mUnk114;                             // 0x114
    u32 unk118[4];
    MResource<DinoUiReviveEffectData> mUnk128; // 0x128
    int mTimer;                             // 0x138
    MInput *mInput;                         // 0x13C
};

#endif
