// DinoBattleCpuController.

#pragma ipa file

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
#include "game/dino_backup.h"

#pragma auto_inline off

extern "C" const u32 data_ov000_0218fa08[3];    // the creature slots, by entry

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
            DecideAction(1);
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
        DecideAction(0);
        break;
    }
    case 0x10020:
        GetSerial()->AddTail(mUnkB0->CreatePostTask(new DinoBattleViewClearAttackBarRequest(0)),
                             DinoCreatePostAndWaitTask(mUnkB0, new DinoBattleViewWaitActionInfoBarRequest(0)));
        DecideAction(0);
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
// (Pushed through a reference, which gives the game's order for push_back's byte temps under -ipa file.)
static inline void PushTarget(std::vector<DinoBattleModelCreature *> &v, DinoBattleModelCreature *const &x) {
    v.push_back(x);
}

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
            PushTarget(mTargets, opponent->GetCreature(i));
        }
    } else if (*DinoOffsetPtr<u8>(def, def->unk14) == 10 || *DinoOffsetPtr<u8>(def, def->unk14) == 13) {
        for (u32 i = 0; i < player->GetCreatureCount(); i++) {
            PushTarget(mTargets, player->GetCreature(i));
        }
    } else if (*DinoOffsetPtr<u8>(def, def->unk10) == 9 || *DinoOffsetPtr<u8>(def, def->unk10) == 10 ||
               *DinoOffsetPtr<u8>(def, def->unk10) == 11) {
        for (u32 i = 0; i < player->GetCreatureCount(); i++) {
            if (player->GetCreature(i)->GetUnkB8() != 3) {
                PushTarget(mTargets, player->GetCreature(i));
            }
        }
    } else if (*DinoOffsetPtr<u8>(def, def->unk14) == 11 || *DinoOffsetPtr<u8>(def, def->unk14) == 14) {
        for (u32 i = 0; i < player->GetCreatureCount(); i++) {
            if (player->GetCreature(i) != creature) {
                PushTarget(mTargets, player->GetCreature(i));
            }
        }
    } else if (*DinoOffsetPtr<u8>(def, def->unk14) == 12) {
        if (player->GetCreatureCount() != 1) {
            PushTarget(mTargets, creature);
        }
    } else if (*DinoOffsetPtr<u8>(def, def->unk14) == 9 || *DinoOffsetPtr<u8>(def, def->unk14) == 2 ||
               *DinoOffsetPtr<u8>(def, def->unk10) == 8) {
        PushTarget(mTargets, creature);
    } else if (*DinoOffsetPtr<u8>(def, def->unk14) == 15) {
        PushTarget(mTargets, player->FindCreature(0));
    } else {
        u32 ids[3];
        int count;
        opponent->CollectCreaturesNearB8(creature->GetB8Category(), ids, &count);
        for (int i = 0; i < count; i++) {
            PushTarget(mTargets, opponent->FindCreatureByB4(ids[i]));
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

extern "C" BOOL func_ov000_0216c83c(int slot);

// NONMATCHING: as in ScoreCreatureConditions, the game puts each compound score's copy above the total's.
MFx32 DinoBattleCpuController::ScoreEscape(DinoBattleModelPlayer *player, DinoBattleModelCreature *creature,
                                           DinoBattleFile file) {
    if (!file.mArchive.IsValid()) {
        return MFx32(0);
    }
    if (creature->GetUnkB8() != 0) {
        return MFx32(0);
    }
    if (!player->IsUnk2163978() || !creature->IsUnkD274()) {
        return MFx32(0);
    }
    DinoBattleModelCreature *support1 = player->FindCreature(1);
    DinoBattleModelCreature *support2 = player->FindCreature(2);
    DinoBattleModelPlayer *opponent = player->GetOpponent();
    DinoBattleModelCreature *rival = opponent->FindCreature(0);
    int best = -1;
    u32 bestSlot = 4;
    for (int i = 0; i < player->GetCreatureCount(); i++) {
        DinoBattleModelCreature *c = player->GetCreature(i);
        if (i == 0 || best < (int)c->GetUnkEC()) {
            best = c->GetUnkEC();
            bestSlot = c->GetUnkB8();
        }
    }
    DinoBattleModelCreature *strongest = player->FindCreature(bestSlot);
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
            total += score * player->GetRandom()->NextFraction();
            break;
        case 0x30:
            if ((support1 != NULL && creature->CompareUnkED0C(rival) < 0 && support1->CompareUnkED0C(rival) >= 0) ||
                (support2 != NULL && creature->CompareUnkED0C(rival) < 0 && support2->CompareUnkED0C(rival) >= 0)) {
                total = total + score;
            }
            break;
        case 0x31:
            if ((support1 != NULL && creature->CompareUnkED0C(rival) < 0 && support1->CompareUnkED0C(rival) > 0) ||
                (support2 != NULL && creature->CompareUnkED0C(rival) < 0 && support2->CompareUnkED0C(rival) > 0)) {
                total = total + score;
            }
            break;
        case 0x32:
            if ((support1 != NULL && creature->CompareUnkED0C(rival) <= 0 && support1->CompareUnkED0C(rival) > 0) ||
                (support2 != NULL && creature->CompareUnkED0C(rival) <= 0 && support2->CompareUnkED0C(rival) > 0)) {
                total = total + score;
            }
            break;
        case 0x33:
            if (creature->GetStatus()->flags & 4) {
                total = total + score;
            }
            break;
        case 0x34:
            if (creature->GetStatus()->flags & 0x10) {
                total = total + score;
            }
            break;
        case 0x35:
            if (creature->GetStatus()->flags & 0x40) {
                total = total + score;
            }
            break;
        case 0x36:
            if (creature->GetStatus()->flags & 0x80) {
                total = total + score;
            }
            break;
        case 0x37:
        case 0x38:
        case 0x39:
            if (func_ov000_0216c83c(bestSlot) && CanUseAttack(player, strongest, conditions[i].kind - 0x37)) {
                total = total + score;
            }
            break;
        case 0x3A:
            if (func_ov000_0216c83c(bestSlot) && CanUseAttack(player, strongest, 4)) {
                total = total + score;
            }
            break;
        case 0x3B:
        case 0x3C:
        case 0x3D:
            if (bestSlot == 0 && !CanUseAttack(player, strongest, conditions[i].kind - 0x3B)) {
                total = total + score;
            }
            break;
        case 0x3E:
            if (bestSlot == 0 && !CanUseAttack(player, strongest, 4)) {
                total = total + score;
            }
            break;
        case 0x3F:
            if (bestSlot == 0) {
                total += score * (MFx32(1) - MFx32(1) * MFx32((int)strongest->GetUnkE4()) /
                                                       MFx32((int)strongest->GetUnkE8()));
            }
            break;
        case 0x40:
            if (player->AllCreaturesShareDefFlag()) {
                total += score * (MFx32(1) - MFx32(1) * MFx32((int)creature->GetUnkE4()) /
                                                       MFx32((int)creature->GetUnkE8()));
            }
            break;
        case 0x41:
            total += score * (MFx32(1) - MFx32(1) * MFx32((int)creature->GetUnkE4()) /
                                                   MFx32((int)creature->GetUnkE8()));
            break;
        case 0x42:
            if ((support1 != NULL && (int)support1->GetUnkE4() > (int)creature->GetUnkE4()) ||
                (support2 != NULL && (int)support2->GetUnkE4() > (int)creature->GetUnkE4())) {
                total = total + score;
            }
            break;
        case 3:
            if (player->GetUnk128() < opponent->GetUnk128()) {
                total = total + score;
            }
            break;
        case 4:
            if (player->GetUnk128() > opponent->GetUnk128()) {
                total = total + score;
            }
            break;
        case 0x43: {
            DinoBattleModelCreature *leader = player->GetCreatureInSlot(0);
            if (leader != NULL && func_ov000_0216c83c(leader->GetUnkB8())) {
                total = total + score;
            }
            break;
        }
        case 0x44: {
            int sum = 0;
            for (int s = 0; s < 2; s++) {
                DinoBattleModelCreature *own = s ? support1 : support2;
                DinoBattleModelCreature *other = player->GetOpponent()->FindCreature(s ? 2 : 1);
                if (own != NULL && (own->GetUnk108() & 1)) {
                    const DinoBattleCreatureDef *def = own->GetDef();
                    const s8 *effect = DinoOffsetPtr<s8>(def, def->mUnk54);
                    sum += effect[0] + effect[2];
                }
                if (other != NULL && (other->GetUnk108() & 2)) {
                    const DinoBattleCreatureDef *def = other->GetDef();
                    const s8 *effect = DinoOffsetPtr<s8>(def, def->mUnk58);
                    sum += effect[0] + effect[2];
                }
            }
            if (sum <= -50) {
                total = total + score;
            }
            break;
        }
        case 0x45: {
            int sum = 0;
            for (int s = 0; s < 2; s++) {
                DinoBattleModelCreature *own = s ? support1 : support2;
                DinoBattleModelCreature *other = player->GetOpponent()->FindCreature(s ? 2 : 1);
                if (own != NULL && (own->GetUnk108() & 1)) {
                    const DinoBattleCreatureDef *def = own->GetDef();
                    const s8 *effect = DinoOffsetPtr<s8>(def, def->mUnk54);
                    sum += effect[1] + effect[3];
                }
                if (other != NULL && (other->GetUnk108() & 2)) {
                    const DinoBattleCreatureDef *def = other->GetDef();
                    const s8 *effect = DinoOffsetPtr<s8>(def, def->mUnk58);
                    sum += effect[1] + effect[3];
                }
            }
            if (sum <= -50) {
                total = total + score;
            }
            break;
        }
        }
    }
    return total;
}

// NONMATCHING: as in ScoreCreatureConditions, the game puts each compound score's copy above the total's.
MFx32 DinoBattleCpuController::ScoreAttack(DinoBattleModelPlayer *player, DinoBattleModelCreature *creature,
                                           DinoBattleFile file, u32 attack) {
    if (!file.mArchive.IsValid()) {
        return MFx32(0);
    }
    const DinoAttackDef *def = creature->GetAttackDef(attack);
    if (def == NULL) {
        return MFx32(0);
    }
    u16 cost = *DinoOffsetPtr<u16>(def, def->cost);
    if (cost > player->GetUnk128()) {
        return MFx32(0);
    }
    if (!player->AnyCreatureUnkD244() || !creature->IsUnkD244()) {
        return MFx32(0);
    }
    if (creature->GetStatusUnk20() & (1 << attack)) {
        return MFx32(0);
    }
    CollectTargets(player, creature, attack);
    if (mTargets.size() == 0) {
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
            total += score * player->GetRandom()->NextFraction();
            break;
        case 5:
            for (u32 t = 0; t < mTargets.size(); t++) {
                if (EstimateDamage(creature, mTargets[t], attack) >= (int)mTargets[t]->GetUnkE4()) {
                    total = total + score;
                    break;
                }
            }
            break;
        case 6:
            if (creature->CompareUnkED0C(opponent->FindCreature(0)) > 0) {
                total = total + score;
            }
            break;
        case 2:
            if (opponent->FindCreature(3) != NULL) {
                total = total + score;
            }
            break;
        case 7:
            total += score * MFx32((int)creature->GetUnkE4()) / MFx32((int)creature->GetUnkE8());
            break;
        case 8:
            total += score * (MFx32(1) - MFx32(1) * MFx32((int)creature->GetUnkE4()) /
                                                   MFx32((int)creature->GetUnkE8()));
            break;
        case 3:
            if (player->GetUnk128() < opponent->GetUnk128()) {
                total = total + score;
            }
            break;
        case 4:
            if (player->GetUnk128() > opponent->GetUnk128()) {
                total = total + score;
            }
            break;
        case 0x9:
            if (!LeaderHasStatus(player, 8)) {
                total = total + score;
            }
            break;
        case 0xA:
            if (!LeaderHasStatus(player, 9)) {
                total = total + score;
            }
            break;
        case 0xB:
            if (!LeaderHasStatus(player, 10)) {
                total = total + score;
            }
            break;
        case 0xC:
            if (!LeaderHasStatus(player, 11)) {
                total = total + score;
            }
            break;
        case 0xD:
            if (!SupportHasStatus(player, 8)) {
                total = total + score;
            }
            break;
        case 0xE:
            if (!SupportHasStatus(player, 9)) {
                total = total + score;
            }
            break;
        case 0xF:
            if (!SupportHasStatus(player, 10)) {
                total = total + score;
            }
            break;
        case 0x10:
            if (!SupportHasStatus(player, 11)) {
                total = total + score;
            }
            break;
        case 0x11:
            if (!(creature->GetStatus()->flags & 0x100)) {
                total = total + score;
            }
            break;
        case 0x12:
            if (creature->GetUnkB8() == 0) {
                total = total + score;
            }
            break;
        case 0x13:
            if (func_ov000_0216c83c(creature->GetUnkB8())) {
                total = total + score;
            }
            break;
        case 0x14:
            for (u32 c = 0; c < player->GetCreatureCount(); c++) {
                if (player->GetCreature(c) != creature &&
                    (int)player->GetCreature(c)->GetUnkE4() <= (int)player->GetCreature(c)->GetUnkE8() / 2) {
                    total = total + score;
                    break;
                }
            }
            break;
        case 0x15:
            for (u32 c = 0; c < player->GetCreatureCount(); c++) {
                if (player->GetCreature(c) != creature &&
                    (int)player->GetCreature(c)->GetUnkE4() <= (int)player->GetCreature(c)->GetUnkE8() / 4) {
                    total = total + score;
                    break;
                }
            }
            break;
        case 0x16:
            if (*DinoOffsetPtr<u8>(def, def->unk14) == 2) {
                u16 cost = *DinoOffsetPtr<u16>(def, def->cost);
                if (cost <= player->GetUnk128()) {
                    total = total + score;
                }
            }
            break;
        case 0x17:
            total += score * MFx32(opponent->GetUnk128()) / MFx32((int)opponent->GetUnk14C());
            break;
        case 0x18:
            if (opponent->FindCreature(0) == opponent->GetCreatureInSlot(0)) {
                total = total + score;
            }
            break;
        case 0x19:
            if (LeaderHasStatus(player, 2)) {
                total = total + score;
            }
            break;
        case 0x1A:
            if (LeaderHasStatus(player, 4)) {
                total = total + score;
            }
            break;
        case 0x1B:
            if (LeaderHasStatus(player, 6)) {
                total = total + score;
            }
            break;
        case 0x1C:
            if (LeaderHasStatus(player, 7)) {
                total = total + score;
            }
            break;
        case 0x1D:
            if (SupportHasStatus(player, 2)) {
                total = total + score;
            }
            break;
        case 0x1E:
            if (SupportHasStatus(player, 4)) {
                total = total + score;
            }
            break;
        case 0x1F:
            if (SupportHasStatus(player, 6)) {
                total = total + score;
            }
            break;
        case 0x20:
            if (SupportHasStatus(player, 7)) {
                total = total + score;
            }
            break;
        case 0x21:
            if (LeaderHasStatus(opponent, 2)) {
                total = total + score;
            }
            break;
        case 0x22:
            if (LeaderHasStatus(opponent, 4)) {
                total = total + score;
            }
            break;
        case 0x23:
            if (LeaderHasStatus(opponent, 6)) {
                total = total + score;
            }
            break;
        case 0x24:
            if (LeaderHasStatus(opponent, 7)) {
                total = total + score;
            }
            break;
        case 0x25:
            if (LeaderHasStatus(opponent, 3)) {
                total = total + score;
            }
            break;
        case 0x26:
            if (LeaderHasStatus(opponent, 5)) {
                total = total + score;
            }
            break;
        case 0x27:
            if (!LeaderHasStatus(opponent, 2)) {
                total = total + score;
            }
            break;
        case 0x28:
            if (!LeaderHasStatus(opponent, 4)) {
                total = total + score;
            }
            break;
        case 0x29:
            if (!LeaderHasStatus(opponent, 6)) {
                total = total + score;
            }
            break;
        case 0x2A:
            if (!LeaderHasStatus(opponent, 7)) {
                total = total + score;
            }
            break;
        case 0x2B:
            if (!LeaderHasStatus(opponent, 3)) {
                total = total + score;
            }
            break;
        case 0x2C:
            if (!LeaderHasStatus(opponent, 5)) {
                total = total + score;
            }
            break;
        case 0x2D: {
            DinoPlayerData data;
            gDinoBackup->GetPlayer(&data, FALSE);
            if (data.unk1c_7 == 1) {
                total = total + score;
            }
            break;
        }
        case 0x61: {
            DinoPlayerData data;
            gDinoBackup->GetPlayer(&data, FALSE);
            if (data.unk1c_7 >= 1 && data.unk1c_7 <= 3) {
                total = total + score;
            }
            break;
        }
        case 0x2F:
            if (opponent->GetCreatureCount() > 1 && !LeaderHasStatus(opponent, 2) && !LeaderHasStatus(opponent, 4) &&
                !LeaderHasStatus(opponent, 6) && !LeaderHasStatus(opponent, 7) && !LeaderHasStatus(opponent, 5) &&
                !LeaderHasStatus(opponent, 3)) {
                total += score * player->GetRandom()->NextFraction();
            }
            break;
        }
    }
    return total;
}
// NONMATCHING: as in ScoreCreatureConditions, the game puts each compound score's copy above the total's.
MFx32 DinoBattleCpuController::ScoreTarget(DinoBattleModelPlayer *player, DinoBattleModelCreature *creature,
                                           DinoBattleModelCreature *target, DinoBattleFile file, u32 attack) {
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
            total += score * player->GetRandom()->NextFraction();
            break;
        case 0x62:
            if ((int)target->GetUnkE4() <= EstimateDamage(creature, target, attack)) {
                total = total + score;
            }
            break;
        case 2:
            if (opponent->FindCreature(3) != NULL) {
                total = total + score;
            }
            break;
        case 0x63:
            if (target == opponent->GetCreatureInSlot(0)) {
                total = total + score;
            }
            break;
        case 0x64:
            if (target == opponent->GetCreatureInSlot(1)) {
                total = total + score;
            }
            break;
        case 0x65:
            if (target == opponent->GetCreatureInSlot(2)) {
                total = total + score;
            }
            break;
        case 0x66:
            if (HasAttackUnk10(target, 2)) {
                total = total + score;
            }
            break;
        case 0x67:
            if (HasAttackUnk10(target, 3)) {
                total = total + score;
            }
            break;
        case 0x68:
            if (HasAttackUnk10(target, 4)) {
                total = total + score;
            }
            break;
        case 0x69:
            if (HasAttackUnk10(target, 5)) {
                total = total + score;
            }
            break;
        case 0x6A:
            if (HasAttackUnk10(target, 6)) {
                total = total + score;
            }
            break;
        case 0x6B:
            if (HasAttackUnk10(target, 7)) {
                total = total + score;
            }
            break;
        case 0x6C:
            if (HasAttackUnk10(target, 8)) {
                total = total + score;
            }
            break;
        case 0x6D:
            if (HasAttackUnk10(target, 9)) {
                total = total + score;
            }
            break;
        case 0x6E:
            if (HasAttackUnk10(target, 0xA)) {
                total = total + score;
            }
            break;
        case 0x6F:
            if (HasAttackUnk10(target, 0xB)) {
                total = total + score;
            }
            break;
        case 0x70:
            if (HasAttackUnk14(target, 2)) {
                total = total + score;
            }
            break;
        case 0x71:
            if (HasAttackUnk14(target, 4)) {
                total = total + score;
            }
            break;
        case 0x72:
            if (HasAttackUnk14(target, 5)) {
                total = total + score;
            }
            break;
        case 0x73:
            if (HasAttackUnk14(target, 6)) {
                total = total + score;
            }
            break;
        case 0x74:
            if (HasAttackUnk14(target, 7)) {
                total = total + score;
            }
            break;
        case 0x75:
            if (HasAttackUnk14(target, 8)) {
                total = total + score;
            }
            break;
        case 0x76:
            if (HasAttackUnk14(target, 9)) {
                total = total + score;
            }
            break;
        case 0x77:
            if (HasAttackUnk14(target, 0xA)) {
                total = total + score;
            }
            break;
        case 0x78:
            if (HasAttackUnk14(target, 0xB)) {
                total = total + score;
            }
            break;
        case 0x79:
            if (HasAttackUnk14(target, 0xC)) {
                total = total + score;
            }
            break;
        case 0x7A:
            if (HasAttackUnk14(target, 0xD)) {
                total = total + score;
            }
            break;
        case 0x7B:
            if (HasAttackUnk14(target, 0xE)) {
                total = total + score;
            }
            break;
        case 0x7C:
            if (HasAttackUnk14(target, 0x10)) {
                total = total + score;
            }
            break;
        case 0x7D:
            if (HasAttackUnk14(target, 0xF)) {
                total = total + score;
            }
            break;
        case 0x7E:
            if (target->GetDefUnk61() == 4) {
                total = total + score;
            }
            break;
        case 0x7F:
            if (target->GetDefUnk61() == 5) {
                total = total + score;
            }
            break;
        case 0x80:
            if (target->GetDefUnk61() == 6) {
                total = total + score;
            }
            break;
        case 0x81:
            if (target->GetDefUnk61() == 7) {
                total = total + score;
            }
            break;
        case 0x82:
            if (target->GetDefUnk61() == 8) {
                total = total + score;
            }
            break;
        case 0x83:
            if (target->GetDefUnk61() == 9) {
                total = total + score;
            }
            break;
        case 0x84: {
            const DinoBattleCreatureDef *def = target->GetDef();
            const s8 *effect = DinoOffsetPtr<s8>(def, def->mUnk58);
            MFx32 value = score * MFx32(effect[0] + effect[1] + effect[2] + effect[3]) / MFx32(-400);
            if (value > MFx32(0)) {
                total = total + value;
            }
            break;
        }
        case 0x85: {
            const DinoBattleCreatureDef *def = target->GetDef();
            const s8 *effect = DinoOffsetPtr<s8>(def, def->mUnk54);
            MFx32 value = score * MFx32(effect[0] + effect[1] + effect[2] + effect[3]) / MFx32(400);
            if (value > MFx32(0)) {
                total = total + value;
            }
            break;
        }
        case 0x86: {
            DinoBattleModelPlayer *owner = target->mPlayer;
            int lowest = 0x7FFFFFFF;
            DinoBattleModelCreature *weakest = NULL;
            for (u32 c = 0; c < owner->GetCreatureCount(); c++) {
                if ((int)owner->GetCreature(c)->GetUnkE4() < lowest) {
                    weakest = owner->GetCreature(c);
                    lowest = weakest->GetUnkE4();
                }
            }
            if (weakest == target) {
                total = total + score;
            }
            break;
        }
        case 0x87:
            total += score * (MFx32(1) - MFx32(1) * MFx32((int)target->GetUnkE4()) /
                                                   MFx32((int)target->GetUnkE8()));
            break;
        case 0x88:
            if ((int)target->GetUnkE4() <= (int)target->GetUnkE8() / 2) {
                total = total + score;
            }
            break;
        case 0x89:
            if (target == creature) {
                total = total + score;
            }
            break;
        case 0x8A:
            if (target != creature) {
                total = total + score;
            }
            break;
        }
    }
    return total;
}


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
            total += score * player->GetRandom()->NextFraction();
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
            total += score * player->GetRandom()->NextFraction();
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
struct DinoBattleCpuCandidate : public DinoBattleCpuEntry {
    DinoBattleCpuCandidate(u32 k) { kind = k; }
    DinoBattleCpuCandidate(u32 k, DinoBattleModelCreature *c, u32 a) {
        kind = k;
        creature = c;
        attack = a;
    }
    DinoBattleCpuCandidate(u32 k, DinoBattleModelCreature *c, DinoBattleModelCreature *t, u32 a) {
        kind = k;
        creature = c;
        target = t;
        attack = a;
    }
    ~DinoBattleCpuCandidate() {}
};
typedef std::multimap<MFx32, DinoBattleCpuCandidate> DinoBattleCpuCandidates;
typedef std::__red_black_tree<MFx32, DinoBattleCpuCandidates::value_type,
                              std::__select_first<DinoBattleCpuCandidates::value_type>, std::less<MFx32>,
                              std::allocator<DinoBattleCpuCandidates::value_type> >
    DinoBattleCpuCandidateTree;

extern "C" u32 func_ov000_0216d2cc(DinoBattleModelCreature *creature, BOOL check);
extern "C" MTask *func_ov000_0218ec8c(u32 tutorial, int kind, BOOL *proceed);
extern "C" const u32 data_0208a1a0, data_0208a1a4;  // the CPU's wait before its first move, and before the others

// NONMATCHING (98.9%): the game keeps each target's score in a stack slot (ours in a register), which shifts the
// frame by a word.
void DinoBattleCpuController::DecideAction(u32 first) {
    mSerial.AddTail(new MTimer(first ? data_0208a1a0 : data_0208a1a4));
    DinoBattleModel *model = (DinoBattleModel *)mUnkAC;
    if (((DinoBattleModelField *)model->GetUnk10C())->mUnkD0 & 0x20) {
        ((DinoBattleModelField *)((DinoBattleModel *)mUnkAC)->GetUnk10C())->mUnkD0 &= ~0x20;
        mSerial.AddTail(((DinoBattleModel *)mUnkAC)->CreatePostTask(new DinoBattleModelTurnChangeSignal()));
        return;
    }
    if (!gDinoResource->GetBattleAdjustDefs()->cpuActs) {
        mSerial.AddTail(((DinoBattleModel *)mUnkAC)->CreatePostTask(new DinoBattleModelTurnChangeSignal()));
        return;
    }
    DinoBattleCpuCandidates candidates;
    DinoBattleModelPlayer *player = ((DinoBattleModel *)mUnkAC)->GetPlayer(mUnkA8);
    if (player->GetOpponent()->FindCreature(0) == NULL) {
        return;
    }
    for (u32 i = 0; i < player->GetCreatureCount(); i++) {
        DinoBattleModelCreature *creature = player->GetCreature(i);
        DinoBattleFileGroup &group = mAi.mGroups[creature->GetUnkBC()];
        MFx32 end;
        end = ScoreEscape(player, creature, group.mDataA);
        if (end != MFx32(0)) {
            candidates.insert(DinoBattleCpuCandidates::value_type(end, DinoBattleCpuCandidate(1)));
        }
        MFx32 swap;
        swap = ScoreCreatureConditions(player, creature, group.mDataB);
        if (swap != MFx32(0)) {
            candidates.insert(DinoBattleCpuCandidates::value_type(swap, DinoBattleCpuCandidate(2)));
        }
        u32 usable = func_ov000_0216d2cc(creature, TRUE);
        u32 count = creature->CountAttacks();
        for (u32 a = 0; a <= count; a++) {
            if (!(usable & (1 << a))) {
                continue;
            }
            int set = 0;
            if (a >= count) {
                set = 4;
            } else {
                creature->GetAttack(a, &set);
            }
            MFx32 attack;
            attack = ScoreAttack(player, creature, group.mSets[set], a);
            if (attack != MFx32(0)) {
                if (a < count || player->AllCreaturesShareDefFlag()) {
                    candidates.insert(
                        DinoBattleCpuCandidates::value_type(attack, DinoBattleCpuCandidate(0, creature, a)));
                }
            }
        }
    }
    if (candidates.empty()) {
        mSerial.AddTail(((DinoBattleModel *)mUnkAC)->CreatePostTask(new DinoBattleModelTurnChangeSignal()));
        return;
    }
    DinoBattleCpuCandidates::iterator last = candidates.end();
    --last;
    std::pair<DinoBattleCpuCandidates::iterator, DinoBattleCpuCandidates::iterator> range =
        candidates.equal_range(last->first);
    std::vector<DinoBattleCpuEntry> best;
    for (DinoBattleCpuCandidates::iterator it = range.first; it != range.second; ++it) {
        best.push_back(it->second);
    }
    DinoBattleCpuEntry pick = best[player->GetRandom()->Next(best.size())];
    switch (pick.kind) {
    case 2:
        mSerial.AddTail(((DinoBattleModel *)mUnkAC)->CreatePostTask(new DinoBattleModelTurnChangeSignal()));
        return;
    case 1: {
        DinoBattleModelCreature *leader = player->FindCreature(0);
        DinoBattleModelCreature *support = player->FindCreature(1);
        DinoBattleModelCreature *other = player->FindCreature(2);
        if (support != NULL && other == NULL) {
        } else if (support == NULL && other != NULL) {
            support = other;
        } else {
            MFx32 a = ScoreConditions(player, support, other, mAi.mFiles[0]);
            MFx32 b = ScoreConditions(player, other, support, mAi.mFiles[1]);
            if (a > b) {
            } else if (a < b) {
                support = other;
            } else if (player->GetRandom()->Next(2) != 0) {
                support = other;
            }
        }
        mSerial.AddTail(((DinoBattleModel *)mUnkAC)
                            ->CreatePostTask(new DinoBattleModelGoEscapeAreaRequest(
                                (u32)(MMessageObserver *)this, mUnkA8, leader->GetUnkB4(), support->GetUnkB4())));
        return;
    }
    case 0: {
        DinoBattleModelCreature *attacker = pick.creature;
        u32 attack = pick.attack;
        CollectTargets(player, attacker, attack);
        candidates.clear();
        for (u32 t = 0; t < mTargets.size(); t++) {
            DinoBattleModelCreature *target = mTargets[t];
            MFx32 score;
            if (target->GetUnkB8() == 3) {
                score = MFx32(0);
            } else {
                u32 pos = target->GetUnkB8();
                if (target->mPlayer == player) {
                    pos += 3;
                }
                DinoBattleFileGroup &group = mAi.mGroups[attacker->GetUnkBC()];
                int set = attack;
                if (attack >= attacker->CountAttacks()) {
                    set = 4;
                } else {
                    attacker->GetAttack(attack, &set);
                }
                score = ScoreTarget(player, attacker, target, group.mSets[set].mFiles[pos], attack);
            }
            DinoBattleCpuCandidate candidate(0, attacker, attack);
            candidate.target = mTargets[t];
            candidates.insert(DinoBattleCpuCandidates::value_type(score, candidate));
        }
        DinoBattleCpuCandidates::iterator top = candidates.end();
        --top;
        range = candidates.equal_range(top->first);
        best.clear();
        for (DinoBattleCpuCandidates::iterator it = range.first; it != range.second; ++it) {
            best.push_back(it->second);
        }
        DinoBattleCpuEntry chosen = best[player->GetRandom()->Next(best.size())];
        MTask *tutorial = func_ov000_0218ec8c((u32)((DinoBattleModel *)mUnkAC)->mTutorial, 0x1B, 0);
        if (tutorial != NULL) {
            mSerial.AddTail(tutorial);
        }
        mSerial.AddTail(((DinoBattleModel *)mUnkAC)
                            ->CreatePostTask(new DinoBattleModelCreatureAbilityRequest(
                                (u32)(MMessageObserver *)this, mUnkA8, chosen.creature->GetUnkB4(),
                                chosen.target->GetUnkB4(), chosen.attack, 3)));
        return;
    }
    default:
        OS_Terminate();
        return;
    }
}

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
