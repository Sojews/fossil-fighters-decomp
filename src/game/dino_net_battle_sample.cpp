// DinoNetBattleSample.
// Only the destructors are decompiled so far.

#include "game/dino_net_battle_sample.h"
#include "game/dino_ui_team_select.h"
#include "m/overlay.h"

DinoNetBattleSample::~DinoNetBattleSample() {
}

void DinoNetBattleSample::Idle() {
    (this->*mState)();
}

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x4];
    void (DinoNetBattleSample::*Unk02034130_04)();
    u8 unk0C[0x14 - 0xC];
    void (DinoNetBattleSample::*Start_14)();
} data_020bd0d8;

M_EXTERN_OVERLAY(ov003);
M_EXTERN_OVERLAY(ov007);

DinoNetBattleSample::DinoNetBattleSample() : MProcess(NULL) {
    mState = data_020bd0d8.Start_14;
}

void DinoNetBattleSample::StateTeamSelect() {
    mTeam = 0;
    MLoadOverlay(0, M_OVERLAY_ID(ov003));
    MLoadOverlay(0, M_OVERLAY_ID(ov007));
    mSerial.AddTail(new DinoUiTeamSelect(&mTeam, 0, 0));
    mState = data_020bd0d8.Unk02034130_04;
}
