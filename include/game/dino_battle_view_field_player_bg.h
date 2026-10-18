#ifndef GAME_DINO_BATTLE_VIEW_FIELD_PLAYER_BG_H
#define GAME_DINO_BATTLE_VIEW_FIELD_PLAYER_BG_H

#include "m/task.h"

class MTextPicture;

// The picture behind a side's creatures: slides off and the other side's slides on.
class DinoBattleViewFieldPlayerBG : public MProcess {
public:
    virtual ~DinoBattleViewFieldPlayerBG();
    virtual void Idle();

    BOOL SetSide(u32 side);
    void StateLoad();
    void StateShow();
    void StateHide();

    void (DinoBattleViewFieldPlayerBG::*mState)(); // 0x74
    u32 mSide;                  // 0x7C: 2 for none
    MTextPicture *mPicture;     // 0x80
    u8 mChanging;               // 0x84
    u32 unk88;
    u32 mBusy;                  // 0x8C
};

#endif
