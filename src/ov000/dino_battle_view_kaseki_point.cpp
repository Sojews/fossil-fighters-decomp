// DinoBattleViewKasekiPoint, WaitKPCountTask.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_battle_view_kaseki_point_menu.h"
#include "game/dino_battle_view_kaseki_point.h"
#include "game/wait_kp_count_task.h"
#include "game/dino_battle_view_creature.h"
#include "game/dino_battle_view_class.h"
#include "m/motion.h"
#include "game/dino_resource.h"

DinoBattleViewKasekiPoint::~DinoBattleViewKasekiPoint() {
}

WaitKPCountTask::~WaitKPCountTask() {
}

DinoBattleViewPlayer::~DinoBattleViewPlayer() {
}

void DinoBattleViewPlayer::Idle() {
    Dispatch();
}

#pragma auto_inline off

u32 DinoBattleViewPlayer::GetUnk108() {
    return mUnk108;
}

u32 DinoBattleViewPlayer::GetUnkAC() {
    return mUnkAC;
}

std::string DinoBattleViewPlayer::GetUnkC0() {
    return mUnkC0;
}

void DinoBattleViewPlayer::SetMotions(MMotion *motion, MTask *task) {
    if (mMotion != NULL) {
        delete motion;
        delete task;
        return;
    }
    mMotion = motion;
    mMotionTask = task;
    mMotion->Play(2, 0);
    MTask *t = mMotionTask;
    MTaskSet *set = &mParallel;
    set->AddTail(mMotion);
    set->AddTail(t);
}

void DinoBattleViewPlayer::TakeMotions(MMotion **motion, MTask **task) {
    if (mMotion != NULL) {
        mParallel.Remove(mMotion);
        mParallel.Remove(mMotionTask);
        *motion = mMotion;
        *task = mMotionTask;
        mMotion = NULL;
    }
}

extern "C" void func_ov000_021817fc(u32 obj, u32 mask, int *sums);

void DinoBattleViewPlayer::UpdateUnk108() {
    if (mUnk108 == 0) {
        return;
    }
    mOpponent = mView->GetUnkE4(mUnkA8 ^ 1);
    int sums[4] = {0, 0, 0, 0};
    u32 mask = 0;
    DinoBattleViewCreature *creature = FindCreatureByB4(0);
    if (creature != NULL) {
        if (creature->GetDefUnk61() == 8) {
            func_ov000_021817fc(mUnk108, 0, sums);
            return;
        }
        for (int side = 0; side < 2; side++) {
            DinoBattleViewPlayer *player = side ? mOpponent : this;
            for (int i = 0; i < player->GetCreatureCount(); i++) {
                DinoBattleViewCreature *member = player->GetCreature(i);
                AccumulateCreature(&mask, sums, member, side != 0);
            }
        }
    }
    func_ov000_021817fc(mUnk108, mask, sums);
}

void DinoBattleViewPlayer::AccumulateCreature(u32 *mask, int *sums, DinoBattleViewCreature *creature, bool opponent) {
    if (creature->GetUnkB4() - 1 > 1) {
        return;
    }
    int bit = opponent ? 2 : 1;
    if (!(creature->GetUnk118() & bit)) {
        return;
    }
    const DinoCreatureDef *def = gDinoResource->GetCreatureDef(creature->GetUnkA8());
    const s8 *src;
    if (opponent) {
        src = def->unk58 == 0 ? NULL : (const s8 *)def + def->unk58;
    } else {
        src = def->unk54 == 0 ? NULL : (const s8 *)def + def->unk54;
    }
    s8 values[4];
    values[0] = src[0];
    values[1] = src[1];
    values[2] = src[2];
    values[3] = src[3];
    for (int i = 0; i < 4; i++) {
        if (values[i] != 0) {
            *mask |= 1 << i;
            sums[i] += values[i];
        }
    }
}

int DinoBattleViewPlayer::GetCreatureCount() {
    return mCreatures.size();
}

DinoBattleViewCreature *DinoBattleViewPlayer::GetCreature(int index) {
    return mCreatures[index];
}

DinoBattleViewCreature *DinoBattleViewPlayer::FindCreatureByB4(u32 id) {
    int count = GetCreatureCount();
    for (int i = 0; i < count; i++) {
        DinoBattleViewCreature *creature = GetCreature(i);
        if (id == creature->GetUnkB4()) {
            return creature;
        }
    }
    return NULL;
}

DinoBattleViewCreature *DinoBattleViewPlayer::FindCreatureByAC(u32 id) {
    for (std::vector<DinoBattleViewCreature *>::iterator it = mCreatures.begin(); it != mCreatures.end(); it++) {
        if (id == (*it)->GetUnkAC()) {
            return *it;
        }
    }
    return NULL;
}

DinoBattleViewPlayer *DinoBattleViewPlayer::GetOpponent() {
    return mView->GetUnkE4(mUnkA8 ^ 1);
}

BOOL DinoBattleViewPlayer::HasCommonCreatureFlags() {
    int count = GetCreatureCount();
    if (count < 3) {
        return FALSE;
    }
    u32 flags = GetCreature(0)->mDef->mUnk24;
    for (int i = 1; i < count; i++) {
        flags &= GetCreature(i)->mDef->mUnk24;
    }
    if (flags != 0) {
        return TRUE;
    }
    return FALSE;
}

#pragma auto_inline on

extern "C" int func_ov000_0215ddf8(void *owner);

void WaitKPCountTask::Execute() {
    if (func_ov000_0215ddf8(mOwner) == 0) {
        Exit();
    }
}
