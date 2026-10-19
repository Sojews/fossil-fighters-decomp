// DinoBattleCpuController.

#include "game/set_live_event_check_task.h"
#include "game/dino_battle_controller.h"
#include "types.h"
#include "game/dino_message.h"
#include "game/dino_battle_cpu_controller.h"
#include "game/create_player_update_ability_icon_task.h"
#include "game/dino_resource.h"
#include "game/dino_common_live.h"
#include <map>
#include "m/fx32.h"

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

// Whether the player's leader (creature 0) has the status bit.
BOOL DinoBattleCpuController::LeaderHasStatus(DinoBattleModelPlayer *player, u32 bit) {
    DinoBattleModelCreature *creature = player->FindCreature(0);
    if (creature != NULL && (creature->GetStatus()->flags & (1 << bit))) {
        return TRUE;
    }
    return FALSE;
}

// Whether either of the player's support creatures (1 and 2) has the status bit.
BOOL DinoBattleCpuController::SupportHasStatus(DinoBattleModelPlayer *player, u32 bit) {
    DinoBattleModelCreature *creature = player->FindCreature(1);
    if (creature != NULL && (creature->GetStatus()->flags & (1 << bit))) {
        return TRUE;
    }
    creature = player->FindCreature(2);
    if (creature != NULL && (creature->GetStatus()->flags & (1 << bit))) {
        return TRUE;
    }
    return FALSE;
}

// A move the computer weighs, kept in a multimap by its (fixed-point) score (16 bytes; only its size is known). Its destructor
// isn't trivial: the game's tree has the header's unused word (see <tree>).
struct DinoBattleCpuCandidate {
    ~DinoBattleCpuCandidate() {}
    u32 unk[4];
};
typedef std::multimap<MFx32, DinoBattleCpuCandidate> DinoBattleCpuCandidates;
typedef std::__red_black_tree<MFx32, DinoBattleCpuCandidates::value_type,
                              std::__select_first<DinoBattleCpuCandidates::value_type>, std::less<MFx32>,
                              std::allocator<DinoBattleCpuCandidates::value_type> >
    DinoBattleCpuCandidateTree;

// The list's vector members and the candidate tree's members the game emits in this file.
#pragma auto_inline off
template DinoBattleCpuCandidateTree::node_base *DinoBattleCpuCandidateTree::upper_bound(const MFx32 &k, node_base *x,
                                                                                       node_base *y);
template DinoBattleCpuCandidateTree::node_base *DinoBattleCpuCandidateTree::lower_bound(const MFx32 &k, node_base *x,
                                                                                       node_base *y);
template std::pair<DinoBattleCpuCandidateTree::iterator, DinoBattleCpuCandidateTree::iterator>
DinoBattleCpuCandidateTree::equal_range(const MFx32 &k);
template class std::__vec_deleter<DinoBattleCpuEntry, std::allocator<DinoBattleCpuEntry> >;
template DinoBattleCpuCandidateTree::iterator DinoBattleCpuCandidateTree::insert_equal(const value_type &v);
template void DinoBattleCpuCandidateTree::clear();
template DinoBattleCpuCandidateTree::node_base *DinoBattleCpuCandidateTree::insert_node_at(node_base *parent, bool left,
                                                                                          bool leftmost,
                                                                                          const value_type &v);
template void DinoBattleCpuCandidateTree::destroy(node_base *n);
template DinoBattleCpuCandidateTree::~__red_black_tree();
template DinoBattleCpuCandidateTree::node_base *
DinoBattleCpuCandidateTree::find_insert_position(const MFx32 &k, node_base *&last_not_greater, bool &left, bool &leftmost);

template MTask *DinoCreatePostAndWaitTask(MMessageObserver *target, DinoBattleViewWaitActionInfoBarRequest *msg);
