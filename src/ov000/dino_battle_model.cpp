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

#pragma auto_inline off

// The creature with the id, on either side.
DinoBattleModelCreature *DinoBattleModel::FindCreature(u32 id) {
    for (int side = 0; side < 2; side++) {
        for (u32 i = 0; i < mPlayers[side]->GetCreatureCount(); i++) {
            DinoBattleModelCreature *creature = mPlayers[side]->GetCreature(i);
            if (id == creature->GetUnkB4()) {
                return creature;
            }
        }
    }
    return NULL;
}
