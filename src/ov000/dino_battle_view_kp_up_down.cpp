// DinoBattleViewKpUpDown.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_battle_view_kp_up_down.h"
#include "types.h"
#include "game/info_panel_window.h"
#include "game/dino_static_init.h"
#include "game/dino_resource.h"
#include <string>
#include <string.h>
#include "game/dino_sound.h"
#include "nitro/os.h"
#include "m/smart_oam.h"
#include "m/motion.h"

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

// The i-th motion's place (by value).
static inline MPoint DinoKPUpDownPoint(u32 i) {
    return DinoKPUpDownPoints()[i];
}

DinoBattleViewKPUpDown::DinoBattleViewKPUpDown() : MProcess(NULL) {
    for (int i = 0; i < 2; i++) {
        KPUpDownMotion *motion = new KPUpDownMotion(DinoKPUpDownPoint(i));
        mMotions[i] = motion;
        mParallel.AddTail(motion);
    }
}

// Clears a side's queued numbers.
extern "C" const char data_ov000_02195114[];   // the change's format ("%+d")
extern "C" u32 data_ov000_0219504c[];           // the TU's pooled statics (+0x04: the loss digits, +0x08: the gain)

// Shows a side's kaseki point change, one digit after another.
// NONMATCHING: the spilled locals (the length, the digit set, the oam) take other stack slots.
extern "C" void func_ov000_0217df4c(DinoBattleViewKPUpDown *view, int value, int side) {
    MPoint pos;
    char text[8];
    KPUpDownMotion *motion;
    MParallelTaskSet *digits;
    int i;
    int len;
    MSmartOam *oam;
    const char *c;
    if (value == 0) {
        return;
    }
    motion = view->mMotions[side];
    OS_SPrintf(text, data_ov000_02195114, value < 0 ? -value : value);
    len = strlen(text);
    motion->mTasks->Clear();
    MSerialTaskSet *tasks = new MSerialTaskSet(TRUE);
    digits = new MParallelTaskSet();
    oam = new MSmartOam(len, 0, TRUE);
    motion->mTasks->AddTail(tasks);
    pos = MPoint(motion->mPos.x - len * 16 / 2 + 8, motion->mPos.y);
    if (value > 0) {
        DinoSoundPlaySe(0x56, 0);
    } else {
        DinoSoundPlaySe(0x57, 0);
    }
    c = text;
    for (i = 0; i < len; i++) {
        MMotion *digit = new MMotion(
            std::string(value >= 0 ? ((const char **)data_ov000_0219504c)[2] : ((const char **)data_ov000_0219504c)[1]),
            oam, 2, 1);
        digit->SetPriority(2);
        digit->SetAnchor(3);
        digit->Show();
        digit->SetPosition(pos);
        digit->GetUnit()->SetMode(1);
        digits->AddTail(digit);
        pos.x += 16;
        int anime = *c == '+' ? 11 : *c - '0' + 1;
        tasks->AddTail(digit->CreatePlayTransfer(anime, 0), new MTimer(3));
        c++;
    }
    tasks->AddTail(new MTimer(60), MTaskExitTransfer::Create(digits));
    digits->AddTail(oam);
    motion->mTasks->AddTail(digits);
}

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
