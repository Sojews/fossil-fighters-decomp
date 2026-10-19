#ifndef GAME_DINO_WIRELESS_BATTLE_H
#define GAME_DINO_WIRELESS_BATTLE_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_backup.h"
#include "game/dino_field_camera_hook.h"

class MMultiPlay;
class MMultiPlayMessageHub;

// A player in a wireless battle, as their DinoBattlePlayerInfoSignal tells.
struct DinoWirelessBattlePlayer {
    u8 mInfoReceived;   // 0x00: sent DinoBattlePlayerInfoSignal
    u8 mTeamChosen;     // 0x01
    u8 mReady;          // 0x02: sent DinoBattleReadyToBeginBattleSignal
    u8 unk03[5];
    DinoPlayerData mData;   // 0x08
    u8 unk74[0x9C - 0x74];
    DinoCreatureState mCreatures[5];    // 0x9C: the team
};

class DinoWirelessBattle : public MProcess, public MMessageObserver {
public:
    DinoWirelessBattle();
    virtual ~DinoWirelessBattle();
    virtual void Idle();
    virtual void OnMessage(MMessage *msg);

    void ShowText(u32 text);
    void Start();
    void FadeIn();
    void StateSendLevel();
    void StateWaitLevels();
    void StateTeamSelect();
    // Shows the opponent's team for the player to pick the order in.
    void StateOrderSelect();
    void StateSendTeam();
    void StateWaitTeams();
    void StateBeginBattle();
    void StateWaitReady();
    void StateCloseHub();
    void StateBattleEnd();
    void StateResult();
    void StateContinue();
    void StateReset();
    void StateFinish();
    void Failed();
    void StateSoftReset();

    void (DinoWirelessBattle::*mState)(); // 0xA8
    MMultiPlay *mMultiPlay;         // 0xB0
    MTask *mUnkB4;                  // 0xB4
    MMultiPlayMessageHub *mHub;     // 0xB8
    u32 mTeam;                      // 0xBC: the team select's
    DinoWirelessBattlePlayer mPlayers[2];   // 0xC0
    u8 mWon;                        // 0x2E8
    u32 mWins;                      // 0x2EC
    u32 mLosses;                    // 0x2F0
    u32 mContinue;                  // 0x2F4
    MTaskSet *mUnk2F8;              // 0x2F8
    DinoHelpDisabler mDisabler;     // 0x2FC
    u32 mLevel;                     // 0x300: the player's horider level
    u8 mUnk304;                     // 0x304
    u8 mUnk305;                     // 0x305
    DinoTeam mTeamData;             // 0x306: the team used
    u8 unk32E[0x330 - 0x32E];
};

#endif
