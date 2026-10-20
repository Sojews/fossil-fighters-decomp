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
#include "m/motion.h"

extern "C" void func_ov000_0218b928(DinoBattleViewFocusLCDFrame *frame, u8 top);

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

// The focus frame's timings (the delay, the slide's duration and its ease), for the top screen and the bottom.
extern "C" const u32 data_0208a108, data_0208a104, data_0208a100;
extern "C" const u32 data_0208a0c4;
extern "C" const u32 data_0208a0f4, data_0208a0f8;
// The TU's pooled frame corner path (word 1) and corner positions.
extern "C" u32 data_ov000_02197054[];
extern "C" const MPoint data_ov000_02197088[4];

// Slides the frame's four corners on both screens in (to the top screen, or back down to the bottom one).
extern "C" void func_ov000_0218b928(DinoBattleViewFocusLCDFrame *frame, u8 top) {
    if (!gDinoResource->GetBattleAdjustDefs()->unk170) {
        return;
    }
    frame->mTasks->Clear();
    u32 timings[2][3] = {
        {data_0208a108, data_0208a104, data_0208a100},
        {data_0208a0c4, data_0208a0f8, data_0208a0f4},
    };
    const u32 *timing = timings[top == 0 ? 1 : 0];
    for (int i = 0; i < 2; i++) {
        MSmartOam *oam = i ? (MSmartOam *)frame->mUnk74 : frame->mOam;
        for (int j = 0; j < 4; j++) {
            MMotion *motion = new MMotion(std::string(((const char **)data_ov000_02197054)[1]), oam, 0, 1);
            motion->Show();
            motion->SetPriority(0);
            motion->SetPosition(MPoint(data_ov000_02197088[j].x, data_ov000_02197088[j].y + (top ? 0 : -192)));
            motion->SetFlip(j & 1 ? true : false, j & 2 ? true : false);
            motion->GetUnit()->SetMode(1);
            frame->mTasks->AddTail(motion);
            MSerialTaskSet *slide = new MSerialTaskSet(TRUE);
            slide->AddTail(new MTimer(timing[0]), motion->CreatePlayTransfer(2, 0), motion->CreateWaitStopTask(),
                           motion->CreateCoordAnime(motion->GetPosition(),
                                                    MPoint(data_ov000_02197088[j].x,
                                                           (top ? -192 : 0) + data_ov000_02197088[j].y),
                                                    timing[1], timing[2]),
                           motion->CreatePlayTransfer(3, 0), motion->CreateWaitStopTask(),
                           MTaskExitTransfer::Create(motion));
            frame->mTasks->AddTail(slide);
        }
    }
}

void DinoBattleViewFocusLCDFrame::Idle() {
}

void DBViewFocusLCDFrameTask::Execute() {
    func_ov000_0218b928((DinoBattleViewFocusLCDFrame *)mUnk10, mUnk14);
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
