// DinoUiReviveStatus, DinoUiReviveStatusDigit, DinoUiReviveEffectLight, DinoUiReviveEffectParticle.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

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
#include "m/display.h"
#include "m/lcd.h"

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
