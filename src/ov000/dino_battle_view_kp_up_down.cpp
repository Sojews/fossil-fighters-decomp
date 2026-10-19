// DinoBattleViewKpUpDown.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_battle_view_kp_up_down.h"
#include "types.h"
#include "game/info_panel_window.h"
#include "game/dino_static_init.h"
#include "game/dino_resource.h"

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

static inline const MPoint *DinoKPUpDownPoints() {
    const DinoBattleAdjustDefs *defs = gDinoResource->GetBattleAdjustDefs();
    return (const MPoint *)((const u8 *)defs + defs->kpUpDownPoints);
}

// NONMATCHING: 88%, the game recomputes the point's offset each time (ours steps it) and keeps i in a register.
DinoBattleViewKPUpDown::DinoBattleViewKPUpDown() : MProcess(NULL) {
    for (int i = 0; i < 2; i++) {
        KPUpDownMotion *motion = new KPUpDownMotion(DinoKPUpDownPoints()[i]);
        mMotions[i] = motion;
        mParallel.AddTail(motion);
    }
}

// Clears a side's queued numbers.
extern "C" void func_ov000_0217e1ec(DinoBattleViewKPUpDown *view, int side) {
    view->mMotions[side]->mTasks->Clear();
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
