// DinoBattle2.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_battle2.h"
#include "types.h"
#include "game/dino_static_init.h"

extern "C" void func_ov000_0218b928(void *, u8);

DinoBattleViewFocusLCDFrame::~DinoBattleViewFocusLCDFrame() {
}

DBViewFocusLCDFrameTask::DBViewFocusLCDFrameTask(u32 a, u8 b) {
    mUnk10 = a;
    mUnk14 = b;
}

DBViewFocusLCDFrameTask::~DBViewFocusLCDFrameTask() {
}

DinoBattleTalkWindow::~DinoBattleTalkWindow() {
}

void DinoBattleViewFocusLCDFrame::Idle() {
}

void DBViewFocusLCDFrameTask::Execute() {
    func_ov000_0218b928((void *)mUnk10, mUnk14);
    Exit();
}

// The file's static initialization (see dino_static_init.h).
extern "C" u32 data_ov000_02197054[];
extern "C" u32 data_ov000_02197090[];
extern "C" u32 data_ov000_02197098[];
extern "C" u32 data_ov000_021970a0[];
static struct DinoBattle2StaticInit {
    DinoBattle2StaticInit() {
        DINO_POOL_WORD(data_ov000_02197054, 0x34) = 16;
        DINO_POOL_WORD(data_ov000_02197054, 0x38) = 16;
        DINO_POOL_WORD(data_ov000_02197090, 0x0) = 240;
        DINO_POOL_WORD(data_ov000_02197090, 0x4) = 16;
        DINO_POOL_WORD(data_ov000_02197098, 0x0) = 16;
        DINO_POOL_WORD(data_ov000_02197098, 0x4) = 176;
        DINO_POOL_WORD(data_ov000_021970a0, 0x0) = 240;
        DINO_POOL_WORD(data_ov000_021970a0, 0x4) = 176;
    }
} sDinoBattle2StaticInit;
