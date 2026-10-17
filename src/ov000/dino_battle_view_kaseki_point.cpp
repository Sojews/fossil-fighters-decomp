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
#include "m/pal.h"
#include "game/dino_battle_view2.h"

DinoBattleViewKasekiPoint::~DinoBattleViewKasekiPoint() {
}

WaitKPCountTask::~WaitKPCountTask() {
}

DinoBattleViewPlayer::~DinoBattleViewPlayer() {
}

#pragma auto_inline off

MPoint GetKasekiPointPos(u32 side) {
    const DinoBattleAdjustDefs *defs = gDinoResource->GetBattleAdjustDefs();
    MPoint pos;
    pos.x = defs->kasekiPointX;
    pos.y = defs->kasekiPointY;
    if (side == 1) {
        pos.x = -pos.x;
    }
    return pos;
}

KasekiPointPalette::KasekiPointPalette(const u16 *from, const u16 *to) {
    Init(from, to);
}

void KasekiPointPalette::Init(const u16 *from, const u16 *to) {
    for (int row = 0; row < 32; row++) {
        for (int i = 1; i < 16; i++) {
            u16 a = from[i];
            u16 b = to[i];
            u16 color = 0;
            int k;
            int shift;
            for (k = 0, shift = 0; k < 3; k++) {
                u16 ca = (a & (0x1F << shift)) >> shift;
                u16 cb = (b & (0x1F << shift)) >> shift;
                color |= (ca + (cb - ca) * row / 32) << shift;
                shift += 5;
            }
            mRows[row][i] = color;
        }
    }
}

extern "C" const u32 data_ov000_0218fa14[][2];
extern "C" const u32 data_ov000_0218fa1c[2];
extern "C" const u32 data_ov000_0218fa3c[2];

const u16 *KasekiPointPalette::GetRow(int index) {
    if (index < 0) {
        index = 0;
    }
    if (index > 31) {
        index = 31;
    }
    return mRows[index];
}

void DinoBattleViewKasekiPoint::UpdateAnime(u32 player) {
    int done = mUnk7C == mUnk78 ? 1 : 0;
    int side = mUnk98 != 0 ? 1 : 0;
    int anime = mUnk98 == player ? 2 : 3;
    int current = mUnk84->GetAnime();
    if (anime == 2) {
        if (current == data_ov000_0218fa1c[side] || current == data_ov000_0218fa3c[side]) {
            anime = 0;
        }
    }
    if (done) {
        anime += 4;
    }
    mUnk84->Play(data_ov000_0218fa14[anime][side], 0);
}

extern "C" void func_ov000_0217df4c(void *obj, int delta, u32 side);

void DinoBattleViewKasekiPoint::SetValue(int value) {
    func_ov000_0217df4c(mView->mUnkE0->mUnk140, value - mValue, mUnk98);
    mValue = value;
}

void DinoBattleViewKasekiPoint::LoadPalette(int index) {
    mPalBlock->Load(mPalette->GetRow(index), 0, -1);
}

DinoBattleViewKasekiPoint *DinoBattleViewPlayer::GetKasekiPoint() {
    return mKasekiPoint;
}

// Runs the task for 50 frames.
void DinoBattleViewPlayer::AddTimedTask(MTask *task, MTask *old) {
    delete old;
    MTask *exit = MTaskExitTransfer::Create(task);
    MTimer *timer = new MTimer(50);
    MSerialTaskSet *set = new MSerialTaskSet(TRUE);
    set->AddTail(timer);
    set->AddTail(exit);
    MTaskSet *parallel = &mParallel;
    parallel->AddTail(task);
    parallel->AddTail(set);
}

BOOL DinoBattleViewKasekiPoint::IsCounting() {
    if (mValue != mUnk7C) {
        return TRUE;
    }
    return FALSE;
}

DinoBattleViewPlayer::DinoBattleViewPlayer(u32 index, u32 side, u32 unkB0, DinoBattleView *view, u8 unk114)
    : MProcess(NULL), mUnkA8(index), mUnkAC(side), mUnkB0(unkB0), mView(view) {
    mMotion = NULL;
    mUnk114 = unk114;
    mKasekiPoint = new DinoBattleViewKasekiPoint(mView, view->mUnk10C, side, mUnkA8);
    mParallel.AddTail(mKasekiPoint);
    mUnk108 = new DinoBattleViewAbilityIcon(view, mUnkB0, GetKasekiPointPos(side));
    mParallel.AddTail(mUnk108);
}

#pragma auto_inline on

void DinoBattleViewPlayer::Idle() {
    Dispatch();
}

#pragma auto_inline off

DinoBattleViewAbilityIcon *DinoBattleViewPlayer::GetUnk108() {
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
            mUnk108->Update(0, sums);
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
    mUnk108->Update(mask, sums);
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

void WaitKPCountTask::Execute() {
    if (((DinoBattleViewKasekiPoint *)mOwner)->IsCounting() == 0) {
        Exit();
    }
}
