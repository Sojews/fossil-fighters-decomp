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
                                                        mParams.sides[mUnkA8].mName.c_str(), mParams.sides[mUnkA8].unk54,
                                                        mParams.sides[mUnkA8].unk58));
        for (int i = 0; i < 3; i++) {
            if ((u8)mParams.sides[mUnkA8].mCreatures[i].mId != 0) {
                ((DinoBattleModel *)mUnkAC)
                    ->Post(new DinoBattleModelCreatureBeginSignal(mUnkA8, data_ov000_0218fa08[i],
                                                                  mParams.sides[mUnkA8].mCreatures[i],
                                                                  mParams.sides[mUnkA8].mLevels[i],
                                                                  (&mParams.sides[mUnkA8].unk7C)[i]));
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


// Lists the creatures the creature's attack would target (none if it can't use it).
void DinoBattleCpuController::CollectTargets(DinoBattleModelPlayer *player, DinoBattleModelCreature *creature,
                                             u32 attack) {
    mTargets.clear();
    if (attack >= creature->CountAttacks() && !player->AllCreaturesShareDefFlag()) {
        return;
    }
    const DinoAttackDef *def = creature->GetAttackDef(attack);
    if (def == NULL) {
        return;
    }
    DinoBattleModelPlayer *opponent = player->GetOpponent();
    if (*DinoOffsetPtr<u8>(def, def->unk14) == 3 || *DinoOffsetPtr<u8>(def, def->unk14) == 16) {
        for (u32 i = 0; i < opponent->GetCreatureCount(); i++) {
            mTargets.push_back(opponent->GetCreature(i));
        }
    } else if (*DinoOffsetPtr<u8>(def, def->unk14) == 10 || *DinoOffsetPtr<u8>(def, def->unk14) == 13) {
        for (u32 i = 0; i < player->GetCreatureCount(); i++) {
            mTargets.push_back(player->GetCreature(i));
        }
    } else if (*DinoOffsetPtr<u8>(def, def->unk10) == 9 || *DinoOffsetPtr<u8>(def, def->unk10) == 10 ||
               *DinoOffsetPtr<u8>(def, def->unk10) == 11) {
        for (u32 i = 0; i < player->GetCreatureCount(); i++) {
            if (player->GetCreature(i)->GetUnkB8() != 3) {
                mTargets.push_back(player->GetCreature(i));
            }
        }
    } else if (*DinoOffsetPtr<u8>(def, def->unk14) == 11 || *DinoOffsetPtr<u8>(def, def->unk14) == 14) {
        for (u32 i = 0; i < player->GetCreatureCount(); i++) {
            if (player->GetCreature(i) != creature) {
                mTargets.push_back(player->GetCreature(i));
            }
        }
    } else if (*DinoOffsetPtr<u8>(def, def->unk14) == 12) {
        if (player->GetCreatureCount() != 1) {
            mTargets.push_back(creature);
        }
    } else if (*DinoOffsetPtr<u8>(def, def->unk14) == 9 || *DinoOffsetPtr<u8>(def, def->unk14) == 2 ||
               *DinoOffsetPtr<u8>(def, def->unk10) == 8) {
        mTargets.push_back(creature);
    } else if (*DinoOffsetPtr<u8>(def, def->unk14) == 15) {
        mTargets.push_back(player->FindCreature(0));
    } else {
        u32 ids[3];
        int count;
        opponent->CollectCreaturesNearB8(creature->GetB8Category(), ids, &count);
        for (int i = 0; i < count; i++) {
            mTargets.push_back(opponent->FindCreatureByB4(ids[i]));
        }
    }
}

// A condition of the computer's scores and the score it adds (both u16; 8 bytes).
struct DinoBattleCpuCondition {
    u16 kind;
    u16 unk2;
    u16 score;
    u16 unk6;
};

struct DinoBattleCpuConditions {
    u32 unk0;
    u32 count;
    u32 entries;    // offset of the conditions
};

// NONMATCHING: in case 1 the operator+ parameter copies swap stack slots, as in ScoreConditions (casting either
// operand fixes the order but costs a stack slot).
MFx32 DinoBattleCpuController::ScoreCreatureConditions(DinoBattleModelPlayer *player, DinoBattleModelCreature *creature,
                                                       DinoBattleFile file) {
    if (!file.mArchive.IsValid()) {
        return MFx32(0);
    }
    MFx32 total = MFx32(0);
    for (u32 i = 0; i < ((DinoBattleCpuConditions *)file.mFile.GetData())->count; i++) {
        DinoBattleCpuConditions *data = (DinoBattleCpuConditions *)file.mFile.GetData();
        const DinoBattleCpuCondition *conditions = (const DinoBattleCpuCondition *)((u8 *)data + data->entries);
        MFx32 score = MFx32(conditions[i].score);
        switch (conditions[i].kind) {
        default:
            OS_Terminate();
            break;
        case 0:
            total = total + score;
            break;
        case 1:
            total = total + score * player->GetRandom()->NextFraction();
            break;
        case 0x46:
            if (HasCostlyAttackUnk10(player, creature, 2)) {
                total = total + score;
            }
            break;
        case 0x47:
            if (HasCostlyAttackUnk10(player, creature, 3)) {
                total = total + score;
            }
            break;
        case 0x48:
            if (HasCostlyAttackUnk10(player, creature, 4)) {
                total = total + score;
            }
            break;
        case 0x49:
            if (HasCostlyAttackUnk10(player, creature, 5)) {
                total = total + score;
            }
            break;
        case 0x4A:
            if (HasCostlyAttackUnk10(player, creature, 6)) {
                total = total + score;
            }
            break;
        case 0x4B:
            if (HasCostlyAttackUnk10(player, creature, 7)) {
                total = total + score;
            }
            break;
        case 0x4C:
            if (HasCostlyAttackUnk10(player, creature, 8)) {
                total = total + score;
            }
            break;
        case 0x4D:
            if (HasCostlyAttackUnk10(player, creature, 9)) {
                total = total + score;
            }
            break;
        case 0x4E:
            if (HasCostlyAttackUnk10(player, creature, 10)) {
                total = total + score;
            }
            break;
        case 0x4F:
            if (HasCostlyAttackUnk10(player, creature, 11)) {
                total = total + score;
            }
            break;
        case 0x50:
            if (HasCostlyAttackUnk14(player, creature, 2)) {
                total = total + score;
            }
            break;
        case 0x51:
            if (HasCostlyAttackUnk14(player, creature, 3)) {
                total = total + score;
            }
            break;
        case 0x52:
            if (HasCostlyAttackUnk14(player, creature, 4)) {
                total = total + score;
            }
            break;
        case 0x53:
            if (HasCostlyAttackUnk14(player, creature, 5)) {
                total = total + score;
            }
            break;
        case 0x54:
            if (HasCostlyAttackUnk14(player, creature, 6)) {
                total = total + score;
            }
            break;
        case 0x55:
            if (HasCostlyAttackUnk14(player, creature, 7)) {
                total = total + score;
            }
            break;
        case 0x56:
            if (HasCostlyAttackUnk14(player, creature, 8)) {
                total = total + score;
            }
            break;
        case 0x57:
            if (HasCostlyAttackUnk14(player, creature, 9)) {
                total = total + score;
            }
            break;
        case 0x58:
            if (HasCostlyAttackUnk14(player, creature, 10)) {
                total = total + score;
            }
            break;
        case 0x59:
            if (HasCostlyAttackUnk14(player, creature, 11)) {
                total = total + score;
            }
            break;
        case 0x5A:
            if (HasCostlyAttackUnk14(player, creature, 12)) {
                total = total + score;
            }
            break;
        case 0x5B:
            if (HasCostlyAttackUnk14(player, creature, 13)) {
                total = total + score;
            }
            break;
        case 0x5C:
            if (HasCostlyAttackUnk14(player, creature, 14)) {
                total = total + score;
            }
            break;
        case 0x5D:
            if (HasCostlyAttackUnk14(player, creature, 16)) {
                total = total + score;
            }
            break;
        case 0x5E:
            if (HasCostlyAttackUnk14(player, creature, 15)) {
                total = total + score;
            }
            break;
        case 0x12:
            if (creature->GetUnkB8() == 0) {
                total = total + score;
            }
            break;
        case 0x13:
            if (creature->GetUnkB8() == 1 || creature->GetUnkB8() == 2) {
                total = total + score;
            }
            break;
        case 0x61: {
            DinoPlayerData data;
            gDinoBackup->GetPlayer(&data, FALSE);
            if (data.unk1c_7 >= 1 && data.unk1c_7 <= 3) {
                total = total + score;
            }
            break;
        }
        }
    }
    return total;
}

// NONMATCHING: in case 1 the operator+ parameter copies swap stack slots, as in CalcUnkEC.
MFx32 DinoBattleCpuController::ScoreConditions(DinoBattleModelPlayer *player, DinoBattleModelCreature *creature,
                                               DinoBattleModelCreature *target, DinoBattleFile file) {
    if (!file.mArchive.IsValid()) {
        return MFx32(0);
    }
    DinoBattleModelPlayer *opponent = player->GetOpponent();
    MFx32 total = MFx32(0);
    for (u32 i = 0; i < ((DinoBattleCpuConditions *)file.mFile.GetData())->count; i++) {
        DinoBattleCpuConditions *data = (DinoBattleCpuConditions *)file.mFile.GetData();
        const DinoBattleCpuCondition *conditions = (const DinoBattleCpuCondition *)((u8 *)data + data->entries);
        MFx32 score = MFx32(conditions[i].score);
        switch (conditions[i].kind) {
        default:
            OS_Terminate();
            break;
        case 0:
            total = total + score;
            break;
        case 1:
            total = total + score * player->GetRandom()->NextFraction();
            break;
        case 0x8B:
            if ((int)creature->GetUnkE4() > (int)target->GetUnkE4()) {
                total = total + score;
            }
            break;
        case 0x8C:
            if ((int)creature->GetUnkEC() > (int)target->GetUnkEC()) {
                total = total + score;
            }
            break;
        case 0x8D:
            if ((int)creature->GetUnkF0() > (int)target->GetUnkF0()) {
                total = total + score;
            }
            break;
        case 0x8E:
            if ((int)creature->GetUnkF4() > (int)target->GetUnkF4()) {
                total = total + score;
            }
            break;
        case 0x8F:
            if ((int)creature->GetUnkF8() > (int)target->GetUnkF8()) {
                total = total + score;
            }
            break;
        case 0x90:
            if (creature->GetDefUnk61() == 7) {
                total = total + score;
            }
            break;
        case 0x91:
            if (creature->GetDefUnk61() == 9) {
                total = total + score;
            }
            break;
        case 0x92:
            if (creature->GetDefUnk61() == 6) {
                total = total + score;
            }
            break;
        case 0x93:
            for (int j = 0; j < 3; j++) {
                const DinoAttackDef *def = creature->GetAttackDef(j);
                if (def != NULL && *DinoOffsetPtr<u8>(def, def->unk10) > 1) {
                    total = total + score;
                    break;
                }
            }
            break;
        case 0x94:
            for (int j = 0; j < 3; j++) {
                const DinoAttackDef *def = creature->GetAttackDef(j);
                if (def != NULL && *DinoOffsetPtr<u8>(def, def->unk14) > 1) {
                    total = total + score;
                    break;
                }
            }
            break;
        case 0x95:
            if (creature->CompareUnkED0C(opponent->FindCreature(0)) > 0) {
                total = total + score;
            }
            break;
        case 0x96:
            if (creature->CompareUnkED0C(opponent->FindCreature(0)) == 0) {
                total = total + score;
            }
            break;
        case 0x97:
            if (creature == player->GetCreatureInSlot(0)) {
                total = total + score;
            }
            break;
        }
    }
    return total;
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
