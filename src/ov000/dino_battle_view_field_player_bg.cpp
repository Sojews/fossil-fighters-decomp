// DinoBattleViewFieldPlayerBG.
// Only the destructors are decompiled so far.

#include "game/dino_battle_view_field_player_bg.h"

DinoBattleViewFieldPlayerBG::~DinoBattleViewFieldPlayerBG() {
}

void DinoBattleViewFieldPlayerBG::Idle() {
    (this->*mState)();
}
