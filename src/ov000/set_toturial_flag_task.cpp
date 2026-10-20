// SetToturialFlagTask.
// Only the destructors are decompiled so far.

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

// Instances the game emits here (for the tutorial's requests to the battle view); until the code that posts them
// is decompiled they are made explicitly.
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
