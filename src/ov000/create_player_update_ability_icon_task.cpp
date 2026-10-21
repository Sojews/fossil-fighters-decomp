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
#include "game/dino_static_init.h"
#include "game/info_panel_window.h"
#include "game/dino_battle_view_class.h"
#include "game/dino_battle_debug_log.h"
#include <stdio.h>







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
    ((DinoBattleModelPlayer *)mUnk10)->SendUpdateStateIcon((DinoBattleModelCreature *)mUnk14, mUnk18);
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

extern "C" MTask *func_ov000_0216dec4(DinoBattleModelCreature *creature);
extern "C" void func_ov000_0216c90c(DinoBattleModelCreature *creature, u32 id, BOOL resetStatus, BOOL fullLife);
extern "C" MTask *func_ov000_0216d4b0(DinoBattleModelCreature *creature, u32 a, u32 d, u32 e, u8 g);
extern "C" MParallelTaskSet *func_ov000_0215bf44(DinoBattleModelField *field, u32 sides);
extern "C" const u32 data_0208a0c8, data_0208a0e0, data_0208a0e4;   // timer frames

// Adds four tasks (the arguments are evaluated last to first), keeping the set in one register.
static inline void DinoAddTail4(MTaskSet *set, MTask *a, MTask *b, MTask *c, MTask *d) {
    MTaskSet *self = set;
    self->AddTail(a);
    self->AddTail(b);
    self->AddTail(c);
    self->AddTail(d);
}

static inline void DinoAddTail9(MTaskSet *set, MTask *a, MTask *b, MTask *c, MTask *d, MTask *e, MTask *f, MTask *g,
                                MTask *h, MTask *i) {
    MTaskSet *self = set;
    self->AddTail(a);
    self->AddTail(b);
    self->AddTail(c);
    self->AddTail(d);
    self->AddTail(e);
    self->AddTail(f);
    self->AddTail(g);
    self->AddTail(h);
    self->AddTail(i);
}

// Carries out a special attack's effect by its kind, then ends.
void ExecSpecialAttackTask::Idle() {
    switch (mKind) {
    case 7: {
        DinoBattleModelCreature *creature = (DinoBattleModelCreature *)mTarget;
        mSerial.AddTail(func_ov000_0216dec4(creature), creature->mPlayer->CreateFieldEffectWaitTask(),
                        MTaskExitTransfer::Create(this));
        return;
    }
    case 16: {
        DinoBattleModelCreature *creature = (DinoBattleModelCreature *)mTarget;
        DinoBattleModelField *field = (DinoBattleModelField *)creature->mPlayer->mUnk100;
        mSerial.AddTail(func_ov000_0215bf44(field, 1 << creature->mPlayer->GetOpponent()->GetUnkB8()));
        mSerial.AddTail(creature->mPlayer->CreateUpdateCreaturesTask(), creature->mPlayer->CreateUpdateAbilityIconTask(1),
                        MTaskExitTransfer::Create(this));
        return;
    }
    case 15: {
        DinoBattleModelCreature *creature = (DinoBattleModelCreature *)mTarget;
        DinoSignalWait *wait = new DinoSignalWait(0x1005C, NULL, NULL, 1);
        creature->Post(new DinoBattleModelCreatureReleaseStatusEffectRequest(wait, 0xFC));
        DinoSoundPlaySe(0x51, 0);
        mSerial.AddTail(wait, MTaskExitTransfer::Create(this));
        return;
    }
    case 9: {
        s32 amount = mValue;
        DinoBattleModelCreature *creature = (DinoBattleModelCreature *)mTarget;
        MTask *add = new DinoBattleModelCreature::AddLifePointTask(creature, amount, TRUE);
        DinoAddTail4(&mSerial, add, creature->CreateUpdateTask(1, 0), DinoSoundCreatePlaySeTask(0x51, 0),
                     MTaskExitTransfer::Create(this));
        return;
    }
    case 10: {
        DinoBattleModelPlayer *player = (DinoBattleModelPlayer *)mTarget;
        u32 ids[6] = {0, 0, 0, 0, 0, 0};
        u32 count = player->GetCreatureCount();
        for (u32 j = 0; j < count; j++) {
            ids[j] = player->GetCreature(j)->GetUnkB4();
        }
        mSerial.AddTail(player->CreateUpdateColorTask(ids, count, 0x20));
        u32 i = 0;
        if (i < count) {
            MSerialTaskSet *serial = &mSerial;
            for (; i < count; i++) {
                DinoBattleModelCreature *creature = player->GetCreature(i);
                serial->AddTail(new AddLifePointTask(creature, mValue, TRUE), DinoSoundCreatePlaySeTask(0x51, 0),
                                creature->CreateUpdateTask(1, 0), new MTimer(data_0208a0c8));
            }
        }
        mSerial.AddTail(MTaskExitTransfer::Create(this));
        return;
    }
    case 11: {
        DinoBattleModelPlayer *player = (DinoBattleModelPlayer *)mTarget;
        DinoBattleModelCreature *user = mUser;
        int count = player->GetCreatureCount();
        if (count > 1) {
            mSerial.AddTail(new MTimer(data_0208a0e0));
            int heal = (mValue - 1) / (count - 1);
            BOOL healed = FALSE;
            for (u32 i = 0; i < (u32)count; i++) {
                DinoBattleModelCreature *creature = player->GetCreature(i);
                if (creature != user) {
                    creature->AddLife(heal, TRUE);
                    mSerial.AddTail(creature->CreateUpdateTask(1, 0));
                    healed = TRUE;
                }
            }
            if (healed) {
                DinoSoundPlaySe(0x3B2, 0);
            }
            MTask *wait;
            MTask *post;
            MTask *exit = MTaskExitTransfer::Create(this);
            MMessageObserver *view = player->mUnkF4;
            post = view->CreatePostTask(new DinoBattleViewSetCreaturePriorityRequest(0));
            wait = player->CreateFieldEffectWaitTask();
            DinoAddTail4(&mSerial, new MTimer(10), wait, post, exit);
            return;
        }
        break;
    }
    case 12: {
        DinoBattleModelPlayer *player = (DinoBattleModelPlayer *)mTarget;
        DinoBattleModelCreature *user = mUser;
        int count = player->GetCreatureCount();
        int best = 0;
        DinoBattleModelCreature *target = NULL;
        if (count > 1) {
            for (u32 i = 0; i < (u32)count; i++) {
                DinoBattleModelCreature *creature = player->GetCreature(i);
                if (creature != user && best < (int)creature->GetUnkE4()) {
                    best = creature->GetUnkE4();
                    target = creature;
                }
            }
            int amount = best - 1;
            if (target != NULL) {
                target->AddLife(-amount, 0);
                u32 ids[6] = {0, 0, 0, 0, 0, 0};
                ids[0] = target->GetUnkB4();
                ids[1] = user->GetUnkB4();
                MMessageObserver *view = player->mUnkF4;
                mSerial.AddTail(view->CreatePostTask(new DinoBattleViewCreatureSetBlendRequest(0, 2, ids, TRUE)));
                ids[1] = 0;
                mSerial.AddTail(player->CreateUpdateColorTask(ids, 1, 0x20));
                DinoAddTail9(&mSerial, func_ov000_0216d4b0(target, 0, amount, 2, 0), new MTimer(data_0208a0e4),
                                new DinoBattleModelCreature::AddLifePointTask(user, amount, TRUE),
                                user->CreateUpdateTask(1, 0), DinoSoundCreatePlaySeTask(0x3B2, 0), new MTimer(1),
                                DinoBattleViewPopNumber::CreateWaitTask(), player->CreateUpdateColorTask(ids, 1, 0),
                                MTaskExitTransfer::Create(this));
                return;
            }
        }
        break;
    }
    case 2: {
        DinoBattleModelCreature *creature = (DinoBattleModelCreature *)mTarget;
        func_ov000_0216c90c(creature, mValue, FALSE, TRUE);
        mSerial.AddTail(creature->mPlayer->CreateUpdateCreaturesTask(), creature->mPlayer->CreateUpdateAbilityIconTask(1),
                        MTaskExitTransfer::Create(this));
        return;
    }
    }
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

extern "C" MTask *func_ov000_02166620(DinoBattleModelPlayer *player, u32 id);

void ExecCreatureEndTask::Idle() {
    MTask *exit = MTaskExitTransfer::Create(this);
    MTask *task = func_ov000_02166620((DinoBattleModelPlayer *)mUnk74, mUnk78);
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

extern "C" BOOL func_ov000_0216cfac(DinoBattleModelCreature *creature, u32 status, u32 a, int b, u32 c, u8 d);

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

// The statuses that wear off, with their turn counters (filled in with the creature's).
struct DinoBattleStatusCountdown {
    u32 status;
    s8 *turns;
};

struct DinoBattleStatusCountdownTable {
    DinoBattleStatusCountdown entries[11];
};

extern "C" const DinoBattleStatusCountdownTable data_ov000_0218fbdc;
extern "C" const u32 data_0208a27c;   // a status wearing off, by status (opponent side)
extern "C" const u32 data_0208a280;   // (own side)
extern "C" const u32 data_0208a294;   // the status's damage (opponent side)
extern "C" const u32 data_0208a298;   // (own side)
extern "C" const u32 data_0208a29c;   // statuses released (opponent side)
extern "C" const u32 data_0208a2a0;   // (own side)

template MTask *DinoCreatePostAndWaitTask(MMessageObserver *target, DinoBattleViewWaitActionInfoBarRequest *msg);

// (The post-and-wait instance is made explicitly above, so the report doesn't dock this function on it.)
void DinoBattleModelCreature::OnMessage(MMessage *msg) {
    switch (msg->mId) {
    case 0x1003A: {
        DinoBattleModelCreatureTurnBeginRequest *req = (DinoBattleModelCreatureTurnBeginRequest *)msg;
        if (req->mUnk10 != mPlayer->GetUnkB8()) {
            return;
        }
        SendUpdate(0, 0);
        SetUnkC0(GetUnkB8());
        ClearUnkC4();
        if (req->mUnk0C != 0) {
            ((MMessageObserver *)req->mUnk0C)->Post(new DinoBattleModelCreatureTurnBeginResult());
        }
        break;
    }
    case 0x10025: {
        DinoBattleModelCreatureAbilityRequest *req = (DinoBattleModelCreatureAbilityRequest *)msg;
        if (req->mUnk14 == mUnkB4) {
            return;
        }
        break;
    }
    case 0x10015: {
        DinoBattleModelTurnEndRequest *req = (DinoBattleModelTurnEndRequest *)msg;
        if (req->mUnk10 != mPlayer->GetUnkB8()) {
            return;
        }
        EndAction();
        (*(int *)req->mUnk14)++;
        if (mStatus.flags & 4) {
            mSerial.AddTail(mView->CreatePostTask(new DinoBattleViewActionInfoBarUpdateRequest(
                mPlayer->mUnkB4 == 0 ? data_0208a298 : data_0208a294)));
            int damage = (int)(GetUnkE8() * mStatus.unk1C) / 100;
            if (damage == 0) {
                damage = 1;
            }
            AddLife(-damage, 0);
            GetSerial()->AddTail(func_ov000_0216d4b0(this, 0, damage, 2, 0), DinoSoundCreatePlaySeTask(0x5B, 0),
                                 new MTimer(10), DinoBattleViewPopNumber::CreateWaitTask());
        }
        if (GetUnkE4() != 0) {
            DinoBattleStatusCountdownTable table = data_ov000_0218fbdc;
            table.entries[0].turns = &mStatus.unk04[0];
            table.entries[1].turns = &mStatus.unk04[1];
            table.entries[2].turns = &mStatus.unk04[2];
            table.entries[3].turns = &mStatus.unk04[3];
            table.entries[4].turns = &mStatus.unk04[5];
            table.entries[5].turns = &mStatus.unk04[6];
            table.entries[6].turns = &mStatus.unk04[7];
            table.entries[7].turns = &mStatus.unk04[4];
            table.entries[8].turns = &mStatus.unk04[8];
            table.entries[9].turns = &mStatus.unk04[9];
            for (int i = 0; table.entries[i].turns != NULL; i++) {
                DinoBattleStatusCountdown *entry = &table.entries[i];
                u32 bit = mStatus.flags & (1 << entry->status);
                if (bit == 0 || (mUnkC4 & bit)) {
                    continue;
                }
                (*entry->turns)--;
                MParallelTaskSet *tasks = new MParallelTaskSet(TRUE);
                DinoSignalWait *wait = new DinoSignalWait(0x10047, NULL, NULL, 1);
                MTask *icon = mPlayer->CreateUpdateStateIconTask((u32)this, (u32)(MMessageObserver *)wait);
                tasks->AddTail(wait);
                tasks->AddTail(icon);
                GetSerial()->AddTail(tasks);
                if (*entry->turns > 0) {
                    continue;
                }
                mStatus.flags &= ~bit;
                *entry->turns = 0;
                GetSerial()->AddTail(DinoCreatePostAndWaitTask(mView, new DinoBattleViewWaitActionInfoBarRequest(0)),
                                mView->CreatePostTask(new DinoBattleViewActionInfoBarUpdateRequest(
                                    i + (mPlayer->mUnkB4 == 0 ? data_0208a280 : data_0208a27c))));
                MParallelTaskSet *tasks2 = new MParallelTaskSet(TRUE);
                DinoSignalWait *wait2 = new DinoSignalWait(0x1006C, NULL, NULL, 1);
                MTask *post = mView->CreatePostTask(
                    new DinoBattleViewWaitActionInfoBarRequest((u32)(MMessageObserver *)wait2));
                tasks2->AddTail(wait2);
                tasks2->AddTail(post);
                GetSerial()->AddTail(tasks2);
            }
        }
        mSerial.AddTail(((MMessageObserver *)req->mUnk0C)->CreatePostTask(new DinoBattleModelTurnEndResult()));
        break;
    }
    case 0x1005B: {
        DinoBattleModelCreatureReleaseStatusEffectRequest *req = (DinoBattleModelCreatureReleaseStatusEffectRequest *)msg;
        mStatus.flags &= ~req->mUnk10;
        MParallelTaskSet *tasks = new MParallelTaskSet(TRUE);
        DinoSignalWait *wait = new DinoSignalWait(0x10047, NULL, NULL, 1);
        MTask *icon = mPlayer->CreateUpdateStateIconTask((u32)this, (u32)(MMessageObserver *)wait);
        tasks->AddTail(wait);
        tasks->AddTail(icon);
        GetSerial()->AddTail(mPlayer->CreateMotionEffectTask(GetUnkB4(), 4, 0, 0, -1),
                             mView->CreatePostTask(new DinoBattleViewActionInfoBarUpdateRequest(
                                 mPlayer->mUnkB4 == 0 ? data_0208a2a0 : data_0208a29c)),
                             tasks, CreateUpdateTask(0, 0));
        if (req->mUnk0C != 0) {
            mSerial.AddTail(((MMessageObserver *)req->mUnk0C)
                                ->CreatePostTask(new DinoBattleModelCreatureReleaseStatusEffectResult()));
        }
        break;
    }
    case 0x1004F: {
        DinoBattleModelCreatureSupportEffectRequest *req = (DinoBattleModelCreatureSupportEffectRequest *)msg;
        if (req->mUnk10 == mUnkB4) {
            SetUnk108(req->mUnk14);
            DinoSignalWait *wait = new DinoSignalWait(0x10052, (MMessageObserver *)req->mUnk0C,
                                                      new DinoBattleModelCreatureSupportEffectResult(), 1);
            mSerial.AddTail(wait);
            mView->Post(new DinoBattleViewCreatureSupportEffectRequest((u32)(MMessageObserver *)wait, req->mUnk10,
                                                                       req->mUnk14));
        }
        break;
    }
    case 0x10012: {
        DinoBattleModelCreatureMoveRequest *req = (DinoBattleModelCreatureMoveRequest *)msg;
        if (req->mUnk14 == mUnkB4) {
            mMoveReply = (MMessageObserver *)req->mUnk0C;
            mView->Post(new DinoBattleViewCreatureMoveRequest((u32)(MMessageObserver *)this, req->mUnk10, req->mUnk14,
                                                              req->mUnk18, req->mUnk1C, req->mUnk20));
        }
        break;
    }
    case 0x10022: {
        DinoBattleViewCreatureMoveResult *res = (DinoBattleViewCreatureMoveResult *)msg;
        ChangeUnkB8(res->mUnk0C);
        SendUpdate(0, 0);
        if (res->mUnk0C == 0) {
            DinoBattleModelCreature *front = mPlayer->FindCreature(0);
            if (front != NULL) {
                mPlayer->SendUpdateStateIcon(front, 0);
            }
        }
        if (mMoveReply != NULL) {
            mMoveReply->Post(new DinoBattleModelCreatureMoveResult());
        }
        mPlayer->SendUpdateAbilityIconRequest(1, 0);
        break;
    }
    case 0x1003E: {
        DinoBattleModelCreatureSpecialLifeRecoveryRequest *req = (DinoBattleModelCreatureSpecialLifeRecoveryRequest *)msg;
        if (req->mUnk10 == mPlayer->GetUnkB8() && GetDefUnk61() == 6) {
            AddLife((MFx32(mUnkE8) * MFx32(mDef->mUnk62) * MFx32::FromRaw(0x29)).v >> FX32_SHIFT, 0);
            SendUpdate(1, 0);
            DinoSoundPlaySe(0x51, 0);
            if (req->mUnk0C != 0) {
                ((MMessageObserver *)req->mUnk0C)->Post(new DinoBattleModelCreatureSpecialLifeRecoveryResult());
            }
        }
        break;
    }
    }
}

void DinoBattleModelCreature::ChangeUnkB8(u32 value) {
    ResetUnk108();
    ResetStatus();
    mPlayer->SendUpdateStateIcon(this, 0);
    mUnkB8 = value;
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
extern "C" int func_ov000_02162ad8(DinoBattleModelCreature *attacker, DinoBattleModelCreature *target, MFx32 unk2,
                                   MFx32 unk3, MFx32 unk4, int *damage, int *out, u32 flags) {
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

extern "C" void func_ov000_0216c90c(DinoBattleModelCreature *creature, u32 id, BOOL resetStatus, BOOL fullLife);

DinoBattleModelCreature::DinoBattleModelCreature(MMessageObserver *view, DinoBattleModelPlayer *player, u32 id,
                                                 u32 unkB4, u32 unkB8, DinoBattleCreatureStats stats, s32 unkFC,
                                                 u32 unk100, u32 unk178)
    : MProcess(NULL), mView(view), mPlayer(player), mUnkB4(unkB4), mUnkB8(unkB8), mUnkBC(unkB8), mStats(stats),
      mUnkFC(unkFC), mUnk100(unk100), mUnk178(unk178) {
    mActing = FALSE;
    func_ov000_0216c90c(this, id, TRUE, FALSE);
}

// What the creature stats are worked out from, and the stats (main's func_02030f38).
struct DinoCreatureStatsInput {
    u32 id;         // 0x00
    int rank;       // 0x04
    int bonus[4];   // 0x08: by part
    int levels[5];  // 0x18: the parts', then the creature's own
};
struct DinoCreatureStats {
    int hp;         // 0x00
    int stats[5];   // 0x04
};
extern "C" DinoCreatureStats func_02030f38(const DinoCreatureStatsInput *info, BOOL fixed, fx32 fraction);

// Sets the creature up as creature id: its definition, the sides its support effects touch, its attacks' element
// masks and its stats (with full life, or the life its rank's experience gives).
// NONMATCHING (94%): the game loads all the stats before storing them, and spills fullLife on entry.
extern "C" void func_ov000_0216c90c(DinoBattleModelCreature *creature, u32 id, BOOL resetStatus, BOOL fullLife) {
    creature->mUnkB0 = id;
    creature->mDef = (DinoBattleCreatureDef *)gDinoResource->GetCreatureDef(id);
    if (resetStatus) {
        creature->ResetStatus();
    }
    creature->unk110[0] = 0;
    for (int i = 0; i < 2; i++) {
        const s8 *effect = i == 0 ? DinoOffsetPtr<s8>(creature->mDef, creature->mDef->mUnk54)
                                  : DinoOffsetPtr<s8>(creature->mDef, creature->mDef->mUnk58);
        if (effect[0] != 0 || effect[1] != 0 || effect[2] != 0 || effect[3] != 0) {
            creature->unk110[0] |= 1 << i;
        }
    }
    creature->unk110[1] = 0;
    creature->unk110[2] = 0;
    for (u32 i = 0; i < creature->CountAttacks(); i++) {
        const DinoAttackDef *def = gDinoResource->GetAttackDef(creature->GetAttack(i, NULL));
        creature->unk110[1] |= 1 << *DinoOffsetPtr<u8>(def, def->unk10);
        int kind = *DinoOffsetPtr<u8>(def, def->unk14);
        if (kind > 1) {
            creature->unk110[2] |= 0x800 << (kind - 2);
        }
    }
    u32 special = creature->GetSpecialAttack();
    if (special != 0) {
        const DinoAttackDef *def = gDinoResource->GetAttackDef(special);
        creature->unk110[1] |= 1 << *DinoOffsetPtr<u8>(def, def->unk10);
        int kind = *DinoOffsetPtr<u8>(def, def->unk14);
        if (kind > 1 && kind != 3) {
            creature->unk110[2] |= 0x800 << (kind - 2);
        }
    }
    int unk61 = creature->GetDefUnk61();
    if (unk61 > 1) {
        creature->unk110[2] |= 1 << (unk61 - 2);
    }
    DinoCreatureStatsInput info;
    info.levels[0] = info.levels[1] = info.levels[2] = info.levels[3] = 0;
    MI_CpuFill8(&info, 0, sizeof(info));
    info.id = id;
    info.rank = creature->mUnkFC;
    const DinoCreatureBody *body = (const DinoCreatureBody *)&creature->mStats.v[1];
    info.levels[4] = ((const DinoCreatureState *)&creature->mStats)->unk0_8;
    for (int i = 0; i < 4; i++) {
        info.bonus[i] = body->parts[i].unk12;
        info.levels[i] = body->parts[i].level;
    }
    const DinoCreatureStats &stats = func_02030f38(&info, creature->mPlayer->IsCpu(), creature->mUnk178);
    int life = stats.hp;
    creature->mUnkEC = stats.stats[0];
    creature->mUnkF0 = stats.stats[1];
    creature->mUnkF4 = stats.stats[2];
    creature->mUnkF8 = stats.stats[3];
    creature->unk104 = stats.stats[4];
    if (fullLife) {
        life = ((const DinoCreatureDef *)creature->mDef)->GetHpTable()[creature->mUnkFC - 1];
    }
    creature->mUnkE8 = creature->mUnkE4 = life;
    creature->mUnk108 = 0;
}

MFx32 DinoBattleModelCreature::RandomFraction() {
    return mPlayer->RandomFraction();
}

// NONMATCHING: each operator+'s two by-value parameter copies take each other's stack slots (also under -ipa file).
MFx32 DinoBattleModelCreature::CalcUnkEC(DinoBattleModelCreature *target) {
    MFx32 base(mUnkEC);
    MFx32 result = base;
    const MFx32 pinch = gDinoResource->GetBattleAdjustDefs()->unk3C;
    if (GetDefUnk61() != 8) {
        if (GetUnkB8() == 0) {
            for (int i = 0; i < 2; i++) {
                DinoBattleModelCreature *ally = mPlayer->FindCreature(i ? 2 : 1);
                DinoBattleModelCreature *enemy = mPlayer->GetOpponent()->FindCreature(i ? 2 : 1);
                if (ally != NULL && (ally->GetUnk108() & 1)) {
                    DinoBattleCreatureDef *def = ally->GetDef();
                    const s8 *support = def->mUnk54 == 0 ? NULL : (const s8 *)def + def->mUnk54;
                    result = result + MFx32(support[0]) * base / MFx32(100);
                }
                if (enemy != NULL && (enemy->GetUnk108() & 2)) {
                    DinoBattleCreatureDef *def = enemy->GetDef();
                    const s8 *support = def->mUnk58 == 0 ? NULL : (const s8 *)def + def->mUnk58;
                    result = result + MFx32(support[0]) * base / MFx32(100);
                }
            }
        }
        if (mStatus.flags & 0x80) {
            result = result + MFx32(mStatus.unk24);
        }
        if (mStatus.flags & 0x200) {
            result = result + MFx32(mStatus.unk30);
        }
    }
    if (IsInPinch() && GetDefUnk61() == 5) {
        result = result * (pinch * MFx32(mDef->mUnk62) * MFx32::FromRaw(0x19A));
    }
    return result;
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

extern "C" const int data_0208a10c;
extern "C" const int data_0208a114;

static inline MTimer *NewTimer(const int &frames) {
    return new MTimer(frames);
}

// Evens out both players' kaseki points, with an effect and a sound for each side that changes.
// NONMATCHING: block order of the side flag (the game puts the FALSE arm first).
extern "C" MTask *func_ov000_0216dec4(DinoBattleModelCreature *creature) {
    int points[2];
    int average = 0;
    for (int i = 0; i < 2; i++) {
        int p = (i != 0 ? creature->mPlayer->GetOpponent() : creature->mPlayer)->GetUnk128();
        average += p;
        points[i] = p;
    }
    average /= 2;
    MSerialTaskSet *tasks = new MSerialTaskSet(TRUE);
    if (points[0] == average && points[1] == average) {
        tasks->AddTail(NewTimer(data_0208a114));
    } else {
        for (int i = 0; i < 2; i++) {
            DinoBattleModelPlayer *player = i != 0 ? creature->mPlayer->GetOpponent() : creature->mPlayer;
            tasks->AddTail(player->CreateSetKasekiPointTask(average));
            if (average != points[i]) {
                BOOL side = player->mUnkB4 == 0 ? FALSE : TRUE;
                if (average > points[i]) {
                    MTask *post = creature->mView->CreatePostTask(new DinoBattleViewFreeCoordEffectRequest(side, 0));
                    tasks->AddTail(DinoSoundCreatePlaySeTask(0x51, 0));
                    tasks->AddTail(post);
                } else {
                    MTask *post = creature->mView->CreatePostTask(new DinoBattleViewFreeCoordEffectRequest(side, 1));
                    tasks->AddTail(DinoSoundCreatePlaySeTask(0x4e, 0));
                    tasks->AddTail(post);
                }
            }
            tasks->AddTail(player->CreateUpdateTask(0));
            if (i == 0) {
                tasks->AddTail(NewTimer(data_0208a10c));
            }
        }
    }
    return tasks;
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

struct DinoBattleStatusEntry {
    BOOL unk00;
    u32 status;
    s8 *unk08;
    u8 *unk0C;
};

struct DinoBattleStatusTable {
    DinoBattleStatusEntry entries[11];
};

extern "C" const DinoBattleStatusTable data_ov000_0218fc34;
extern "C" const u32 data_0208a284;  // first status text, opponent side
extern "C" const u32 data_0208a288;  // first status text, own side

extern "C" BOOL func_ov000_0216cfac(DinoBattleModelCreature *creature, u32 status, u32 a, int b, u32 c, u8 d) {
    BOOL result = FALSE;
    if (creature->GetDefUnk61() == 8) {
        return FALSE;
    }
    DinoBattleStatusTable table = data_ov000_0218fc34;
    table.entries[0].unk08 = &creature->mStatus.unk04[0];
    table.entries[0].unk0C = &creature->mStatus.unk10[0];
    table.entries[1].unk08 = &creature->mStatus.unk04[1];
    table.entries[1].unk0C = &creature->mStatus.unk10[1];
    table.entries[2].unk08 = &creature->mStatus.unk04[2];
    table.entries[2].unk0C = &creature->mStatus.unk10[2];
    table.entries[3].unk08 = &creature->mStatus.unk04[3];
    table.entries[3].unk0C = &creature->mStatus.unk10[3];
    table.entries[4].unk08 = &creature->mStatus.unk04[5];
    table.entries[4].unk0C = &creature->mStatus.unk10[5];
    table.entries[5].unk08 = &creature->mStatus.unk04[6];
    table.entries[5].unk0C = &creature->mStatus.unk10[6];
    table.entries[6].unk08 = &creature->mStatus.unk04[7];
    table.entries[6].unk0C = &creature->mStatus.unk10[7];
    table.entries[7].unk08 = &creature->mStatus.unk04[4];
    table.entries[7].unk0C = &creature->mStatus.unk10[4];
    table.entries[8].unk08 = &creature->mStatus.unk04[8];
    table.entries[8].unk0C = &creature->mStatus.unk10[8];
    table.entries[9].unk08 = &creature->mStatus.unk04[9];
    table.entries[9].unk0C = &creature->mStatus.unk10[9];

    u32 text = data_0208a288;
    if (creature->mPlayer->mUnkB4 == 0) {
        text = data_0208a284;
    }
    creature->mView->Post(new DinoBattleViewActionInfoBarUpdateRequest(text + status - 2));

    u32 old = creature->mStatus.flags;
    u32 bit = 1 << status;
    creature->mStatus.flags = old | bit;
    for (int i = 0; table.entries[i].unk08 != NULL; i++) {
        if (status == table.entries[i].status) {
            if (table.entries[i].unk00) {
                creature->mUnkC4 |= bit;
            }
            if (old & bit) {
                result = TRUE;
            }
            *table.entries[i].unk08 = a;
            *table.entries[i].unk0C = d;
            switch (status) {
            case 2:
                creature->mStatus.unk1C = b;
                break;
            case 4: {
                creature->mStatus.unk20 = 0;
                int count = creature->CountAttacks();
                if (b >= count) {
                    creature->mStatus.unk20 = (1 << count) - 1;
                } else {
                    while (b != 0) {
                        u32 mask = 1 << creature->mPlayer->GetRandom()->Next(count);
                        if (!(creature->mStatus.unk20 & mask)) {
                            b--;
                            creature->mStatus.unk20 |= mask;
                        }
                    }
                }
                break;
            }
            case 7:
                creature->mStatus.unk24 = b;
                creature->mStatus.unk28 = c;
                break;
            case 8:
                creature->mStatus.guardChance = b;
                break;
            case 9:
                creature->mStatus.unk30 = b;
                creature->mStatus.unk34 = c;
                break;
            case 10:
                creature->mStatus.unk38 = b;
                break;
            case 11:
                creature->mStatus.unk3C = b;
                break;
            }
            break;
        }
    }
    return result;
}

void DinoBattleModelCreature::ClearStatus8() {
    mStatus.flags &= ~8;
    mStatus.unk04[1] = 0;
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

// Takes the creature off the field: the view ends it, then the game ends (the other side winning) when the side has
// no creatures left, or else the side gets the kaseki points for it; then the icons and creatures update.
extern "C" MTask *func_ov000_02166620(DinoBattleModelPlayer *player, u32 id) {
    DinoBattleModelCreature *creature = player->FindCreatureByB4(id);
    creature->UpdateMarks();
    if (creature->GetUnkB8() == 0) {
        player->SendUpdateStateIcon(creature, 0);
    }
    player->SendUpdateAbilityIconRequest(TRUE, 0);
    player->mCreatures.erase(std::find(player->mCreatures.begin(), player->mCreatures.end(), creature));
    player->mSubject.RemoveObserver(creature);
    player->mParallel.Remove(creature);
    delete creature;
    MSerialTaskSet *serial = new MSerialTaskSet(TRUE);
    MParallelTaskSet *end = new MParallelTaskSet(TRUE);
    DinoSignalWait *wait = new DinoSignalWait(0x10034, NULL, NULL, 1);
    MTask *post = player->mUnkF4->CreatePostTask(
        new DinoBattleViewCreatureEndRequest((u32)(MMessageObserver *)wait, player->mUnkB8, id));
    end->AddTail(wait);
    end->AddTail(post);
    BOOL over = FALSE;
    DinoBattleParams *params = &player->mBattle->mParams;
    if (player->GetCreatureCount() == 0 || (params->unk158 && (&params->unk08)[player->mUnkB8] == 0)) {
        over = TRUE;
    }
    if (over) {
        ((DinoBattleModel *)player->mBattle->GetUnk1FC())->mUnkA8 = TRUE;
        serial->AddTail(((DinoBattleModel *)player->mUnk100)
                            ->CreatePostTask(new DinoBattleModelGameEndSignal(1 - player->mUnkB8, FALSE, FALSE)));
    } else {
        serial->AddTail(player->CreateAddKasekiPointCreatureDie());
    }
    serial->AddTail(end, player->CreateUpdateAllCreatureTask(), player->CreateUpdateAbilityIconTask(TRUE));
    return serial;
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

// Updates every creature on both sides, waits for them all, and exits.
// NONMATCHING: 87%, the game spills this and keeps the wait in a register (ours the other way round).
void UpdateAllCreatureTask::Idle() {
    int count = mPlayer->GetCreatureCount() + mPlayer->GetOpponent()->GetCreatureCount();
    MParallelTaskSet *set = new MParallelTaskSet(TRUE);
    DinoSignalWait *wait = new DinoSignalWait(0x10030, NULL, NULL, count);
    set->AddTail(wait);
    DinoBattleModelPlayer *player = mPlayer;
    for (int side = 0; side < 2; side++) {
        for (int i = 0; i < player->GetCreatureCount(); i++) {
            set->AddTail(player->GetCreature(i)->CreateUpdateTask(0, (u32)(MMessageObserver *)wait));
        }
        player = player->GetOpponent();
    }
    MTask *exit = MTaskExitTransfer::Create(this);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(set);
    serial->AddTail(exit);
}

extern "C" MTask *func_ov000_02166470(DinoBattleModelPlayer *player);

// (Through a non-member inline, the found creature gets the game's stack slot.)
static inline DinoBattleModelCreature *DinoFindByB4(DinoBattleModelPlayer *player, u32 id) {
    return player->FindCreatureByB4(id);
}

// Swaps two of the player's creatures (the first moving to the escape area, position 3): a task set that waits for
// both moves and then signals reply with msg.
extern "C" void func_ov000_021661b4(DinoBattleModelPlayer *player, MMessageObserver *reply, MMessage *msg, u32 id,
                                    u32 otherId) {
    player->SetUnk124(player->GetUnk124() | 2);
    player->mUnk120 = 0;
    MParallelTaskSet *set = new MParallelTaskSet(TRUE);
    DinoSignalWait *wait = new DinoSignalWait(0x10013, reply, msg, 2);
    set->AddTail(wait);
    DinoBattleModelCreature *creature = player->FindCreatureByB4(id);
    creature->ResetStatus();
    MParallelTaskSet *icons = new MParallelTaskSet(TRUE);
    DinoSignalWait *iconWait = new DinoSignalWait(0x10047, NULL, NULL, 1);
    icons->AddTail(iconWait, player->CreateUpdateStateIconTask((u32)creature, (u32)(MMessageObserver *)iconWait));
    player->mSerial.AddTail(icons);
    creature->BeginAction();
    set->AddTail(creature->CreatePostTask(new DinoBattleModelCreatureMoveRequest(
        (u32)(MMessageObserver *)wait, player->mUnkB8, id, creature->GetUnkB8(), 3, 0, (u32)&player->mUnk120)));
    DinoBattleModelCreature *other = DinoFindByB4(player, otherId);
    set->AddTail(other->CreatePostTask(new DinoBattleModelCreatureMoveRequest(
        (u32)(MMessageObserver *)wait, player->mUnkB8, otherId, other->GetUnkB8(), 0, 0, (u32)&player->mUnk120)));
    player->GetSerial()->AddTail(set, func_ov000_02166470(player),
                                 new CreatePlayerUpdateAbilityIconTask(TRUE, 0, (u32)player),
                                 player->mUnkF4->CreatePostTask(new DinoBattleViewCreatureActionUpdateSignal(id)));
}

// Updates every creature on both sides, and waits for them all (NULL with no creatures).
extern "C" MTask *func_ov000_02166470(DinoBattleModelPlayer *player) {
    int count = player->GetCreatureCount() + player->GetOpponent()->GetCreatureCount();
    if (count != 0) {
        DinoSignalWait *wait = new DinoSignalWait(0x10030, NULL, NULL, count);
        MParallelTaskSet *set = new MParallelTaskSet(TRUE);
        set->AddTail(wait);
        for (int side = 0; side < 2; side++) {
            for (int i = 0; i < player->GetCreatureCount(); i++) {
                set->AddTail(player->GetCreature(i)->CreateUpdateTask(0, (u32)(MMessageObserver *)wait));
            }
            player = player->GetOpponent();
        }
        return set;
    }
    return NULL;
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

extern "C" void func_ov000_02166904(DinoBattleModelPlayer *player, MMessageObserver *reply, u32 side, u32 id,
                                    u32 targetId, u32 attack, u32 unk);
extern "C" void func_ov000_021661b4(DinoBattleModelPlayer *player, MMessageObserver *reply, MMessage *msg, u32 id,
                                    u32 otherId);
extern "C" const u32 data_0208a290, data_0208a28c;  // the confusion's info bar texts (ours, theirs)

// A confused leader's turn: it may lose the turn, use a random attack on a random target, or swap with a support
// creature. reply (if any) gets the confusion's result.
// NONMATCHING: the attack count and the swap target take each other's spill slots.
extern "C" void func_ov000_02165bd0(DinoBattleModelPlayer *player, MMessageObserver *reply) {
    DinoBattleModelCreature *creature = player->FindCreature(0);
    player->mBattle->GetUnk1FC();
    if (creature == NULL || !(creature->GetStatus()->flags & 0x40)) {
        return;
    }
    player->mSerial.AddTail(player->mUnkF4->CreatePostTask(new DinoBattleViewActionInfoBarUpdateRequest(
                                player->mUnkB4 == 0 ? data_0208a290 : data_0208a28c)),
                            DinoCreatePostAndWaitTask(player->mUnkF4, new DinoBattleViewWaitActionInfoBarRequest(0)));
    DinoBattleModelCreature *other;
    u32 attack;
    u32 count = creature->CountAttacks();
    u32 blocked = 0;
    for (u32 i = 0; i < count; i++) {
        const DinoAttackDef *def = gDinoResource->GetAttackDef(creature->GetAttack(i, NULL));
        int cost = *DinoOffsetPtr<u16>(def, def->cost);
        if (player->GetUnk128() < cost) {
            blocked |= 1 << i;
        }
    }
    BOOL allBlocked = FALSE;
    if ((creature->GetStatus()->flags & 0x10) || blocked != 0) {
        blocked |= creature->GetStatusUnk20();
        allBlocked = TRUE;
        for (u32 i = 0; i < count; i++) {
            if (!((1 << i) & blocked)) {
                allBlocked = FALSE;
                break;
            }
        }
    }
    if (!(creature->IsUnkD244() && !allBlocked) && !(creature->IsUnkD274() && creature->mPlayer->IsUnk2163978())) {
        creature->BeginAction();
        MTask *post = player->mUnkF4->CreatePostTask(new DinoBattleViewSetActionedSignal());
        player->mSerial.AddTail(DinoCreatePostAndWaitTask(player->mUnkF4, new DinoBattleViewWaitActionInfoBarRequest(0)),
                                post);
        if (reply == NULL) {
            return;
        }
        player->mSerial.AddTail(reply->CreatePostTask(new DinoBattleModelPlayerConfusionExecResult()));
        return;
    }
    int action;
    do {
        action = player->GetRandom()->Next(2);
        if (action == 0) {
            if (!creature->IsUnkD244()) {
                action = -1;
            }
            attack = player->GetRandom()->Next(creature->CountAttacks());
            const DinoAttackDef *def = creature->GetAttackDef(attack);
            if (*DinoOffsetPtr<u8>(def, def->unk14) == 14 && player->GetCreatureCount() == 1) {
                action = -1;
            }
            if (blocked & (1 << attack)) {
                action = -1;
            }
        }
        if (action == 1) {
            if (!creature->IsUnkD274() || !creature->mPlayer->IsUnk2163978()) {
                action = -1;
            }
        }
    } while (action == -1);
    switch (action) {
    case 0: {
        DinoSignalWait *wait;
        if (reply != NULL) {
            wait = new DinoSignalWait(0x10026, reply, new DinoBattleModelPlayerConfusionExecResult(), 1);
        } else {
            wait = new DinoSignalWait(0x10026, NULL, NULL, 1);
        }
        MSerialTaskSet *seq = new MSerialTaskSet(TRUE);
        seq->AddTail(wait, player->mUnkF4->CreatePostTask(new DinoBattleViewSetCreaturePriorityRequest(0)));
        const DinoAttackDef *def = creature->GetAttackDef(attack);
        DinoBattleModelCreature *target;
        if (*DinoOffsetPtr<u8>(def, def->unk14) == 9 || *DinoOffsetPtr<u8>(def, def->unk14) == 2 ||
            *DinoOffsetPtr<u8>(def, def->unk10) == 8) {
            target = creature;
        } else if (*DinoOffsetPtr<u8>(def, def->unk14) == 15) {
            target = player->FindCreature(0);
        } else if (*DinoOffsetPtr<u8>(def, def->unk14) == 13 || *DinoOffsetPtr<u8>(def, def->unk10) == 9 ||
                   *DinoOffsetPtr<u8>(def, def->unk10) == 10 || *DinoOffsetPtr<u8>(def, def->unk10) == 11) {
            do {
                target = player->GetCreature(player->GetRandom()->Next(player->GetCreatureCount()));
            } while (target->GetUnkB8() == 3);
        } else if (*DinoOffsetPtr<u8>(def, def->unk14) == 14) {
            do {
                target = player->GetCreature(player->GetRandom()->Next(player->GetCreatureCount()));
            } while (target == creature);
        } else {
            do {
                DinoBattleModelPlayer *opponent = player->GetOpponent();
                target = opponent->GetCreature(player->GetRandom()->Next(player->GetOpponent()->GetCreatureCount()));
            } while (target->GetUnkB8() == 3);
        }
        creature->BeginAction();
        player->mParallel.AddTail(seq);
        func_ov000_02166904(player, wait, player->GetUnkB8(), creature->GetUnkB4(), target->GetUnkB4(), attack, 1);
        break;
    }
    case 1: {
        int first = player->GetRandom()->Next(2);
        for (int j = 0; j < 2; j++) {
            other = player->FindCreature(j == first ? 1 : 2);
            if (other != NULL) {
                break;
            }
        }
        if (other != NULL) {
            if (reply != NULL) {
                func_ov000_021661b4(player, reply, new DinoBattleModelPlayerConfusionExecResult(), creature->GetUnkB4(),
                                    other->GetUnkB4());
            } else {
                func_ov000_021661b4(player, NULL, NULL, creature->GetUnkB4(), other->GetUnkB4());
            }
        }
        break;
    }
    }
    player->mUnk124 |= 3;
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

// Moves the creature in the escape area back to the front (with no other creature) or to the first free position,
// then has the user's menu come back up.
void MoveCreatureFromEscape::Idle() {
    DinoBattleController *controller = (DinoBattleController *)static_cast<DinoBattleView *>(
        ((DinoBattleModelField *)mPlayer->mUnk100)->mUnkB4)->GetUnkEC();
    DinoBattleModelCreature *escaped = mPlayer->FindCreature(3);
    if (escaped != NULL && escaped->GetUnkC0() == 3) {
        if (mPlayer->GetCreatureCount() <= 1) {
            MParallelTaskSet *set = new MParallelTaskSet(TRUE);
            DinoSignalWait *wait = new DinoSignalWait(0x10013, NULL, NULL, 1);
            MTask *move = escaped->CreatePostTask(new DinoBattleModelCreatureMoveRequest(
                (u32)(MMessageObserver *)wait, mPlayer->GetUnkB8(), escaped->GetUnkB4(), escaped->GetUnkB8(), 0, 0, 0));
            set->AddTail(wait);
            set->AddTail(move);
            mSerial.AddTail(set);
            if (controller != NULL) {
                mSerial.AddTail(
                    controller->CreatePostTask(new DinoBattleViewUiRootMenuRequest(0, mPlayer->GetUnkB8(), 0, 0, 0, 0)));
            }
        } else {
            for (int i = 0; i < 2; i++) {
                u32 pos = i + 1;
                if (mPlayer->FindCreature(pos) == NULL) {
                    MParallelTaskSet *set = new MParallelTaskSet(TRUE);
                    DinoSignalWait *wait = new DinoSignalWait(0x10013, NULL, NULL, 1);
                    MTask *move = escaped->CreatePostTask(new DinoBattleModelCreatureMoveRequest(
                        (u32)(MMessageObserver *)wait, mPlayer->GetUnkB8(), escaped->GetUnkB4(), escaped->GetUnkB8(),
                        pos, 0, 0));
                    set->AddTail(wait);
                    set->AddTail(move);
                    mSerial.AddTail(set);
                    if (controller != NULL) {
                        mSerial.AddTail(controller->CreatePostTask(
                            new DinoBattleViewUiRootMenuRequest(0, mPlayer->GetUnkB8(), 0, 0, 0, 0)));
                    }
                    break;
                }
            }
        }
    }
    mSerial.AddTail(MTaskExitTransfer::Create(this));
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

// Opens the root menu for the player (on the given item, or one chosen from what it can do with -1), or, with
// nothing to do and ending allowed, ends the turn.
extern "C" void func_ov000_0216b6c0(DinoBattleUserController *controller, int menu, u8 canEnd) {
    if (((DinoBattleModel *)controller->mUnkAC)->mUnkA8) {
        return;
    }
    u32 flags = 0;
    DinoBattleModelPlayer *player = ((DinoBattleModel *)controller->mUnkAC)->GetPlayer(controller->mUnkA8);
    DinoBattleModelCreature *front = player->FindCreature(0);
    u32 mask = 0;
    for (u32 i = 0; i < player->GetCreatureCount(); i++) {
        if (player->GetCreature(i)->IsUnkD244()) {
            mask |= 1 << player->GetCreature(i)->GetUnkB8();
        }
    }
    if (front != NULL) {
        if (!player->AnyCreatureUnkD244()) {
            flags |= 8;
        }
        if (!player->IsUnk2163978()) {
            flags |= 4;
        }
    } else {
        flags |= 0xC;
    }
    if (menu == -1) {
        if (flags == 0xC) {
            if (canEnd) {
                controller->GetSerial()->AddTail(
                    player->CreateFieldEffectWaitTask(), DinoBattleViewPopNumber::CreateWaitTask(),
                    controller->mUnkB0->CreatePostTask(new DinoBattleViewClearAttackBarRequest(0)),
                    DinoCreatePostAndWaitTask(controller->mUnkB0, new DinoBattleViewWaitActionInfoBarRequest(0)),
                    controller->mUnkB0->CreatePostTask(new DinoBattleViewUiTurnEndRequest(
                        (u32)(MMessageObserver *)controller, controller->mUnkA8, TRUE)));
                return;
            }
            menu = 0;
        } else if (!(flags & 8)) {
            menu = 3;
        } else {
            menu = 1;
        }
    }
    controller->mSerial.AddTail(controller->mUnkB0->CreatePostTask(new DinoBattleViewUiRootMenuRequest(
        (u32)(MMessageObserver *)controller, controller->mUnkA8, flags, menu, mask, controller->GetCreatureMask(player))));
    if (((DinoBattleModelField *)((DinoBattleModel *)controller->mUnkAC)->GetUnk10C())->mLive != NULL) {
        controller->mParallel.AddTail(
            ((DinoBattleModel *)controller->mUnkAC)->CreatePostTask(new DinoBattleModelLiveEventSignal(TRUE)));
    }
}

DinoBattleUserController::DinoBattleUserController(void *unkBC, u32 arg, u8 unkB8) : DinoBattleController(arg) {
    mUnkB8 = unkB8;
    mUnkBC = unkBC;
    mThinkTimer = new PlayerThinkTimeCountTask;
    mParallel.AddTail(mThinkTimer);
}

extern "C" const u32 data_ov000_0218fb8c[3];    // the creature slots, by entry
extern "C" MTask *func_ov000_0218ec8c(u32 tutorial, int kind, BOOL *proceed);

// The player's side: sends its player and creatures at the start, and turns the UI's answers into the model's
// requests (or the next menu).
// NONMATCHING: same calls; mwcc's switch tree differs (the game's root is 0x10018 with the jump table over
// 0x1001A-0x10026; ours roots at 0x10011 and tables 0x10018-0x10026), which also moves the literal pools.
void DinoBattleUserController::OnMessage(MMessage *msg) {
    switch (msg->mId) {
    default:
        OS_Terminate();
        break;
    case 0x10003: {
        u32 side = mUnkA8;
        std::string name = ((DinoBattleParams *)mUnkBC)->sides[side].mName;
        ((DinoBattleModel *)mUnkAC)
            ->Post(new DinoBattleModelPlayerBeginSignal(mUnkA8, SeedGenerator::GenerateSeed(), name.c_str(),
                                                        ((DinoBattleParams *)mUnkBC)->sides[side].unk54,
                                                        ((DinoBattleParams *)mUnkBC)->sides[side].unk58,
                                                        ((DinoBattleParams *)mUnkBC)->sides[side].unk89));
        for (int i = 0; i < 3; i++) {
            if ((u8)((DinoBattleParams *)mUnkBC)->sides[side].mCreatures[i].mId != 0) {
                ((DinoBattleModel *)mUnkAC)
                    ->Post(new DinoBattleModelCreatureBeginSignal(
                        mUnkA8, data_ov000_0218fb8c[i], ((DinoBattleParams *)mUnkBC)->sides[side].mCreatures[i],
                        ((DinoBattleParams *)mUnkBC)->sides[mUnkA8].mLevels[i], 0));
            }
        }
        ((DinoBattleModel *)mUnkAC)->Post(new DinoBattleControllerInitResult(mUnkA8));
        break;
    }
    case 0x1000D:
        func_ov000_0216b6c0(this, 3, 0);
        mThinkTimer->Reset();
        mThinkTimer->Start((PlayerThinkTimeCountTask::Player *)static_cast<DinoBattleView *>(mUnkB0));
        break;
    case 0x10011: {
        DinoBattleViewUiRootMenuResult *res = (DinoBattleViewUiRootMenuResult *)msg;
        u32 leaderId = 0;
        DinoBattleModelPlayer *player = ((DinoBattleModel *)mUnkAC)->GetPlayer(mUnkA8);
        DinoBattleModelCreature *leader = player->FindCreature(0);
        if (leader != NULL) {
            leaderId = leader->GetUnkB4();
        }
        switch (res->mUnk0C) {
        case 0:
            if (res->mUnk10 == 12) {
                ((DinoBattleModel *)mUnkAC)->Post(new DinoBattleModelTurnChangeSignal());
            } else {
                mUnkB0->Post(new DinoBattleViewUiTurnEndRequest((u32)(MMessageObserver *)this, mUnkA8, TRUE));
            }
            break;
        case 1:
            mUnkB0->Post(new DinoBattleViewUiCreatureInfoMenuRequest((u32)(MMessageObserver *)this, mUnkA8));
            break;
        case 2: {
            u32 ids[3];
            int count;
            player->CollectCreaturesB8In12(ids, &count);
            mUnkB0->Post(new DinoBattleViewUiChangeMenuRequest((u32)(MMessageObserver *)this, mUnkA8, leaderId, ids, count));
            break;
        }
        case 3: {
            u32 ids[3];
            int count;
            player->CollectCreaturesUnkD244(ids, &count);
            int best = 0x7FFFFFFF;
            u32 attacker = leader->GetUnkB4();
            for (u32 i = 0; i < count; i++) {
                if (player->FindCreatureByB4(ids[i])->IsUnkD244() && best > (int)player->FindCreatureByB4(ids[i])->GetUnkB8()) {
                    best = player->FindCreatureByB4(ids[i])->GetUnkB8();
                    attacker = ids[i];
                }
            }
            if (res->mUnk14 != 0) {
                attacker = res->mUnk14;
            }
            mUnkB0->Post(new DinoBattleViewUiAttackerMenuRequest((u32)(MMessageObserver *)this, mUnkA8, ids, count, attacker,
                                                                 GetCreatureMask(player)));
            break;
        }
        case 4:
            ((DinoBattleModel *)mUnkAC)->Post(new DinoBattleModelGameEndSignal(mUnkA8, TRUE, FALSE));
            break;
        case 5:
            ((DinoBattleModel *)mUnkAC)->Post(new DinoBattleModelGameEndSignal(1 - mUnkA8, TRUE, FALSE));
            break;
        }
        break;
    }
    case 0x1004C: {
        DinoBattleViewUiTurnEndResult *res = (DinoBattleViewUiTurnEndResult *)msg;
        if (res->mUnk0C) {
            if (((DinoBattleModelField *)((DinoBattleModel *)mUnkAC)->GetUnk10C())->mLive != NULL) {
                mParallel.AddTail(((DinoBattleModel *)mUnkAC)->CreatePostTask(new DinoBattleModelLiveEventSignal(FALSE)));
            }
            mSerial.AddTail(((DinoBattleModel *)mUnkAC)->CreatePostTask(new DinoBattleModelTurnChangeSignal()));
            mThinkTimer->mActive = FALSE;
        } else {
            func_ov000_0216b6c0(this, 0, 0);
        }
        break;
    }
    case 0x1001E:
        func_ov000_0216b6c0(this, 1, 0);
        break;
    case 0x1001C: {
        DinoBattleViewUiChangeMenuResult *res = (DinoBattleViewUiChangeMenuResult *)msg;
        if (res->mUnk0C) {
            func_ov000_0216b6c0(this, 2, 0);
        } else {
            ((DinoBattleModel *)mUnkAC)
                ->Post(new DinoBattleModelGoEscapeAreaRequest((u32)(MMessageObserver *)this, mUnkA8, res->mUnk10,
                                                              res->mUnk14));
        }
        break;
    }
    case 0x10020: {
        MTask *tutorial = func_ov000_0218ec8c((u32)((DinoBattleModel *)mUnkAC)->mTutorial, 0x1C, NULL);
        if (tutorial != NULL) {
            mSerial.AddTail(tutorial);
        }
        func_ov000_0216b6c0(this, -1, 1);
        break;
    }
    case 0x10018: {
        DinoBattleViewUiAttackerMenuResult *res = (DinoBattleViewUiAttackerMenuResult *)msg;
        if (res->mUnk10) {
            mSerial.AddTail(mUnkB0->CreatePostTask(new DinoBattleViewClearAttackBarRequest(0)));
            func_ov000_0216b6c0(this, 3, 0);
            break;
        }
        mUnkC4[0] = res->mUnk0C;
        u32 id = res->mUnk0C;
        DinoBattleModelPlayer *player = ((DinoBattleModel *)mUnkAC)->GetPlayer(mUnkA8);
        DinoBattleModelCreature *creature = player->FindCreatureByB4(id);
        mUnkB0->Post(new DinoBattleViewUiAbilityMenuRequest((u32)(MMessageObserver *)this, mUnkA8, id,
                                                            func_ov000_0216d2cc(creature, TRUE), creature->GetStatusUnk20(),
                                                            creature->GetStats(), player->AllCreaturesShareDefFlag(), 0));
        break;
    }
    case 0x10024: {
        DinoBattleViewUiAbilityMenuResult *res = (DinoBattleViewUiAbilityMenuResult *)msg;
        if (res->mUnk14) {
            DinoBattleModelPlayer *player = ((DinoBattleModel *)mUnkAC)->GetPlayer(mUnkA8);
            u32 ids[3];
            int count;
            player->CollectCreaturesUnkD244(ids, &count);
            mSerial.AddTail(mUnkB0->CreatePostTask(new DinoBattleViewUiAttackerMenuRequest(
                (u32)(MMessageObserver *)this, mUnkA8, ids, count, mUnkC4[0], GetCreatureMask(player))));
            break;
        }
        mUnkC4[0] = res->mUnk0C;
        mUnkC4[1] = res->mUnk10;
        DinoBattleModelCreature *creature = ((DinoBattleModel *)mUnkAC)->FindCreature(mUnkC4[0]);
        u32 attack = mUnkC4[1];
        const DinoAttackDef *def = gDinoResource->GetAttackDef(attack < creature->CountAttacks()
                                                                  ? creature->GetAttack(attack, NULL)
                                                                  : creature->GetSpecialAttack());
        u8 all = FALSE;
        if (*DinoOffsetPtr<u8>(def, def->unk14) == 3 || *DinoOffsetPtr<u8>(def, def->unk14) == 10 ||
            *DinoOffsetPtr<u8>(def, def->unk14) == 11 || *DinoOffsetPtr<u8>(def, def->unk14) == 12 ||
            *DinoOffsetPtr<u8>(def, def->unk14) == 7) {
            all = TRUE;
        }
        DinoBattleModelPlayer *player = ((DinoBattleModel *)mUnkAC)->GetPlayer(mUnkA8);
        int category = player->FindCreatureByB4(mUnkC4[0])->GetB8Category();
        DinoBattleModelPlayer *side;
        if (*DinoOffsetPtr<u8>(def, def->unk14) == 10 || *DinoOffsetPtr<u8>(def, def->unk14) == 11 ||
            *DinoOffsetPtr<u8>(def, def->unk14) == 12) {
            side = player;
        } else {
            side = player->GetOpponent();
        }
        if (all) {
            category = 0;
        }
        u32 ids[3];
        int count;
        if (*DinoOffsetPtr<u8>(def, def->unk14) == 9 || *DinoOffsetPtr<u8>(def, def->unk14) == 2 ||
            *DinoOffsetPtr<u8>(def, def->unk10) == 8) {
            all = TRUE;
            count = 1;
            ids[0] = mUnkC4[0];
        } else if (*DinoOffsetPtr<u8>(def, def->unk14) == 15) {
            all = TRUE;
            count = 1;
            ids[0] = creature->mPlayer->FindCreature(0)->GetUnkB4();
        } else if (*DinoOffsetPtr<u8>(def, def->unk10) == 9 || *DinoOffsetPtr<u8>(def, def->unk10) == 10 ||
                   *DinoOffsetPtr<u8>(def, def->unk10) == 11) {
            player->CollectCreaturesNearB8(0, ids, &count);
        } else if (*DinoOffsetPtr<u8>(def, def->unk14) == 13 || *DinoOffsetPtr<u8>(def, def->unk14) == 10) {
            count = 0;
            for (u32 i = 0; i < player->GetCreatureCount(); i++) {
                ids[count++] = player->GetCreature(i)->GetUnkB4();
            }
        } else if (*DinoOffsetPtr<u8>(def, def->unk14) == 14 || *DinoOffsetPtr<u8>(def, def->unk14) == 11 ||
                   *DinoOffsetPtr<u8>(def, def->unk14) == 12) {
            count = 0;
            for (int i = 0; i < player->GetCreatureCount(); i++) {
                if (mUnkC4[0] != player->GetCreature(i)->GetUnkB4()) {
                    ids[count++] = player->GetCreature(i)->GetUnkB4();
                }
            }
        } else if (*DinoOffsetPtr<u8>(def, def->unk14) == 16 || *DinoOffsetPtr<u8>(def, def->unk14) == 7) {
            all = TRUE;
            count = side->GetCreatureCount();
            for (int i = 0; i < count; i++) {
                ids[i] = side->GetCreature(i)->GetUnkB4();
            }
        } else {
            side->CollectCreaturesNearB8(category, ids, &count);
        }
        mUnkB0->Post(new DinoBattleViewUiDefenderMenuRequest((u32)(MMessageObserver *)this, mUnkA8, ids, count, all));
        break;
    }
    case 0x1001A: {
        DinoBattleViewUiDefenderMenuResult *res = (DinoBattleViewUiDefenderMenuResult *)msg;
        if (res->mUnk10) {
            DinoBattleModelPlayer *player = ((DinoBattleModel *)mUnkAC)->GetPlayer(mUnkA8);
            DinoBattleModelCreature *creature = player->FindCreatureByB4(mUnkC4[0]);
            mSerial.AddTail(mUnkB0->CreatePostTask(new DinoBattleViewClearAttackBarRequest(0)),
                                 mUnkB0->CreatePostTask(new DinoBattleViewUiAbilityMenuRequest(
                                     (u32)(MMessageObserver *)this, mUnkA8, mUnkC4[0], func_ov000_0216d2cc(creature, TRUE),
                                     creature->GetStatusUnk20(), creature->GetStats(), player->AllCreaturesShareDefFlag(),
                                     mUnkC4[1])));
            break;
        }
        DinoBattleModelPlayer *player = ((DinoBattleModel *)mUnkAC)->GetPlayer(mUnkA8);
        player->SetUnk124(((DinoBattleModel *)mUnkAC)->GetPlayer(mUnkA8)->GetUnk124() | 1);
        mSerial.AddTail(((DinoBattleModel *)mUnkAC)
                                 ->CreatePostTask(new DinoBattleModelCreatureAbilityRequest(
                                     (u32)(MMessageObserver *)this, mUnkA8, mUnkC4[0], res->mUnk0C, mUnkC4[1],
                                     1 << (1 - mUnkA8))));
        break;
    }
    case 0x10026: {
        ((DinoBattleModelField *)((DinoBattleModel *)mUnkAC)->GetUnk10C())->mUnkD0 = 0;
        MTask *tutorial = func_ov000_0218ec8c((u32)((DinoBattleModel *)mUnkAC)->mTutorial, 9, NULL);
        if (tutorial != NULL) {
            mSerial.AddTail(tutorial);
        }
        mSerial.AddTail(mUnkB0->CreatePostTask(new DinoBattleViewClearAttackBarRequest(0)),
                             mUnkB0->CreatePostTask(new DinoBattleViewSetCreaturePriorityRequest(0)));
        func_ov000_0216b6c0(this, -1, 1);
        break;
    }
    }
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

// The status effects a support creature's kind (2-7) clears from its leader.
struct DinoSupportCure {
    u32 mask;   // 0x00: 0 ends the table
    u32 kind;   // 0x04
};
extern "C" const DinoSupportCure data_ov000_0218faa4[];
extern "C" const u32 data_0208a2b8, data_0208a2b4;  // the cure's info bar texts (ours, theirs)
extern "C" const u32 data_0208a300, data_0208a2fc;  // the support effect's (on ours, on theirs)
extern "C" const u32 data_0208a150;                 // the support motion's last argument

// The support creatures' turn-start effects: each cures its leader's status effects (kinds 2-9) and gives its
// support to a leader, then the ability icons and the support animations are updated.
extern "C" MSerialTaskSet *func_ov000_02169fb8(MMessageObserver *view, DinoBattleModelPlayer *player,
                                               DinoBattleModelPlayer *other) {
    MSerialTaskSet *tasks = new MSerialTaskSet(TRUE);
    DinoBattleModelCreature *leader = player->FindCreature(0);
    u32 status[2] = {0, 0};
    status[0] = leader->GetStatus()->flags;
    DinoBattleModelCreature *otherLeader = other->FindCreature(0);
    if (otherLeader != NULL) {
        status[1] = otherLeader->GetStatus()->flags;
    }
    for (int k = 0; k < 2; k++) {
        DinoBattleModelCreature *support = player->FindCreature(k + 1);
        if (support != NULL) {
            u8 kind = ((u8 *)support->GetDef())[0x5C];
            switch (kind) {
            case 0:
            case 1:
                break;
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7: {
                int index = -1;
                int j = 0;
                const DinoSupportCure *cure = data_ov000_0218faa4;
                do {
                    if (kind == cure->kind) {
                        index = j;
                        break;
                    }
                    cure++;
                    j++;
                } while (cure->mask != 0);
                if (index == -1) {
                    break;
                }
                u32 mask = data_ov000_0218faa4[index].mask;
                if (!(status[0] & mask)) {
                    break;
                }
                tasks->AddTail(player->CreateMotionEffectTask(support->GetUnkB4(), 2, 0, 0, -1));
                MParallelTaskSet *release = new MParallelTaskSet(TRUE);
                DinoSignalWait *wait = new DinoSignalWait(0x1005C, NULL, NULL, 1);
                release->AddTail(wait, leader->CreatePostTask(new DinoBattleModelCreatureReleaseStatusEffectRequest(wait, mask)));
                tasks->AddTail(view->CreatePostTask(new DinoBattleViewActionInfoBarUpdateRequest(
                                   player->mUnkB4 == 0 ? data_0208a2b8 : data_0208a2b4)),
                               release);
                status[0] &= ~mask;
                break;
            }
            case 8:
            case 9: {
                int sides = 1;
                u32 any = status[0];
                if (kind == 9) {
                    any |= status[1];
                    sides = 2;
                }
                if (any == 0) {
                    break;
                }
                MParallelTaskSet *release = new MParallelTaskSet(TRUE);
                for (int j = 0; j < sides; j++) {
                    u32 *s = &status[j];
                    if (*s & 0xFC) {
                        DinoBattleModelCreature *target = (j ? other : player)->FindCreature(0);
                        if (target != NULL) {
                            DinoSignalWait *wait = new DinoSignalWait(0x1005C, NULL, NULL, 1);
                            release->AddTail(wait, target->CreatePostTask(new DinoBattleModelCreatureReleaseStatusEffectRequest(wait, 0xFC)));
                        }
                        *s &= ~0xFC;
                    }
                }
                tasks->AddTail(player->CreateMotionEffectTask(support->GetUnkB4(), 2, 0, 0, -1),
                               view->CreatePostTask(new DinoBattleViewActionInfoBarUpdateRequest(
                                   player->mUnkB4 == 0 ? data_0208a2b8 : data_0208a2b4)),
                               release);
                break;
            }
            }

            u32 effect = support->GetUnk108();
            effect |= support->unk110[0] ^ 3;
            int side = 0;
            if (!(effect & 1)) {
                effect |= 1;
                side = 1;
            }
            if (side == 0 && !(effect & 2) && player->GetOpponent()->FindCreature(0) != NULL) {
                side = 2;
                effect |= 2;
            }
            if (side != 0) {
                DinoBattleModelCreature *target = (side == 1 ? player : player->GetOpponent())->FindCreature(0);
                tasks->AddTail(view->CreatePostTask(
                    new DinoBattleView3DSupportEffectRequest(support->GetUnkB4(), target->GetUnkB4(), TRUE)));
                MParallelTaskSet *give = new MParallelTaskSet(TRUE);
                DinoSignalWait *wait = new DinoSignalWait(0x10050, NULL, NULL, 1);
                give->AddTail(wait, support->CreatePostTask(
                                        new DinoBattleModelCreatureSupportEffectRequest(wait, support->GetUnkB4(), effect)));
                tasks->AddTail(give);
                int motion = target->GetDefUnk61() == 8 ? 1 : 3;
                int flags = 0;
                if (target->GetDefUnk61() != 8) {
                    flags = 8;
                }
                u32 text = side == 1 ? data_0208a300 : data_0208a2fc;
                if (player->mUnkB4 == 0) {
                    text++;
                }
                tasks->AddTail(view->CreatePostTask(new DinoBattleViewActionInfoBarUpdateRequest(text)),
                               player->CreateMotionEffectTask(target->GetUnkB4(), motion, flags, 0, data_0208a150));
            }
        }
        MParallelTaskSet *icons = new MParallelTaskSet(TRUE);
        DinoSignalWait *iconWait = new DinoSignalWait(0x1004A, NULL, NULL, 1);
        icons->AddTail(iconWait, new CreatePlayerUpdateAbilityIconTask(TRUE, (u32)(MMessageObserver *)iconWait, (u32)player));
        tasks->AddTail(icons);
        MParallelTaskSet *anime = new MParallelTaskSet(TRUE);
        DinoSignalWait *animeWait = new DinoSignalWait(0x10062, NULL, NULL, 1);
        anime->AddTail(animeWait, view->CreatePostTask(new DinoBattleViewWaitUpdateSupportEffectAnimeRequest(animeWait)));
        tasks->AddTail(anime);
    }
    return tasks;
}

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

// A status: its bit in the flags, its level and whether it's lasting.
struct DinoStatusIconSource {
    u32 kind;
    int bit;
    const s8 *level;
    const s8 *lasting;
};

// NONMATCHING: the bit counter and the table's bit swap r3/r5.
void DinoBattleModelPlayer::SendUpdateStateIcon(DinoBattleModelCreature *creature, u32 reply) {
    u8 icons[12] = {0};
    int count = 0;
    if (creature != NULL) {
        DinoBattleCreatureStatus *status = creature->GetStatus();
        const s8 *p = (const s8 *)status;
        DinoStatusIconSource sources[] = {
            {0, 2, &p[0x04], &p[0x10]}, {0, 3, &p[0x05], &p[0x11]}, {0, 4, &p[0x06], &p[0x12]},
            {0, 5, &p[0x07], &p[0x13]}, {0, 6, &p[0x09], &p[0x15]}, {0, 7, &p[0x0A], &p[0x16]},
            {1, 8, &p[0x0B], &p[0x17]}, {1, 9, &p[0x08], &p[0x14]}, {1, 10, &p[0x0C], &p[0x18]},
            {1, 11, &p[0x0D], &p[0x19]}, {0, 0, NULL, NULL},
        };
        for (int bit = 2; bit < 12; bit++) {
            if (!(status->flags & (1 << bit))) {
                continue;
            }
            for (int i = 0; sources[i].level != NULL; i++) {
                if (bit == sources[i].bit) {
                    u8 icon = bit;
                    if (*sources[i].lasting) {
                        icon |= 0x80;
                    }
                    icons[count++] = icon | (*sources[i].level << 5);
                    break;
                }
            }
        }
    }
    mUnkF4->Post(new DinoBattleViewPlayerUpdateStateIconRequest(reply, mUnkB8, creature ? creature->GetUnkB4() : 0,
                                                                count, icons));
    SendUpdateAbilityIconRequest(1, 0);
}

MTask *DinoBattleModelPlayer::CreateFieldEffectWaitTask() {
    MParallelTaskSet *wait = new MParallelTaskSet(TRUE);
    DinoSignalWait *signal = new DinoSignalWait(0x10056, NULL, NULL, 1);
    wait->AddTail(signal, mUnkF4->CreatePostTask(new DinoBattleViewFieldEffectWaitRequest(signal)));
    return (new MSerialTaskSet(TRUE))->Add(new MTimer(1))->Add(wait);
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

// The target's unkF0 with the support effects and statuses applied: those of the attacker's side against it and
// those of its own side for it.
// NONMATCHING: the operator+/- parameter copies swap stack slots, as in CalcUnkEC.
extern "C" MFx32 func_ov000_0216e2d8(DinoBattleModelCreature *attacker, DinoBattleModelCreature *target) {
    const MFx32 pinch = gDinoResource->GetBattleAdjustDefs()->unk40;
    MFx32 base(target->GetUnkF0());
    MFx32 result = base;
    if (target->GetDefUnk61() != 8) {
        if (target->GetUnkB8() == 0) {
            for (int i = 0; i < 2; i++) {
                DinoBattleModelCreature *ally = attacker->mPlayer->FindCreature(i ? 2 : 1);
                DinoBattleModelCreature *enemy = attacker->mPlayer->GetOpponent()->FindCreature(i ? 2 : 1);
                if (ally != NULL && (ally->GetUnk108() & 2)) {
                    DinoBattleCreatureDef *def = ally->GetDef();
                    const s8 *support = def->mUnk58 == 0 ? NULL : (const s8 *)def + def->mUnk58;
                    result = result + MFx32(support[1]) * base / MFx32(100);
                }
                if (enemy != NULL && (enemy->GetUnk108() & 1)) {
                    DinoBattleCreatureDef *def = enemy->GetDef();
                    const s8 *support = def->mUnk54 == 0 ? NULL : (const s8 *)def + def->mUnk54;
                    result = result + MFx32(support[1]) * base / MFx32(100);
                }
            }
        }
        if (target->mStatus.flags & 0x400) {
            result = result + MFx32(target->mStatus.unk38);
        }
        if (target->mStatus.flags & 0x200) {
            result = result - MFx32(target->mStatus.unk34);
        }
    }
    if (target->IsInPinch() && target->GetDefUnk61() == 5) {
        result = result * (pinch * MFx32(target->GetDef()->mUnk62) * MFx32::FromRaw(0x19A));
    }
    return result;
}

// The creature's unkF4 with the support effects and statuses applied.
// NONMATCHING: the operator+/- parameter copies swap stack slots, as in CalcUnkEC.
extern "C" MFx32 func_ov000_0216e4c0(DinoBattleModelCreature *creature, DinoBattleModelCreature *target) {
    const MFx32 pinch = gDinoResource->GetBattleAdjustDefs()->pinchBoost;
    MFx32 base(creature->mUnkF4);
    MFx32 result = base;
    if (creature->GetDefUnk61() != 8) {
        if (creature->GetUnkB8() == 0) {
            for (int i = 0; i < 2; i++) {
                DinoBattleModelCreature *ally = creature->mPlayer->FindCreature(i ? 2 : 1);
                DinoBattleModelCreature *enemy = creature->mPlayer->GetOpponent()->FindCreature(i ? 2 : 1);
                if (ally != NULL && (ally->GetUnk108() & 1)) {
                    DinoBattleCreatureDef *def = ally->GetDef();
                    const s8 *support = def->mUnk54 == 0 ? NULL : (const s8 *)def + def->mUnk54;
                    result = result + MFx32(support[2]) * base / MFx32(100);
                }
                if (enemy != NULL && (enemy->GetUnk108() & 2)) {
                    DinoBattleCreatureDef *def = enemy->GetDef();
                    const s8 *support = def->mUnk58 == 0 ? NULL : (const s8 *)def + def->mUnk58;
                    result = result + MFx32(support[2]) * base / MFx32(100);
                }
            }
        }
        if (creature->mStatus.flags & 0x80) {
            result = result - MFx32(creature->mStatus.unk28);
        }
    }
    if (creature->IsInPinch() && creature->GetDefUnk61() == 5) {
        result = result * (pinch * MFx32(creature->mDef->mUnk62) * MFx32::FromRaw(0x19A));
    }
    return result;
}

// The target's unkF8 with the support effects and statuses applied: those of the attacker's side against it and
// those of its own side for it; 0 under status 3.
// NONMATCHING: the operator+ parameter copies swap stack slots, as in CalcUnkEC.
extern "C" MFx32 func_ov000_0216e650(DinoBattleModelCreature *attacker, DinoBattleModelCreature *target) {
    if (target->GetStatus()->flags & 8) {
        return MFx32(0);
    }
    const MFx32 pinch = gDinoResource->GetBattleAdjustDefs()->unk48;
    MFx32 base(target->GetUnkF8());
    MFx32 result = base;
    if (target->GetDefUnk61() != 8) {
        if (target->GetUnkB8() == 0) {
            for (int i = 0; i < 2; i++) {
                DinoBattleModelCreature *ally = attacker->mPlayer->FindCreature(i ? 2 : 1);
                DinoBattleModelCreature *enemy = attacker->mPlayer->GetOpponent()->FindCreature(i ? 2 : 1);
                if (ally != NULL && (ally->GetUnk108() & 2)) {
                    DinoBattleCreatureDef *def = ally->GetDef();
                    const s8 *support = def->mUnk58 == 0 ? NULL : (const s8 *)def + def->mUnk58;
                    result = result + MFx32(support[3]) * base / MFx32(100);
                }
                if (enemy != NULL && (enemy->GetUnk108() & 1)) {
                    DinoBattleCreatureDef *def = enemy->GetDef();
                    const s8 *support = def->mUnk54 == 0 ? NULL : (const s8 *)def + def->mUnk54;
                    result = result + MFx32(support[3]) * base / MFx32(100);
                }
            }
        }
        if (target->mStatus.flags & 0x800) {
            result = result + MFx32(target->mStatus.unk3C);
        }
    }
    if (target->IsInPinch() && target->GetDefUnk61() == 5) {
        result = result * (pinch * MFx32(target->GetDef()->mUnk62) * MFx32::FromRaw(0x19A));
    }
    return result;
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

extern "C" DinoBattleDebugLog *data_ov000_021978c0;
extern "C" const u32 data_0208a40c; // debug log texts
extern "C" const u32 data_0208a410;

// Rolls a hit against chance: a hit gives a random factor (kind 2). A miss rolls again against the adjust table's
// chance, which gives 0 and sets *flag (kind 0); otherwise a random factor again.
extern "C" MFx32 func_ov000_0216e918(DinoBattleModelCreature *creature, MFx32 chance, u8 *flag, int *kind) {
    if (chance >= creature->RandomFraction()) {
        char text[256];
        sprintf(text, gDinoResource->GetText(data_0208a410), 0);
        data_ov000_021978c0->Print(text);
        const MFx32 min = gDinoResource->GetBattleAdjustDefs()->randomMin;
        const MFx32 max = gDinoResource->GetBattleAdjustDefs()->randomMax;
        *kind = 2;
        return min + (max - min) * creature->RandomFraction();
    }
    if (MFx32(gDinoResource->GetBattleAdjustDefs()->unk4C) >= creature->RandomFraction()) {
        char text[256];
        sprintf(text, gDinoResource->GetText(data_0208a40c), 0);
        data_ov000_021978c0->Print(text);
        *kind = 0;
        *flag = TRUE;
        return MFx32(0);
    }
    char text[256];
    sprintf(text, gDinoResource->GetText(data_0208a410), 0);
    data_ov000_021978c0->Print(text);
    const MFx32 min = gDinoResource->GetBattleAdjustDefs()->randomMin;
    const MFx32 max = gDinoResource->GetBattleAdjustDefs()->randomMax;
    *kind = 2;
    return min + (max - min) * creature->RandomFraction();
}

extern "C" const u32 data_0208a414;

// Rolls an attack's hit: within chance it's a critical hit by the def's rate (a factor between the adjust table's
// critical bounds, kind 3) or a normal hit (kind 2); past it, a miss by the table's chance (0, kind 0) or a normal hit.
extern "C" MFx32 func_ov000_0216ea64(DinoBattleModelCreature *creature, DinoBattleModelCreature *target, MFx32 chance,
                                     u8 *critical, u8 *missed, int *kind, u32 arg) {
    MFx32 result;
    const MFx32 unused0 = gDinoResource->GetBattleAdjustDefs()->unk14;
    const MFx32 unused1 = gDinoResource->GetBattleAdjustDefs()->unk14;
    const MFx32 unused2 = gDinoResource->GetBattleAdjustDefs()->unk14;
    const MFx32 unused3 = gDinoResource->GetBattleAdjustDefs()->unk14;
    const MFx32 min = gDinoResource->GetBattleAdjustDefs()->criticalMin;
    const MFx32 max = gDinoResource->GetBattleAdjustDefs()->criticalMax;
    *missed = FALSE;
    if (chance >= creature->RandomFraction()) {
        if (creature->GetDefUnk20(creature->CompareUnkECA0(target, arg) == 1) >
            (int)creature->mPlayer->GetRandom()->Next(100)) {
            char text[256];
            sprintf(text, gDinoResource->GetText(data_0208a414), 0);
            data_ov000_021978c0->Print(text);
            result = min + (max - min) * creature->RandomFraction();
            *critical = TRUE;
            *kind = 3;
            return result;
        }
        char text[256];
        sprintf(text, gDinoResource->GetText(data_0208a410), 0);
        data_ov000_021978c0->Print(text);
        result = creature->RandomDamageFactor();
        *critical = FALSE;
        *kind = 2;
        return result;
    }
    if (MFx32(gDinoResource->GetBattleAdjustDefs()->unk4C) >= creature->RandomFraction()) {
        char text[256];
        sprintf(text, gDinoResource->GetText(data_0208a40c), 0);
        data_ov000_021978c0->Print(text);
        *missed = TRUE;
        *kind = 0;
        result = MFx32(0);
    } else {
        char text[256];
        sprintf(text, gDinoResource->GetText(data_0208a410), 0);
        data_ov000_021978c0->Print(text);
        result = creature->RandomDamageFactor();
        *critical = FALSE;
        *kind = 2;
    }
    *critical = FALSE;
    return result;
}

// A vector member the game emits in this file (made explicitly until the code that uses it is decompiled).
// NONMATCHING: its two int2type temporaries swap stack slots (they match with `#pragma ipa file`, which this file
// can't take yet: three other functions stop matching).
#pragma auto_inline off
template void std::__vec_deleter<DinoBattlePlayerEvent, std::allocator<DinoBattlePlayerEvent> >::reallocate_clear(
    size_t n, size_t max);

// The file's static initialization (see dino_static_init.h).
static struct CreatePlayerUpdateAbilityIconTaskStaticInit {
    CreatePlayerUpdateAbilityIconTaskStaticInit() {
        DINO_POOL_WORD(data_ov000_021924bc, 0x18) = 128;
        DINO_POOL_WORD(data_ov000_021924bc, 0x1c) = 176;
        DINO_POOL_WORD(data_ov000_021924bc, 0x30) = 8;
        DINO_POOL_WORD(data_ov000_021924bc, 0x34) = 150;
        DINO_POOL_WORD(data_ov000_021924bc, 0x28) = 64;
        DINO_POOL_WORD(data_ov000_021924bc, 0x2c) = 112;
    }
} sCreatePlayerUpdateAbilityIconTaskStaticInit;

extern "C" MFx32 func_ov000_0216e2d8(DinoBattleModelCreature *attacker, DinoBattleModelCreature *target);
extern "C" MFx32 func_ov000_0216e650(DinoBattleModelCreature *attacker, DinoBattleModelCreature *target);
extern "C" MFx32 func_ov000_0216e918(DinoBattleModelCreature *creature, MFx32 chance, u8 *flag, int *kind);
MFx32 GetAffinityRatio(DinoBattleModelCreature *creature, DinoBattleModelCreature *other, u32 attack);
extern "C" DinoBattleDebugLog *data_ov000_021978c0;
extern "C" BOOL func_ov000_0218472c(int flag);
extern "C" const u32 data_0208a304, data_0208a308;  // the debug log's attack and damage formats

// The damage after the special attack's affinity (for a special) and the position.
static inline int DinoAttackDamage(DinoBattleModelCreature *attacker, DinoBattleModelCreature *target, MFx32 value,
                                   MFx32 affinity, u8 special) {
    if (special) {
        value = attacker->ApplyRatio2(value, affinity);
    }
    return MFx32ToInt(MFx32(attacker->ApplyPositionRatio(value, target->GetB8Category())));
}
// (For the debug log.)
static inline float DinoFxToFloat(MFx32 x) {
    return (float)x.v / 4096.0f;
}



// The attacker's attack on the target: whether it hits, the damage (and recoil), the life tasks, and the view's
// request to show it. (Defined here at the end: placed after func_ov000_02162ad8, as in the game, it changes
// func_ov000_0216d2cc's branch layout.)
// NONMATCHING: 99.97%, the inlined damage's ApplyRatio2/ApplyPositionRatio temporaries take lower stack slots than
// the game's (which has them in creation order with this function's), and one zero store is scheduled earlier.
MTask *DinoBattleModelPlayer::CreateAttackTask(DinoBattleModelCreature *attacker, DinoBattleModelCreature *target,
                                               u32 attack, u8 special, int unk, u8 fade) {
    MSerialTaskSet *set = new MSerialTaskSet(TRUE);
    int damage = 0;
    int recoil = 0;
    int guard = 0;
    BOOL attackerDown = FALSE;
    BOOL targetDown = FALSE;
    attacker->RollUnk10C();
    u8 result[2];   // [0] hit, [1] missed
    result[1] = 0;
    int kind = 0;
    if (attacker->RollDefUnk23()) {
        MFx32 base;
        MFx32 ratio = attacker->GetUnkECRatio();
        MFx32 defense = func_ov000_0216e2d8(attacker, target);
        MFx32 hit = attacker->ReduceByDefense(ratio, defense);
        MFx32 chanceBase = func_ov000_0216f028(attacker);
        MFx32 chanceRatio = func_ov000_0216e650(attacker, target);
        MFx32 chance = attacker->CurveRatio(chanceBase, chanceRatio);
        MFx32 critical = func_ov000_0216e918(attacker, chance, &result[1], &kind);
        if (result[1]) {
            result[1] = 1;
        } else {
            MFx32 affinity = GetAffinityRatio(attacker, target, attack);
            const DinoAttackDef *def = gDinoResource->GetAttackDef(attack);
            char text[0x100];
            sprintf(text, gDinoResource->GetText(data_0208a304), def->hitCount);
            data_ov000_021978c0->Print(text);
            const u16 *cost = DinoOffsetPtr<u16>(def, def->cost);
            result[0] = 1;
            base = attacker->CalcDamage(target, MFx32(cost[1]), hit, result);
            damage = DinoAttackDamage(attacker, target, attacker->ApplyRatio(base, critical), affinity, special);
            if (damage < 1 && result[0]) {
                damage = 1;
            }
            recoil = 0;
            result[1] = 0;
            guard = func_ov000_02162ad8(attacker, target, ratio, MFx32(cost[1]), critical, &damage, &recoil,
                                        *(const u32 *)((const u8 *)def + 0x1C));
            if (damage >= (int)target->GetUnkE4() && !target->mPlayer->GetUnkC8()) {
                guard = 0;
                recoil = 0;
            }
            set->AddTail(new ModelCretureAddLifeTask(target, -damage), new ModelCretureAddLifeTask(attacker, -recoil));
            if ((int)(target->GetUnkE4() - damage) <= 0 && !target->mPlayer->GetUnkC8()) {
                targetDown = TRUE;
            }
            if (!func_ov000_0218472c(2) && (int)(attacker->GetUnkE4() - recoil) <= 0 && !attacker->mPlayer->GetUnkC8()) {
                attackerDown = TRUE;
            }
            float shown = DinoFxToFloat(MFx32(damage));
            char text2[0x100];
            sprintf(text2, gDinoResource->GetText(data_0208a308), shown);
            data_ov000_021978c0->Print(text2);
        }
    } else {
        result[1] = 1;
    }
    if (fade) {
        mSerial.AddTail(CreateLcdFadeTask(TRUE));
    }
    u8 show = 1;
    if (!special && unk > 0) {
        show = 0;
    }
    set->AddTail(mUnkF4->CreatePostTask(new DinoBattleViewCreatureAbilityRequest(
        (u32)(MMessageObserver *)this, GetUnkB8(), attacker->GetUnkB4(), attack, target->GetUnkB4(), kind, result[1],
        damage, targetDown, guard, recoil, attackerDown, special, show, special ? 1 : 2,
        attacker->CompareUnkECA0(target, attack), 0)));
    return set;
}
