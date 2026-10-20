// DinoBattleModel.
// Only the destructor is decompiled so far.

#include "game/send_local_message_task_menu.h"
#include "game/dino_battle_model.h"
#include "game/create_player_update_ability_icon_task.h"
#include "game/dino_static_init.h"
#include "game/dino_battle_multi_play_message_hub.h"
#include "game/dino_backup.h"

// Sets up both players and the field (and the wireless message hub), copying the parameters.
DinoBattleModel::DinoBattleModel(const DinoBattleParams *params, DinoBattleTutorial *tutorial)
    : MProcess(NULL), mUnkA8(0), mTutorial(tutorial), mParams(*params) {
    switch (params->unk18) {
    case 0:
        break;
    case 1: {
        DinoBattleMultiPlayMessageHub *hub = new DinoBattleMultiPlayMessageHub(&mSubject2);
        mSubject.AddObserver(hub);
        mParallel.AddTail(hub);
        break;
    }
    }
    mUnkFC = params->unk04;
    mPlayers[0] = new DinoBattleModelPlayer(0, params->unk10);
    mPlayers[1] = new DinoBattleModelPlayer(1, params->unk14);
    mField = new DinoBattleModelField();
    mPlayers[0]->SetUnkC8(params->sides[0].unk88);
    mPlayers[1]->SetUnkC8(params->sides[1].unk88);
    for (u32 i = 0; i < 2; i++) {
        switch ((&params->unk08)[i]) {
        case 2:
            if (params->unk164) {
                mPlayers[i]->SetUnk15C();
            }
            break;
        case 1:
            break;
        case 0:
            if (gDinoBackup->GetSystemFlag(0x28)) {
                mPlayers[i]->SetUnk15C();
            }
            break;
        }
    }
    mSubject2.AddObserver(mPlayers[0]);
    mSubject2.AddObserver(mPlayers[1]);
    mSubject2.AddObserver(mField);
    GetParallel()->AddTail(mPlayers[0], mPlayers[1], mField);
}

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

void DinoBattleModel::SetBattle(DinoBattle *battle) {
    mPlayers[0]->SetBattle(battle);
    mPlayers[1]->SetBattle(battle);
    mField->SetBattle(battle);
    mBattle = battle;
}

DinoBattleModelPlayer *DinoBattleModel::GetPlayer(int index) {
    return mPlayers[index];
}

u32 DinoBattleModel::GetUnk10C() {
    return (u32)mField;
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

// The file's static initialization (see dino_static_init.h).
extern "C" u32 data_ov000_0219140c[];
static struct DinoBattleModelStaticInit {
    DinoBattleModelStaticInit() {
        DINO_POOL_WORD(data_ov000_0219140c, 0x1c) = 128;
        DINO_POOL_WORD(data_ov000_0219140c, 0x20) = 176;
        DINO_POOL_WORD(data_ov000_0219140c, 0x4) = 8;
        DINO_POOL_WORD(data_ov000_0219140c, 0x8) = 150;
        DINO_POOL_WORD(data_ov000_0219140c, 0xc) = 64;
        DINO_POOL_WORD(data_ov000_0219140c, 0x10) = 112;
    }
} sDinoBattleModelStaticInit;
