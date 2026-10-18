#ifndef GAME_DINO_UI_REVIVE_EFFECT_H
#define GAME_DINO_UI_REVIVE_EFFECT_H

#include "m/picture.h"
#include "m/resource.h"
#include "m/task.h"
#include "game/dino_backup.h"

class MExtAffinePicture;
class MIndexBitmap;
class MInput;
class MView;
struct MPalUnit;

// The reviving effect (lights and particles).
// (The data's layout isn't known yet.)
struct DinoUiReviveEffectData;

class DinoUiReviveEffect : public MProcess {
public:
    virtual ~DinoUiReviveEffect();
    virtual void Idle();

    void State0215e030();
    void State0215e220();
    void StateScroll();
    void State0215ec2c();
    void State0215e274();
    void State0215ee24();
    void StartWait();
    void CheckSkip();
    void CheckSkip2();
    void State0215f668();
    void State0215da04();
    void StateWait();
    void Leave();

    void (DinoUiReviveEffect::*mState)(); // 0x74
    DinoCreatureState mCreature;            // 0x7C
    DinoCreatureState mUnk94;               // 0x94
    DinoCreatureState mUnkAC;               // 0xAC
    u32 *mResult;                           // 0xC4
    u32 mUnkC8;                             // 0xC8
    MPictureFile mPicture;                  // 0xCC
    MIndexBitmap *mBitmap;                  // 0x100
    MPalUnit *mPalBlock;                   // 0x104
    u32 unk108[3];
    u8 mUnk114;                             // 0x114
    MExtAffinePicture *mScroller;           // 0x118: scrolled up over the effect
    int mScrollFrame;                       // 0x11C
    u32 unk120;
    MView *mUnk124;                         // 0x124
    MResource<DinoUiReviveEffectData> mUnk128; // 0x128
    int mTimer;                             // 0x138
    MInput *mInput;                         // 0x13C
    u8 mShowText;                           // 0x140: tell the player the creature's name after the effect
};

#endif
