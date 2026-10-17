// CreatePlayerUpdateAbilityIconTask.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_message.h"
#include "game/create_player_update_ability_icon_task.h"
#include "types.h"
#include "game/dino_battle_user_controller.h"

extern "C" void func_ov000_0216d1e4(void *);

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
    func_ov000_0216d1e4((void *)mUnk10);
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

extern "C" u32 func_ov000_02163454(void *player);

void AddKasekiPointCreatureDie::Idle() {
    u32 point = mPlayer->mKasekiPoint;
    u32 add = func_ov000_02163454(mPlayer);
    func_ov000_02163464(mPlayer, add + point);
    DinoSignalWait *wait = new DinoSignalWait(0x10039, NULL, NULL, 1);
    MTask *exit = MTaskExitTransfer::Create(this);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(wait);
    serial->AddTail(exit);
    func_ov000_021631c0(mPlayer, (u32)(MMessageObserver *)wait);
}
