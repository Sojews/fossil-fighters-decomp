// DinoBattleViewKpUpDown.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_battle_view_kp_up_down.h"
#include "types.h"
#include "game/info_panel_window.h"
#include "game/dino_static_init.h"

DinoBattleViewKPUpDown::~DinoBattleViewKPUpDown() {
}

WaitPopNumberTask::~WaitPopNumberTask() {
}

KPUpDownMotion::~KPUpDownMotion() {
}

void DinoBattleViewPopNumber::Idle() {
}

void KPUpDownMotion::Idle() {
}

void DinoBattleViewKPUpDown::Idle() {
}

// Pooled file statics.
struct DinoBattleViewKPUpDownStatics {
    u32 unk0;
    int mPopNumberCount;    // 0x4
};
extern "C" DinoBattleViewKPUpDownStatics data_ov000_021978e0;

#pragma auto_inline off

int DinoBattleViewPopNumber::GetCount() {
    return data_ov000_021978e0.mPopNumberCount;
}

MTask *DinoBattleViewPopNumber::CreateWaitTask() {
    return new WaitPopNumberTask();
}

#pragma auto_inline on

void WaitPopNumberTask::Execute() {
    if (DinoBattleViewPopNumber::GetCount() == 0) {
        Exit();
    }
}

// The file's static initialization (see dino_static_init.h).
extern "C" u32 data_ov000_0219504c[];
extern "C" u32 data_ov000_021950cc[];
extern "C" u32 data_ov000_0219510c[];
static struct DinoBattleViewKpUpDownStaticInit {
    DinoBattleViewKpUpDownStaticInit() {
        DINO_POOL_WORD(data_ov000_0219504c, 0x2c) = 128;
        DINO_POOL_WORD(data_ov000_0219504c, 0x30) = 176;
        DINO_POOL_WORD(data_ov000_0219504c, 0x1c) = 8;
        DINO_POOL_WORD(data_ov000_0219504c, 0x20) = 150;
        DINO_POOL_WORD(data_ov000_0219504c, 0xc) = 64;
        DINO_POOL_WORD(data_ov000_0219504c, 0x10) = 112;
        DINO_POOL_WORD(data_ov000_0219504c, 0x14) = 0;
        DINO_POOL_WORD(data_ov000_0219504c, 0x18) = -12;
        DINO_POOL_WORD(data_ov000_021950cc, 0x38) = 72;
        DINO_POOL_WORD(data_ov000_021950cc, 0x3c) = 196;
        DINO_POOL_WORD(data_ov000_0219510c, 0x0) = 184;
        DINO_POOL_WORD(data_ov000_0219510c, 0x4) = 196;
    }
} sDinoBattleViewKpUpDownStaticInit;
