// DinoBattleCpuController.

#include "game/set_live_event_check_task.h"
#include "game/dino_battle_controller.h"
#include "types.h"
#include "game/dino_message.h"
#include "game/dino_battle_cpu_controller.h"
#include "game/create_player_update_ability_icon_task.h"
#include "game/dino_resource.h"
#include "game/dino_common_live.h"

#pragma auto_inline off

BOOL DinoBattleController::Unk10() {
    return FALSE;
}

BOOL DinoBattleController::Unk14() {
    return FALSE;
}


void SetLiveEventCheckTask::Execute() {
    if (mLive != NULL) {
        if (!mCheck) {
            mLive->CloseIfOpen();
        }
        mLive->SetUnkD2(mCheck);
    }
    Exit();
}

#pragma auto_inline on

DinoBattleCpuController::~DinoBattleCpuController() {
}

SetLiveEventCheckTask::~SetLiveEventCheckTask() {
}

#pragma auto_inline off

// Whether the creature's attack can be used: one its level allows (or any, if the player's creatures all have the
// flag), that the player has the points for.
BOOL DinoBattleCpuController::CanUseAttack(DinoBattleModelPlayer *player, DinoBattleModelCreature *creature,
                                           u32 index) {
    if (index >= creature->CountAttacks() && !player->AllCreaturesShareDefFlag()) {
        return FALSE;
    }
    const DinoAttackDef *def = creature->GetAttackDef(index);
    if (def != NULL) {
        u16 cost = *DinoOffsetPtr<u16>(def, def->cost);
        if (player->GetUnk128() >= cost) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL DinoBattleCpuController::HasAttackUnk10(DinoBattleModelCreature *creature, u8 value) {
    for (u32 i = 0; i < 5; i++) {
        const DinoAttackDef *def = creature->GetAttackDef(i);
        if (def != NULL && value == *DinoOffsetPtr<u8>(def, def->unk10)) {
            return TRUE;
        }
    }
    return FALSE;
}

// Whether the creature has an attack with that value that costs more than the player has.
BOOL DinoBattleCpuController::HasCostlyAttackUnk10(DinoBattleModelPlayer *player, DinoBattleModelCreature *creature,
                                                   u8 value) {
    for (u32 i = 0; i < 5; i++) {
        const DinoAttackDef *def = creature->GetAttackDef(i);
        if (def != NULL && value == *DinoOffsetPtr<u8>(def, def->unk10)) {
            u16 cost = *DinoOffsetPtr<u16>(def, def->cost);
            if (cost > player->GetUnk128()) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

BOOL DinoBattleCpuController::HasAttackUnk14(DinoBattleModelCreature *creature, u8 value) {
    for (u32 i = 0; i < 5; i++) {
        const DinoAttackDef *def = creature->GetAttackDef(i);
        if (def != NULL && value == *DinoOffsetPtr<u8>(def, def->unk14)) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL DinoBattleCpuController::HasCostlyAttackUnk14(DinoBattleModelPlayer *player, DinoBattleModelCreature *creature,
                                                   u8 value) {
    for (u32 i = 0; i < 5; i++) {
        const DinoAttackDef *def = creature->GetAttackDef(i);
        if (def != NULL && value == *DinoOffsetPtr<u8>(def, def->unk14)) {
            u16 cost = *DinoOffsetPtr<u16>(def, def->cost);
            if (cost > player->GetUnk128()) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

// The list's vector members the game emits in this file.
#pragma auto_inline off
template class std::__vec_deleter<DinoBattleCpuEntry, std::allocator<DinoBattleCpuEntry> >;

template MTask *DinoCreatePostAndWaitTask(MMessageObserver *target, DinoBattleViewWaitActionInfoBarRequest *msg);
