// DinoBattleModel.
// Only the destructor is decompiled so far.

#include "game/dino_battle_model.h"

DinoBattleModel::~DinoBattleModel() {
}

void DinoBattleModel::Idle() {
    Dispatch();
}

void DinoBattleModel::OnMessage(MMessage *msg) {
    mSubject.Notify(msg);
    mSubject2.Notify(msg);
}

#pragma auto_inline off

void DinoBattleModel::SendLocalMessage(MMessage *msg) {
    mSubject2.Notify(msg);
}

u32 DinoBattleModel::GetUnk104(int index) {
    return mUnk104[index];
}

u32 DinoBattleModel::GetUnk10C() {
    return mUnk10C;
}
