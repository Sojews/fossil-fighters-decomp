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
