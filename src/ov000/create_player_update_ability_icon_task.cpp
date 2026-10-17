// CreatePlayerUpdateAbilityIconTask.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_message.h"
#include "game/create_player_update_ability_icon_task.h"
#include "m/seed_generator.h"
#include "nitro/mi.h"
#include "types.h"
#include "game/dino_battle_user_controller.h"
#include "game/set_status_effect_task.h"


extern "C" void func_ov000_0216cc64(void *, u32, u8);

extern "C" void func_ov000_0216cc20(void *, u32);

extern "C" void func_ov000_021631c0(void *, u32);

extern "C" void func_ov000_02163258(void *, u32, u32);

extern "C" void func_ov000_02163408(void *, u8, u32);

extern "C" void func_ov000_02163464(void *, u32);

CreatePlayerUpdateAbilityIconTask::~CreatePlayerUpdateAbilityIconTask() {
}

UpdateAllCreatureTask::~UpdateAllCreatureTask() {
}

MoveCreatureFromEscape::~MoveCreatureFromEscape() {
}

DinoBattleModelCreature::~DinoBattleModelCreature() {
}

ModelCretureAddLifeTask::~ModelCretureAddLifeTask() {
}

ResetStatusEffectSleepTask::~ResetStatusEffectSleepTask() {
}

AddLifePointTask::~AddLifePointTask() {
}

ExecSpecialAttackTask::~ExecSpecialAttackTask() {
}

SendModelCreatureAbilityResultTask::~SendModelCreatureAbilityResultTask() {
}

SetLifePointTask::~SetLifePointTask() {
}

PlayerUpdateTask::~PlayerUpdateTask() {
}

DinoCreatureUpdateStateIconTask::~DinoCreatureUpdateStateIconTask() {
}

UpdateAbilityIconTask::~UpdateAbilityIconTask() {
}

SetKasekiPointTask::~SetKasekiPointTask() {
}

ExecCreatureEndTask::~ExecCreatureEndTask() {
}

MoveToHeatAreaAnyCreature::~MoveToHeatAreaAnyCreature() {
}

AddKasekiPointCreatureDie::~AddKasekiPointCreatureDie() {
}

DinoBattleModelCreature::AddLifePointTask::~AddLifePointTask() {
}

void SetKasekiPointTask::Execute() {
    func_ov000_02163464((void *)mUnk10, mUnk14);
    Exit();
}

void UpdateAbilityIconTask::Execute() {
    func_ov000_02163408((void *)mUnk10, mUnk18, mUnk14);
    Exit();
}

void DinoCreatureUpdateStateIconTask::Execute() {
    func_ov000_02163258((void *)mUnk10, mUnk14, mUnk18);
    Exit();
}

void PlayerUpdateTask::Execute() {
    func_ov000_021631c0((void *)mUnk10, mUnk14);
    Exit();
}

void SetLifePointTask::Execute() {
    func_ov000_0216cc20((void *)mUnk10, mUnk14);
    Exit();
}

void AddLifePointTask::Execute() {
    func_ov000_0216cc64((void *)mUnk10, mUnk14, mUnk18);
    Exit();
}

void ResetStatusEffectSleepTask::Execute() {
    ((DinoBattleModelCreature *)mUnk10)->ClearStatus8();
    Exit();
}

void CreatePlayerUpdateAbilityIconTask::Execute() {
    func_ov000_02163408((void *)mUnk18, mUnk10, mUnk14);
    Exit();
}

DinoBattleUserController::~DinoBattleUserController() {
}

PlayerThinkTimeCountTask::~PlayerThinkTimeCountTask() {
}

void ModelCretureAddLifeTask::Execute() {
    func_ov000_0216cc64((void *)mUnk10, mUnk14, 0);
    Exit();
}

extern "C" MTask *func_ov000_02166620(u32 arg0, u32 arg1);

void ExecCreatureEndTask::Idle() {
    MTask *exit = MTaskExitTransfer::Create(this);
    MTask *task = func_ov000_02166620(mUnk74, mUnk78);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(task);
    serial->AddTail(exit);
}

void PlayerThinkTimeCountTask::Execute() {
    if (mActive && mPlayer != NULL) {
        mPlayer->mModel->mThinkTime = ++mCount;
    }
}

void AddKasekiPointCreatureDie::Idle() {
    u32 point = mPlayer->mKasekiPoint;
    u32 add = mPlayer->GetUnk128();
    func_ov000_02163464(mPlayer, add + point);
    DinoSignalWait *wait = new DinoSignalWait(0x10039, NULL, NULL, 1);
    MTask *exit = MTaskExitTransfer::Create(this);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(wait);
    serial->AddTail(exit);
    func_ov000_021631c0(mPlayer, (u32)(MMessageObserver *)wait);
}

extern "C" void func_ov000_0216d3c0(void *, u8, u32);

SetStatusEffectTask::~SetStatusEffectTask() {
}

DinoCreatureUpdateTask::~DinoCreatureUpdateTask() {
}

DamagedUpdate::~DamagedUpdate() {
}

void DinoBattleModelCreature::Idle() {
    Dispatch();
}

void DinoCreatureUpdateTask::Execute() {
    func_ov000_0216d3c0((void *)mUnk10, mUnk14, mUnk18);
    Exit();
}

#pragma auto_inline off

u32 DinoBattleModelCreature::GetUnkB0() {
    return mUnkB0;
}

u32 DinoBattleModelCreature::GetUnkB4() {
    return mUnkB4;
}

u32 DinoBattleModelCreature::GetUnkB8() {
    return mUnkB8;
}

u32 DinoBattleModelCreature::GetUnkBC() {
    return mUnkBC;
}

void DinoBattleModelCreature::SetUnkB8(u32 value) {
    mUnkB8 = value;
}

u32 DinoBattleModelCreature::GetUnkC0() {
    return mUnkC0;
}

void DinoBattleModelCreature::SetUnkC0(u32 value) {
    mUnkC0 = value;
}

void DinoBattleModelCreature::ClearUnkC4() {
    mUnkC4 = 0;
}

DinoBattleCreatureStats DinoBattleModelCreature::GetStats() {
    return mStats;
}

u32 DinoBattleModelCreature::GetUnkE4() {
    return mUnkE4;
}

u32 DinoBattleModelCreature::GetUnkE8() {
    return mUnkE8;
}

u8 DinoBattleModelCreature::GetUnk174() {
    return mUnk174;
}

u32 DinoBattleModelCreature::GetUnkEC() {
    return mUnkEC;
}

u32 DinoBattleModelCreature::GetUnkF0() {
    return mUnkF0;
}

u32 DinoBattleModelCreature::GetUnkF4() {
    return mUnkF4;
}

u32 DinoBattleModelCreature::GetUnkF8() {
    return mUnkF8;
}

u32 DinoBattleModelCreature::GetUnkFC() {
    return mUnkFC;
}

u8 DinoBattleModelCreature::GetDefUnk20(BOOL second) {
    if (second) {
        return mDef->mUnk21;
    }
    return mDef->mUnk20;
}

u8 DinoBattleModelCreature::GetDefUnk61() {
    return mDef->mUnk61;
}

DinoBattleCreatureDef *DinoBattleModelCreature::GetDef() {
    return mDef;
}

void *DinoBattleModelCreature::GetUnk12C() {
    return &mStatus;
}

u8 DinoBattleModelCreature::GetDefTable(int index) {
    u8 *table = (u8 *)mDef + mDef->mUnk68;
    return table[index - 2];
}

u32 DinoBattleModelCreature::GetUnk108() {
    return mUnk108;
}

void DinoBattleModelCreature::SetUnk108(u32 value) {
    mUnk108 = value;
}

MFx32 DinoBattleModelCreature::RandomFraction() {
    return mPlayer->RandomFraction();
}

void DinoBattleModelCreature::RollUnk10C() {
    mUnk10C = mPlayer->GetRandom()->Next(5) + 1;
}

void DinoBattleModelCreature::ClearStatus8() {
    mStatus.flags &= ~8;
    mStatus.unk05 = 0;
}

void DinoBattleModelCreature::ResetStatus() {
    MI_CpuFill8(&mStatus, 0, sizeof(mStatus));
}

BOOL DinoBattleModelCreature::IsUnkD244() {
    if (mUnk174) {
        return FALSE;
    }
    if (mUnkB8 == 3) {
        return FALSE;
    }
    if (mStatus.flags & 8) {
        return FALSE;
    }
    return TRUE;
}

BOOL DinoBattleModelCreature::IsUnkD274() {
    if (mUnkB8 == 3) {
        return FALSE;
    }
    if (mStatus.flags & 0x28) {
        return FALSE;
    }
    return TRUE;
}

u32 DinoBattleModelCreature::GetStatusUnk20() {
    if (mStatus.flags & 0x10) {
        return mStatus.unk20;
    }
    return 0;
}

u32 DinoBattleModelPlayer::GetUnkB8() {
    return mUnkB8;
}

u32 DinoBattleModelPlayer::GetUnk128() {
    return mUnk128;
}

u32 DinoBattleModelPlayer::GetUnk14C() {
    return mUnk14C;
}

u8 DinoBattleModelPlayer::GetUnkC8() {
    return mUnkC8;
}

void DinoBattleModelPlayer::SetUnkC8(u8 value) {
    mUnkC8 = value;
}

u32 DinoBattleModelPlayer::GetUnk124() {
    return mUnk124;
}

void DinoBattleModelPlayer::SetUnk124(u32 value) {
    mUnk124 = value;
}

int DinoBattleModelPlayer::GetCreatureCount() {
    return mCreatureCount;
}

DinoBattleModelCreature *DinoBattleModelPlayer::GetCreature(int index) {
    return mCreatures[index];
}

DinoBattleModelCreature *DinoBattleModelPlayer::FindCreature(u32 id) {
    int count = GetCreatureCount();
    int i;

    for (i = 0; i < count; i++) {
        DinoBattleModelCreature *creature = GetCreature(i);
        if (id == creature->GetUnkB8()) {
            return creature;
        }
    }
    return NULL;
}

u32 DinoBattleModelPlayer::GetUnkFC() {
    return mUnkFC;
}

MRandom *DinoBattleModelPlayer::GetRandom() {
    return mRandom;
}

u32 DinoBattleModelPlayer::GetUnkAC() {
    return mUnkAC;
}

// A random fraction in [0, 1].
MFx32 DinoBattleModelPlayer::RandomFraction() {
    MFx32 r;
    r.v = GetRandom()->Next(FX32_ONE + 1);
    return r;
}

void DinoBattleModelPlayer::Idle() {
    Dispatch();
}

MTask *DinoBattleModelPlayer::CreateMoveToHeatAreaAnyCreature() {
    return new MoveToHeatAreaAnyCreature(this);
}

MTask *DinoBattleModelPlayer::CreateAddKasekiPointCreatureDie() {
    return new AddKasekiPointCreatureDie(this);
}

UpdateAllCreatureTask::UpdateAllCreatureTask(DinoBattleModelPlayer *player) : MProcess(NULL) {
    mPlayer = player;
}

MTask *DinoBattleModelPlayer::CreateUpdateAllCreatureTask() {
    return new UpdateAllCreatureTask(this);
}

MoveCreatureFromEscape::MoveCreatureFromEscape(DinoBattleModelPlayer *player) : MProcess(NULL) {
    mPlayer = player;
}

BOOL DinoBattleUserController::Unk14() {
    return TRUE;
}

extern "C" BOOL func_ov000_0216c83c(int value) {
    return (value == 1 || value == 2) ? TRUE : FALSE;
}

#pragma auto_inline on
