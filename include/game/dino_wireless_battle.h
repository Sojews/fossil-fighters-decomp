#ifndef GAME_DINO_WIRELESS_BATTLE_H
#define GAME_DINO_WIRELESS_BATTLE_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class MMultiPlay;
class MMultiPlayMessageHub;

// A player in a wireless battle, as their DinoBattlePlayerInfoSignal tells.
struct DinoWirelessBattlePlayer {
    u8 unk00;
    u8 unk01;
    u8 mReady;      // 0x02: sent DinoBattleReadyToBeginBattleSignal
    u8 unk03[0x114 - 3];
};

class DinoWirelessBattle : public MProcess, public MMessageObserver {
public:
    DinoWirelessBattle();
    virtual ~DinoWirelessBattle();
    virtual void Idle();
    virtual void OnMessage(MMessage *msg);

    void ShowText(u32 text);
    void FadeIn();
    void StateTeamSelect();
    void StateBeginBattle();
    void StateWaitReady();
    void StateCloseHub();
    void StateBattleEnd();
    void StateContinue();

    void (DinoWirelessBattle::*mState)(); // 0xA8
    MMultiPlay *mMultiPlay;         // 0xB0
    u32 unkB4;
    MMultiPlayMessageHub *mHub;     // 0xB8
    u32 mTeam;                      // 0xBC: the team select's
    DinoWirelessBattlePlayer mPlayers[2];   // 0xC0
    u8 mWon;                        // 0x2E8
    u32 mWins;                      // 0x2EC
    u32 mLosses;                    // 0x2F0
    u32 mContinue;                  // 0x2F4
    MTaskSet *mUnk2F8;              // 0x2F8
    DinoHelpDisabler mDisabler;     // 0x2FC
    u8 unk300[4];
    u8 mUnk304;                     // 0x304
    u8 mUnk305;                     // 0x305
    u8 unk306[0x330 - 0x306];
};

#endif
