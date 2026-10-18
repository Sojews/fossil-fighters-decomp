// DinoUiReviveStatus, DinoUiReviveStatusDigit, DinoUiReviveEffectLight, DinoUiReviveEffectParticle.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include <algorithm>
#include "game/dino_backup.h"

#include "game/dino_ui_revive_status_menu.h"
#include "game/dino_ui_revive_status.h"
#include "game/dino_ui_revive_status_digit.h"
#include "game/dino_ui_revive_effect_light.h"
#include "game/dino_ui_revive_effect_particle.h"
#include "m/bg.h"
#include "m/bg_new_task.h"
#include "m/bitmap.h"
#include "m/call_task.h"
#include "game/dino_ui_revive_effect.h"
#include "m/blend.h"
#include "m/bright.h"
#include "m/display.h"
#include "m/input.h"
#include "m/lcd.h"
#include "m/pal.h"
#include "m/picture.h"
#include "m/seed_generator.h"
#include "game/dino_resource.h"
#include "game/dino_sound.h"

DinoUiReviveStatus::~DinoUiReviveStatus() {
}

DinoUiReviveStatusDigit::~DinoUiReviveStatusDigit() {
}

DinoUiReviveEffectLight::~DinoUiReviveEffectLight() {
}

DinoUiReviveEffectParticle::~DinoUiReviveEffectParticle() {
}

void DinoUiReviveStatusDigit::Idle() {
}

void DinoUiReviveStatus::Idle() {
    (this->*mState)();
}

DinoUiReviveStatusParts::~DinoUiReviveStatusParts() {
}

void DinoUiReviveStatusParts::Idle() {
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
class MPalUnit {};
template class MCallTask1<MBg, MBg, MIndexBitmap *>;
template MTask *MCreateCallTask(MBg *obj, void (MBg::*func)(MIndexBitmap *), MIndexBitmap * a0);
template class MCallTask0<MPalUnit, MPalUnit>;
template MTask *MCreateCallTask(MPalUnit *obj, void (MPalUnit::*func)());

// Puts the screens back as they were.
DinoUiReviveEffect::~DinoUiReviveEffect() {
    mParallel.Clear();
    gLcd->mMain->ResetHooks();
    gLcd->mSub->ResetHooks();
    gLcd->mMain->Setup(5, 4, 0, 0);
    gLcd->mSub->Setup(5, 4, 0, 0);
    gLcd->SetBottomDisplay(gLcd->mSub);
    gCurrentDisplay = gLcd->GetBottomDisplay();
    gLcd->mMain->mBlend->Reset();
    gLcd->mSub->mBlend->Reset();
    gLcd->VSyncNow();
}

void DinoUiReviveEffect::Idle() {
    (this->*mState)();
}

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x4C];
    void (DinoUiReviveEffect::*Leave_4C)();         // 0x4C
    u8 unk54[0x5C - 0x54];
    void (DinoUiReviveEffect::*StateWait_5C)();     // 0x5C
    u8 unk64[0x84 - 0x64];
    void (DinoUiReviveEffect::*State0215ee24_84)(); // 0x84
    u8 unk8C[0xCC - 0x8C];
    void (DinoUiReviveEffect::*State0215e274_CC)(); // 0xCC
    u8 unkD4[0xE4 - 0xD4];
    void (DinoUiReviveEffect::*State0215ec2c_E4)(); // 0xE4
} data_ov003_02161d20;

// Puts the picture away.
void DinoUiReviveEffect::State0215e220() {
    gCurrentDisplay->mBg->DeleteIndexBitmap(mBitmap);
    mPalBlock->pal->Free(mPalBlock);
    if (mUnk114 && mUnkC8 == 0) {
        mState = data_ov003_02161d20.State0215e274_CC;
        return;
    }
    mState = data_ov003_02161d20.State0215ee24_84;
}

extern "C" u8 data_ov003_021624c0;
extern "C" int func_ov003_0215b480(int frames);

// Scrolls the picture up from a random place across, with a sound on the way.
void DinoUiReviveEffect::StateScroll() {
    mScrollFrame++;
    int x = gDinoResource->GetRandom()->Next(mScroller->GetWidth() - 256);
    int height = mScroller->GetHeight();
    mScroller->SetPosition(-x, mScrollFrame * -(height - 192) / func_ov003_0215b480(360));
    if (mScrollFrame == func_ov003_0215b480(200)) {
        DinoSoundPlaySe(mUnkAC.index + 0x475, 0);
    }
    if (mScrollFrame >= func_ov003_0215b480(360)) {
        mState = data_ov003_02161d20.State0215ec2c_E4;
    }
}
extern "C" const int data_020899d0;  // how long the effect waits

void DinoUiReviveEffect::StartWait() {
    gCurrentDisplay->mBg->SetMode(0);
    if (data_ov003_021624c0) {
        mTimer = func_ov003_0215b480(0xB4);
    } else {
        mTimer = data_020899d0;
    }
    mState = data_ov003_02161d20.StateWait_5C;
}

// Waits, or until a button.
void DinoUiReviveEffect::StateWait() {
    if (!(mInput->GetPressed() & 0x100B)) {
        if (--mTimer != 0) {
            return;
        }
    }
    mState = data_ov003_02161d20.Leave_4C;
}

// Fades both screens out and exits.
void DinoUiReviveEffect::Leave() {
    *mResult = 1;
    MTask *mainFade;
    MTask *subFade;
    MTask *exit;
    exit = MTaskExitTransfer::Create(this);
    subFade = gLcd->mSub->mBright->CreateFadeOutToBlack(60, 0);
    mainFade = gLcd->mMain->mBright->CreateFadeOutToBlack(60, 0);
    MParallelTaskSet *fades = new MParallelTaskSet(TRUE);
    fades->AddTail(mainFade);
    fades->AddTail(subFade);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(fades);
    serial->AddTail(exit);
}

extern const u32 kCreatureLevelLimits[2];

// A creature's experience, up to what the top level needs.
u32 DinoGetCreatureTotal(const DinoCreatureState *creature) {
    if (creature->parts[0].level == 0) {
        return 0;
    }
    u32 max, sum;
    sum = creature->unk0_8 + (creature->parts[0].level + creature->parts[1].level + creature->parts[2].level +
                              creature->parts[3].level);
    max = kCreatureLevelLimits[0] * 11 + 50;
    return std::min(max, sum);
}

u32 DinoGetCreatureLevel(const DinoCreatureState *creature) {
    if (creature->parts[0].level == 0) {
        return 0;
    }
    return (DinoGetCreatureTotal(creature) - 50) / kCreatureLevelLimits[0] + 1;
}

