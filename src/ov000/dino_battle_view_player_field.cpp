// DinoBattleViewPlayerField.
// Only the destructor is decompiled so far.

#include "game/dino_battle_view_player_field.h"

DinoBattleViewPlayerField::~DinoBattleViewPlayerField() {
    if (mBg) {
        mBg->DeleteText(mText);
    }
}
