// DinoWirelessBattle.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_wireless_battle.h"
#include "game/dino_message.h"
#include "game/dino_ui_team_select.h"
#include "m/multi_play.h"
#include "m/overlay.h"
#include "types.h"

DinoWirelessBattle::~DinoWirelessBattle() {
}

void DinoWirelessBattle::Idle() {
    (this->*mState)();
}

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x10];
    void (DinoWirelessBattle::*StateBattleEnd_10)();
    u8 unk18[0x38 - 0x18];
    void (DinoWirelessBattle::*StateCloseHub_38)();
    u8 unk40[0x50 - 0x40];
    void (DinoWirelessBattle::*Unk0204a39c_50)();
    u8 unk58[0x78 - 0x58];
    void (DinoWirelessBattle::*Failed_78)();
} data_020c0400;
extern "C" struct {
    u8 unk00[0x10];
    void (DinoWirelessBattle::*Unk02049f3c_10)();
    u8 unk18[0x30 - 0x18];
    void (DinoWirelessBattle::*Unk02049ae4_30)();
    u8 unk38[0x40 - 0x38];
    void (DinoWirelessBattle::*Start_40)();
    u8 unk48[0x68 - 0x48];
    void (DinoWirelessBattle::*StateWaitReady_68)();
    void (DinoWirelessBattle::*Failed_70)();
} data_020c0480;

// "Waiting for the other player" (a text id).
extern "C" const u32 data_0208ad10;

M_EXTERN_OVERLAY(ov000);
M_EXTERN_OVERLAY(ov003);
M_EXTERN_OVERLAY(ov006);
M_EXTERN_OVERLAY(ov007);

DinoWirelessBattle::DinoWirelessBattle() : MProcess(NULL) {
    mState = data_020c0480.Start_40;
}

void DinoWirelessBattle::StateTeamSelect() {
    MLoadOverlay(0, M_OVERLAY_ID(ov003));
    MLoadOverlay(0, M_OVERLAY_ID(ov007));
    for (u32 i = 0; i < 2; i++) {
        mPlayers[i].unk01 = 0;
    }
    mSerial.AddTail(new DinoUiTeamSelect(&mTeam, mUnk304, mUnk305));
    mUnk304 = mUnk305 = 0;
    mState = data_020c0480.Unk02049ae4_30;
}

void DinoWirelessBattle::StateBeginBattle() {
    MUnloadOverlay(0, M_OVERLAY_ID(ov003));
    MUnloadOverlay(0, M_OVERLAY_ID(ov006));
    mHub->SendToAll(new DinoBattleReadyToBeginBattleSignal(mMultiPlay->GetPlayerIndex()));
    ShowText(data_0208ad10);
    mState = data_020c0480.StateWaitReady_68;
}

void DinoWirelessBattle::StateWaitReady() {
    if (mMultiPlay->IsFailed()) {
        mState = data_020c0400.Failed_78;
        return;
    }
    Dispatch();
    if (mPlayers[0].mReady && mPlayers[1].mReady) {
        FadeIn();
        mState = data_020c0400.StateCloseHub_38;
    }
}

void DinoWirelessBattle::StateCloseHub() {
    mHub->Exit();
    mState = data_020c0480.Unk02049f3c_10;
}

void DinoWirelessBattle::StateBattleEnd() {
    if (mMultiPlay->IsFailed()) {
        mState = data_020c0480.Failed_70;
        return;
    }
    MUnloadOverlay(0, M_OVERLAY_ID(ov000));
    if (mWon) {
        mWins++;
    } else {
        mLosses++;
    }
    mState = data_020c0400.StateBattleEnd_10;
}

void DinoWirelessBattle::StateContinue() {
    mHub->SendToAll(new DinoBattleReadyToContinueBattleSignal(mMultiPlay->GetPlayerIndex(), mContinue != 0));
    ShowText(data_0208ad10);
    mState = data_020c0400.Unk0204a39c_50;
}
