// DinoCleaningTalkFilter.
// Only the destructors are decompiled so far.

#include "game/dino_cleaning_talk_filter.h"
#include "game/dino_cleaning_talk_window.h"
#include "m/blend.h"
#include "m/display.h"
#include "m/lcd.h"
#include "m/console.h"
#include "game/dino_resource.h"

DinoCleaningTalkFilter::~DinoCleaningTalkFilter() {
}

DinoCleaningTalkWindow::~DinoCleaningTalkWindow() {
}

// The pooled constants the code reads.
extern "C" struct {
    u8 unk00[0x8];
    const char *mMotionPath;    // 0x08
    const char *mPicturePath;   // 0x0C
} data_ov002_0216431c;

DinoCleaningTalkWindow::DinoCleaningTalkWindow(u32 arg)
    : MProcess(NULL), mUnk74(arg), mPicture(std::string(data_ov002_0216431c.mPicturePath), 7),
      mMotion(std::string(data_ov002_0216431c.mMotionPath), 7) {
    mUnkCC = 0;
    mUnkD0 = 0;
    mRate.v = ((const u32 *)gDinoResource->GetExcavate7C())[5];
    mTasksB4 = new MSerialTaskSet(FALSE);
    mTasksB8 = new MParallelTaskSet(FALSE);
    MParallelTaskSet *tasks = mTasksB8;
    MParallelTaskSet *parallel = GetParallel();
    parallel->AddTail(mTasksB4);
    parallel->AddTail(tasks);
}

#pragma auto_inline off

void DinoCleaningTalkWindow::SetUnkD0(u32 value) {
    mUnkD0 = value;
}

void DinoCleaningTalkWindow::SetUnkCC(u32 value) {
    mUnkCC = value;
}

void DinoCleaningTalkWindow::SetRate(MFx32 rate) {
    mRate = rate;
    mConsole->SetRate(rate);
}

void DinoCleaningTalkWindow::Reset() {
    mUnkCC = 0;
    mUnkD0 = 0;
    mTasksB4->Clear();
    mTasksB8->Clear();
    gLcd->GetTopDisplay()->mBlend->Reset();
}
