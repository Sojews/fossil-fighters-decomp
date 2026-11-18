// DinoFieldBattleStartEffect.

#pragma ipa file

#include <algorithm>
#include "game/dino_field_battle_start_effect.h"
#include "game/dino_sound.h"
#include "m/lcd.h"
#include "m/blend.h"
#include "m/bright.h"
#include "m/raster.h"
#include "m/fx32.h"

DinoFieldBattleStartEffect::DinoFieldBattleStartEffect(DinoField *field) : MProcess(NULL), mField(field) {
    mState = &DinoFieldBattleStartEffect::Start;
}

void DinoFieldBattleStartEffect::Idle() {
    (this->*mState)();
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
    mState = &DinoFieldBattleStartEffect::StateClose;
}

void DinoFieldBattleStartEffect::StateClose() {
    if (++mFrame > 90) {
        gLcd->GetBottomDisplay()->mBright->SetMode(1);
        gLcd->GetBottomDisplay()->mBright->SetFactor(16);
        MTask *clear;
        MTask *exit = MTaskExitTransfer::Create(this);
        clear = gLcd->GetBottomDisplay()->mBlend->CreateClearTransfer();
        MTask *timer = new MTimer(1);
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(timer);
        serial->AddTail(clear);
        serial->AddTail(exit);
        return;
    }
    int radius = 160 - mFrame * 160 / 90;
    radius -= MFx32ToInt(MFx32(30) * MFx32::FromRaw(FX_SinCosTable_[(((u16) ((mFrame * 3 << 16) / 90)) >> 4) * 2]));
    radius = std::max(0, radius);
    mWindow->SetCircle(MPoint(128, 96), radius);
}

DinoFieldBattleStartEffect::~DinoFieldBattleStartEffect() {
}
