// DinoBattleController, DinoBattleDummyController.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#define DINO_BATTLE_CONTROLLER_OWNER
#include "game/dino_battle_controller.h"
#include "game/dino_battle.h"

#pragma auto_inline off

DinoBattleController::DinoBattleController(u32 arg) : MProcess(NULL) {
    mUnkA8 = arg;
    mUnkAC = 0;
    mUnkB0 = NULL;
}

void DinoBattleController::Setup(DinoBattle *battle) {
    mUnkAC = battle->GetUnk1FC();
    mUnkB0 = battle->GetUnk200();
}

DinoBattleDummyController::DinoBattleDummyController(u32 arg) : DinoBattleController(arg) {
}

#pragma auto_inline on

DinoBattleController::~DinoBattleController() {
}

DinoBattleDummyController::~DinoBattleDummyController() {
}

void DinoBattleDummyController::OnMessage(MMessage *msg) {
}

void DinoBattleController::Idle() {
    Dispatch();
}

BOOL DinoBattleDummyController::Unk10() {
    return TRUE;
}
