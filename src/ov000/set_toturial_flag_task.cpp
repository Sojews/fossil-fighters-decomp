// SetToturialFlagTask.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/set_toturial_flag_task.h"
#include "types.h"
#include "game/dino_battle_tutorial.h"
#include "game/dino_battle_view_class.h"
#include "game/dino_message.h"
#include "game/dino_battle_model.h"
#include "game/create_player_update_ability_icon_task.h"
#include "game/dino_resource.h"
#include "game/dino_backup.h"
#include "game/dino_static_init.h"

SetToturialFlagTask::~SetToturialFlagTask() {
}

void SetToturialFlagTask::Execute() {
    mTutorial->mFlags |= mFlag;
    Exit();
}

#pragma auto_inline off

// A tutorial script command's parameters (the first is its type).
static inline const s32 *DinoTutorialParams(const u8 *entry) {
    return (const s32 *)(entry + ((const u32 *)entry)[1]);
}
// The script's command at index i.
static inline const u8 *DinoTutorialEntry(DinoBattleTutorial *tutorial, u32 i) {
    const u8 *data = (const u8 *)tutorial->mData.Get();
    return data + ((const u32 *)(data + ((const u32 *)data)[2]))[i];
}

// Runs the tutorial's script on an event: first the conditions at the current position pick where to go (1-2: on
// this event, 3-4: on a comparison; 2 and 4 also stop the script when they fail), then the commands from there make
// the tasks (pointers, rects, the window, ...) until the next condition. Returns NULL when there is nothing to show.
// NONMATCHING: in the two pointer (SetArroe) commands the game schedules the position temporaries' stores and the
// parameter loads differently (ctor forms tried: by value, by reference, x/y, an MPoint member, 1-based index).
#pragma opt_strength_reduction off
extern "C" MTask *func_ov000_0218ec8c(DinoBattleTutorial *tutorial, int event, BOOL *proceed) {
    if (tutorial->mUnk0C) {
        return NULL;
    }
    u32 i = tutorial->mUnk08;
    MSerialTaskSet *tasks;
    BOOL window;
    BOOL again = TRUE;
    do {
        int type = DinoTutorialParams(DinoTutorialEntry(tutorial, i))[0];
        switch (type) {
        case 1:
        case 2:
            if (event == DinoTutorialParams(DinoTutorialEntry(tutorial, i))[1]) {
                int next = DinoTutorialParams(DinoTutorialEntry(tutorial, i))[2];
                if (next == -1) {
                    for (;;) {
                        const u8 *data = (const u8 *)tutorial->mData.Get();
                        u32 n = i + 1;
                        int nextType = DinoTutorialParams(data + ((const u32 *)(data + ((const u32 *)data)[2]))[n])[0];
                        if (nextType < 1 || nextType > 4) {
                            break;
                        }
                        i = n;
                    }
                    tutorial->mUnk08 = i;
                } else {
                    tutorial->mUnk08 = next - 1;
                }
                again = FALSE;
            } else {
                if (type == 2 || type == 4) {
                    return NULL;
                }
                i++;
            }
            break;
        case 3:
        case 4: {
            int args[3];
            int j = 0;
            int *arg = args;
            while (j < 3) {
                *arg++ = DinoTutorialParams(DinoTutorialEntry(tutorial, i))[++j];
            }
            j = 0;
            arg = args;
            for (; j < 2; j++, arg++) {
                if (args[1] & (1 << j)) {
                    *arg = tutorial->GetValue(*arg);
                }
            }
            BOOL holds = FALSE;
            switch (args[1] >> 2) {
            case 0:
                if (args[0] > args[2]) {
                    holds = TRUE;
                }
                break;
            case 1:
                if (args[0] >= args[2]) {
                    holds = TRUE;
                }
                break;
            case 2:
                if (args[0] < args[2]) {
                    holds = TRUE;
                }
                break;
            case 3:
                if (args[0] <= args[2]) {
                    holds = TRUE;
                }
                break;
            case 4:
                if (args[0] == args[2]) {
                    holds = TRUE;
                }
                break;
            case 5:
                if (args[0] != args[2]) {
                    holds = TRUE;
                }
                break;
            }
            again = FALSE;
            if (holds) {
                int next = DinoTutorialParams(DinoTutorialEntry(tutorial, i))[4];
                tutorial->mUnk08 = next == -1 ? i : next - 1;
            } else {
                if (type == 4) {
                    return NULL;
                }
                i++;
            }
            break;
        }
        default:
            return NULL;
        }
    } while (again);
    tasks = NULL;
    window = FALSE;

    for (;;) {
        u32 n = ++tutorial->mUnk08;
        const u8 *entry = DinoTutorialEntry(tutorial, n);
        int type = DinoTutorialParams(entry)[0];
        switch (type) {
        case 1:
        case 2:
        case 3:
        case 4:
            if (window) {
                tasks->AddTail(DinoCreatePostAndWaitTask(tutorial->mView, new DinoBattleViewTutorialWindowRequest(0)));
            }
            return tasks;
        case 11:
            tutorial->mUnk08 = DinoTutorialParams(entry)[1] - 1;
            break;
        case 19:
            tutorial->mUnk0C = TRUE;
            if (window) {
                tasks->AddTail(DinoCreatePostAndWaitTask(tutorial->mView, new DinoBattleViewTutorialWindowRequest(0)));
            }
            return tasks;
        case 7:
            if (tasks == NULL) {
                tasks = new MSerialTaskSet(TRUE);
            }
            tasks->AddTail(DinoCreatePostAndWaitTask(
                tutorial->mView,
                new DinoBattleViewTutorialSetArroeRequest(DinoTutorialParams(entry)[1] - 1, DinoTutorialParams(entry)[2],
                                                          MPoint(DinoTutorialParams(entry)[3], DinoTutorialParams(entry)[4]))));
            break;
        case 8:
            if (tasks == NULL) {
                tasks = new MSerialTaskSet(TRUE);
            }
            const s32 *params = DinoTutorialParams(entry);
            tasks->AddTail(DinoCreatePostAndWaitTask(
                tutorial->mView, new DinoBattleViewTutorialSetArroeRequest(params[1] - 1, 0, MPoint(0, 0))));
            break;
        case 9:
            if (tasks == NULL) {
                tasks = new MSerialTaskSet(TRUE);
            }
            tasks->AddTail(DinoCreatePostAndWaitTask(
                tutorial->mView,
                new DinoBattleViewTutorialSetRectRequest(MPoint(DinoTutorialParams(entry)[1], DinoTutorialParams(entry)[2]),
                                                         MPoint(DinoTutorialParams(entry)[3], DinoTutorialParams(entry)[4]))));
            break;
        case 5:
            window = TRUE;
            if (tasks == NULL) {
                tasks = new MSerialTaskSet(TRUE);
            }
            tasks->AddTail(DinoCreatePostAndWaitTask(tutorial->mView,
                                                     new DinoBattleViewTutorialWindowRequest(DinoTutorialParams(entry)[1])));
            break;
        case 12:
            if (proceed != NULL) {
                *proceed = DinoTutorialParams(entry)[1];
            }
            break;
        case 18: {
            if (tasks == NULL) {
                tasks = new MSerialTaskSet(TRUE);
            }
            MTask *end = ((DinoBattleModel *)tutorial->mUnk24)->CreatePostTask(new DinoBattleModelTutorialGameEndSignal());
            tasks->AddTail(DinoCreatePostAndWaitTask(tutorial->mView, new DinoBattleViewTutorialWindowRequest(0)));
            tasks->AddTail(end);
            tutorial->mUnk0C = TRUE;
            ((DinoBattleModel *)tutorial->mUnk24)->mUnkA8 = TRUE;
            return tasks;
        }
        case 13:
            if (tasks == NULL) {
                tasks = new MSerialTaskSet(TRUE);
            }
            tasks->AddTail(new SetToturialFlagTask(((DinoBattleModel *)tutorial->mUnk24)->GetUnk10C(),
                                                   DinoTutorialParams(entry)[1]));
            break;
        case 14:
        case 15:
            for (int k = 0; k < 2; k++) {
                if (((DinoBattleModel *)tutorial->mUnk24)->GetPlayer(k)->mUnkB4 == 0) {
                    ((DinoBattleModel *)tutorial->mUnk24)->GetPlayer(k)->SetUnkC8(type == 14);
                    break;
                }
            }
            break;
        case 16:
        case 17:
            for (int k = 0; k < 2; k++) {
                if (((DinoBattleModel *)tutorial->mUnk24)->GetPlayer(k)->mUnkB4 == 1) {
                    ((DinoBattleModel *)tutorial->mUnk24)->GetPlayer(k)->SetUnkC8(type == 16);
                    break;
                }
            }
            break;
        }
    }
}

#pragma opt_strength_reduction reset

// Instances the game emits here (for the tutorial's requests to the battle view).
template MTask *DinoCreatePostAndWaitTask(DinoBattleView *target, DinoBattleViewTutorialSetRectRequest *msg);
template MTask *DinoCreatePostAndWaitTask(DinoBattleView *target, DinoBattleViewTutorialSetArroeRequest *msg);
template MTask *DinoCreatePostAndWaitTask(DinoBattleView *target, DinoBattleViewTutorialWindowRequest *msg);

// Gives the tutorial its view, and the view its tutorial.
void DinoBattleTutorial::SetView(DinoBattleView *view) {
    mView = view;
    view->SetUnk12C((u32)this);
}

void DinoBattleTutorial::SetUnk24(u32 value) {
    mUnk24 = value;
}

u32 DinoBattleTutorial::GetValue(u32 kind) {
    u32 value = 0;
    if (kind < 13) {
        DinoBattleModelPlayer *mine = ((DinoBattleModel *)mUnk24)->GetPlayer(0);
        DinoBattleModelPlayer *other = ((DinoBattleModel *)mUnk24)->GetPlayer(1);
        DinoBattleModelCreature *myCreature = mine->FindCreature(0);
        DinoBattleModelCreature *otherCreature = other->FindCreature(0);
        switch (kind) {
        case 0:
            if (myCreature != NULL) {
                value = (int)myCreature->GetUnkE4() * 100 / (int)myCreature->GetUnkE8();
            }
            break;
        case 1:
            if (mine != NULL) {
                value = mine->GetUnk128();
            }
            break;
        case 2:
            if (mine != NULL) {
                value = mine->GetUnk14C();
            }
            break;
        case 3:
        case 4:
        case 5:
        case 6:
            if (myCreature != NULL) {
                const DinoCreatureDef *def = gDinoResource->GetCreatureDef(myCreature->GetUnkB0());
                const u32 *attacks = (const u32 *)((const u8 *)def + def->attackIds);
                u32 attack = attacks[kind - 3];
                if (attack != 0) {
                    const DinoAttackDef *attackDef = gDinoResource->GetAttackDef(attack);
                    value = *(const u16 *)(attackDef->cost == 0 ? NULL : (const u8 *)attackDef + attackDef->cost);
                } else {
                    value = 9999;
                }
            }
            break;
        case 7:
            if (myCreature != NULL) {
                const DinoCreatureDef *def = gDinoResource->GetCreatureDef(myCreature->GetUnkB0());
                if (def->specialAttack != 0) {
                    const DinoAttackDef *attackDef = gDinoResource->GetAttackDef(def->specialAttack);
                    value = *(const u16 *)(attackDef->cost == 0 ? NULL : (const u8 *)attackDef + attackDef->cost);
                } else {
                    value = 9999;
                }
            }
            break;
        case 8:
            if (otherCreature != NULL) {
                value = (int)otherCreature->GetUnkE4() * 100 / (int)otherCreature->GetUnkE8();
            }
            break;
        case 9:
            if (other != NULL) {
                value = other->GetUnk128();
            }
            break;
        case 10:
            if (other != NULL) {
                value = other->GetUnk14C();
            }
            break;
        case 11:
            value = mView->mUnkE0->mUnk150;
            break;
        case 12: {
            DinoPlayerData player;
            gDinoBackup->GetPlayer(&player, FALSE);
            value = player.unk1c_7;
            break;
        }
        }
    }
    return value;
}

// The file's static initialization (see dino_static_init.h).
extern "C" u32 data_ov000_021976ec[];
static struct SetToturialFlagTaskStaticInit {
    SetToturialFlagTaskStaticInit() {
        DINO_POOL_WORD(data_ov000_021976ec, 0x4) = 128;
        DINO_POOL_WORD(data_ov000_021976ec, 0x8) = 176;
        DINO_POOL_WORD(data_ov000_021976ec, 0x24) = 8;
        DINO_POOL_WORD(data_ov000_021976ec, 0x28) = 150;
        DINO_POOL_WORD(data_ov000_021976ec, 0xc) = 64;
        DINO_POOL_WORD(data_ov000_021976ec, 0x10) = 112;
    }
} sSetToturialFlagTaskStaticInit;
