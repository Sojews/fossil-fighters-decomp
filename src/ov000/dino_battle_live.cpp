// DinoBattleLive.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_battle_live.h"
#include "types.h"
#include "game/dino_battle_model.h"
#include "game/dino_battle_view_class.h"
#include "game/dino_message.h"
#include "game/dino_static_init.h"
#include "game/create_player_update_ability_icon_task.h"
#include "game/dino_backup.h"
#include "nitro/mi.h"

extern "C" BOOL func_ov000_0218c354(void *obj);

DinoBattleLive::DinoBattleLive(DinoBattleModel *model, const char *name) : DinoCommonLive(name, 1, 1) {
    mModel = model;
    mSource = (DinoBattleLiveSource *)mModel->GetUnk10C();
    mView = static_cast<DinoBattleView *>(mSource->mView);
}

DinoBattleLive::~DinoBattleLive() {
}

void DinoBattleLive::Open() {
}

void DinoBattleLive::ShowTutorial(MDisplay *display, const char *text) {
}

void DinoBattleLive::Signal(u32 id) {
    DinoBattleViewUnkE0 *target = mView->mUnkE0;
    target->Post(new DinoBattleViewLiveMessageSignal(id));
}

void DinoBattleLive::Close() {
    mView->Post(new DinoBattleViewLiveMessageSignal(0));
}

BOOL DinoBattleLive::IsBusy() {
    if (!func_ov000_0218c354(mView->mUnkE0->mUnk138)) {
        return TRUE;
    }
    return FALSE;
}

// One side's creatures as the commentary looks at them, by slot.
struct DinoBattleLiveSide {
    DinoBattleModelPlayer *player;          // 0x00
    DinoBattleModelCreature *creatures[4];  // 0x04
    u32 slots;                              // 0x14: a bit per filled slot
    u32 unk18[4];                           // 0x18
    u32 unk28[4];                           // 0x28
    u32 status[4];                          // 0x38
    u32 unk48[4];                           // 0x48
    int types[4];                           // 0x58
};

// The commentary flags of each of our slots: the bases of five runs of flags.
struct DinoBattleLiveSlotFlags {
    u32 unk18;      // 0x00: 5 flags
    u32 unk28;      // 0x04: 11 flags
    u32 unk48;      // 0x08: 10 flags
    u32 status;     // 0x0C: 10 flags
    u32 unkCE50;    // 0x10
};
// The other side's slots: the statuses' flags and one more.
struct DinoBattleLiveEnemySlotFlags {
    u32 status;     // 0x00: 10 flags
    u32 unkCE50;    // 0x04
};
extern "C" const DinoBattleLiveEnemySlotFlags data_ov000_02190480[3];
extern "C" const DinoBattleLiveSlotFlags data_ov000_02190498[3];

extern "C" u32 func_ov000_0216d2cc(DinoBattleModelCreature *creature, BOOL check);
int DinoGetTypeAdvantage(int type, int other); // 1: type beats other, 2: other beats type

// The side the commentary speaks for. (Through a non-member inline, the side gets the game's lowest stack slot.)
static inline int DinoLiveSide(void *source) {
    return ((u32 *)source)[0xC4 / 4];
}

// Sets the commentary's flags from both sides' creatures, then runs the commentary.
void DinoBattleLive::Idle() {
    ClearUnkA0();
    DinoBattleModelPlayer *us;
    int side;
    side = DinoLiveSide(mSource);
    us = mModel->GetPlayer(side);
    SetUnkA0(3, us->mUnkB4 == 0);
    DinoBattleLiveSide sides[2];
    MI_CpuFill8(sides, 0, sizeof(sides));
    for (int k = 0; k < 2; k++) {
        DinoBattleLiveSide *s = &sides[k];
        s->player = mModel->GetPlayer(side ^ k);
        for (int i = 0; i < s->player->GetCreatureCount(); i++) {
            DinoBattleModelCreature *creature = s->player->GetCreature(i);
            u32 slot = creature->GetUnkB8();
            s->slots |= 1 << slot;
            s->unk18[slot] = func_ov000_0216d2cc(creature, TRUE);
            s->creatures[slot] = creature;
            s->unk28[slot] = creature->unk110[2];
            s->status[slot] = creature->GetStatus()->flags;
            s->unk48[slot] = creature->unk110[1];
            s->types[slot] = creature->GetUnkCE7C(0);
        }
    }
    SetUnkA0(0x3B, (sides[0].slots & 2) != 0);
    SetUnkA0(0x3C, (sides[0].slots & 4) != 0);
    SetUnkA0(0x3D, (sides[0].slots & 8) != 0);
    SetUnkA0(0x3A, (sides[0].unk18[0] & 0x80) != 0);
    for (u32 i = 0; i < 3; i++) {
        const DinoBattleLiveSlotFlags *flags = &data_ov000_02190498[i];
        u32 v = sides[0].unk18[i];
        for (int j = 0; j < 5; j++) {
            SetUnkA0(j + flags->unk18, (v & (1 << j)) != 0);
        }
        v = sides[0].unk28[i];
        SetUnkA0(flags->unk28, (v & 0x1000) != 0);
        SetUnkA0(flags->unk28 + 1, (v & 0x2000) != 0);
        SetUnkA0(flags->unk28 + 2, (v & 0x4000) != 0);
        SetUnkA0(flags->unk28 + 3, (v & 0x10000) != 0);
        SetUnkA0(flags->unk28 + 4, (v & 0x20000) != 0);
        SetUnkA0(flags->unk28 + 5, (v & 0x40000) != 0);
        SetUnkA0(flags->unk28 + 6, (v & 0x80000) != 0);
        SetUnkA0(flags->unk28 + 7, (v & 0x400000) != 0);
        SetUnkA0(flags->unk28 + 8, (v & 0x800000) != 0);
        SetUnkA0(flags->unk28 + 9, (v & 0x1000000) != 0);
        SetUnkA0(flags->unk28 + 10, (v & 0x2000000) != 0);
        v = sides[0].unk48[i];
        for (int j = 0; j < 10; j++) {
            SetUnkA0(j + flags->unk48, (v & (1 << (j + 2))) != 0);
        }
        v = sides[0].status[i];
        for (int j = 0; j < 10; j++) {
            SetUnkA0(j + flags->status, (v & (1 << (j + 2))) != 0);
        }
        if (sides[0].creatures[i] != NULL && sides[0].creatures[i]->IsUnkCE50()) {
            SetUnkA0(flags->unkCE50, TRUE);
        }
    }
    for (u32 i = 0; i < 3; i++) {
        const DinoBattleLiveEnemySlotFlags *flags = &data_ov000_02190480[i];
        u32 v = sides[1].status[i];
        for (int j = 0; j < 10; j++) {
            SetUnkA0(j + flags->status, (v & (1 << (j + 2))) != 0);
        }
        if (sides[1].creatures[i] != NULL && sides[1].creatures[i]->IsUnkCE50()) {
            SetUnkA0(flags->unkCE50, TRUE);
        }
    }
    SetUnkA0(0x9E, (sides[1].unk28[0] & 0x20) != 0);
    SetUnkA0(0x9F, (sides[1].unk28[1] & 0x20) != 0);
    SetUnkA0(0xA0, (sides[1].unk28[2] & 0x20) != 0);
    SetUnkA0(0xA1, (sides[1].slots & 2) != 0);
    SetUnkA0(0xA2, (sides[1].slots & 4) != 0);
    SetUnkA0(0xA3, (sides[1].slots & 8) != 0);
    int advantage = DinoGetTypeAdvantage(sides[0].types[0], sides[1].types[0]);
    SetUnkA0(4, advantage == 1);
    SetUnkA0(5, advantage == 2);
    for (int i = 0; i < 2; i++) {
        SetUnkA0(i + 6, DinoGetTypeAdvantage(sides[0].types[i + 1], sides[1].types[0]) == 1);
    }
    for (int i = 0; i < 2; i++) {
        SetUnkA0(i + 8, DinoGetTypeAdvantage(sides[0].types[0], sides[1].types[i + 1]) == 1);
    }
    BOOL unkD274 = FALSE;
    BOOL unkD244 = FALSE;
    if (sides[0].creatures[0] != NULL && sides[0].player != NULL) {
        if (sides[0].creatures[0]->IsUnkD274() && sides[0].player->IsUnk2163978()) {
            unkD274 = TRUE;
        }
        if (sides[0].creatures[0]->IsUnkD244() && sides[0].player->AnyCreatureUnkD244()) {
            unkD244 = TRUE;
        }
    }
    SetUnkA0(0xA4, unkD244);
    SetUnkA0(0xA5, unkD274);
    SetUnkA0(0xA7, unkD244);
    SetUnkA0(0xA8, unkD274);
    if (sides[0].creatures[1] != NULL && sides[0].creatures[1]->IsUnkD244()) {
        SetUnkA0(0xA9, TRUE);
    }
    if (sides[0].creatures[2] != NULL && sides[0].creatures[2]->IsUnkD244()) {
        SetUnkA0(0xAA, TRUE);
    }
    SetUnkA0(0xAB, us->mUnk12D);
    DinoPlayerData player;
    gDinoBackup->GetPlayer(&player, FALSE);
    u32 unk = player.unk1c_7;
    if (unk == 1) {
        SetUnkA0(0xAC, TRUE);
    }
    if (unk >= 1 && unk <= 3) {
        SetUnkA0(0xAD, TRUE);
    }
    if (unk >= 4 && unk <= 9) {
        SetUnkA0(0xAE, TRUE);
    }
    if (unk >= 10) {
        SetUnkA0(0xAF, TRUE);
    }
    // (Each creature down to a quarter of its life.)
    DinoBattleModelCreature *creature;
    if ((creature = sides[0].creatures[0]) != NULL && (int)creature->GetUnkE4() <= (int)creature->GetUnkE8() / 4) {
        SetUnkA0(0xB0, TRUE);
    }
    if ((creature = sides[0].creatures[1]) != NULL && (int)creature->GetUnkE4() <= (int)creature->GetUnkE8() / 4) {
        SetUnkA0(0xB1, TRUE);
    }
    if ((creature = sides[0].creatures[2]) != NULL && (int)creature->GetUnkE4() <= (int)creature->GetUnkE8() / 4) {
        SetUnkA0(0xB2, TRUE);
    }
    if ((creature = sides[0].creatures[3]) != NULL && (int)creature->GetUnkE4() <= (int)creature->GetUnkE8() / 4) {
        SetUnkA0(0xB3, TRUE);
    }
    if ((creature = sides[1].creatures[0]) != NULL && (int)creature->GetUnkE4() <= (int)creature->GetUnkE8() / 4) {
        SetUnkA0(0xB4, TRUE);
    }
    if ((creature = sides[1].creatures[1]) != NULL && (int)creature->GetUnkE4() <= (int)creature->GetUnkE8() / 4) {
        SetUnkA0(0xB5, TRUE);
    }
    if ((creature = sides[1].creatures[2]) != NULL && (int)creature->GetUnkE4() <= (int)creature->GetUnkE8() / 4) {
        SetUnkA0(0xB6, TRUE);
    }
    if ((creature = sides[1].creatures[3]) != NULL && (int)creature->GetUnkE4() <= (int)creature->GetUnkE8() / 4) {
        SetUnkA0(0xB7, TRUE);
    }
    DinoCommonLive::Idle();
}

// The file's static initialization (see dino_static_init.h).
extern "C" u32 data_ov000_021974ac[];
static struct DinoBattleLiveStaticInit {
    DinoBattleLiveStaticInit() {
        DINO_POOL_WORD(data_ov000_021974ac, 0x14) = 128;
        DINO_POOL_WORD(data_ov000_021974ac, 0x18) = 176;
        DINO_POOL_WORD(data_ov000_021974ac, 0x4) = 8;
        DINO_POOL_WORD(data_ov000_021974ac, 0x8) = 150;
        DINO_POOL_WORD(data_ov000_021974ac, 0xc) = 64;
        DINO_POOL_WORD(data_ov000_021974ac, 0x10) = 112;
    }
} sDinoBattleLiveStaticInit;
