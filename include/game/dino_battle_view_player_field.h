#ifndef GAME_DINO_BATTLE_VIEW_PLAYER_FIELD_H
#define GAME_DINO_BATTLE_VIEW_PLAYER_FIELD_H

#include "m/view.h"
#include "m/bg.h"

class MText;

class DinoBattleViewPlayerField : public MView {
public:
    virtual ~DinoBattleViewPlayerField();
    virtual void Execute();

    u32 mUnk10[2];  // 0x10
    MBg *mBg;       // 0x18
    MText *mText;   // 0x1C
};

#endif
