// CreatePlayerUpdateAbilityIconTask.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_message.h"
#include <algorithm>
#include "game/dino_sound.h"
#include "m/resource.h"
#include "game/create_player_update_ability_icon_task.h"
#include "game/send_local_message_task_menu.h"
#include "m/seed_generator.h"
#include "game/dino_resource.h"
#include "nitro/mi.h"
#include "types.h"
#include "game/dino_battle_model.h"
#include "game/dino_battle.h"
#include "game/dino_battle_cpu_controller.h"
#include "game/dino_battle_user_controller.h"
#include "game/set_status_effect_task.h"




extern "C" void func_ov000_02163258(void *, u32, u32);



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

// Answers the model, then has the view reset the creatures' priorities and mark them as having acted.
void SendModelCreatureAbilityResultTask::Execute() {
    if (mTarget != NULL) {
        mTarget->Post(new DinoBattleModelCreatureAbilityResult());
    }
    mView->Post(new DinoBattleViewSetCreaturePriorityRequest(0));
    mView->Post(new DinoBattleViewSetActionedSignal());
    Exit();
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

// Moves the frontmost creature into the heat area, unless the battle is over or one is already there.
void MoveToHeatAreaAnyCreature::Idle() {
    if (((u8 *) mPlayer->mBattle->GetUnk1FC())[0xA8]) {
        Exit();
        return;
    }
    if (mPlayer->FindCreature(0) != NULL) {
        Exit();
        return;
    }
    int frontPos = 4;
    DinoBattleModelCreature *front = NULL;
    for (int i = 0; i < mPlayer->GetCreatureCount(); i++) {
        DinoBattleModelCreature *creature = mPlayer->GetCreature(i);
        if ((int) creature->GetUnkB8() < frontPos) {
            front = creature;
            frontPos = creature->GetUnkB8();
        }
    }
    if (front != NULL) {
        DinoSignalWait *wait = new DinoSignalWait(0x10013, NULL, NULL, 1);
        MTask *exit = MTaskExitTransfer::Create(this);
        MSerialTaskSet *serial = &mSerial;
        serial->AddTail(wait);
        serial->AddTail(exit);
        front->Post(new DinoBattleModelCreatureMoveRequest((u32)(MMessageObserver *)wait, mPlayer->GetUnkB8(),
                                                           front->GetUnkB4(), front->GetUnkB8(), 0, 0, 0));
    } else {
        Exit();
    }
}

AddKasekiPointCreatureDie::~AddKasekiPointCreatureDie() {
}

DinoBattleModelCreature::AddLifePointTask::~AddLifePointTask() {
}

void SetKasekiPointTask::Execute() {
    ((DinoBattleModelPlayer *)mUnk10)->SetKasekiPoints(mUnk14);
    Exit();
}

void UpdateAbilityIconTask::Execute() {
    ((DinoBattleModelPlayer *)mUnk10)->SendUpdateAbilityIconRequest(mUnk18, mUnk14);
    Exit();
}

void DinoCreatureUpdateStateIconTask::Execute() {
    func_ov000_02163258((void *)mUnk10, mUnk14, mUnk18);
    Exit();
}

void PlayerUpdateTask::Execute() {
    ((DinoBattleModelPlayer *)mUnk10)->SendUpdateRequest(mUnk14);
    Exit();
}

void SetLifePointTask::Execute() {
    ((DinoBattleModelCreature *)mUnk10)->SetLife(mUnk14);
    Exit();
}

void AddLifePointTask::Execute() {
    ((DinoBattleModelCreature *)mUnk10)->AddLife(mUnk14, mUnk18);
    Exit();
}

void DinoBattleModelCreature::AddLifePointTask::Execute() {
    ((DinoBattleModelCreature *)mUnk10)->AddLife(mUnk14, mUnk18);
    Exit();
}

void ResetStatusEffectSleepTask::Execute() {
    ((DinoBattleModelCreature *)mUnk10)->ClearStatus8();
    Exit();
}

void CreatePlayerUpdateAbilityIconTask::Execute() {
    ((DinoBattleModelPlayer *)mUnk18)->SendUpdateAbilityIconRequest(mUnk10, mUnk14);
    Exit();
}

DinoBattleUserController::~DinoBattleUserController() {
}

PlayerThinkTimeCountTask::~PlayerThinkTimeCountTask() {
}

void ModelCretureAddLifeTask::Execute() {
    ((DinoBattleModelCreature *)mUnk10)->AddLife(mUnk14, 0);
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
    mPlayer->SetKasekiPoints(add + point);
    DinoSignalWait *wait = new DinoSignalWait(0x10039, NULL, NULL, 1);
    MTask *exit = MTaskExitTransfer::Create(this);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(wait);
    serial->AddTail(exit);
    mPlayer->SendUpdateRequest((u32)(MMessageObserver *)wait);
}


SetStatusEffectTask::~SetStatusEffectTask() {
}

DinoCreatureUpdateTask::~DinoCreatureUpdateTask() {
}

DamagedUpdate::~DamagedUpdate() {
}

extern "C" BOOL func_ov000_0216cfac(DinoBattleModelCreature *creature, u32 a, u32 b, u32 c, u32 d, u8 e);

SetStatusEffectTask::SetStatusEffectTask(DinoBattleModelCreature *creature, u32 a, u32 b, u32 c, u32 d, u8 e,
                                         MMessageObserver *view) {
    mCreature = creature;
    mUnk14 = a;
    mUnk18 = b;
    mUnk1C = c;
    mUnk20 = d;
    mUnk24 = e;
    mView = view;
}

void SetStatusEffectTask::Execute() {
    if (func_ov000_0216cfac(mCreature, mUnk14, mUnk18, mUnk1C, mUnk20, mUnk24) && mView != NULL) {
        mView->Post(new DinoBattleViewDeleteStatusEffectSignal(mCreature->GetUnkB4(), mUnk14));
    }
    Exit();
}

void DamagedUpdate::Execute() {
    if (mCreature != NULL) {
        mUnk20 = mCreature->GetUnkE4();
    }
    mView->Post(new DinoBattleViewCreatureDamagedUpdateRequest(mUnk18, mUnk1C, mUnk20, mUnk24, mUnk28, mUnk2C, mUnk2D));
    Exit();
}

void DinoBattleModelCreature::Idle() {
    Dispatch();
}

void DinoCreatureUpdateTask::Execute() {
    mCreature->SendUpdate(mUnk14, mUnk18);
    Exit();
}

#pragma auto_inline off

// The damage update for the creature's view (with its life now: dead or not).
extern "C" MTask *func_ov000_0216d4b0(DinoBattleModelCreature *creature, u32 a, u32 d, u32 e, u8 g) {
    return new DamagedUpdate(NULL, creature->mView, a, creature->mUnkB4, creature->mUnkE4, d, e, creature->mUnkE4 == 0,
                             g);
}

// The same, reading the creature's life when it runs.
extern "C" MTask *func_ov000_0216d520(DinoBattleModelCreature *creature, u32 a, u32 d, u32 e, u8 f) {
    return new DamagedUpdate(creature, creature->mView, a, creature->mUnkB4, 0, d, e, f, 0);
}

// The creature's attack power (higher in a pinch, for kind 5).
extern "C" MFx32 func_ov000_0216f028(DinoBattleModelCreature *creature) {
    MFx32 power = MFx32(creature->mUnkF4);
    if (creature->IsInPinch() && creature->GetDefUnk61() == 5) {
        const DinoBattleAdjustDefs *defs = gDinoResource->GetBattleAdjustDefs();
        power *= defs->pinchBoost.Mul(MFx32(creature->mDef->mUnk62)) * MFx32::FromRaw(0x19A);  // 0.1
    }
    return power;
}

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

extern "C" BOOL func_ov000_0218472c(int flag); // a debug switch, always off

static const int kUnkFCMax = 12;
static const int kUnkFCMin = 1;

void DinoBattleModelCreature::AddUnkFC(int delta) {
    int value = mUnkFC + delta;
    mUnkFC = std::max(kUnkFCMin, std::min(kUnkFCMax, value));
}

// Sets the creature's life: at most its maximum, at least the player's floor.
void DinoBattleModelCreature::SetLife(int life) {
    mUnkE4 = life;
    s32 floor = mPlayer->GetUnkC8();
    mUnkE4 = std::max(floor, std::min(mUnkE8, mUnkE4));
}

// Adds to (or takes from) the creature's life, within the same bounds; a gain can show an effect.
void DinoBattleModelCreature::AddLife(int amount, u8 effect) {
    if (amount < 0 && func_ov000_0218472c(2)) {
        return;
    }
    mUnkE4 += amount;
    s32 floor = mPlayer->GetUnkC8();
    mUnkE4 = std::max(floor, std::min(mUnkE8, mUnkE4));
    if (amount > 0 && effect) {
        mView->Post(CreateMotionEffectRequest());
    }
}

BOOL DinoBattleModelCreature::IsInPinch() {
    if (MFx32(mUnkE4) / MFx32(mUnkE8) <= MFx32(gDinoResource->GetBattleAdjustDefs()->pinchRatio)) {
        return TRUE;
    }
    return FALSE;
}

MTask *DinoBattleModelCreature::CreateUpdateTask(u8 a, u32 b) {
    return new DinoCreatureUpdateTask(this, a, b);
}

// The number of the creature's attacks its level allows.
// NONMATCHING (and GetAttack): the game indexes the level table with i * 4 each time; ours strength-reduces it.
u32 DinoBattleModelCreature::CountAttacks() {
    const DinoCreatureDef *def = gDinoResource->GetCreatureDef(mUnkB0);
    u32 count = 0;
    int i = 0;
    if (i < def->attackCount) {
        const u32 *levels = (const u32 *) ((const u8 *) def + def->attackLevels);
        for (; i < def->attackCount; i++) {
            if (levels[i] <= mUnk100) {
                count++;
            }
        }
    }
    return count;
}

// The index'th attack its level allows (and, with arg, that attack's value from the u8 table, less one); 0 if none.
u32 DinoBattleModelCreature::GetAttack(u32 index, int *arg) {
    const DinoCreatureDef *def = gDinoResource->GetCreatureDef(mUnkB0);
    for (int i = 0; i < def->attackCount; i++) {
        if (((const u32 *) ((const u8 *) def + def->attackLevels))[i] <= mUnk100 && index-- == 0) {
            if (arg != NULL) {
                *arg = ((const u8 *) def + def->attackUnk78)[i] - 1;
            }
            return ((const u32 *) ((const u8 *) def + def->attackIds))[i];
        }
    }
    return 0;
}

// The creature's special attack, if the whole team qualifies and its level allows.
u32 DinoBattleModelCreature::GetSpecialAttack() {
    if (mPlayer->AllCreaturesShareDefFlag() && mPlayer->GetUnk15C()) {
        const DinoCreatureDef *def = gDinoResource->GetCreatureDef(mUnkB0);
        if (def->specialAttackLevel <= mUnk100) {
            return def->specialAttack;
        }
    }
    return 0;
}

// The index'th attack, counting the special attack after the level ones; NULL if there's none.
const DinoAttackDef *DinoBattleModelCreature::GetAttackDef(u32 index) {
    u32 attack = index < CountAttacks() ? GetAttack(index, NULL) : GetSpecialAttack();
    if (attack != 0) {
        return gDinoResource->GetAttackDef(attack);
    }
    return NULL;
}

MMessage *DinoBattleModelCreature::CreateMotionEffectRequest() {
    return new DinoBattleViewCreatureMotionEffectRequest(0, mUnkB4, 4, 0, 0, -1);
}

void DinoBattleModelCreature::BeginAction() {
    if (!func_ov000_0218472c(0)) {
        mActing = TRUE;
        mView->Post(new DinoBattleViewCreatureActionSignal(GetUnkB4(), TRUE));
    }
}

void DinoBattleModelCreature::EndAction() {
    mActing = FALSE;
    mView->Post(new DinoBattleViewCreatureActionSignal(GetUnkB4(), FALSE));
}

// Sends the view the creature's state.
void DinoBattleModelCreature::SendUpdate(u8 a, u32 b) {
    mView->Post(new DinoBattleViewCreatureUpdateRequest(b, mUnkB4, GetUnkE8(), GetUnkE4(), GetUnkEC(), GetUnkF0(),
                                                        GetUnkF4(), GetUnkF8(), GetUnkFC(), GetUnk108(), 0,
                                                        mStatus.flags, a));
}

u8 DinoBattleModelCreature::GetUnk174() {
    return mActing;
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

DinoBattleCreatureStatus *DinoBattleModelCreature::GetStatus() {
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

// Whether the target wards off the damage: 1 by its status (taking none of it), 2 by its kind 7 def's chance (taking
// it scaled down), else 0; out gets what it takes (at least 1).
// NONMATCHING: 99.9%, the multiplication's two argument copies swap stack slots.
extern "C" int func_ov000_02162ad8(DinoBattleModelCreature *attacker, DinoBattleModelCreature *target, u32 unk2,
                                   u32 unk3, u32 unk4, int *damage, int *out, u32 flags) {
    int result = 0;
    if (*damage != 0) {
        if ((flags & 1) && (target->GetStatus()->flags & 0x100)) {
            DinoBattleModelPlayer *player = attacker->mPlayer;
            DinoBattleCreatureStatus *status = target->GetStatus();
            if ((int)status->guardChance >= (int)player->GetRandom()->Next(100)) {
                result = 1;
            }
        }
        if (result == 0 && (flags & 2) && target->GetDefUnk61() == 7) {
            DinoBattleModelPlayer *player = attacker->mPlayer;
            int chance = target->GetDef()->mUnk62;
            if (chance >= (int)player->GetRandom()->Next(100)) {
                result = 2;
            }
        }
        if (result != 0) {
            MFx32 taken = MFx32(*damage);
            if (result == 2) {
                MFx32 ratio = gDinoResource->GetBattleAdjustDefs()->unk50;
                taken = taken * ratio;
            }
            int value = MFx32ToInt(taken);
            if (value < 1) {
                value = 1;
            }
            *out = value;
            if (result == 1) {
                *damage = 0;
            }
        }
    }
    return result;
}

extern "C" BOOL func_ov000_0216c83c(int value);

// When a creature (with B8 clear, kind 1 and no flag) acts, each of the player's creatures that qualifies may get an
// event: by chance (its def's 0x22 percentage, plus the actor's 0x62 one for an actor of kind 9), or for certain while
// the field's flag 8 is set (which this uses up).
extern "C" void func_ov000_02169404(DinoBattleModelPlayer *player, DinoBattleModelCreature *actor, u32 event, int kind,
                                    u8 flag) {
    if (actor->GetUnkB8() == 0 && kind == 1 && !flag) {
        for (int i = 0; i < player->GetCreatureCount(); i++) {
            DinoBattleModelCreature *creature = player->GetCreature(i);
            if (func_ov000_0216c83c(creature->GetUnkB8())) {
                int chance = creature->GetDef()->mUnk22;
                if (actor->GetDefUnk61() == 9) {
                    chance += actor->GetDef()->mUnk62;
                }
                if (chance > (int)player->GetRandom()->Next(100) ||
                    (((DinoBattleModelField *)player->mUnk100)->mUnkD0 & 8)) {
                    player->PushEvent((u32)creature, event, 0);
                    ((DinoBattleModelField *)player->mUnk100)->mUnkD0 &= ~8;
                }
            }
        }
    }
}

extern "C" void func_ov000_0216c90c(DinoBattleModelCreature *creature, u32 id, BOOL arg);

DinoBattleModelCreature::DinoBattleModelCreature(MMessageObserver *view, DinoBattleModelPlayer *player, u32 id,
                                                 u32 unkB4, u32 unkB8, DinoBattleCreatureStats stats, s32 unkFC,
                                                 u32 unk100, u32 unk178)
    : MProcess(NULL), mView(view), mPlayer(player), mUnkB4(unkB4), mUnkB8(unkB8), mUnkBC(unkB8), mStats(stats),
      mUnkFC(unkFC), mUnk100(unk100), mUnk178(unk178) {
    mActing = FALSE;
    func_ov000_0216c90c(this, id, TRUE);
}

MFx32 DinoBattleModelCreature::RandomFraction() {
    return mPlayer->RandomFraction();
}

extern "C" BOOL func_ov000_0216ce04(DinoBattleModelCreature *creature);
extern "C" BOOL func_ov000_0216f13c(DinoBattleModelCreature *creature);
// Which type each type beats, and which beats it.
struct DinoTypeAdvantage {
    int type;
    int beats;
    int beatenBy;
};

static const DinoTypeAdvantage sTypeAdvantages[] = {
    {1, 3, 4},
    {3, 2, 1},
    {2, 4, 3},
    {4, 1, 2},
    {0, 0, 0},
};

// 1 if `type` beats `other`, 2 if `other` beats `type`, else 0.
int DinoGetTypeAdvantage(int type, int other) {
    int i = 0;
    do {
        if (type == sTypeAdvantages[i].type) {
            if (other == sTypeAdvantages[i].beats) {
                return 1;
            }
            if (other == sTypeAdvantages[i].beatenBy) {
                return 2;
            }
            break;
        }
        i++;
    } while (sTypeAdvantages[i].type != 0);
    return 0;
}

int DinoBattleModelCreature::GetB8Category() {
    switch (mUnkB8) {
    case 0:
        return 0;
    case 1:
    case 2:
        return 1;
    case 3:
        return 2;
    }
    return 0;
}

BOOL DinoBattleModelCreature::IsUnkCE50() {
    if (GetDefUnk61() == 5 && func_ov000_0216ce04(this)) {
        return TRUE;
    }
    return FALSE;
}

int DinoBattleModelCreature::GetUnkCE7C(u32 attack) {
    if (gDinoResource->GetAttackDef(attack)->unk18 >= 2) {
        return mUnk10C;
    }
    return mDef->mUnk0D;
}

int DinoBattleModelCreature::CompareUnkECA0(DinoBattleModelCreature *other, u32 attack) {
    int a = GetUnkCE7C(attack);
    int b = other->GetUnkCE7C(0);
    return DinoGetTypeAdvantage(a, b);
}

int DinoBattleModelCreature::CompareUnkED0C(DinoBattleModelCreature *other) {
    switch (CompareUnkECA0(other, 0)) {
    case 2:
        return -1;
    case 0:
        return 0;
    case 1:
        return 1;
    default:
        return 0;
    }
}

BOOL DinoBattleModelCreature::GetUnkD298() {
    if (func_ov000_0216f13c(this)) {
        return mPlayer->AllCreaturesShareDefFlag();
    }
    return FALSE;
}

BOOL DinoBattleModelCreature::RollDefUnk23() {
    u8 chance = mDef->mUnk23;
    if (chance >= (int)mPlayer->GetRandom()->Next(100)) {
        return TRUE;
    }
    return FALSE;
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

// Clears bit 0 of the other creatures' unk108 on this side and bit 1 on the opponent's.
void DinoBattleModelCreature::ClearOthersUnk108() {
    if (GetUnkB8() != 0) {
        return;
    }
    int i;
    DinoBattleModelPlayer *player = mPlayer;
    for (i = 0; i < player->GetCreatureCount(); i++) {
        DinoBattleModelCreature *creature = player->GetCreature(i);
        if (creature != this) {
            creature->SetUnk108(creature->GetUnk108() & ~1);
        }
    }
    DinoBattleModelPlayer *opponent = player->GetOpponent();
    for (i = 0; i < opponent->GetCreatureCount(); i++) {
        DinoBattleModelCreature *creature = opponent->GetCreature(i);
        creature->SetUnk108(creature->GetUnk108() & ~2);
    }
}

// (The game keeps this as its own function, which only forwards.)
void DinoBattleModelCreature::UpdateMarks() {
    ClearOthersUnk108();
}

void DinoBattleModelCreature::ResetUnk108() {
    SetUnk108(0);
    if (GetUnkB8() == 0) {
        ClearOthersUnk108();
    }
}

void DinoBattleModelCreature::ChangeUnkB8(u32 value) {
    ResetUnk108();
    ResetStatus();
    func_ov000_02163258(mPlayer, (u32)this, 0);
    mUnkB8 = value;
}

BOOL DinoBattleModelCreature::IsUnkD244() {
    if (mActing) {
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

// The attacks the creature can use now, as a bitmask by index (the special attack's bit is after the others):
// those it has the points for, and (with check) not sealed by its status, nor needing more life or company than it
// has.
extern "C" u32 func_ov000_0216d2cc(DinoBattleModelCreature *creature, BOOL check) {
    u32 mask = 0;
    int points = creature->mPlayer->GetUnk128();
    gDinoResource->GetCreatureDef(creature->mUnkB0);
    for (u32 i = 0; i <= creature->CountAttacks(); i++) {
        u32 attack;
        if (i != creature->CountAttacks()) {
            if (check && (creature->mStatus.flags & 0x10) && (creature->mStatus.unk20 & (1 << i))) {
                continue;
            }
            const DinoAttackDef *def = gDinoResource->GetAttackDef(creature->GetAttack(i, NULL));
            switch (*DinoOffsetPtr<u8>(def, def->unk14)) {
            case 11:
            case 12:
                if (creature->mPlayer->GetCreatureCount() <= 1) {
                    continue;
                }
                break;
            case 6:
                if ((int)creature->GetUnkE4() <= 1) {
                    continue;
                }
                break;
            }
            attack = creature->GetAttack(i, NULL);
        } else {
            if (!creature->GetUnkD298()) {
                continue;
            }
            attack = creature->GetSpecialAttack();
        }
        const DinoAttackDef *def = gDinoResource->GetAttackDef(attack);
        if (points >= *DinoOffsetPtr<u16>(def, def->cost)) {
            mask |= 1 << i;
        }
    }
    return mask;
}

// Has the view recolor the creatures, all at once.
MTask *DinoBattleModelPlayer::CreateUpdateColorTask(const u32 *ids, u32 count, u32 color) {
    if (count != 0) {
        MParallelTaskSet *set = new MParallelTaskSet(TRUE);
        for (u32 i = 0; i < count; i++) {
            set->AddTail(mUnkF4->CreatePostTask(new DinoBattleViewCreatureUpdateColorRequest(0, *ids, color)));
            ids++;
        }
        return set;
    }
    return NULL;
}

// Has the view play a motion effect, and waits for it to finish.
MTask *DinoBattleModelPlayer::CreateMotionEffectTask(u32 a, u32 b, u32 c, u32 d, u32 e) {
    MParallelTaskSet *set = new MParallelTaskSet(TRUE);
    DinoSignalWait *wait = new DinoSignalWait(0x10054, NULL, NULL, 1);
    MTask *post = mUnkF4->CreatePostTask(
        new DinoBattleViewCreatureMotionEffectRequest((u32)(MMessageObserver *)wait, a, b, c, d, e));
    set->AddTail(wait);
    set->AddTail(post);
    return set;
}

// Has the view update the ability icons, and waits for it.
MTask *DinoBattleModelPlayer::CreateUpdateAbilityIconTask(u8 flag) {
    MParallelTaskSet *set = new MParallelTaskSet(TRUE);
    DinoSignalWait *wait = new DinoSignalWait(0x1004A, NULL, NULL, 1);
    UpdateAbilityIconTask *update = new UpdateAbilityIconTask(this, wait, flag);
    set->AddTail(wait);
    set->AddTail(update);
    return set;
}

// Has the view show an ability (type 6), and waits for it to finish.
MTask *DinoBattleModelPlayer::CreateAbilityTask(u32 a, u32 b, u32 c) {
    MParallelTaskSet *set = new MParallelTaskSet(TRUE);
    DinoSignalWait *wait = new DinoSignalWait(0x1002A, NULL, NULL, 1);
    MTask *post = mUnkF4->CreatePostTask(new DinoBattleViewCreatureAbilityRequest(
        (u32)(MMessageObserver *)wait, a, b, c, 0, 6, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0));
    set->AddTail(wait);
    set->AddTail(post);
    return set;
}

// Fade settings (constants in main).
extern const u32 data_0208a110;
extern const u32 data_0208a11c;
extern const u32 data_0208a16c;
extern const u32 data_0208a198;
extern const u32 data_0208a1c0;

// Has the view fade the screens (first clearing the status effect icons' blending), and waits for it.
MTask *DinoBattleModelPlayer::CreateLcdFadeTask(BOOL clearIcons) {
    MParallelTaskSet *set = new MParallelTaskSet(TRUE);
    DinoSignalWait *wait = new DinoSignalWait(0x1006A, NULL, NULL, 1);
    if (clearIcons) {
        MTask *clear = mUnkF4->CreatePostTask(new DinoBattleViewStatusEffectIconBlendClearSignal());
        set->AddTail(mUnkF4->CreatePostTask(new DinoBattleViewLCDFadeRequest(
            (u32)(MMessageObserver *)wait, 0, 0, -1, data_0208a110, data_0208a11c, data_0208a16c)));
        set->AddTail(clear);
    } else {
        set->AddTail(mUnkF4->CreatePostTask(new DinoBattleViewLCDFadeRequest(
            (u32)(MMessageObserver *)wait, 0, 0, -1, 0, data_0208a198, data_0208a1c0)));
    }
    set->AddTail(wait);
    return set;
}

// The same with one target (or none: the attacker's own).
void DinoBattleModelPlayer::AddAbilitySequence(MTaskSet *set, u32 a, u32 b, u32 attacker, void *target, u8 recolor) {
    std::vector<void *> targets;
    if ((void *)attacker != target) {
        targets.push_back(target);
    }
    AddAbilitySequence(set, a, b, attacker, targets, recolor);
}

// Adds the view's side of an ability to set: the attack bar, the ability itself, the fade and the targets'
// coloring, then undoes them.
void DinoBattleModelPlayer::AddAbilitySequence(MTaskSet *set, u32 a, u32 b, u32 attacker,
                                               std::vector<void *> &targets, u8 recolor) {
    u32 ids[6] = {attacker};
    u32 n = 1;
    for (u32 i = 0; i < targets.size(); i++) {
        if (attacker != (u32)targets[i]) {
            ids[n++] = (u32)targets[i];
        }
    }
    MTask *color;
    MTask *fadeOut;
    MTask *barOn;
    MTask *ability;
    MTask *clearBar;
    MTask *barOff;
    barOff = mUnkF4->CreatePostTask(new DinoBattleViewSetAttackBarUpperLCDRequest(0, 0, 0));
    clearBar = mUnkF4->CreatePostTask(new DinoBattleViewClearAttackBarRequest(0));
    ability = CreateAbilityTask(a, attacker, b);
    barOn = mUnkF4->CreatePostTask(new DinoBattleViewSetAttackBarUpperLCDRequest(0, b, 0));
    fadeOut = CreateLcdFadeTask(TRUE);
    color = CreateUpdateColorTask(ids, targets.size() + 1, 0x20);
    set->AddTail(mUnkF4->CreatePostTask(new DinoBattleViewCreatureSetBlendRequest(0, targets.size() + 1, ids, TRUE)));
    set->AddTail(color);
    set->AddTail(fadeOut);
    set->AddTail(barOn);
    set->AddTail(ability);
    set->AddTail(clearBar);
    set->AddTail(barOff);
    if (recolor) {
        set->AddTail(CreateUpdateColorTask(ids, targets.size() + 1, 0));
    }
    set->AddTail(CreateLcdFadeTask(FALSE));
}

u32 DinoBattleModelPlayer::GetUnkB8() {
    return mUnkB8;
}

// The TU's pooled constants.
extern "C" const struct {
    int unk00;
    int kasekiPointsMin;    // 0x04
} data_ov000_021924bc;

// Sets the kaseki points, within the bounds (a sound when they were full and more were added).
void DinoBattleModelPlayer::SetKasekiPoints(int points) {
    if (mUnk128 == mUnk14C && points > (int)mUnk14C) {
        DinoSoundPlaySe(0x564, 0);
    }
    mUnk128 = std::max(data_ov000_021924bc.kasekiPointsMin, std::min((int)mUnk14C, points));
}

int DinoBattleModelPlayer::GetUnk128() {
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

DinoBattleModelPlayer::DinoBattleModelPlayer(u32 side, u32 unkB4)
    : MProcess(NULL), mUnkB4(unkB4), mUnkB8(side), mUnkBC(1), mUnkC0(1), mUnkC8(0), mRandom(NULL), mUnk12C(0),
      mUnk12D(0), mUnk15C(0) {
}

DinoBattleModelPlayer::~DinoBattleModelPlayer() {
    delete mRandom;
}

void DinoBattleModelPlayer::SetBattle(DinoBattle *battle) {
    mBattle = battle;
    mUnkF4 = battle->GetUnk200();
    mUnk100 = ((DinoBattleModel *)battle->GetUnk1FC())->GetUnk10C();
    mOpponent = ((DinoBattleModel *)battle->GetUnk1FC())->GetPlayer(1 - mUnkB8);
}

void DinoBattleModelPlayer::SendUpdateRequest(u32 arg) {
    mUnkF4->Post(new DinoBattleViewPlayerUpdateRequest(arg, mUnkB8, GetUnk128()));
}

void DinoBattleModelPlayer::SendUpdateAbilityIconRequest(u8 a, u32 b) {
    mUnkF4->Post(new DinoBattleViewPlayerUpdateAbilityIconRequest(b, mUnkB8, a));
}

void DinoBattleModelPlayer::PushEvent(u32 a, u32 b, u8 c) {
    if (a != 0 && b != 0) {
        mUnkA8.push_back(DinoBattlePlayerEvent(a, b, c));
    }
}

// NONMATCHING: the game keeps all three MSL tag temporaries in separate stack bytes and compares field to id
void DinoBattleModelPlayer::RemoveEvents(u32 id) {
    std::vector<DinoBattlePlayerEvent> kept;
    for (u32 i = 0; i < mUnkA8.size(); i++) {
        if (id != mUnkA8[i].mUnk4) {
            kept.push_back(mUnkA8[i]);
        }
    }
    mUnkA8 = kept;
}

DinoBattlePlayerEvent DinoBattleModelPlayer::PopEvent(bool remove) {
    if (GetUnkAC() != 0) {
        DinoBattlePlayerEvent &front = mUnkA8[0];
        u32 a = front.mUnk0;
        u32 b = front.mUnk4;
        u8 c = front.mUnk8;
        if (remove) {
            mUnkA8.erase(mUnkA8.begin());
        }
        return DinoBattlePlayerEvent(a, b, c);
    }
    return DinoBattlePlayerEvent(0, 0, 0);
}


int DinoBattleModelPlayer::GetCreatureCount() {
    return mCreatures.size();
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

DinoBattleModelPlayer *DinoBattleModelPlayer::GetOpponent() {
    return mOpponent;
}

MRandom *DinoBattleModelPlayer::GetRandom() {
    return mRandom;
}

u32 DinoBattleModelPlayer::GetUnkAC() {
    return mUnkA8.size();
}

void DinoBattleModelPlayer::SetUnk12C(u8 value) {
    mUnk12C = mUnk12D = value;
}

u32 DinoBattleModelPlayer::NextUnkId() {
    u32 id = mUnkBC++;
    return (mUnkB8 << 16) | id;
}

void DinoBattleModelPlayer::CollectCreaturesNearB8(int pos, u32 *ids, int *count) {
    *count = 0;
    for (int i = 0; i < GetCreatureCount(); i++) {
        if (pos + GetCreature(i)->GetB8Category() + 1 <= 2) {
            ids[(*count)++] = GetCreature(i)->GetUnkB4();
        }
    }
}

DinoBattleModelCreature *DinoBattleModelPlayer::FindCreatureByB4(u32 id) {
    for (std::vector<DinoBattleModelCreature *>::iterator it = mCreatures.begin(); it != mCreatures.end(); it++) {
        if (id == (*it)->GetUnkB4()) {
            return *it;
        }
    }
    return NULL;
}

DinoBattleModelCreature *DinoBattleModelPlayer::GetCreatureInSlot(int slot) {
    if (mUnk130[slot] == 0) {
        return NULL;
    }
    return FindCreatureByB4(mUnk130[slot]);
}

void DinoBattleModelPlayer::ClearEvents() {
    mUnkA8.clear();
}

BOOL DinoBattleModelPlayer::AnyCreatureUnkD244() {
    int i;

    for (i = 0; i < GetCreatureCount(); i++) {
        if (GetCreature(i)->IsUnkD244()) {
            return TRUE;
        }
    }
    return FALSE;
}

void DinoBattleModelPlayer::CollectCreaturesUnkD244(u32 *ids, int *count) {
    int i;

    *count = 0;
    for (i = 0; i < GetCreatureCount(); i++) {
        if (GetCreature(i)->IsUnkD244()) {
            ids[(*count)++] = GetCreature(i)->GetUnkB4();
        }
    }
}

BOOL DinoBattleModelPlayer::IsUnk2163978() {
    if (FindCreature(3)) {
        return FALSE;
    }
    if (!FindCreature(0)) {
        return FALSE;
    }
    if (!FindCreature(0)->IsUnkD274()) {
        return FALSE;
    }
    return (FindCreature(1) || FindCreature(2)) ? TRUE : FALSE;
}

void DinoBattleModelPlayer::CollectCreaturesB8In12(u32 *ids, int *count) {
    int i;

    *count = 0;
    for (i = 0; i < GetCreatureCount(); i++) {
        if (GetCreature(i)->GetUnkB8() == 1 || GetCreature(i)->GetUnkB8() == 2) {
            ids[(*count)++] = GetCreature(i)->GetUnkB4();
        }
    }
}

BOOL DinoBattleModelPlayer::AllCreaturesShareDefFlag() {
    int count = GetCreatureCount();
    u32 flags;
    int i;

    if (count < 3) {
        return FALSE;
    }
    flags = GetCreature(0)->GetDef()->mUnk24;
    for (i = 1; i < count; i++) {
        flags &= GetCreature(i)->GetDef()->mUnk24;
    }
    if (flags) {
        return TRUE;
    }
    return FALSE;
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

// Collects the ids of both players' creatures (either player may be missing).
void DinoCollectCreatureIds(std::vector<void *> &ids, DinoBattleModelPlayer *first, DinoBattleModelPlayer *second) {
    for (u32 side = 0; side < 2; side++) {
        DinoBattleModelPlayer *player = side ? second : first;
        if (player != NULL) {
            for (u32 i = 0; i < player->GetCreatureCount(); i++) {
                ids.push_back((void *)player->GetCreature(i)->GetUnkB4());
            }
        }
    }
}

// Recolors every creature on both sides.
MTask *DinoBattleModelPlayer::CreateUpdateAllColorTask(u32 color) {
    u32 ids[6];
    u32 count = 0;
    DinoBattleModelPlayer *player = this;
    for (u32 side = 0; side < 2; side++) {
        for (u32 i = 0; i < player->GetCreatureCount(); i++) {
            ids[count++] = player->GetCreature(i)->GetUnkB4();
        }
        player = player->GetOpponent();
    }
    return CreateUpdateColorTask(ids, count, color);
}

// Updates every creature on both sides, then waits until they have all signalled back.
MTask *DinoBattleModelPlayer::CreateUpdateCreaturesTask() {
    DinoBattleModelPlayer *players[2];
    players[0] = this;
    players[1] = GetOpponent();
    MParallelTaskSet *set = new MParallelTaskSet(TRUE);
    int total = players[0]->GetCreatureCount() + players[1]->GetCreatureCount();
    DinoSignalWait *wait = new DinoSignalWait(0x10030, NULL, NULL, total);
    set->AddTail(wait);
    for (u32 side = 0; side < 2; side++) {
        for (u32 i = 0; i < players[side]->GetCreatureCount(); i++) {
            set->AddTail(players[side]->GetCreature(i)->CreateUpdateTask(0, (u32)(MMessageObserver *)wait));
        }
    }
    return set;
}

// Adds to (or takes from) the player's kaseki points.
void DinoBattleModelPlayer::AddKasekiPoints(int delta) {
    if (delta < 0 && func_ov000_0218472c(1)) {
        return;
    }
    SetKasekiPoints(mUnk128 + delta);
}

MTask *DinoBattleModelPlayer::CreateUpdateTask(u32 arg) {
    return new PlayerUpdateTask(this, arg);
}

MTask *DinoBattleModelPlayer::CreateUpdateStateIconTask(u32 creature, u32 arg) {
    return new DinoCreatureUpdateStateIconTask(this, creature, arg);
}

MTask *DinoBattleModelPlayer::CreateSetKasekiPointTask(u32 points) {
    return new SetKasekiPointTask(this, points);
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

// Whether the player has a creature in the escape area (position 3).
BOOL MoveCreatureFromEscape::IsNeeded() {
    DinoBattleModelCreature *creature = mPlayer->FindCreature(3);
    if (creature != NULL && creature->GetUnkC0() == 3) {
        return TRUE;
    }
    return FALSE;
}


template MTask *DinoCreatePostAndWaitTask(MMessageObserver *target, DinoBattleViewPlayerWaitKPCountStopRequest *msg);
template MTask *DinoCreatePostAndWaitTask(MMessageObserver *target, DinoBattleViewCreaturePlayMotionRequest *msg);
template MTask *DinoCreatePostAndWaitTask(MMessageObserver *target, DinoBattleView3DTeamAttackRequest *msg);

// Whether the attack has bit 0 of the fourth byte of its unk10 data set.
BOOL DinoAttackHasUnk10Flag(u32 id) {
    const DinoAttackDef *def = gDinoResource->GetAttackDef(id);
    if (DinoOffsetPtr<u8>(def, def->unk10)[3] & 1) {
        return TRUE;
    }
    return FALSE;
}

DinoBattleUserController::DinoBattleUserController(void *unkBC, u32 arg, u8 unkB8) : DinoBattleController(arg) {
    mUnkB8 = unkB8;
    mUnkBC = unkBC;
    mThinkTimer = new PlayerThinkTimeCountTask;
    mParallel.AddTail(mThinkTimer);
}

u32 DinoBattleUserController::GetCreatureMask(DinoBattleModelPlayer *player) {
    u32 mask = 0;
    u32 i;

    for (i = 0; i < player->GetCreatureCount(); i++) {
        DinoBattleModelCreature *creature = player->GetCreature(i);
        if (creature->GetUnkB8() != 3 && !creature->GetUnk174() && func_ov000_0216d2cc(creature, 0)) {
            mask |= 1 << creature->GetUnkB8();
        }
    }
    return mask;
}

BOOL DinoBattleUserController::Unk14() {
    return TRUE;
}

extern "C" BOOL func_ov000_0216c83c(int value) {
    return (value == 1 || value == 2) ? TRUE : FALSE;
}

#pragma auto_inline on

#pragma auto_inline off

BOOL DinoBattleModelPlayer::IsCpu() {
    if (dynamic_cast<DinoBattleCpuController *>((DinoBattleController *)mBattle->GetUnk1F4(mUnkB8)) != NULL) {
        return TRUE;
    }
    return FALSE;
}

u8 DinoBattleModelPlayer::GetUnk15C() {
    return mUnk15C;
}

void DinoBattleModelPlayer::SetUnk15C() {
    mUnk15C = 1;
}

MTask *DinoBattleModelPlayer::CreateSendAbilityResultTask(MMessageObserver *target) {
    return new SendModelCreatureAbilityResultTask(mUnkF4, target);
}

// Resources the file loads (their data types are unknown); until the code that constructs them is decompiled,
// these keep out-of-line copies of the constructors.
struct DinoAbilityIconData;
#pragma dont_inline on
MResource<DinoAbilityIconData> DinoAbilityIconDataLoad(std::string path) {
    return MResource<DinoAbilityIconData>(path);
}
#pragma dont_inline reset

MFx32 GetAffinityRatio(DinoBattleModelCreature *creature, DinoBattleModelCreature *other, u32 attack) {
    int result = creature->CompareUnkECA0(other, attack);
    if (result == 1) {
        return gDinoResource->GetBattleAdjustDefs()->affinityRatio[0];
    }
    if (result == 2) {
        return gDinoResource->GetBattleAdjustDefs()->affinityRatio[1];
    }
    return gDinoResource->GetBattleAdjustDefs()->affinityRatio[2];
}

MFx32 DinoBattleModelCreature::ReduceByDefense(MFx32 damage, MFx32 defense) {
    return damage - gDinoResource->GetBattleAdjustDefs()->unk14.Mul(defense);
}

MFx32 DinoBattleModelCreature::ApplyRatio(MFx32 damage, MFx32 ratio) {
    return damage * ratio;
}

MFx32 DinoBattleModelCreature::ApplyRatio2(MFx32 damage, MFx32 ratio) {
    return damage * ratio;
}

// The creature's unkEC as a ratio, raised when it is in a pinch with the right ability.
MFx32 DinoBattleModelCreature::GetUnkECRatio() {
    MFx32 ratio = MFx32(mUnkEC);
    if (IsInPinch() && GetDefUnk61() == 5) {
        const DinoBattleAdjustDefs *defs = gDinoResource->GetBattleAdjustDefs();
        ratio *= defs->unk3C.Mul(MFx32(mDef->mUnk62)) * MFx32::FromRaw(0x19A); // 0.1
    }
    return ratio;
}

// Scales damage by the creature's percentages for the two relative positions.
MFx32 DinoBattleModelCreature::ApplyPositionRatio(MFx32 damage, int otherCategory) {
    int distance = otherCategory + GetB8Category() + 1;
    if (distance == 2) {
        return damage * MFx32(mDef->mUnk6C) / MFx32(100);
    }
    if (distance == 3) {
        return damage * MFx32(mDef->mUnk70) / MFx32(100);
    }
    return damage;
}

// The damage from attack against defense, raised or lowered by the support effects of the creatures beside it.
// NONMATCHING: the copies of percent for the additions and the division take the stack slot below their other
// operand's copy in the game, above it here.
MFx32 DinoBattleModelCreature::CalcDamage(DinoBattleModelCreature *target, MFx32 attack, MFx32 defense, u8 *hit) {
    if (gDinoResource->GetBattleAdjustDefs()->unk168) {
        if (attack > MFx32(0)) {
            MFx32 percent = MFx32(100);
            if (GetDefUnk61() != 8 && GetUnkB8() == 0) {
                for (int i = 0; i < 2; i++) {
                    if (GetUnkB8() == 0) {
                        DinoBattleModelCreature *ally = mPlayer->FindCreature(i ? 2 : 1);
                        DinoBattleModelCreature *enemy = mPlayer->GetOpponent()->FindCreature(i ? 2 : 1);
                        if (ally != NULL && (ally->GetUnk108() & 1)) {
                            DinoBattleCreatureDef *def = ally->GetDef();
                            percent = percent + MFx32(DinoOffsetPtr<s8>(def, def->mUnk54)[0]);
                        }
                        if (enemy != NULL && (enemy->GetUnk108() & 2)) {
                            DinoBattleCreatureDef *def = enemy->GetDef();
                            percent = percent + MFx32(DinoOffsetPtr<s8>(def, def->mUnk58)[0]);
                        }
                    }
                }
            }
            percent = percent / MFx32(100);
            return (attack * percent + defense) / MFx32(gDinoResource->GetBattleAdjustDefs()->unk10);
        }
        *hit = FALSE;
        return MFx32(0);
    } else {
        return attack * defense / MFx32(gDinoResource->GetBattleAdjustDefs()->unk10);
    }
}

// A random damage factor between the minimum and maximum.
MFx32 DinoBattleModelCreature::RandomDamageFactor() {
    const MFx32 min = gDinoResource->GetBattleAdjustDefs()->randomMin;
    return min + (MFx32(gDinoResource->GetBattleAdjustDefs()->randomMax) - min) * RandomFraction();
}

// A ratio that follows a quarter sine wave of (a - b) over the curve's range, capped at 1.
// NONMATCHING: the copies std::min and std::max make for their comparisons take the lowest stack slots here, the
// ones right after the subtraction's in the game.
MFx32 DinoBattleModelCreature::CurveRatio(MFx32 a, MFx32 b) {
    const MFx32 range = gDinoResource->GetBattleAdjustDefs()->curveRange;
    const MFx32 base = gDinoResource->GetBattleAdjustDefs()->curveBase;
    const MFx32 low = gDinoResource->GetBattleAdjustDefs()->curveLow;
    MFx32 x = (a - b) / range;
    x = std::max(MFx32(-1), std::min(MFx32(1), x));
    MFx32 angle = MFx32::FromRaw(0x800) * MFx32::FromRaw(0x3244) * x; // pi / 2 * x
    MFx32 sine;
    sine.v = FX_SinIdx((u16)((angle * MFx32(0x10000) / MFx32::FromRaw(0x6488)).v >> FX32_SHIFT)); // 2 pi
    MFx32 ratio;
    if (a >= b) {
        ratio = (MFx32(1) - base) * sine + base;
    } else {
        ratio = (base - low) * sine + base;
    }
    if (ratio > MFx32(1)) {
        ratio = MFx32(1);
    }
    return ratio;
}

// A vector member the game emits in this file (made explicitly until the code that uses it is decompiled).
// NONMATCHING: its two int2type temporaries swap stack slots (they match with `#pragma ipa file`, which this file
// can't take yet: three other functions stop matching).
#pragma auto_inline off
template void std::__vec_deleter<DinoBattlePlayerEvent, std::allocator<DinoBattlePlayerEvent> >::reallocate_clear(
    size_t n, size_t max);
