// DinoBattleHelp.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#define DINO_BATTLE_HELP_BASE_OWNER
#include "game/dino_battle_help.h"
#include "types.h"

DinoBattleHelpBase::~DinoBattleHelpBase() {
}

DinoBattleHelpSupport::~DinoBattleHelpSupport() {
}

MDisplay *DinoBattleHelpBase::GetDisplay() {
    return MProcess::GetDisplay();
}

const char *DinoBattleHelpBase::GetText() {
    return gDinoResource->GetText(mTextId);
}

MRect DinoBattleHelpBase::GetRect() {
    return mRect;
}
