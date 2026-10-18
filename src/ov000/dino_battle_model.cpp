// DinoBattleModel.
// Only the destructor is decompiled so far.

#include "game/dino_battle_model.h"
#include "game/create_player_update_ability_icon_task.h"

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

extern "C" void func_ov000_0215c218(void *controller, DinoBattle *battle);

void DinoBattleModel::SetBattle(DinoBattle *battle) {
    mPlayers[0]->SetBattle(battle);
    mPlayers[1]->SetBattle(battle);
    func_ov000_0215c218((void *) mUnk10C, battle);
    mBattle = battle;
}

DinoBattleModelPlayer *DinoBattleModel::GetPlayer(int index) {
    return mPlayers[index];
}

u32 DinoBattleModel::GetUnk10C() {
    return mUnk10C;
}
