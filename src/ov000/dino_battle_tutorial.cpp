// DinoBattleTutorial.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_battle_tutorial.h"
#include "types.h"

DinoBattleViewTutorialWindow::~DinoBattleViewTutorialWindow() {
}

DinoBattleTutorialConsoleFilter::~DinoBattleTutorialConsoleFilter() {
}

KeyWaitTask::~KeyWaitTask() {
}

DinoBattleTutorial::~DinoBattleTutorial() {
    delete mFilter;
}
