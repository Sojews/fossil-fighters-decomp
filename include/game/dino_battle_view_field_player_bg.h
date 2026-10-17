#ifndef GAME_DINO_BATTLE_VIEW_FIELD_PLAYER_BG_H
#define GAME_DINO_BATTLE_VIEW_FIELD_PLAYER_BG_H

#include "m/task.h"

class DinoBattleViewFieldPlayerBG : public MProcess {
public:
    virtual ~DinoBattleViewFieldPlayerBG();
    virtual void Idle();
};

#endif
