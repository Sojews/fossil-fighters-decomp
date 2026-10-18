// DinoFieldBattleStartEffect.
// Only the destructors are decompiled so far.

#include "game/dino_field_battle_start_effect.h"

DinoFieldBattleStartEffect::~DinoFieldBattleStartEffect() {
}

void DinoFieldBattleStartEffect::Idle() {
    (this->*mState)();
}

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0xC];
    void (DinoFieldBattleStartEffect::*Start_0C)();
} data_ov001_0216aad4;

DinoFieldBattleStartEffect::DinoFieldBattleStartEffect(DinoField *field) : MProcess(NULL), mField(field) {
    mState = data_ov001_0216aad4.Start_0C;
}
