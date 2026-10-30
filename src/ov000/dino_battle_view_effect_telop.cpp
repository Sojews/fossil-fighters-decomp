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
#include "m/blend.h"
#include <algorithm>
#include "m/observer.h"
#include "game/dino_battle_view_kaseki_point_menu.h"
#include "game/dino_battle_view_class.h"
#include "m/smart_oam.h"
#include "m/picture.h"
#include "m/ext_affine.h"
#include "m/file.h"
#include "m/seed_generator.h"
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
    u32 unk00;
    const char *countPath;          // 0x04: the win count's picture (by count)
    int loseFrames;                 // 0x08: the least the lost telop stays up
    int winFrames;                  // 0x0C: the least the won telop stays up
    void (DinoBattleViewEffectTelop::*Load_10)();
    void (DinoBattleViewEffectTelop::*State_18)();
    u8 unk20[0x28 - 0x20];
    void (DinoBattleViewEffectTelop::*StateFinish_28)();
    void (DinoBattleViewEffectTelop::*State_30)();
    u8 unk38[0x40 - 0x38];
    void (DinoBattleViewEffectTelop::*Start_40)();
    u8 unk48[0x50 - 0x48];
    void (DinoBattleViewEffectTelop::*State_50)();
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

extern "C" MMotion *func_ov000_02173d64(MSmartOam *oam, const std::string &name, u32 arg2, u32 arg3, bool arg4);

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

extern "C" const int data_0208a158, data_0208a15c;    // the slide out's ease and frames
extern "C" const int data_0208a160;                    // frames the telop stays up
extern "C" const int data_0208a164, data_0208a168;    // the slide in's ease and frames

// The two at once.
static inline MTask *DinoTelopParallel(MTask *a, MTask *b) {
    MParallelTaskSet *set = new MParallelTaskSet(TRUE);
    set->AddTail(a);
    set->AddTail(b);
    return set;
}

static inline MPoint DinoNeg(const MPoint &p) {
    return MPoint(-p.x, -p.y);
}

// Slides the telop in from the side its kind picks (with the player's name), holds it, and slides it back out.
// (One expression: the game gives its temporaries stack slots in this order.)
void DinoBattleViewEffectTelop::StateSlide() {
    mPicture->Show();
    if (mUnk9C) {
        mName->Show();
    }
    MPoint offsets[4] = {MPoint(0, mPicture->GetHeight()), MPoint(0, -mPicture->GetHeight()),
                         MPoint(mPicture->GetWidth(), 0), MPoint(-mPicture->GetWidth(), 0)};
    mPicture->SetPriority(0);
    if (!mUnk9C) {
        mSerial.AddTail(mPicture->CreateCoordAnime(offsets[mUnk98], MPoint(0, 0), data_0208a168, data_0208a164),
                        new MTimer(data_0208a160),
                        mPicture->CreateCoordAnime(MPoint(0, 0), DinoNeg(offsets[mUnk98]),
                                                   data_0208a15c, data_0208a158));
    } else {
        mSerial.AddTail(
            DinoTelopParallel(mPicture->CreateCoordAnime(offsets[mUnk98], MPoint(0, 0), data_0208a168, data_0208a164),
                              mName->CreateCoordAnime(mUnk98 == 3 ? offsets[mUnk98] + MPoint(8, 0)
                                                                  : offsets[mUnk98] + MPoint(248 - mName->GetWidth(), 0),
                                                      mUnk98 == 3 ? MPoint(8, 0) : MPoint(248 - mName->GetWidth(), 0),
                                                      data_0208a168, data_0208a164)),
            new MTimer(data_0208a160),
            DinoTelopParallel(mPicture->CreateCoordAnime(MPoint(0, 0), DinoNeg(offsets[mUnk98]), data_0208a15c,
                                                         data_0208a158),
                              mName->CreateCoordAnime(mUnk98 == 3 ? MPoint(8, 0) : MPoint(248 - mName->GetWidth(), 0),
                                                      mUnk98 == 3 ? MPoint(8, -128)
                                                                  : MPoint(248 - mName->GetWidth(), -128),
                                                      data_0208a15c, data_0208a158)));
    }
    mState = data_ov000_0219387c.StateFinish_28;
}

// Takes the telop away, then reports that it's done.
void DinoBattleViewEffectTelop::StateFinish() {
    GetSerial()->AddTail(MTaskExitTransfer::Create(mPicture), new MTimer(1), new SetBGModeTask(gLcd->mSub->mBg, 0));
    if (mUnk8C != 0) {
        mSerial.AddTail(((MMessageObserver *)mUnk8C)->CreatePostTask((MMessage *)mUnk90));
    }
    mSerial.AddTail(MTaskExitTransfer::Create(this));
}

extern "C" const int data_0208a120;   // frames before a telop can be skipped

// Runs task (the telop), letting a press skip it.
inline SkipTelopTask::SkipTelopTask(MTask *task, u8 noSkip)
    : MProcess(NULL), mDelay(data_0208a120), mUnk80(0), mNoSkip(noSkip) {
    MSingleInputHub *hub = new MSingleInputHub(1);
    mInput = hub->GetInput(0);
    mParallel.AddTail(hub);
    hub->EnableTouch();
    mTasks = new MSerialTaskSet();
    mParallel.AddTail(mTasks);
    mTasks->AddTail(task);
}

inline SkipWinTelopTask::SkipWinTelopTask(MView *picture, DinoBattleView *view, MTask *task)
    : SkipTelopTask(task, view->GetUnk138()) {
    mView = picture;
    mView3D = (DinoBattleView3D *)view;
}

extern "C" const int data_0208a128, data_0208a12c;    // the win count's slide out: ease, frames
extern "C" const int data_0208a130;                    // frames before it slides out
extern "C" const int data_0208a134, data_0208a138;    // its slide in: ease, frames
extern "C" const int data_0208a13c;                    // frames before it shows

// The won telop's least frames. (An inline's result, so its temporary goes on the stack below the random generator.)
static inline int DinoWonLeast() {
    return 0xFE;
}

// The won telop: zooms the picture in from the corner, shakes it less and less, settles it with a hop and waits
// (skippable); the player's win count slides up from below meanwhile.
void DinoBattleViewEffectTelop::StateWin() {
    int frames = std::max(data_ov000_0219387c.winFrames, DinoWonLeast());
    mPicture->Show();
    mParallel.AddTail(mPicture);
    mState = data_ov000_0219387c.State_50;
    MTask *wait = NULL;
    int count = ((DinoBattleView *)mUnk7C)->GetUnk140();
    if (count >= 0) {
        if (count > 99) {
            count = 99;
        }
        char path[256];
        OS_SPrintf(path, data_ov000_0219387c.countPath, count);
        if (!MFileExists(std::string(path))) {
            OS_SPrintf(path, data_ov000_0219387c.countPath, 1);
        }
        MTextPicture *text = new MTextPicture(std::string(path), TRUE, false);
        text->Show();
        text->SetPriority(1);
        text->SetPosition(MPoint(0, 192));
        MSerialTaskSet *slide = new MSerialTaskSet(TRUE);
        wait = new NopWaitTask();
        slide->AddTail(new MTimer(data_0208a13c), text->CreateShowTransfer(),
                       text->CreateCoordAnime(MPoint(0, 192), MPoint(0, 0), data_0208a138, data_0208a134), wait,
                       new MTimer(data_0208a130),
                       text->CreateCoordAnime(MPoint(0, 0), MPoint(0, 192), data_0208a12c, data_0208a128));
        MTaskSet *parallel = &mParallel;
        parallel->AddTail(slide);
        parallel->AddTail(text);
    }
    MRandom random;
    MSerialTaskSet *tasks = new MSerialTaskSet(TRUE);
    tasks->AddTail(DinoTelopParallel(mPicture->GetScreen()->CreateScaleAnime(MFx32Point(MFx32(2), MFx32(2)),
                                                                             MFx32Point(MFx32(1), MFx32(1)), 20, 1),
                                     mPicture->CreateCoordAnime(MPoint(-128, -96), MPoint(0, 0), 20, 1)));
    for (int i = 0; i < 16; i++) {
        int shake = i / 4 + 1;
        int dx = random.Next(shake * 2 + 1) - shake;
        int dy = random.Next(shake * 2 + 1) - shake;
        tasks->AddTail(mPicture->CreateCoordTransfer(MPoint(dx, dy)), new MTimer(1));
    }
    tasks->AddTail(mPicture->CreateCoordTransfer(MPoint(0, 0)), new MTimer(frames += 30));
    if (wait != NULL) {
        tasks->AddTail(MTaskExitTransfer::Create(wait));
    }
    tasks->AddTail(mPicture->CreateCoordAnime(MPoint(0, 0), MPoint(0, 20), 30, 2),
                   mPicture->CreateCoordAnime(MPoint(0, 20), MPoint(0, 40), 30, -2));
    MParallelTaskSet *skip = new MParallelTaskSet();
    mParallel.AddTail(skip);
    skip->AddTail(new SkipWinTelopTask(mPicture, (DinoBattleView *)mUnk7C, tasks));
    mSerial.AddTail(MTaskSetWaitEmptyTask::Create(skip));
}

// (The picture first: the game loads the view before it.)
inline SkipLoseTelopTask::SkipLoseTelopTask(MView *picture, DinoBattleView *view, MTask *task)
    : SkipTelopTask(task, view->GetUnk138()) {
    mView = picture;
    mView3D = (DinoBattleView3D *)view;
}

// The lost telop: slides the picture in from above with a bounce, fades the screen behind it, and waits (skippable).
extern "C" const int data_ov000_02193884;   // (the frames' own label: the game takes their address from it)

// (Through a non-member inline, the game's blend gets the lowest stack slot.)
static inline MBlend *DinoSubBlend() {
    return gLcd->mSub->mBlend;
}

void DinoBattleViewEffectTelop::StateLose() {
    int least = 0xDC;
    int frames = *(*(const int *)((const u8 *)&data_ov000_0219387c + 8) < least ? &least : &data_ov000_02193884);
    mPicture->Show();
    mPicture->SetPosition(MPoint(0, -mPicture->GetHeight()));
    mParallel.AddTail(mPicture);
    mState = data_ov000_0219387c.State_30;
    MBlend *blend = DinoSubBlend();
    blend->SetMode(3);
    blend->SetTarget1(~mPicture->GetPlaneMask() & 0x3F);
    blend->SetTarget2(0);
    MSerialTaskSet *tasks = new MSerialTaskSet(TRUE);
    tasks->AddTail(blend->CreateFadeRatioAnime(0, 8, 30, 0), new MTimer(90),
                   mPicture->CreateCoordAnime(MPoint(0, -mPicture->GetHeight()), MPoint(0, 0), 90, -2), new MTimer(30),
                   mPicture->CreateCoordAnime(MPoint(0, 0), MPoint(0, -10), 20, 2),
                   mPicture->CreateCoordAnime(MPoint(0, -10), MPoint(0, -20), 20, -2), new MTimer(frames));
    MParallelTaskSet *skip = new MParallelTaskSet();
    mParallel.AddTail(skip);
    skip->AddTail(new SkipLoseTelopTask(mPicture, (DinoBattleView *)mUnk7C, tasks));
    mSerial.AddTail(MTaskSetWaitEmptyTask::Create(skip));
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
