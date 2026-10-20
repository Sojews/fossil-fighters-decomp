// DinoBattleViewEffectTelop.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "m/input.h"
#include "game/dino_battle_view_effect_telop.h"
#include "types.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "game/dino_static_init.h"
#include "m/observer.h"
#include "game/dino_battle_view_kaseki_point_menu.h"
#include "game/dino_battle_view_class.h"
#include "m/smart_oam.h"
#include "m/picture.h"
#include "game/dino_sound.h"
#include "m/bg.h"
#include "m/display.h"
#include "m/lcd.h"
#include "m/view.h"
#include "m/bright.h"
#include "game/dino_message.h"
#include "game/dino_battle_view_3d.h"

DinoBattleViewEffectTelop::~DinoBattleViewEffectTelop() {
}

SetBGModeTask::~SetBGModeTask() {
}

SkipTelopTask::~SkipTelopTask() {
}

SkipWinTelopTask::~SkipWinTelopTask() {
}

SkipLoseTelopTask::~SkipLoseTelopTask() {
}

NopWaitTask::~NopWaitTask() {
}

void NopWaitTask::Execute() {
}

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x10];
    void (DinoBattleViewEffectTelop::*Load_10)();
    void (DinoBattleViewEffectTelop::*State_18)();
    u8 unk20[0x40 - 0x20];
    void (DinoBattleViewEffectTelop::*Start_40)();
    u8 unk48[0x58 - 0x48];
    void (DinoBattleViewEffectTelop::*State_58)();
    u8 unk60[0x70 - 0x60];
    void (DinoBattleViewEffectTelop::*State_70)();
} data_ov000_0219387c;

DinoBattleViewEffectTelop::DinoBattleViewEffectTelop(u32 unk7C, std::string text, u32 unk98, u32 unk8C, u32 unk90, u8 unk9C)
    : MProcess(NULL), mUnk7C(unk7C), mUnk80(text), mUnk8C(unk8C), mUnk90(unk90), mUnk98(unk98), mUnk9C(unk9C) {
    mState = data_ov000_0219387c.Start_40;
}

void DinoBattleViewEffectTelop::Idle() {
    (this->*mState)();
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
#pragma auto_inline off
template class MBgCallTask0<DinoBattleViewEffectTelop>;
template MTask *MCreateBgCallTask(DinoBattleViewEffectTelop *obj, void (DinoBattleViewEffectTelop::*func)(), int thread);
#pragma auto_inline reset

#pragma auto_inline off

// Starts the telop: loads it in the background, then goes on by its kind (with a sound for most).
void DinoBattleViewEffectTelop::StateStart() {
    MTaskSet *parallel = &mParallel;
    parallel->mUseMutex = TRUE;
    MTaskSet *serial = &mSerial;
    serial->mUseMutex = TRUE;
    gLcd->mSub->mBg->SetMode(3);
    mSerial.AddTail(MCreateBgCallTask(this, data_ov000_0219387c.Load_10, 1));
    switch (mUnk98) {
    case 4:
        mState = data_ov000_0219387c.State_58;
        return;
    case 5:
        mState = data_ov000_0219387c.State_18;
        return;
    }
    mSerial.AddTail(DinoSoundCreatePlaySeTask(0x52, 0));
    mState = data_ov000_0219387c.State_70;
}

extern "C" MMotion *func_ov000_02173d64(MSmartOam *oam, const std::string &name, u32 arg2, u32 arg3, u8 arg4);

// (In the background.) Loads the telop's picture and, for a player's telop, their name.
void DinoBattleViewEffectTelop::Load() {
    mPicture = new MExtAffinePicture(std::string(mUnk80), TRUE, true);
    mParallel.AddTail(mPicture);
    if (mUnk9C) {
        BOOL side = mUnk98 == 2;
        MSmartOam *oam = new MSmartOam(11, 0, TRUE);
        mName = func_ov000_02173d64(oam, ((DinoBattleView *)mUnk7C)->GetPlayerByAC(side)->GetUnkC0(), side,
                                     ((DinoBattleView *)mUnk7C)->GetPlayerByAC(side)->mUnkCC, 0);
        MTaskSet *parallel = &mParallel;
        parallel->AddTail(mName);
        parallel->AddTail(oam);
    } else {
        mName = NULL;
    }
}

// Takes the telop away, then reports that it's done.
void DinoBattleViewEffectTelop::StateFinish() {
    GetSerial()->AddTail(MTaskExitTransfer::Create(mPicture), new MTimer(1), new SetBGModeTask(gLcd->mSub->mBg, 0));
    if (mUnk8C != 0) {
        mSerial.AddTail(((MMessageObserver *)mUnk8C)->CreatePostTask((MMessage *)mUnk90));
    }
    mSerial.AddTail(MTaskExitTransfer::Create(this));
}

void SetBGModeTask::Execute() {
    mBg->SetMode(mMode);
    Exit();
}

void SkipTelopTask::Idle() {
    if (mTasks->IsEmpty()) {
        OnFinished();
        mSerial.AddTail(MTaskExitTransfer::Create(this));
        return;
    }
    if (mDelay != 0) {
        mDelay--;
        return;
    }
    if (!mNoSkip && (mInput->GetPressed() & 0x1003)) {
        OnSkipped();
        mSerial.AddTail(MTaskExitTransfer::Create(this));
    }
}

// (Sound settings in main.)
extern "C" int data_0208a144;
extern "C" const int data_0208a0ec;

// (Takes the frames by reference: the game reads them after the timer's vtable is set.)
static inline MTimer *NewTimer(const int &frames) {
    return new MTimer(frames);
}

// Slides the picture away, cancels the 3D view's motion and fades out to black.
void SkipLoseTelopTask::OnSkipped() {
    mTasks->AddTail(mView->CreateCoordAnime(MPoint(0, -20), MPoint(0, mView->GetHeight()), 120, 1));
    mParallel.AddTail((new MSerialTaskSet(TRUE))
                          ->Add(new MTimer(110))
                          ->Add(mView3D->CreatePostTask(new DinoBattleView3DGameSetMotionCancelRequest(0))));
    mSerial.AddTail((new MParallelTaskSet(TRUE))
                        ->Add(gLcd->mMain->mBright->CreateFadeOutToBlack(125, 0))
                        ->Add(gLcd->mSub->mBright->CreateFadeOutToBlack(125, 0))
                        ->Add((new MSerialTaskSet(TRUE))
                                  ->Add(NewTimer(data_0208a0ec))
                                  ->Add(DinoSoundCreateStopSeTask(0x74, data_0208a144))));
}

// Slides the picture away and fades out to black.
void SkipLoseTelopTask::OnFinished() {
    MSerialTaskSet *stopSe = (new MSerialTaskSet(TRUE))
                                 ->Add(NewTimer(data_0208a0ec))
                                 ->Add(DinoSoundCreateStopSeTask(0x74, data_0208a144));
    mSerial.AddTail((new MParallelTaskSet(TRUE))
                        ->Add(mView->CreateCoordAnime(MPoint(0, -20), MPoint(0, mView->GetHeight()), 120, 1))
                        ->Add(gLcd->mMain->mBright->CreateFadeOutToBlack(125, 0))
                        ->Add(gLcd->mSub->mBright->CreateFadeOutToBlack(125, 0))
                        ->Add(stopSe));
}

// Slides the picture away, cancels the 3D view's motion and fades out to white.
void SkipWinTelopTask::OnSkipped() {
    mTasks->AddTail(mView->CreateCoordAnime(MPoint(0, 40), MPoint(0, -mView->GetHeight()), 90, 2));
    mSerial.AddTail(mView3D->CreatePostTask(new DinoBattleView3DGameSetMotionCancelRequest(0)));
    mParallel.AddTail((new MSerialTaskSet(TRUE))
                          ->Add(new MTimer(65))
                          ->Add(mView3D->CreatePostTask(new DinoBattleView3DGameSetMotionCancelRequest(0))));
    mSerial.AddTail((new MParallelTaskSet(TRUE))
                        ->Add(gLcd->mMain->mBright->CreateFadeOutToWhite(80, 0))
                        ->Add(gLcd->mSub->mBright->CreateFadeOutToWhite(80, 0))
                        ->Add((new MSerialTaskSet(TRUE))
                                  ->Add(NewTimer(data_0208a0ec))
                                  ->Add(DinoSoundCreateStopSeTask(0x73, data_0208a144))));
}

// Slides the picture away and fades out to white.
void SkipWinTelopTask::OnFinished() {
    MSerialTaskSet *stopSe = (new MSerialTaskSet(TRUE))
                                 ->Add(NewTimer(data_0208a0ec))
                                 ->Add(DinoSoundCreateStopSeTask(0x73, data_0208a144));
    mSerial.AddTail((new MParallelTaskSet(TRUE))
                        ->Add(mView->CreateCoordAnime(MPoint(0, 40), MPoint(0, -mView->GetHeight()), 90, 2))
                        ->Add(gLcd->mMain->mBright->CreateFadeOutToWhite(80, 0))
                        ->Add(gLcd->mSub->mBright->CreateFadeOutToWhite(80, 0))
                        ->Add(stopSe));
}

// The file's static initialization (see dino_static_init.h).
static struct DinoBattleViewEffectTelopStaticInit {
    DinoBattleViewEffectTelopStaticInit() {
        DINO_POOL_WORD(data_ov000_0219387c, 0x38) = 128;
        DINO_POOL_WORD(data_ov000_0219387c, 0x3c) = 176;
        DINO_POOL_WORD(data_ov000_0219387c, 0x48) = 8;
        DINO_POOL_WORD(data_ov000_0219387c, 0x4c) = 150;
        DINO_POOL_WORD(data_ov000_0219387c, 0x68) = 64;
        DINO_POOL_WORD(data_ov000_0219387c, 0x6c) = 112;
    }
} sDinoBattleViewEffectTelopStaticInit;
