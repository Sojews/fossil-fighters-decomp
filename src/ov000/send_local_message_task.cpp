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
#include "game/dino_resource.h"
#include "game/dino_backup.h"
#include "m/lcd.h"
#include "m/bright.h"

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

extern "C" u32 func_ov000_0215be2c(DinoBattleModelField *field);
extern "C" u32 func_ov000_0215bee8(DinoBattleModelField *field);
extern "C" MParallelTaskSet *func_ov000_0215bf44(DinoBattleModelField *field, u32 sides);
extern "C" MTask *func_ov000_0218ec8c(u32 tutorial, int kind, BOOL *proceed);
extern "C" MTask *func_ov000_02169fb8(MMessageObserver *view, DinoBattleModelPlayer *player, DinoBattleModelPlayer *other,
                                      BOOL unk);
extern "C" const u32 data_0208a2d8, data_0208a2dc;     // action info bar texts
extern "C" const u32 data_0208a1e0, data_0208a0dc, data_0208a0cc;  // frames before the live's game-end line, by creatures left

// NONMATCHING
void DinoBattleModelField::OnMessage(MMessage *msg) {
    switch (msg->mId) {
    case 0x10002: {
        func_ov000_0215c174(this, 0);
        for (int i = 0; i < 2; i++) {
            mControllers[i]->Post(new DinoBattleControllerInitRequest());
            mControllerReady[i] = FALSE;
        }
        DinoPlayerData data;
        gDinoBackup->GetPlayer(&data, 0);
        data.unk1c_7 = 0;
        gDinoBackup->SetPlayer(&data);
        break;
    }
    case 0x10004: {
        DinoBattleControllerInitResult *res = (DinoBattleControllerInitResult *)msg;
        mControllerReady[res->mUnk0C] = TRUE;
        if (!mControllerReady[0] || !mControllerReady[1]) {
            break;
        }
        MTask *post = mUnkB4->CreatePostTask(new DinoBattleViewEffectGameBeginRequest((u32)(MMessageObserver *)this));
        mSerial.AddTail(new MTimer(2), post);
        break;
    }
    case 0x1000A: {
        u32 sides = func_ov000_0215bee8(this);
        if (sides != 0) {
            mSerial.AddTail(mUnkB4->CreatePostTask(new DinoBattleViewActionInfoBarUpdateRequest(data_0208a2d8)));
            if (mLive != NULL) {
                func_ov000_0215c174(this, 0xA6);
            }
            mSerial.AddTail(func_ov000_0215bf44(this, sides));
            mSerial.AddTail(DinoCreatePostAndWaitTask(mUnkB4, new DinoBattleViewWaitActionInfoBarRequest(0)));
        }
        MTask *tutorial = func_ov000_0218ec8c((u32)mModel->mTutorial, 1, NULL);
        if (tutorial != NULL) {
            mSerial.AddTail(tutorial);
        }
        MTask *support1 = func_ov000_02169fb8(mUnkB4, mModel->GetPlayer(1), mModel->GetPlayer(0), TRUE);
        mSerial.AddTail(func_ov000_02169fb8(mUnkB4, mModel->GetPlayer(0), mModel->GetPlayer(1), TRUE), support1);
        tutorial = func_ov000_0218ec8c((u32)mModel->mTutorial, 0x15, NULL);
        if (tutorial != NULL) {
            mSerial.AddTail(tutorial);
        }
        mUnkC4 = func_ov000_0215be2c(this);
        mUnkC8 = mUnkC4;
        mModel->GetPlayer(mUnkC8)->SetUnk12C(TRUE);
        int points[2] = {0, 0};
        for (int side = 0; side < 2; side++) {
            DinoBattleModelPlayer *player = mModel->GetPlayer(side);
            for (int i = 0; i < player->GetCreatureCount(); i++) {
                points[side] += player->GetCreature(i)->GetUnkE4();
            }
        }
        mSerial.AddTail(DinoCreatePostAndWaitTask(mUnkB4, new DinoBattleViewDecideFirstAttackTeamRequest(0, mUnkC4, points)));
        tutorial = func_ov000_0218ec8c((u32)mModel->mTutorial, 0x14, NULL);
        if (tutorial != NULL) {
            mSerial.AddTail(tutorial);
        }
        mSerial.AddTail(
            mUnkB4->CreatePostTask(new DinoBattleViewEffectTurnBeginRequest((u32)(MMessageObserver *)this, mUnkC4)));
        break;
    }
    case 0x1000C: {
        MTask *tutorial;
        if (mUnkC4 == 0) {
            tutorial = func_ov000_0218ec8c((u32)mModel->mTutorial, 0x17, NULL);
        } else {
            tutorial = func_ov000_0218ec8c((u32)mModel->mTutorial, 0x18, NULL);
        }
        if (tutorial != NULL) {
            mSerial.AddTail(tutorial);
        }
        MParallelTaskSet *together = new MParallelTaskSet(TRUE);
        DinoSignalWait *wait = new DinoSignalWait(
            0x1003B, mModel->GetPlayer(mUnkC4),
            new DinoBattleModelPlayerTurnBeginRequst((u32)(MMessageObserver *)this, mUnkC4),
            mModel->GetPlayer(mUnkC4)->GetCreatureCount());
        SendLocalMessageTask *send = new SendLocalMessageTask(
            new DinoBattleModelCreatureTurnBeginRequest((u32)(MMessageObserver *)wait, mUnkC4), mModel);
        together->AddTail(wait);
        together->AddTail(send);
        MoveCreatureFromEscape *move = new MoveCreatureFromEscape(mModel->GetPlayer(mUnkC4));
        mSerial.AddTail(mUnkB4->CreatePostTask(new DinoBattleViewCreatureTurnBeginSignal(mUnkC4, move->IsNeeded())), move,
                        together);
        if (mUnkC8 == mUnkC4) {
            DinoPlayerData data;
            gDinoBackup->GetPlayer(&data, 0);
            int turns = data.unk1c_7;
            if (turns < 0x7FFFFFFF) {
                data.unk1c_7 = turns + 1;
                gDinoBackup->SetPlayer(&data);
            }
        }
        break;
    }
    case 0x1000F: {
        DinoBattleModelCreature *leader = mModel->GetPlayer(mUnkC4)->FindCreature(0);
        DinoBattleController *controller = static_cast<DinoBattleController *>(mControllers[mUnkC4]);
        mSerial.AddTail(DinoCreatePostAndWaitTask(mUnkB4, new DinoBattleViewWaitActionInfoBarRequest(0)));
        if (!controller->Unk10() && leader != NULL && (leader->GetStatus()->flags & 0x40)) {
            // Confused: the model acts for it.
            DinoSignalWait *wait =
                new DinoSignalWait(0x1004E, mControllers[mUnkC4], new DinoBattleControllerWakeupSignal(), 1);
            mParallel.AddTail(wait);
            mModel->Post(new DinoBattleModelPlayerConfusionExecRequest((u32)(MMessageObserver *)wait, mUnkC4));
        } else {
            MTask *post = mControllers[mUnkC4]->CreatePostTask(new DinoBattleControllerWakeupSignal());
            mSerial.AddTail(new MTimer(2), post);
        }
        break;
    }
    case 0x10014: {
        if (mModel->mUnkA8) {
            break;
        }
        mSerial.AddTail(mUnkB4->CreatePostTask(new DinoBattleView3DPreTurnEndRequest(0, mUnkC4)));
        MParallelTaskSet *together = new MParallelTaskSet(TRUE);
        DinoSignalWait *wait = new DinoSignalWait(0x1006C, NULL, NULL, 1);
        MTask *post = mUnkB4->CreatePostTask(new DinoBattleViewWaitActionInfoBarRequest((u32)(MMessageObserver *)wait));
        together->AddTail(wait);
        together->AddTail(post);
        mSerial.AddTail(together);
        if (mLive != NULL) {
            mLive->RefreshEntries();
        }
        mUnkCC = 0;
        mSerial.AddTail(new SendLocalMessageTask(
            new DinoBattleModelTurnEndRequest((u32)(MMessageObserver *)this, mUnkC4, (u32)&mUnkCC), mModel));
        break;
    }
    case 0x10016: {
        if (--mUnkCC != 0) {
            break;
        }
        DinoBattleModelPlayer *player = mModel->GetPlayer(mUnkC4);
        int count = player->GetCreatureCount();
        int ending = 0;
        for (int i = 0; i < count; i++) {
            if (player->GetCreature(i)->GetUnkE4() == 0) {
                ending++;
            }
        }
        if (ending != 0) {
            MParallelTaskSet *together = new MParallelTaskSet(TRUE);
            mSerial.AddTail(together);
            for (int i = 0; i < count; i++) {
                DinoBattleModelCreature *creature = player->GetCreature(i);
                if (creature->GetUnkE4() != 0) {
                    continue;
                }
                DinoBattleParams *params = &mBattle->mParams;
                if (--count == 0 || (params->unk158 && (&params->unk08)[mUnkC4] == 0)) {
                    count = 0;
                    mSerial.AddTail(player->CreatePostTask(new DinoBattleModelCreatureEndRequest(0, mUnkC4, creature->GetUnkB4())));
                } else {
                    DinoSignalWait *wait = new DinoSignalWait(0x10032, NULL, NULL, 1);
                    MTask *post = player->CreatePostTask(
                        new DinoBattleModelCreatureEndRequest((u32)(MMessageObserver *)wait, mUnkC4, creature->GetUnkB4()));
                    together->AddTail(wait);
                    together->AddTail(post);
                    mSerial.AddTail(mModel->GetPlayer(mUnkC4)->CreateMoveToHeatAreaAnyCreature());
                }
            }
        }
        if (count == 0) {
            break;
        }
        mModel->GetPlayer(mUnkC4);
        mSerial.AddTail(mUnkB4->CreatePostTask(new DinoBattleViewCreatureTurnEndSignal(mUnkC4)));
        mSerial.AddTail(mUnkB4->CreatePostTask(new DinoBattleView3DTurnEndRequest(0, mUnkC4)));
        MTask *tutorial = func_ov000_0218ec8c((u32)mModel->mTutorial, 10, NULL);
        if (tutorial != NULL) {
            mSerial.AddTail(tutorial);
        }
        mUnkC4 ^= 1;
        DinoBattleParams *params = &mBattle->mParams;
        BOOL timeUp = FALSE;
        if (params->unk15C != 0 && params->unk0C != 0 && mUnkC8 == mUnkC4) {
            DinoPlayerData data;
            gDinoBackup->GetPlayer(&data, 0);
            if (data.unk1c_7 + 1 >= params->unk15C) {
                timeUp = TRUE;
            }
        }
        if (timeUp) {
            mBattle->SetUnk1E0(TRUE);
            mSerial.AddTail(mModel->CreatePostTask(new DinoBattleModelGameEndSignal(1, 0, 1)));
        } else {
            mSerial.AddTail(
                mUnkB4->CreatePostTask(new DinoBattleViewEffectTurnBeginRequest((u32)(MMessageObserver *)this, mUnkC4)));
        }
        break;
    }
    case 0x10035: {
        DinoBattleModelGameEndSignal *sig = (DinoBattleModelGameEndSignal *)msg;
        u8 won = mModel->GetPlayer(sig->mUnk0C)->mUnkB4 == 0;
        if (won && sig->mUnk10) {
            MTask *wait = DinoCreatePostAndWaitTask(mUnkB4, new DinoBattleViewWaitActionInfoBarRequest(0));
            mSerial.AddTail(mUnkB4->CreatePostTask(new DinoBattleViewActionInfoBarUpdateRequest(data_0208a2dc)), wait);
        }
        if (gDinoResource->GetBattleAdjustDefs()->unk192 && gDinoResource->GetBattleAdjustDefs()->unk170) {
            DinoSignalWait *wait = new DinoSignalWait(0x1008C, NULL, NULL, 1);
            mSerial.AddTail(
                mUnkB4->CreatePostTask(new DinoBattleView3DGameSetRequest((u32)(MMessageObserver *)wait, sig->mUnk0C)));
            MSerialTaskSet *seq = new MSerialTaskSet(TRUE);
            seq->AddTail(wait);
            if (mLive != NULL) {
                ((DinoBattleModelField *)mModel->GetUnk10C())->mLive->SetUnkD2(FALSE);
                ((DinoBattleModelField *)mModel->GetUnk10C())->mLive->CloseIfOpen();
                switch (mModel->GetPlayer(sig->mUnk0C)->GetCreatureCount()) {
                case 1:
                    seq->AddTail(new MTimer(data_0208a1e0));
                    break;
                case 2:
                    seq->AddTail(new MTimer(data_0208a0dc));
                    break;
                case 3:
                    seq->AddTail(new MTimer(data_0208a0cc));
                    break;
                }
                SetLiveOneShotFlagTask *flag = new SetLiveOneShotFlagTask(this, won ? 1 : 2);
                seq->AddTail(new MTimer(30), flag);
            }
            mParallel.AddTail(seq);
        }
        if (won) {
            int prize = mBattle->mParams.unk160;
            if (prize < 0) {
                prize = 0;
            }
            int *out = (int *)mBattle->mUnk1E4[1];
            if (prize != 0 && out != NULL) {
                mModel->GetPlayer(sig->mUnk0C);
                *out = prize;
            }
        }
        mBattle->SetUnk1DC(won);
        MTask *tutorial = func_ov000_0218ec8c((u32)mModel->mTutorial, won ? 7 : 8, NULL);
        if (tutorial != NULL) {
            mSerial.AddTail(tutorial);
        }
        MParallelTaskSet *together = new MParallelTaskSet(TRUE);
        DinoSignalWait *wait = new DinoSignalWait(
            0x1006C, mUnkB4, new DinoBattleViewEffectGameEndRequest((u32)(MMessageObserver *)this, sig->mUnk0C), 1);
        MTask *post = mUnkB4->CreatePostTask(new DinoBattleViewWaitActionInfoBarRequest((u32)(MMessageObserver *)wait));
        together->AddTail(wait);
        together->AddTail(post);
        mSerial.AddTail(together);
        break;
    }
    case 0x10037:
        mBattle->Exit();
        break;
    case 0x10094: {
        mBattle->SetUnk1DC(TRUE);
        if (mModel->GetPlayer(0)->GetCreatureCount() == 0 || mModel->GetPlayer(1)->GetCreatureCount() == 0) {
            break;
        }
        MTask *fadeSub, *fadeMain, *bgm, *exit;
        exit = MTaskExitTransfer::Create(mBattle);
        bgm = DinoSoundCreateStopBgmTask(-1, 60);
        fadeSub = gLcd->mSub->mBright->CreateFadeOutToBlack(60, 0);
        fadeMain = gLcd->mMain->mBright->CreateFadeOutToBlack(60, 0);
        MParallelTaskSet *together = new MParallelTaskSet(TRUE);
        together->AddTail(fadeMain);
        together->AddTail(fadeSub);
        together->AddTail(bgm);
        mSerial.AddTail(together, exit);
        break;
    }
    case 0x1009F:
        if (mLive != NULL) {
            if (!((DinoBattleModelLiveEventSignal *)msg)->mUnk0C) {
                mLive->CloseIfOpen();
            }
            mLive->SetUnkD2(((DinoBattleModelLiveEventSignal *)msg)->mUnk0C);
        }
        break;
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 0x10026:
        break;
    }
}

template MTask *DinoCreatePostAndWaitTask(MMessageObserver *target, DinoBattleViewDecideFirstAttackTeamRequest *msg);
