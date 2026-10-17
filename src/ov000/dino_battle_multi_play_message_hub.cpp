// DinoBattleMultiPlayMessageHub.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_battle_multi_play_message_hub.h"
#include "types.h"

DinoBattleMultiPlayMessageHub::~DinoBattleMultiPlayMessageHub() {
}

DinoBattleMultiPlayMessageReceiver::~DinoBattleMultiPlayMessageReceiver() {
}

void DinoBattleMultiPlayMessageReceiver::Idle() {
    Dispatch();
}

void DinoBattleMultiPlayMessageHub::Idle() {
    Dispatch();
}

void DinoBattleMultiPlayMessageReceiver::OnMessage(MMessage *msg) {
    mSubject->Notify(msg);
}

extern "C" void func_0206af18(void *obj, MMessage *msg);

void DinoBattleMultiPlayMessageHub::OnMessage(MMessage *msg) {
    func_0206af18(mUnkA8, msg);
}
