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
#include "nitro/os.h"
#include "game/send_local_message_task_menu.h"
#include "m/seed_generator.h"
#include "game/dino_battle.h"
#include "game/dino_battle_model.h"

#pragma auto_inline off

extern "C" const u32 data_ov000_0218fa08[3];    // the creature slots, by entry
extern "C" void func_ov000_02158b50(DinoBattleCpuController *controller, u32 arg);

// Starts the CPU side (its player and creatures), and clears the attack bar when its turn ends.
// NONMATCHING (98.6%): the game stores each creature's unk7C value twice (one dead copy), shifting the creature
// copy's stack slot; ours keeps one.
void DinoBattleCpuController::OnMessage(MMessage *msg) {
    switch (msg->mId) {
    default:
        OS_Terminate();
        break;
    case 0x10003: {
        ((DinoBattleModel *)mUnkAC)
            ->Post(new DinoBattleModelPlayerBeginSignal(mUnkA8, SeedGenerator::GenerateSeed(),
                                                        mSides[mUnkA8].mName.c_str(), mSides[mUnkA8].unk54,
                                                        mSides[mUnkA8].unk58));
        for (int i = 0; i < 3; i++) {
            if ((u8)mSides[mUnkA8].mCreatures[i].mId != 0) {
                ((DinoBattleModel *)mUnkAC)
                    ->Post(new DinoBattleModelCreatureBeginSignal(mUnkA8, data_ov000_0218fa08[i],
                                                                  mSides[mUnkA8].mCreatures[i],
                                                                  mSides[mUnkA8].mLevels[i],
                                                                  (&mSides[mUnkA8].unk7C)[i]));
            }
        }
        ((DinoBattleModel *)mUnkAC)->Post(new DinoBattleControllerInitResult(mUnkA8));
        break;
    }
    case 0x1000D:
        if (!((DinoBattleModel *)mUnkAC)->mUnkA8) {
            GetSerial()->AddTail(mUnkB0->CreatePostTask(new DinoBattleViewClearAttackBarRequest(0)),
                                 DinoCreatePostAndWaitTask(mUnkB0, new DinoBattleViewWaitActionInfoBarRequest(0)));
            func_ov000_02158b50(this, 1);
        }
        break;
    case 0x10026: {
        DinoBattleModel *model = (DinoBattleModel *)mUnkAC;
        ((DinoBattleModelField *)model->GetUnk10C())->mUnkD0 = 0;
        mUnkB0->Post(new DinoBattleViewSetCreaturePriorityRequest(0));
        GetSerial()->AddTail(
            new SetLiveEventCheckTask(((DinoBattleModelField *)((DinoBattleModel *)mUnkAC)->GetUnk10C())->mLive, 0),
            mUnkB0->CreatePostTask(new DinoBattleViewClearAttackBarRequest(0)),
            DinoCreatePostAndWaitTask(mUnkB0, new DinoBattleViewWaitActionInfoBarRequest(0)));
        func_ov000_02158b50(this, 0);
        break;
    }
    case 0x10020:
        GetSerial()->AddTail(mUnkB0->CreatePostTask(new DinoBattleViewClearAttackBarRequest(0)),
                             DinoCreatePostAndWaitTask(mUnkB0, new DinoBattleViewWaitActionInfoBarRequest(0)));
        func_ov000_02158b50(this, 0);
        break;
    }
}

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

// The attack and defense values the damage is figured from (by how the creatures stand).
extern "C" MFx32 func_ov000_0216e2d8(DinoBattleModelCreature *attacker, DinoBattleModelCreature *target);
extern "C" MFx32 func_ov000_0216e4c0(DinoBattleModelCreature *attacker, DinoBattleModelCreature *target);
extern "C" MFx32 func_ov000_0216e650(DinoBattleModelCreature *attacker, DinoBattleModelCreature *target);
MFx32 GetAffinityRatio(DinoBattleModelCreature *creature, DinoBattleModelCreature *other, u32 attack);

int DinoBattleCpuController::EstimateDamage(DinoBattleModelCreature *attacker, DinoBattleModelCreature *target,
                                            u32 attack) {
    const DinoAttackDef *def = attacker->GetAttackDef(attack);
    if (def == NULL) {
        return 0;
    }
    const u16 *cost = DinoOffsetPtr<u16>(def, def->cost);
    MFx32 power = attacker->CalcUnkEC(target);
    MFx32 guard = func_ov000_0216e2d8(attacker, target);
    MFx32 defense = attacker->ReduceByDefense(power, guard);
    MFx32 curveA = func_ov000_0216e4c0(attacker, target);
    MFx32 curveB = func_ov000_0216e650(attacker, target);
    MFx32 curve = attacker->CurveRatio(curveA, curveB);    // (unused)
    MFx32 ratio = MFx32(1);
    MFx32 affinity = GetAffinityRatio(attacker, target, def->unk00);
    u8 hit = TRUE;
    MFx32 damage = attacker->CalcDamage(target, MFx32(cost[1]), defense, &hit);
    damage = attacker->ApplyRatio(damage, ratio);
    damage = attacker->ApplyRatio2(damage, affinity);
    damage = attacker->ApplyPositionRatio(damage, target->GetB8Category());
    int value = MFx32ToInt(damage);
    if (value < 1 && hit) {
        return 1;
    }
    return value;
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
