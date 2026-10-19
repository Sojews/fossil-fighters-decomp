// DinoWirelessBattle.

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
#include "game/dino_ui_order_select.h"
#include "m/text_codec.h"
#include "nitro/mi.h"
#include "game/dino_ui_result.h"
#include "game/dino_battle.h"
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
    void (DinoWirelessBattle::*StateResult_10)(); // 0x10
    void (DinoWirelessBattle::*StateFinish_18)(); // 0x18
    void (DinoWirelessBattle::*StateBattleEnd_20)(); // 0x20
    void (DinoWirelessBattle::*StateSendLevel_28)(); // 0x28
    u8 unk30[0x38 - 0x30];
    void (DinoWirelessBattle::*StateCloseHub_38)(); // 0x38
    void (DinoWirelessBattle::*StateWaitLevels_40)(); // 0x40
    u8 unk48[0x50 - 0x48];
    void (DinoWirelessBattle::*Unk0204a39c_50)(); // 0x50
    void (DinoWirelessBattle::*StateOrderSelect_58)(); // 0x58
    void (DinoWirelessBattle::*StateSoftReset_60)(); // 0x60
    u8 unk68[0x78 - 0x68];
    void (DinoWirelessBattle::*Failed_78)(); // 0x78
} data_020c0400;
extern "C" struct {
    void (DinoWirelessBattle::*StateReset_00)(); // 0x00
    void (DinoWirelessBattle::*Failed_08)(); // 0x08
    void (DinoWirelessBattle::*StateBattle_10)(); // 0x10
    u8 unk18[0x20 - 0x18];
    void (DinoWirelessBattle::*StateBeginBattle_20)(); // 0x20
    void (DinoWirelessBattle::*StateTeamSelect_28)(); // 0x28
    void (DinoWirelessBattle::*StateSendTeam_30)(); // 0x30
    u8 unk38[0x40 - 0x38];
    void (DinoWirelessBattle::*Start_40)(); // 0x40
    void (DinoWirelessBattle::*Failed_48)(); // 0x48
    void (DinoWirelessBattle::*StateWaitTeams_50)(); // 0x50
    u8 unk58[0x60 - 0x58];
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

// Takes over the connection from the resources and starts listening to the other console.
void DinoWirelessBattle::Start() {
    mUnk2F8 = new MParallelTaskSet();
    mParallel.AddTail(mUnk2F8);
    mWins = 0;
    mLosses = 0;
    gDinoResource->GetUnk90((u32 *) &mMultiPlay, (u32 *) &mUnkB4);
    MTask *unkB4 = mUnkB4;
    MTaskSet *parallel = &mParallel;
    parallel->AddTail(mMultiPlay);
    parallel->AddTail(unkB4);
    if (mMultiPlay->IsFailed()) {
        mState = data_020c0480.Failed_08;
        return;
    }
    mUnk304 = 1;
    mUnk305 = mMultiPlay->GetPlayerIndex() != 0 ? 1 : 0;
    mHub = new MMultiPlayMessageHub(0);
    mHub->AddObserver(this);
    mParallel.AddTail(mHub);
    MI_CpuFill8(mPlayers, 0, sizeof(mPlayers));
    mSerial.AddTail(new MTimer(10));
    mState = data_020c0400.StateSendLevel_28;
}

void DinoWirelessBattle::StateTeamSelect() {
    MLoadOverlay(0, M_OVERLAY_ID(ov003));
    MLoadOverlay(0, M_OVERLAY_ID(ov007));
    for (u32 i = 0; i < 2; i++) {
        mPlayers[i].mTeamChosen = 0;
    }
    mSerial.AddTail(new DinoUiTeamSelect(&mTeam, mUnk304, mUnk305));
    mUnk304 = mUnk305 = 0;
    mState = data_020c0480.StateSendTeam_30;
}

void DinoWirelessBattle::StateOrderSelect() {
    MLoadOverlay(0, M_OVERLAY_ID(ov003));
    MLoadOverlay(0, M_OVERLAY_ID(ov006));
    u32 other = (mMultiPlay->GetPlayerIndex() + 1) & 1;
    DinoWirelessBattlePlayer *opponent = &mPlayers[other];
    DinoUiOrderSelectInfo info;
    MI_CpuFill8(&info, 0, sizeof(info));
    info.name = MEncodeUtf8(std::wstring((const wchar_t *) opponent->mData.name));
    for (u32 i = 0; i < 5; i++) {
        info.creatures[i] = opponent->mCreatures[i];
    }
    for (u32 i = 0; i < 2; i++) {
        mPlayers[i].mReady = FALSE;
    }
    mSerial.AddTail(new DinoUiOrderSelect(mTeam, info, 1, 0, 0, 1));
    mState = data_020c0480.StateBeginBattle_20;
}

// Sends the chosen team (with the creatures in it) to both consoles.
void DinoWirelessBattle::StateSendTeam() {
    MUnloadOverlay(0, M_OVERLAY_ID(ov003));
    MUnloadOverlay(0, M_OVERLAY_ID(ov007));
    DinoPlayerData player;
    DinoCreatureState creatures[5];
    MI_CpuFill8(creatures, 0, sizeof(creatures));
    gDinoBackup->GetPlayer(&player, 1);
    DinoTeam team;
    gDinoBackup->GetTeam(mTeam, &team);
    for (u32 i = 0; i < 5; i++) {
        if (team.slots[i]) {
            gDinoBackup->GetCreature(team.slots[i], &creatures[i]);
        }
    }
    mHub->SendToAll(new DinoBattlePlayerInfoSignal(mMultiPlay->GetPlayerIndex(), player, team, creatures));
    ShowText(data_0208ad10);
    mState = data_020c0480.StateWaitTeams_50;
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
    mState = data_020c0480.StateBattle_10;
}

// Sets up this console's side of the battle (the other's comes over the connection) and runs it.
void DinoWirelessBattle::StateBattle() {
    MLoadOverlay(0, M_OVERLAY_ID(ov000));
    DinoBattleParams params;
    MI_CpuFill8(&params, 0, sizeof(params));
    u32 me = mMultiPlay->GetPlayerIndex();
    params.unk00 = 2;
    params.unk04 = (u32) mMultiPlay;
    u32 other = (me + 1) & 1;
    (&params.unk08)[me] = 0;
    (&params.unk08)[other] = 1;
    (&params.unk10)[me] = 0;
    (&params.unk10)[other] = 1;
    params.unk18 = 1;
    params.unk1C = me == 0;
    params.unk148[3] = gDinoResource->GetBattleAdjustDefs()->wirelessUnk1B4;
    params.unk20[2] = gDinoResource->GetBattleAdjustDefs()->wirelessUnk1B8;
    params.unk20[3] = gDinoResource->GetBattleAdjustDefs()->wirelessUnk1BC;
    params.unk20[1] = gDinoResource->GetBattleAdjustDefs()->wirelessUnk1C0;
    DinoTeam team;
    gDinoBackup->GetTeam(mTeam, &team);
    DinoBattleSide *side = &params.sides[me];
    DinoPlayerData *player = &mPlayers[me].mData;
    side->mName = MEncodeUtf8(std::wstring((const wchar_t *) player->name));
    if (player->unk14_0 != 0) {
        *(u32 *) &side->unk54[4] = player->unk14_0 + 8;
    } else {
        *(u32 *) &side->unk54[4] = player->rank + 1;
    }
    for (u32 i = 0; i < 3; i++) {
        DinoCreatureState *creature = (DinoCreatureState *) &side->mCreatures[i];
        gDinoBackup->GetCreature(team.slots[i], creature);
        u32 j;
        u32 parts = 0;
        for (j = 0; j < 4; j++) {
            if (creature->parts[j].level) {
                parts++;
            }
        }
        side->mLevels[i] = parts;
    }
    *(u32 *) side->unk54 = mPlayers[me].mData.horiderLevel;
    side->unk7C[0xD] = gDinoBackup->GetSystemFlag(0x28);
    mSerial.AddTail(new DinoBattle(params, (u32) &mWon, 0, &mUnk2E9, NULL));
    mState = data_020c0400.StateBattleEnd_20;
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
    mState = data_020c0400.StateResult_10;
}

// Shows the wins and losses so far (asking whether to go on), listening to the other console again.
void DinoWirelessBattle::StateResult() {
    MLoadOverlay(0, M_OVERLAY_ID(ov004));
    mContinue = 1;
    mHub = new MMultiPlayMessageHub(0);
    mHub->AddObserver(this);
    mParallel.AddTail(mHub);
    for (u32 i = 0; i < 2; i++) {
        mPlayers[i].unk03[0] = 0;
        mPlayers[i].unk03[1] = 0;
    }
    u32 other = (mMultiPlay->GetPlayerIndex() + 1) & 1;
    mSerial.AddTail(new DinoUiResult((u32) &mContinue, mWon, mWins, mLosses,
                                     MEncodeUtf8(std::wstring((const wchar_t *) mPlayers[other].mData.name))));
    mState = data_020c0480.StateReset_00;
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
        mState = data_020c0400.StateOrderSelect_58;
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
