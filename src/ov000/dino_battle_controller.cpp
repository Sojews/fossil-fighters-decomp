// DinoBattleController, DinoBattleDummyController.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_battle_controller.h"

DinoBattleController::~DinoBattleController() {
}

DinoBattleDummyController::~DinoBattleDummyController() {
}

void DinoBattleDummyController::OnMessage(MMessage *msg) {
}

void DinoBattleController::Idle() {
    Dispatch();
}
