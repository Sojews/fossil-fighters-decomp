// CreatePlayerUpdateAbilityIconTask.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_message.h"
#include <algorithm>
#include "m/resource.h"
#include "game/create_player_update_ability_icon_task.h"
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
    func_ov000_02163464((void *)mUnk10, mUnk14);
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
    func_ov000_02163464(mPlayer, add + point);
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

// Adds the view's side of an ability to set: the attack bar, the ability itself, the fade and the targets'
// coloring, then undoes them.
void DinoBattleModelPlayer::AddAbilitySequence(MTaskSet *set, u32 a, u32 b, u32 attacker,
                                               std::vector<u32> &targets, u8 recolor) {
    u32 ids[6] = {attacker};
    u32 n = 1;
    for (u32 i = 0; i < targets.size(); i++) {
        if (attacker != targets[i]) {
            ids[n++] = targets[i];
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

extern "C" BOOL func_ov000_0216d2cc(DinoBattleModelCreature *creature, int arg);

template MTask *DinoCreatePostAndWaitTask(MMessageObserver *target, DinoBattleViewPlayerWaitKPCountStopRequest *msg);
template MTask *DinoCreatePostAndWaitTask(MMessageObserver *target, DinoBattleViewCreaturePlayMotionRequest *msg);
template MTask *DinoCreatePostAndWaitTask(MMessageObserver *target, DinoBattleView3DTeamAttackRequest *msg);

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

static MFx32 GetAffinityRatio(DinoBattleModelCreature *creature, DinoBattleModelCreature *other, u32 attack) {
    int result = creature->CompareUnkECA0(other, attack);
    if (result == 1) {
        return gDinoResource->GetBattleAdjustDefs()->affinityRatio[0];
    }
    if (result == 2) {
        return gDinoResource->GetBattleAdjustDefs()->affinityRatio[1];
    }
    return gDinoResource->GetBattleAdjustDefs()->affinityRatio[2];
}
