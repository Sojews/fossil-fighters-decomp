// DinoFieldBattleStartEffect.

#include <algorithm>
#include "game/dino_field_battle_start_effect.h"
#include "game/dino_sound.h"
#include "m/lcd.h"
#include "m/blend.h"
#include "m/bright.h"
#include "m/raster.h"
#include "m/fx32.h"

DinoFieldBattleStartEffect::~DinoFieldBattleStartEffect() {
}

void DinoFieldBattleStartEffect::Idle() {
    (this->*mState)();
}

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    int mMinRadius;     // 0x00
    void (DinoFieldBattleStartEffect::*StateClose_04)();
    void (DinoFieldBattleStartEffect::*Start_0C)();
} data_ov001_0216aad4;

DinoFieldBattleStartEffect::DinoFieldBattleStartEffect(DinoField *field) : MProcess(NULL), mField(field) {
    mState = data_ov001_0216aad4.Start_0C;
}

// Whitens the top screen and opens a window over the whole bottom one, its sprites blended.
void DinoFieldBattleStartEffect::Start() {
    mWindow = new MRasterWindow(-1, gLcd->GetBottomDisplay(), TRUE);
    mParallel.AddTail(mWindow);
    MBlend *blend = gLcd->GetBottomDisplay()->mBlend;
    blend->SetMode(2);
    blend->SetBrightness(16);
    blend->SetTarget1(0x3F);
    mWindow->SetTarget(0x3F);
    mWindow->SetObjTarget(0x3F);
    mWindow->SetBlendEnabled(FALSE);
    mWindow->SetObjBlendEnabled(TRUE);
    mWindow->AddRect(MRect(0, 0, 256, 192));
    mWindow->Show();
    mFrame = 0;
    mParallel.AddTail(gLcd->GetTopDisplay()->mBright->CreateFadeOutToWhite(0x59, 0));
    DinoSoundPlayBgm(0x75, 0);
    mState = data_ov001_0216aad4.StateClose_04;
}

// NONMATCHING: the game spills two copies of each fixed-point operand and of the product (as in DinoHelpStatus::Scroll).
void DinoFieldBattleStartEffect::StateClose() {
    if (++mFrame > 90) {
        gLcd->GetBottomDisplay()->mBright->SetMode(1);
        gLcd->GetBottomDisplay()->mBright->SetFactor(16);
        MTask *exit = MTaskExitTransfer::Create(this);
        MTask *clear = gLcd->GetBottomDisplay()->mBlend->CreateClearTransfer();
        MTask *timer = new MTimer(1);
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(timer);
        serial->AddTail(clear);
        serial->AddTail(exit);
        return;
    }
    int radius = 160 - mFrame * 160 / 90;
    MFx32 wave = MFx32::FromRaw(FX_SinCosTable_[(((u16) ((mFrame * 3 << 16) / 90)) >> 4) * 2]);
    wave *= MFx32(30);
    radius -= MFx32ToInt(wave);
    radius = std::max(data_ov001_0216aad4.mMinRadius, radius);
    mWindow->SetCircle(MPoint(128, 96), radius);
}
