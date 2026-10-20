// SendLocalMessageTask, SetLiveOneShotFlagTask.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_message.h"
#include "game/send_local_message_task_menu.h"
#include "game/send_local_message_task.h"
#include "game/dino_battle_model.h"
#include "game/set_live_one_shot_flag_task.h"
#include "game/dino_common_live.h"
#include "game/create_player_update_ability_icon_task.h"
#include "game/dino_static_init.h"
#include "game/dino_battle.h"
#include "game/dino_battle_tutorial.h"
#include "game/dino_battle_controller.h"
#include "game/dino_battle_live.h"
#include "m/seed_generator.h"
#include "game/dino_sound.h"

extern "C" void func_ov000_0215c174(DinoBattleModelField *field, u32 line);

SendLocalMessageTask::~SendLocalMessageTask() {
}

SetLiveOneShotFlagTask::~SetLiveOneShotFlagTask() {
}

DinoBattleModelField::~DinoBattleModelField() {
}

void DinoBattleModelField::Idle() {
    Dispatch();
}

template MTask *DinoCreatePostAndWaitTask(MMessageObserver *target, DinoBattleViewDecideFirstAttackTeamRequest *msg);

void SetLiveOneShotFlagTask::Execute() {
    func_ov000_0215c174((DinoBattleModelField *)mUnk10, mUnk14);
    Exit();
}

void SendLocalMessageTask::Execute() {
    mModel->SendLocalMessage(mMessage);
    Exit();
}

#pragma auto_inline off

// Whether the first side goes first: the side whose creatures' E4 values sum higher (then counting only those with
// B8 clear), else at random.
extern "C" u32 func_ov000_0215be2c(DinoBattleModelField *field) {
    int total[2] = {0, 0};
    int clear[2] = {0, 0};
    for (int side = 0; side < 2; side++) {
        DinoBattleModelPlayer *player = field->mModel->GetPlayer(side);
        for (int i = 0; i < player->GetCreatureCount(); i++) {
            DinoBattleModelCreature *creature = player->GetCreature(i);
            int value = creature->GetUnkE4();
            if (creature->GetUnkB8() == 0) {
                clear[side] += value;
            }
            total[side] += value;
        }
    }
    if (total[0] != total[1]) {
        return total[0] > total[1] ? TRUE : FALSE;
    }
    if (clear[0] != clear[1]) {
        return clear[0] > clear[1] ? TRUE : FALSE;
    }
    return field->mModel->GetPlayer(0)->GetRandom()->Next(2);
}

// The sides (bit 0 the second, bit 1 the first) facing a creature of kind 3.
extern "C" u32 func_ov000_0215bee8(DinoBattleModelField *field) {
    u32 sides = 0;
    for (int side = 0; side < 2; side++) {
        DinoBattleModelPlayer *player = field->mModel->GetPlayer(side);
        for (int i = 0; i < player->GetCreatureCount(); i++) {
            if (player->GetCreature(i)->GetDefUnk61() == 3) {
                sides |= 1 << (1 - side);
            }
        }
    }
    return sides;
}

// Moves the creatures of the given sides (bit n for side n) to random distinct positions, one of them the front:
// a task set that waits for all of them to arrive.
extern "C" MParallelTaskSet *func_ov000_0215bf44(DinoBattleModelField *field, u32 sides) {
    u32 side;
    MParallelTaskSet *set = new MParallelTaskSet(TRUE);
    for (side = 0; (int)side < 2; side++) {
        if (!(sides & (1 << side))) {
            continue;
        }
        DinoBattleModelPlayer *player = field->mModel->GetPlayer(side);
        for (u32 i = 0; i < player->GetCreatureCount(); i++) {
            player->GetCreature(i)->SetUnk108(0);
        }
        u32 used;
        u32 positions[8];
        do {
            used = 0;
            for (int i = 0; i < player->GetCreatureCount(); i++) {
                player->GetCreature(i);
                u32 pos;
                do {
                    pos = player->GetRandom()->Next(4);
                } while (used & (1 << pos));
                used |= 1 << pos;
                positions[i] = pos;
            }
        } while (!(used & 1));
        DinoSoundPlaySe(0x56F, 0);
        for (int i = 0; i < player->GetCreatureCount(); i++) {
            DinoBattleModelCreature *creature = player->GetCreature(i);
            DinoSignalWait *wait = new DinoSignalWait(0x10013, NULL, NULL, 1);
            set->AddTail(wait);
            creature->Post(new DinoBattleModelCreatureMoveRequest((u32)(MMessageObserver *)wait, side,
                                                                  creature->GetUnkB4(), creature->GetUnkB8(),
                                                                  positions[i], 1, 0));
            creature->SetUnkB8(positions[i]);
        }
        DinoBattleModelPlayer *other = field->mModel->GetPlayer(1 - side);
        for (u32 i = 0; i < other->GetCreatureCount(); i++) {
            DinoBattleModelCreature *creature = other->GetCreature(i);
            creature->SetUnk108(creature->GetUnk108() & 1);
        }
    }
    return set;
}

// Has the commentary say the line now (once, not from inside itself).
extern "C" void func_ov000_0215c174(DinoBattleModelField *field, u32 line) {
    if (field->mLive != NULL) {
        field->mLive->SetUnkAC(line);
        if (!field->mLive->GetUnkD2()) {
            field->mLive->SetUnkD2(TRUE);
            if (field->mLive->mState == MTask::STATE_RUN) {
                field->mLive->Execute();
            }
            field->mLive->SetUnkD2(FALSE);
        }
    }
}

DinoBattleModelField::DinoBattleModelField() : MProcess(NULL), mLive(NULL), mUnkC4(0), mUnkD0(0) {
}

// The file's static initialization (see dino_static_init.h).
extern "C" u32 data_ov000_021916e0[];
static struct SendLocalMessageTaskStaticInit {
    SendLocalMessageTaskStaticInit() {
        DINO_POOL_WORD(data_ov000_021916e0, 0x8) = 128;
        DINO_POOL_WORD(data_ov000_021916e0, 0xc) = 176;
        DINO_POOL_WORD(data_ov000_021916e0, 0x18) = 8;
        DINO_POOL_WORD(data_ov000_021916e0, 0x1c) = 150;
        DINO_POOL_WORD(data_ov000_021916e0, 0x20) = 64;
        DINO_POOL_WORD(data_ov000_021916e0, 0x24) = 112;
    }
} sSendLocalMessageTaskStaticInit;

extern "C" const char data_ov000_021918ec[];    // the live commentary's name format

void DinoBattleModelField::SetBattle(DinoBattle *battle) {
    mBattle = battle;
    mControllers[0] = (DinoBattleController *)battle->GetUnk1F4(0);
    mControllers[1] = (DinoBattleController *)battle->GetUnk1F4(1);
    mModel = (DinoBattleModel *)battle->GetUnk1FC();
    mUnkB4 = battle->GetUnk200();
    if (mLive == NULL && mModel->mTutorial->mUnk0D == 0 && battle->mParams.mLiveName != NULL) {
        char name[64];
        OS_SPrintf(name, data_ov000_021918ec, battle->mParams.mLiveName);
        mLive = new DinoBattleLive(mModel, name);
        mParallel.AddTail(mLive);
        mLive->SetUnkD2(FALSE);
    }
}
