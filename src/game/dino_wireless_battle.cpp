// DinoWirelessBattle.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "m/lcd.h"
#include "m/bright.h"
#include "game/dino_common_window_message_box.h"
#include "m/picture.h"
#include <algorithm>
#include "game/dino_backup.h"
#include "game/dino_resource.h"
#include "game/dino_sound.h"
#include "game/dino_wireless_battle.h"
#include "game/dino_message.h"
#include "game/dino_ui_team_select.h"
#include "m/multi_play.h"
#include "m/overlay.h"
#include "m/soft_reset.h"
#include "types.h"

DinoWirelessBattle::~DinoWirelessBattle() {
}

void DinoWirelessBattle::Idle() {
    (this->*mState)();
}

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x8];
    void (DinoWirelessBattle::*StateContinue_08)(); // 0x08
    void (DinoWirelessBattle::*StateBattleEnd_10)(); // 0x10
    void (DinoWirelessBattle::*StateFinish_18)(); // 0x18
    u8 unk20[0x38 - 0x20];
    void (DinoWirelessBattle::*StateCloseHub_38)(); // 0x38
    void (DinoWirelessBattle::*StateWaitLevels_40)(); // 0x40
    u8 unk48[0x50 - 0x48];
    void (DinoWirelessBattle::*Unk0204a39c_50)(); // 0x50
    void (DinoWirelessBattle::*Unk02049d30_58)(); // 0x58
    void (DinoWirelessBattle::*StateSoftReset_60)(); // 0x60
    u8 unk68[0x78 - 0x68];
    void (DinoWirelessBattle::*Failed_78)(); // 0x78
} data_020c0400;
extern "C" struct {
    u8 unk00[0x10];
    void (DinoWirelessBattle::*Unk02049f3c_10)(); // 0x10
    u8 unk18[0x28 - 0x18];
    void (DinoWirelessBattle::*StateTeamSelect_28)(); // 0x28
    void (DinoWirelessBattle::*Unk02049ae4_30)(); // 0x30
    u8 unk38[0x40 - 0x38];
    void (DinoWirelessBattle::*Start_40)(); // 0x40
    void (DinoWirelessBattle::*Failed_48)(); // 0x48
    u8 unk50[0x60 - 0x50];
    void (DinoWirelessBattle::*Failed_60)(); // 0x60
    void (DinoWirelessBattle::*StateWaitReady_68)(); // 0x68
    void (DinoWirelessBattle::*Failed_70)(); // 0x70
} data_020c0480;

// "Waiting for the other player" (a text id).
extern "C" const u32 data_0208ad10;

M_EXTERN_OVERLAY(ov000);
M_EXTERN_OVERLAY(ov003);
M_EXTERN_OVERLAY(ov004);
M_EXTERN_OVERLAY(ov006);
M_EXTERN_OVERLAY(ov007);

DinoWirelessBattle::DinoWirelessBattle() : MProcess(NULL) {
    mState = data_020c0480.Start_40;
}

void DinoWirelessBattle::StateTeamSelect() {
    MLoadOverlay(0, M_OVERLAY_ID(ov003));
    MLoadOverlay(0, M_OVERLAY_ID(ov007));
    for (u32 i = 0; i < 2; i++) {
        mPlayers[i].mTeamChosen = 0;
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

void DinoWirelessBattle::StateSendLevel() {
    DinoPlayerData player;
    gDinoBackup->GetPlayer(&player, 0);
    mLevel = player.horiderLevel;
    gDinoBackup->GetTeam(gDinoBackup->GetUnk34_26(), &mTeamData);
    mHub->SendToAll(new DinoBattlePlayerLevelSignal(mMultiPlay->GetPlayerIndex(), mLevel));
    mState = data_020c0400.StateWaitLevels_40;
}

// Both play at the lower horider level.
void DinoWirelessBattle::StateWaitLevels() {
    if (mMultiPlay->IsFailed()) {
        mState = data_020c0480.Failed_48;
        return;
    }
    Dispatch();
    if (mPlayers[0].mInfoReceived && mPlayers[1].mInfoReceived) {
        DinoPlayerData player;
        gDinoBackup->GetPlayer(&player, 0);
        u32 level0;
        u32 level1;
        level1 = mPlayers[1].mData.horiderLevel;
        level0 = mPlayers[0].mData.horiderLevel;
        player.horiderLevel = std::min(level0, level1);
        gDinoBackup->SetPlayer(&player);
        mState = data_020c0480.StateTeamSelect_28;
    }
}

void DinoWirelessBattle::StateWaitTeams() {
    if (mMultiPlay->IsFailed()) {
        mState = data_020c0480.Failed_60;
        return;
    }
    Dispatch();
    if (mPlayers[0].mTeamChosen && mPlayers[1].mTeamChosen) {
        FadeIn();
        DinoSoundStopBgm(-1, 10);
        mSerial.AddTail(new MTimer(10));
        mState = data_020c0400.Unk02049d30_58;
    }
}

void DinoWirelessBattle::StateReset() {
    MUnloadOverlay(0, M_OVERLAY_ID(ov004));
    if (mContinue) {
        mState = data_020c0400.StateContinue_08;
        return;
    }
    FadeIn();
    MTask *reset = mMultiPlay->CreateResetTask();
    MTask *wait = new MTimer(5);
    MTaskSet &serial = mSerial;
    serial.AddTail(wait);
    serial.AddTail(reset);
    mState = data_020c0400.StateFinish_18;
}

// Keeps the level and team, and leaves the connection to the resources.
void DinoWirelessBattle::StateFinish() {
    DinoPlayerData player;
    gDinoBackup->GetPlayer(&player, 0);
    player.horiderLevel = mLevel;
    gDinoBackup->SetPlayer(&player);
    gDinoBackup->SetTeam(gDinoBackup->GetUnk34_26(), &mTeamData);
    MTask *unkB4 = mUnkB4;
    MTaskSet *parallel = &mParallel;
    parallel->Remove(mMultiPlay);
    parallel->Remove(unkB4);
    gDinoResource->SetUnk90((u32)mMultiPlay, (u32)mUnkB4);
    Exit();
}

// The connection was lost: closes it, then resets the game.
void DinoWirelessBattle::Failed() {
    FadeIn();
    DinoSoundStopBgm(-1, 10);
    MTask *timer = new MTimer(10);
    MTask *finish = mMultiPlay->CreateFinishTask();
    MParallelTaskSet *tasks = new MParallelTaskSet(TRUE);
    tasks->AddTail(finish);
    tasks->AddTail(timer);
    mSerial.AddTail(tasks);
    mState = data_020c0400.StateSoftReset_60;
}

void DinoWirelessBattle::StateSoftReset() {
    MSoftReset(1);
}

// Whitens both screens (for the fade back in) and drops what was showing.
void DinoWirelessBattle::FadeIn() {
    gLcd->GetTopDisplay()->mBright->SetMode(1);
    gLcd->GetTopDisplay()->mBright->SetFactor(16);
    gLcd->GetTopDisplay()->Update();
    gLcd->GetBottomDisplay()->mBright->SetMode(1);
    gLcd->GetBottomDisplay()->mBright->SetFactor(16);
    gLcd->GetBottomDisplay()->Update();
    mUnk2F8->Clear();
}

// Whites out both screens and shows the text in a waiting message window.
void DinoWirelessBattle::ShowText(u32 text) {
    mUnk2F8->Clear();
    gLcd->GetTopDisplay()->mBright->Reset();
    gLcd->GetBottomDisplay()->mBright->Reset();
    MTextPicture *top = new MTextPicture(gLcd->GetTopDisplay(), "/image/white", TRUE, false);
    top->SetPriority(3);
    top->Show();
    MTextPicture *bottom = new MTextPicture(gLcd->GetBottomDisplay(), "/image/white", TRUE, false);
    bottom->SetPriority(3);
    bottom->Show();
    mUnk2F8->AddTail(top, bottom,
                     new DinoCommonWindowMessageWait1(gDinoResource->GetText(text), MPoint(128, 96), 4, 0));
}
