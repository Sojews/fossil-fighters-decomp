// DinoCleaningTimer.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_sound.h"
#include "game/dino_cleaning_timer.h"
#include "types.h"
#include "m/resource.h"
#include "game/dino_resource.h"
#include "game/dino_cleaning.h"
#include "game/dino_common_live.h"
#include <algorithm>
#include "game/dino_static_init.h"

DinoCleaningTimer::~DinoCleaningTimer() {
}

#pragma auto_inline off

// Runs the state, then shows the time: the bar's frame for the seconds left and the elapsed seconds' digits.
void DinoCleaningTimer::Idle() {
    (this->*mState)();
    int limit = mLimit;
    int left = (limit - mFrames) / 60;
    u8 *back = mRaster->mBack;
    u8 *data = (u8 *)mBar.mFile.archive->GetData(mBar.mFile.index);
    u32 *frames = (u32 *)(data + *(u32 *)(data + 8));
    u8 *frame = data + frames[left + (90 - limit / 60)];
    MI_CpuCopy32(frame + *(u32 *)(frame + 4), back, 0x300);
    mRaster->Flip();
    u32 elapsed = mLimit / 60 - left;
    mDigits[0]->Play(elapsed % 10 + 1, 0);
    mDigits[1]->Play(elapsed / 10 + 1, 0);
}

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x4];
    const char *mDigitPath;                             // 0x04
    int mZero;                                          // 0x08: what the countdown stops at
    const char *mBarPath;                               // 0x0C
    void (DinoCleaningTimer::*StateCountdown_10)();
    void (DinoCleaningTimer::*StateIdle_18)();
    void (DinoCleaningTimer::*StateIdle_20)();
    u8 unk28[0x48 - 0x28];
    void (DinoCleaningTimer::*StateExtend_48)();
    void (DinoCleaningTimer::*StateIdle_50)();
} data_ov002_02164c74;
extern "C" const MPoint data_ov002_02164d24[2];   // the digits' offsets from the screen's center
extern "C" const MRect data_ov002_02164d34;       // the help's area
extern "C" const u32 data_0208ad30;               // the help's text

DinoCleaningTimer::DinoCleaningTimer(DinoCleaning *cleaning, u32 planes, MSmartOam *oam, u32 frames)
    : MProcess(NULL), mOwner(cleaning), mFrames(0), mLimit(frames),
      mBar(std::string(data_ov002_02164c74.mBarPath)) {
    mRaster = new MRasterWindow(-1, NULL, TRUE);
    mRaster->SetBlendEnabled(TRUE);
    mRaster->SetObjBlendEnabled(TRUE);
    mRaster->SetTarget(0x3F);
    mRaster->SetObjTarget(0x3F ^ planes);
    mRaster->Show();
    for (u32 i = 0; i < 2; i++) {
        mDigits[i] = new MMotion(std::string(data_ov002_02164c74.mDigitPath), oam, 0, 1);
        mDigits[i]->SetPriority(2);
        mDigits[i]->SetAnchor(1);
        mDigits[i]->SetPosition(MPoint(data_ov002_02164d24[i].x + 128, data_ov002_02164d24[i].y + 96));
        mDigits[i]->Show();
        mParallel.AddTail(mDigits[i]);
    }
    mOwner->GetUnkE4()->SetUnkB8(1, (void *)(mLimit / 60));
    mOwner->GetUnkE4()->SetUnkB8(0, NULL);
    GetParallel()->AddTail(mRaster, new DinoHelpDummyItem(gCurrentDisplay, data_ov002_02164d34,
                                                         std::string(gDinoResource->GetText(data_0208ad30))));
    mState = data_ov002_02164c74.StateIdle_50;
}

void DinoCleaningTimer::Start() {
    mState = data_ov002_02164c74.StateExtend_48;
}

void DinoCleaningTimer::Stop() {
    DinoSoundPlaySe(0x45, 0);
    mState = data_ov002_02164c74.StateCountdown_10;
}

void DinoCleaningTimer::Pause() {
    mState = data_ov002_02164c74.StateIdle_18;
}

// Whether no time has passed yet.
BOOL DinoCleaningTimer::IsAtStart() {
    return mFrames == 0;
}

int DinoCleaningTimer::GetRemainingFrames() {
    return mLimit - mFrames;
}

int DinoCleaningTimer::GetSeconds() {
    return mFrames / 60;
}

// Counts a frame down; each second, shows the time left and ticks (louder in the last ten seconds).
void DinoCleaningTimer::StateCountdown() {
    mFrames = std::max(data_ov002_02164c74.mZero, mFrames - 1);
    int frames = mFrames;
    if (frames % 60 == 0) {
        u32 seconds = frames / 60;
        mOwner->GetUnkE4()->SetUnkB8(1, (void *)seconds);
        mOwner->GetUnkE4()->SetUnkB8(0, (void *)(mLimit / 60 - seconds));
        switch (seconds) {
        case 60:
            DinoSoundPlaySe(0x1C, 0);
            break;
        case 30:
            DinoSoundPlaySe(0x1D, 0);
            break;
        case 15:
            DinoSoundPlaySe(0x1E, 0);
            break;
        }
        if (seconds > 10) {
            DinoSoundPlaySe(0x20, 0);
        } else if (seconds != 0) {
            DinoSoundPlaySe(0x21, 0);
        }
    }
}

// A second more per frame until the time is up.
void DinoCleaningTimer::StateExtend() {
    DinoSoundFunc_0201feec(0x2E);
    mFrames += 60;
    if (mFrames == mLimit) {
        mState = data_ov002_02164c74.StateIdle_20;
    }
}

// (Emits the out-of-line copy of the resource constructor the game emits in this file.)
#pragma dont_inline on
void DinoCleaningTimerMessagesAnchor(std::string path) {
    MResource<DinoMessageData> messages(path);
}
#pragma dont_inline reset

// Stopped, paused or done.
void DinoCleaningTimer::StateIdle() {
}

// The file's static initialization (see dino_static_init.h).
extern "C" u32 data_ov002_02164d2c[];
#pragma always_inline on
static struct DinoCleaningTimerStaticInit {
    DinoCleaningTimerStaticInit() {
        DINO_COMMON_LAYOUT_INIT(data_ov002_02164c74, 0x40, 0x70, 0x30, 0x28, 0x80, 0x90, 0x98, 0xa0);
        DINO_POOL(MPoint, data_ov002_02164c74, 0xb0) = MPoint(15, -80);
        DINO_POOL(MPoint, data_ov002_02164d2c, 0) = MPoint(-7, -80);
        DINO_POOL(MRect, data_ov002_02164c74, 0xc0) = MRect(106, 0, 149, 33);
    }
} sDinoCleaningTimerStaticInit;
