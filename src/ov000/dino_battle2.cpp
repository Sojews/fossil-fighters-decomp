// DinoBattle2.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_battle2.h"
#include "m/smart_oam.h"
#include "m/lcd.h"
#include "types.h"
#include "game/dino_static_init.h"
#include "game/dino_resource.h"
#include "game/dino_common_font.h"
#include <string>

extern "C" void func_ov000_0218b928(void *, u8);

DinoBattleViewFocusLCDFrame::DinoBattleViewFocusLCDFrame(u32 unk74) : MProcess(NULL) {
    mUnk74 = unk74;
    mTasks = new MParallelTaskSet(FALSE);
    mParallel.AddTail(mTasks);
    mOam = new MSmartOam(gLcd->mMain, 8, 4, 1);
    mParallel.AddTail(mOam);
    mOam->SetOffset(MPoint(0, -192));
}

DinoBattleViewFocusLCDFrame::~DinoBattleViewFocusLCDFrame() {
}

DBViewFocusLCDFrameTask::DBViewFocusLCDFrameTask(u32 a, u8 b) {
    mUnk10 = a;
    mUnk14 = b;
}

DBViewFocusLCDFrameTask::~DBViewFocusLCDFrameTask() {
}

DinoBattleTalkWindow::~DinoBattleTalkWindow() {
    mFont->Release();
}

// The TU's pooled paths.
extern "C" struct {
    u32 unk00;
    const char *facePath;   // 0x04
    const char *windowPath; // 0x08
} data_ov000_02197138;

DinoBattleTalkWindow::DinoBattleTalkWindow(u32 unkD8)
    : MProcess(NULL), mFace(0), mSide(0), unk7C(0), mPicture(std::string(data_ov000_02197138.windowPath), 7),
      mUnkD8(unkD8), mRate(*(const MFx32 *)((const u8 *)gDinoResource->GetExcavate7C() + 0x18)),
      mFaceFile(std::string(data_ov000_02197138.facePath), 7),
      mPicture2(std::string(data_ov000_02197138.windowPath), 7) {
    mUnk120 = 0;
    mState = 0;
    mTasksC0 = new MSerialTaskSet();
    mTasksC4 = new MParallelTaskSet();
    mOam = new MSmartOam(gLcd->GetTopDisplay(), 1, 0, 1);
    mFont = new DinoCommonFont(0, 10, 11);
    mFont->AddRef();
    GetParallel()->AddTail(mTasksC0, mTasksC4, mOam);
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
