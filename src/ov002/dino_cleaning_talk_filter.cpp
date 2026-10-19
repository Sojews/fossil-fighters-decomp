// DinoCleaningTalkFilter.
// Only the destructors are decompiled so far.

#include "game/dino_cleaning_talk_filter.h"
#include "game/dino_cleaning_talk_window.h"
#include "m/blend.h"
#include "m/display.h"
#include "m/lcd.h"
#include "m/console.h"
#include "m/seed_generator.h"
#include "m/smart_oam.h"
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
    u8 unk10[0x20 - 0x10];
    MPoint mPos;                // 0x20: the window's place
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

// Resumes the text once the wait is over (while B8 runs and B4 doesn't); shakes the window while a shake is left.
// NONMATCHING: 96%, the shake's range and first offset swap registers.
void DinoCleaningTalkWindow::Idle() {
    if (mUnkD0 != 0) {
        mUnkD0--;
        if (mUnkD0 == 0 && !mTasksB8->IsEmpty() && mTasksB4->IsEmpty()) {
            mConsole->Run();
        }
    }
    if (mUnkCC != 0) {
        mUnkCC--;
        if (!mTasksB8->IsEmpty() && mTasksB4->IsEmpty()) {
            int range = mUnkCC;
            MPoint d(gDinoResource->GetRandom()->Next(range * 2 + 1) - mUnkCC,
                     gDinoResource->GetRandom()->Next(range * 2 + 1) - range);
            mConsole->SetPosition(data_ov002_0216431c.mPos + d);
            mFace->SetPosition(d);
            mOam->SetOffset(MPoint(-d.x, -d.y));
        }
    }
}

// NONMATCHING: 87%, the points' temporaries take other stack slots.
void DinoCleaningTalkWindow::Close() {
    mUnkCC = 0;
    mUnkD0 = 0;
    mConsole->Run();
    MTask *clear = gLcd->GetTopDisplay()->mBlend->CreateClearTransfer();
    MTask *empty = MTaskSetMakeEmptyTask::Create(mTasksB8);
    MPoint frameFrom(256, 192);
    MTask *frame = mFrame->CreateCoordAnime(frameFrom, MPoint(frameFrom.x + 64, frameFrom.y), 20, 2);
    MTask *window = mConsole->CreateCoordAnime(data_ov002_0216431c.mPos,
                                               MPoint(data_ov002_0216431c.mPos.x, data_ov002_0216431c.mPos.y + 40), 20, 2);
    MTask *face = mFace->CreateCoordAnime(MPoint(0, 0), MPoint(0, 40), 20, 2);
    MParallelTaskSet *set = new MParallelTaskSet(TRUE);
    set->AddTail(face);
    set->AddTail(window);
    set->AddTail(frame);
    MSerialTaskSet *serial = mTasksB4;
    serial->AddTail(set);
    serial->AddTail(empty);
    serial->AddTail(clear);
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
