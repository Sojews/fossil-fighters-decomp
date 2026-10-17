// DinoBattleMultiPlayMessageHub.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_battle_multi_play_message_hub.h"
#include "m/multi_play.h"
#include "types.h"

#pragma auto_inline off

// Passes the messages that arrive from the other consoles on to the battle.
DinoBattleMultiPlayMessageReceiver::DinoBattleMultiPlayMessageReceiver(MMessageSubject *source,
                                                                       MMessageSubject *target)
    : MProcess(NULL) {
    mSubject = target;
    source->AddObserver(this);
}

DinoBattleMultiPlayMessageHub::DinoBattleMultiPlayMessageHub(MMessageSubject *target) : MProcess(NULL) {
    mHub = new MMultiPlayMessageHub(0);
    DinoBattleMultiPlayMessageReceiver *receiver = new DinoBattleMultiPlayMessageReceiver(mHub, target);
    MParallelTaskSet *parallel = &mParallel;
    parallel->AddTail(mHub);
    parallel->AddTail(receiver);
}

#pragma auto_inline on

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

void DinoBattleMultiPlayMessageHub::OnMessage(MMessage *msg) {
    mHub->Broadcast(msg);
}
