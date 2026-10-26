#ifndef GAME_DINO_BATTLE_VIEW3_H
#define GAME_DINO_BATTLE_VIEW3_H

#include "m/menu_item.h"
#include "m/menu.h"
#include "m/display.h"
#include "m/observer.h"
#include "m/sleep.h"
#include "m/looper.h"
#include "game/help.h"
#include "game/dino_field_camera_hook.h"

class MMotion;
class MSmartOam;

class DinoBattleViewLPCountPanel : public MProcess {
public:
    DinoBattleViewLPCountPanel(MSmartOam *oam, u32 side, u32 unk98, bool unk9C);
    virtual ~DinoBattleViewLPCountPanel();
    virtual void Idle();

    u32 unk74[2];
    MMotion *mPanel;        // 0x7C
    MMotion *mUnk80;        // 0x80
    MMotion *mDigits[4];    // 0x84
    u32 mSide;              // 0x94
    u32 mUnk98;             // 0x98
    bool mUnk9C;            // 0x9C
};

class DinoBattleViewDecideFirstAttackTeam : public MProcess {
public:
    DinoBattleViewDecideFirstAttackTeam(u32 unk88, u32 unk8C, u32 unk90, u32 unk94);
    virtual ~DinoBattleViewDecideFirstAttackTeam();
    virtual void Idle();

    void StateEnd();
    // Slides both sides' life point panels in, counts them up and drops the losing side's panel.
    void StateStart();

    void (DinoBattleViewDecideFirstAttackTeam::*mState)(); // 0x74
    DinoBattleViewLPCountPanel *mPanels[2]; // 0x7C: by side
    MSmartOam *mOam;                        // 0x84
    u32 mUnk88;     // 0x88
    u32 mUnk8C;     // 0x8C
    u32 mUnk90;     // 0x90
    u32 mUnk94;     // 0x94
    u32 mUnk98;     // 0x98: (the game allocates 0x9C bytes)
};

#endif
