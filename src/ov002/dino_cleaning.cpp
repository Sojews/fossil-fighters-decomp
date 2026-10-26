// DinoCleaning: the fossil cleaning scene, with DinoCleaningLive.

#include <algorithm>
#include "m/lcd.h"
#include "game/dino_cleaning.h"
#include "game/dino_cleaning_kaseki_rock.h"
#include "game/dino_cleaning_dummy_item_menu.h"
#include "game/dino_cleaning_live_task.h"
#include "game/dino_cleaning_stop_confirm_menu_item_menu.h"
#include "game/dino_cleaning_tool.h"
#include "game/dino_common_live.h"
#include "game/dino_sound.h"
#include "game/dino_cleaning_rock.h"
#include "game/dino_cleaning_image.h"
#include "game/dino_cleaning_image_view.h"
#include "m/display.h"
#include "m/smart_oam.h"
#include "m/blend.h"
#include "game/dino_cleaning_paper.h"
#include "game/dino_se_motion.h"
#include "game/dino_cleaning_report.h"
#include "m/bright.h"
#include "game/dino_cleaning_kaseki_rock_xray_blend_manager.h"
#include "game/help.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "types.h"
#include "game/dino_cleaning_status.h"
#include "game/dino_cleaning_score.h"
#include "game/dino_cleaning_timer.h"
#include "m/input.h"
#include "game/dino_cleaning_neighbour.h"
#include "m/ext_affine.h"
#include "m/picture.h"
#include "m/menu.h"
#include "m/seed_generator.h"
#include "m/window.h"
#include <stdio.h>
#include "game/dino_static_init.h"
#include "game/dino_cleaning_drill.h"
#include "game/dino_cleaning_hammer.h"
#include "game/dino_cleaning_xray.h"
#include "game/dino_cleaning_blower.h"
#include "m/bg.h"
#include "m/obj.h"

#pragma auto_inline off

// Minus half of d (to centre something d bigger than the screen).
static inline MPoint DinoHalfBack(const MPoint &d) {
    return MPoint(-d.x / 2, -d.y / 2);
}

// (In the kaseki rock's file.)
extern "C" void func_ov002_0215fdec(DinoCleaningKasekiRock *rock);

// NONMATCHING: the members' destructors are missing (the member layout isn't known yet).
// Puts both screens back the way the field wants them.
DinoCleaning::~DinoCleaning() {
    Unregister();
    mParallel.Clear();
    delete mRandom;
    gLcd->mMain->ResetHooks();
    gLcd->mSub->ResetHooks();
    gLcd->mMain->SetBackdrop(0x8000);
    gLcd->mMain->Setup(5, 4, 0, 0);
    gLcd->mSub->Setup(5, 4, 0, 0);
    gLcd->VSyncNow();
    gLcd->SetBottomDisplay(gLcd->mSub);
    gCurrentDisplay = gLcd->GetBottomDisplay();
}

MInput *DinoCleaning::GetUnkDC() {
    return mUnkDC;
}

MRandom *DinoCleaning::GetRandom() {
    return mRandom;
}

u32 DinoCleaning::GetUnkD4() {
    return mUnkD4;
}

DinoCleaningKasekiRock *DinoCleaning::GetKasekiRock() {
    return mKasekiRock;
}

DinoCommonLive *DinoCleaning::GetUnkE4() {
    return mUnkE4;
}

void DinoCleaning::SetUnk128(BOOL value) {
    mUnk128 = value;
}

void DinoCleaning::Execute() {
    if (!mUnk128) {
        MProcess::Execute();
    } else if (mUnk144->mState == STATE_RUN) {
        mUnk144->Execute();
    }
}

void DinoCleaning::Idle() {
    (this->*mState)();
}

void DinoCleaning::OnSleep() {
    mSleepTimer = 180;
}

void DinoCleaning::OnWake() {
}

void DinoCleaning::OnPreLoop() {
    if (mSleepTimer != 0) {
        mSleepTimer--;
    }
}

// The scene's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x4];
    MFx32 mChipPower;           // 0x04: what each cell takes when the rock is cleared at once
    const char *mCountFormat;   // 0x08
    const char *mOnesPath;      // 0x0C: the passed score's ones digit
    const char *mFailPath;      // 0x10: the picture of a failed cleaning
    const char *mCleanPath;     // 0x14: the picture shown when the rock comes clean
    const char *mBestScorePath; // 0x18: the best score's frame
    const char *mBestTensPath;  // 0x1C
    const char *mBestPath;      // 0x20: the picture of a best cleaning
    const char *mTitlePath;     // 0x24: the title (the first mode's)
    u32 mMaxBonus;              // 0x28: the most the time bonus gives
    int mZero_2C;               // 0x2C: (the constant the count's shake is clamped to)
    const char *mBestHundredsPath; // 0x30
    const char *mBestOnesPath;  // 0x34
    const char *mTensPath;      // 0x38
    const char *mPassPath;      // 0x3C: the picture of a passed cleaning
    const char *mScorePath;     // 0x40: the passed score's frame
    const char *mTitlePath2;    // 0x44: the other modes' title
    void (DinoCleaning::*State02160638_48)(); // 0x48
    u8 unk50[0x58 - 0x50];
    void (DinoCleaning::*State02160de8_58)(); // 0x58
    u8 unk60[0x68 - 0x60];
    void (DinoCleaning::*State0216177c_68)(); // 0x68
    void (DinoCleaning::*State02161d48_70)(); // 0x70
    void (DinoCleaning::*StatePlay_78)(); // 0x78
} data_ov002_021659ec;
extern "C" struct {
    void (DinoCleaning::*StateEnableToolMenu_00)(); // 0x00
    u8 unk08[0x10 - 0x8];
    void (DinoCleaning::*State02161164_10)(); // 0x10
    void (DinoCleaning::*StateCheckDone_18)(); // 0x18
    void (DinoCleaning::*StateQuit_20)(); // 0x20
    void (DinoCleaning::*StatePlay_28)(); // 0x28
    u8 unk30[0x38 - 0x30];
    void (DinoCleaning::*State02162544_38)(); // 0x38
    void (DinoCleaning::*State021618c8_40)(); // 0x40
    u8 unk48[0x50 - 0x48];
    void (DinoCleaning::*State02161d48_50)(); // 0x50
    void (DinoCleaning::*StateCheckDone_58)(); // 0x58
    void (DinoCleaning::*State02161f9c_60)(); // 0x60
    void (DinoCleaning::*StatePlay_68)(); // 0x68
    void (DinoCleaning::*StateWake_70)();
    void (DinoCleaning::*State02160f54_78)(); // 0x78
} data_ov002_02165a6c;
extern "C" struct {
    u8 unk00[0x8];
    void (DinoCleaning::*State02161d48_08)(); // 0x08
    void (DinoCleaning::*StateStopTool_10)();
    u8 unk18[0x20 - 0x18];
    void (DinoCleaning::*StateStopAnswered_20)();
    void (DinoCleaning::*State02162cec_28)(); // 0x28
    u8 unk30[0x40 - 0x30];
    void (DinoCleaning::*State02162cec_40)(); // 0x40
    void (DinoCleaning::*State0216177c_48)(); // 0x48
    void (DinoCleaning::*StateSwitchTool_50)(); // 0x50
    void (DinoCleaning::*State02160de8_58)(); // 0x58
    void (DinoCleaning::*StateStopTool_60)();
    u8 unk68[0x78 - 0x68];
    void (DinoCleaning::*State021610b0_78)();
    void (DinoCommonLive::*SetUnkA0_80)(u32, bool); // 0x80 (the next block's first entry)
} data_ov002_02165aec;
extern "C" struct {
    void (DinoCommonLive::*SetUnkA0_00)(u32, bool); // 0x00 (DinoCommonLive::SetUnkA0)
    void (DinoCleaning::*State02162cec_08)(); // 0x08
    void (DinoCleaning::*State02161260_10)(); // 0x10
    void (DinoHelpBase::*Enable_18)(); // 0x18
    void (DinoCleaning::*State021610b0_20)(); // 0x20
} data_ov002_02165b6c;

DinoCleaning::DinoCleaning(u32 unk9C, void *unkA0, u32 unkA8, u32 unkAC, int mode, u32 unk140)
    : MProcess(NULL), MLooperPreLoopHook(FALSE, TRUE) {
    mUnk9C = unk9C;
    mUnkA0 = unkA0;
    mUnkA8 = unkA8;
    mUnkAC = unkAC;
    mMode = mode;
    mUnk128 = FALSE;
    mPaper = NULL;
    mUnk140 = unk140;
    mState = data_ov002_021659ec.State02160638_48;
}

// Chips at every cell left of the top rock and shakes the rock; once nothing is left, settles it and checks the result.
void DinoCleaning::State0216177c() {
    BOOL left = FALSE;
    DinoHelpBase::Disable();
    for (int col = 0; col < 28; col++) {
        for (int row = 0; row < 36; row++) {
            if (!mKasekiRock->GetRock90()->IsCellClear(row, col)) {
                left = TRUE;
                mKasekiRock->GetRock90()->Hit(row, col, data_ov002_021659ec.mChipPower);
            }
        }
    }
    MFx32 amp = MFx32(6);
    mKasekiRock->SetOffsetAll(MPoint(MFx32ToInt(amp * mRandom->NextFraction() - amp / MFx32(2)),
                                     MFx32ToInt(amp * mRandom->NextFraction() - amp / MFx32(2))));
    if (!left) {
        mKasekiRock->SetOffsetAll(MPoint(0, 0));
        mState = data_ov002_02165a6c.StateCheckDone_58;
    }
}

extern "C" const int data_0208a238;    // the score below which the result is poor

// Stops everything and judges the cleaning by the score.
void DinoCleaning::State021618c8() {
    mUnkE4->SetUnkA0(1, FALSE);
    mUnkE4->SetUnkA0(2, TRUE);
    DinoHelpBase::Disable();
    mUnk10C->Exit();
    mStatus->StatePause();
    mToolMenu->DisableAll();
    mKasekiRock->Finish();
    mToolMenu->Deactivate();
    gCurrentDisplay->mBlend->Reset();
    gCurrentDisplay->mWin->Reset();
    DinoHelpBase::Disable();
    mSerial.AddTail(new MTimer(60));
    if (mUnkC8) {
        mState = data_ov002_02165a6c.StateQuit_20;
        return;
    }
    if (mStatus->GetScore()->GetUnk80() < MFx32(mUnk130)) {
        DinoSoundPlaySe(0x37, 0);
        mState = data_ov002_021659ec.State02161d48_70;
        return;
    }
    if (mStatus->GetScore()->GetUnk84() > MFx32(100 - mUnk130)) {
        DinoSoundPlaySe(0x37, 0);
        mState = data_ov002_02165a6c.State02161d48_50;
        return;
    }
    if (mStatus->GetScore()->GetUnk80() < MFx32(data_0208a238)) {
        DinoSoundPlaySe(0x39, 0);
        mState = data_ov002_02165a6c.State02161f9c_60;
        return;
    }
    DinoSoundPlaySe(0x3A, 0);
    mState = data_ov002_02165a6c.State02162544_38;
}

extern "C" void (DinoCleaning::*const data_ov002_02165a44)();  // (pooled: State02160de8)
extern "C" const char data_ov002_02165f58[];    // the live's script, by mode
extern "C" const char data_ov002_02165f6c[];
extern "C" const char data_ov002_02165f84[];

static inline MParallelTaskSet *DinoCleaningPar(MTask *a, MTask *b) {
    MParallelTaskSet *set = new MParallelTaskSet(TRUE);
    set->AddTail(a);
    set->AddTail(b);
    return set;
}

// Sets the cleaning up: its fossil's numbers, the screens, the input, the live, the tools and the menus.
// NONMATCHING: 99.7%, the game loads the title size's y before its x when centring it (scheduling).
void DinoCleaning::State02160638() {
    DinoSoundLoadScene(0xD);
    DinoHelpBase::Disable();
    if (mUnkA0 != NULL) {
        ((u32 *)mUnkA0)[0] = 0;
        ((u32 *)mUnkA0)[1] = 0;
    }
    mUnk128 = 0;
    mSleepTimer = 0;
    MSleepHook::Register();
    MResource<DinoKasekiDefs> defs = gDinoResource->LoadKasekiDefs();
    const u8 *def = (const u8 *)defs.Get()->GetDef(mUnk9C);
    mUnkA4 = ((const u32 *)def)[0x24 / 4];
    mUnkB4 = ((const u32 *)def)[0x8 / 4];
    mUnkB8 = ((const u32 *)def)[0xC / 4];
    mUnkC0 = (MFx32::FromRaw(0x800) * MFx32::FromRaw(((const u32 *)def)[0x10 / 4])).v;
    mUnkC4 = (MFx32::FromRaw(0x800) * MFx32::FromRaw(((const u32 *)def)[0x14 / 4])).v;
    mUnkBC = def[1];
    mUnk12C = ((const u32 *)def)[0x2C / 4] - 1;
    mUnk130 = ((const u32 *)def)[0x34 / 4];
    mUnkC8 = def[2];
    mUnkC9 = def[3];
    mUnkCA = def[5];
    mUnkCC = *(const u32 *)(def + ((const u32 *)def)[0x40 / 4]);
    mUnkD0 = ((const u32 *)(def + ((const u32 *)def)[0x40 / 4]))[1];
    gLcd->mMain->ResetHooks();
    gLcd->mSub->ResetHooks();
    gLcd->mSub->Setup(5, 2, 4, 1, 0, 0);
    gLcd->mMain->Setup(6, 1, 3, 1, 0, 0);
    gLcd->mMain->mObj->Show();
    gLcd->mSub->mObj->Show();
    gLcd->mMain->SetBackdrop(0xFFFF);
    gLcd->SetBottomDisplay(gLcd->mMain);
    gCurrentDisplay = gLcd->mMain;
    gLcd->VSyncNow();
    SetDisplay(gCurrentDisplay);
    gCurrentDisplay->mBg->SetMode(5);
    mRandom = new MRandom();
    MSingleInputHub *hub = new MSingleInputHub(1);
    hub->EnableTouch();
    mUnkDC = hub->GetInput(0);
    mParallel.AddTail(hub);
    std::string script;
    switch (mMode) {
    case 0:
        script = data_ov002_02165f58;
        break;
    case 3:
        script = data_ov002_02165f58;
        break;
    case 1:
        script = data_ov002_02165f6c;
        break;
    case 2:
        script = data_ov002_02165f84;
        break;
    }
    mUnk144 = new MParallelTaskSet();
    mUnkE4 = new DinoCleaningLive((u32)this, std::string(script));
    mUnkE4->SetUnkB8(7, (void *)mUnkAC);
    mUnkE4->SetUnkB8(10, (void *)mUnkA8);
    mUnkE4->SetUnkB8(8, (void *)((const u32 *)def)[0x24 / 4]);
    mUnkE4->SetUnkB8(9, (void *)((mUnk9C - 1) & 3));
    mUnkE4->SetUnkB8(5, (void *)100);
    mUnkE4->SetUnkA0(0, 1);
    mUnkE4->SetUnkA0(0x17, mUnkAC == 0 && ((const u32 *)def)[0x24 / 4] == 1);
    mUnk144->AddTail(mUnkE4);
    mParallel.AddTail(mUnk144);
    mUnk13C = new MParallelTaskSet();
    mParallel.AddTail(mUnk13C);
    mUnkD4 = (u32) new MSmartOam(100, 50, TRUE);
    ((MSmartOam *)mUnkD4)->SetUnk1C(7);
    mTools[0] = new DinoCleaningDrill(this, 0);
    mParallel.AddTail(mTools[0]);
    mTools[1] = new DinoCleaningDrill(this, 1);
    mParallel.AddTail(mTools[1]);
    mTools[2] = new DinoCleaningHammer(this, 0);
    mParallel.AddTail(mTools[2]);
    mTools[3] = new DinoCleaningHammer(this, 1);
    mParallel.AddTail(mTools[3]);
    mTools[4] = new DinoCleaningXray(this);
    mParallel.AddTail(mTools[4]);
    mToolMenu = new DinoCleaningToolMenu((u32)this, mUnkD4, (u32)&mNextTool);
    mToolMenu->Deactivate();
    mParallel.AddTail(mToolMenu);
    mUnk10C = new DinoCleaningBlower(this);
    mParallel.AddTail(mUnk10C);
    mUnk120 = (u32) new MTextPicture(
        std::string(mMode == 0 ? data_ov002_021659ec.mTitlePath : data_ov002_021659ec.mTitlePath2), 1, true);
    ((MTextPicture *)mUnk120)->SetPosition(DinoHalfBack(((MTextPicture *)mUnk120)->GetSize() - MPoint(256, 192)));
    ((MTextPicture *)mUnk120)->SetPriority(3);
    ((MTextPicture *)mUnk120)->Show();
    mKasekiRock = NULL;
    mUnk11C = new MParallelTaskSet();
    mParallel.AddTail(mUnk11C);
    mUnk11C->AddTail(MCreateBgCallTask(this, data_ov002_02165a44, 1));
    mParallel.AddTail((MTextPicture *)mUnk120);
    mParallel.AddTail((MSmartOam *)mUnkD4);
    mNextTool = 2;
    mTool = mNextTool;
    mStatus = new DinoCleaningStatus(this, ((const u32 *)def)[0x30 / 4] * 60, mUnk130, mUnkC9);
    mParallel.AddTail(mStatus);
    mSerial.AddTail(DinoCleaningPar(gLcd->mMain->mBright->CreateFadeInFromWhite(30, 0),
                                    gLcd->mSub->mBright->CreateFadeInFromWhite(30, 0)));
    mState = data_ov002_02165aec.State02160de8_58;
}

// Creates the kaseki rock (and runs it once).
void DinoCleaning::State02160de8() {
    DinoCleaningKasekiRock *rock = new DinoCleaningKasekiRock(this, mUnk120, mUnkB4, mUnkC0, mUnkA8, mUnkBC, mUnkB8,
                                                              mUnkC4, mUnkAC, mUnkC8, mUnkC9, mUnkCC, mUnkD0);
    if (rock->MTask::mState == STATE_RUN) {
        rock->Execute();
    }
    mKasekiRock = rock;
}

// Starts the status display, then waits a moment.
void DinoCleaning::State02160e78() {
    DinoSoundPlaySe(0x38, 0);
    mStatus->Start();
    MTask *timer = new MTimer(30);
    MTask *wait = MTaskSetWaitEmptyTask::Create(mUnk11C);
    MParallelTaskSet *waiting = new MParallelTaskSet(TRUE);
    waiting->AddTail(wait);
    waiting->AddTail(timer);
    mSerial.AddTail(waiting);
    mState = data_ov002_02165a6c.State02160f54_78;
}

// Shows the count (if any left) as text for a second, counting it down.
void DinoCleaning::State021610b0() {
    mToolMenu->Activate();
    if (mUnk110 == 0) {
        mState = data_ov002_02165b6c.State02161260_10;
        return;
    }
    char text[0x100];
    sprintf(text, data_ov002_021659ec.mCountFormat, mUnk110);
    mUnk118 = new MTextPicture(text, TRUE, true);
    mParallel.AddTail(mUnk118);
    mUnk114 = 60;
    mUnk110 = mUnk110 - 1;
    DinoSoundPlaySe(0x3B, 0);
    mState = data_ov002_02165a6c.State02161164_10;
}

// A random offset in [-range/2, range/2).
// Shakes the count's text, less as its second runs out, then counts on.
extern "C" const int data_ov002_02165a18;  // (the pool's +0x2C by its own name)

// std::max on a pooled operand: its value read through the pool, the operand returned by its own symbol.
static inline const int &DinoMaxPooled(const int &a, const int &b, const int &bNamed) {
    return a < b ? bNamed : a;
}

void DinoCleaning::State02161164() {
    MFx32 scale = MFx32(DinoMaxPooled((int)mUnk114 - 45, data_ov002_021659ec.mZero_2C, data_ov002_02165a18)) /
                  MFx32(2);
    mUnk118->Show();
    mUnk118->SetPosition(MFx32ToInt(scale * mRandom->NextFraction() - scale / MFx32(2)),
                         MFx32ToInt(scale * mRandom->NextFraction() - scale / MFx32(2)));
    mUnk114--;
    if (mUnk114 == 0) {
        mUnk118->Exit();
        mState = data_ov002_02165aec.State021610b0_78;
    }
}



static inline void AddSerial(DinoCleaning *process, MTask *a, MTask *b, MTask *c, MTask *d) {
    MSerialTaskSet *serial = &process->mSerial;
    serial->AddTail(a);
    serial->AddTail(b);
    serial->AddTail(c);
    serial->AddTail(d);
}

// Starts the rock and, after a moment, the help and the live commentary.
void DinoCleaning::State02160f54() {
    mUnk11C->AddTail(mKasekiRock);
    mKasekiRock->StartDrop();
    AddSerial(this, new MTimer(60), MCreateCallTask((DinoCleaningLive *)mUnkE4, data_ov002_02165aec.SetUnkA0_80, (u32)3, true),
              new MTimer(2), MCreateCallTask(this, data_ov002_02165b6c.Enable_18));
    mUnk110 = 3;
    mState = data_ov002_02165b6c.State021610b0_20;
}

extern "C" struct {
    void (DinoHelpBase::*Enable_00)(); // 0x00
} data_ov002_02165b5c;

static inline void AddSerial(DinoCleaning *process, MTask *a, MTask *b, MTask *c, MTask *d, MTask *e, MTask *f) {
    MSerialTaskSet *serial = &process->mSerial;
    serial->AddTail(a);
    serial->AddTail(b);
    serial->AddTail(c);
    serial->AddTail(d);
    serial->AddTail(e);
    serial->AddTail(f);
}

// The rock is clean: stops the x-ray and the status, flashes the picture, then gives back the help.
void DinoCleaning::State02161260() {
    mUnkE4->SetUnkA0(0, FALSE);
    mUnkE4->SetUnkA0(1, TRUE);
    mUnkE4->SetUnkAC(4);
    if (mMode != 2) {
        DinoSoundPlayBgm(0x36, 0);
    }
    DinoSoundPlaySe(0x1B, 0);
    mKasekiRock->StopXray();
    mStatus->Stop();
    mToolMenu->FocusFirst();
    mToolMenu->DisableItem();
    mTool = mNextTool;
    MTextPicture *picture = new MTextPicture(std::string(data_ov002_021659ec.mCleanPath), TRUE, false);
    picture->Show();
    mParallel.AddTail(picture);
    MBlend *blend = gCurrentDisplay->mBlend;
    blend->SetMode(1);
    blend->SetTarget1(picture->GetPlaneMask());
    blend->SetTarget2(0x3F);
    blend->SetAlpha(16, 16);
    DinoHelpBase::Disable();
    AddSerial(this, new MTimer(1), blend->CreateAlphaRatioAnime(16, 0, 16, 16, 5, 0), MTaskExitTransfer::Create(picture),
              blend->CreateClearTransfer(), new MTimer(1), MCreateCallTask(this, data_ov002_02165b5c.Enable_00));
    mState = data_ov002_02165a6c.StateEnableToolMenu_00;
}

extern "C" struct {
    void (DinoCommonLive::*SetUnkAC_00)(u32); // 0x00
} data_ov002_02165b24;

static inline void AddSerial(DinoCleaning *process, MTask *a, MTask *b) {
    MSerialTaskSet *serial = &process->mSerial;
    serial->AddTail(a);
    serial->AddTail(b);
}

static inline void AddSerial(DinoCleaning *process, MTask *a, MTask *b, MTask *c, MTask *d, MTask *e) {
    MSerialTaskSet *serial = &process->mSerial;
    serial->AddTail(a);
    serial->AddTail(b);
    serial->AddTail(c);
    serial->AddTail(d);
    serial->AddTail(e);
}

// The cleaning failed: the picture rises, swaying, then spins in.
void DinoCleaning::State02161d48() {
    mUnkE4->SetUnkA0(6, TRUE);
    if (mUnkA0 != NULL) {
        ((u32 *)mUnkA0)[0] = 0;
        ((u32 *)mUnkA0)[3] = 0;
    }
    mResultPicture = new MExtAffinePicture(std::string(data_ov002_021659ec.mFailPath), TRUE, false);
    mParallel.AddTail(mResultPicture);
    mResultPicture->Show();
    mResultPicture->GetScreen()->SetCenter(0x33, 0x2E);
    for (u32 i = 0; i <= 120; i++) {
        MPoint pos;
        pos.x = MFx32ToInt(MFx32::FromRaw(FX_SinIdx((u16)(i * 0x10000 / 40))) * MFx32(30));
        pos.y = 192 - i * 192 / 120;
        AddSerial(this, mResultPicture->CreateCoordTransfer(pos), new MTimer(1));
    }
    AddSerial(this, new MTimer(60), DinoSoundCreatePlaySeTask(0x41, 0),
              mResultPicture->GetScreen()->CreateRotAnime(0, 0x1000, 20, 2),
              MCreateCallTask((DinoCleaningLive *)mUnkE4, data_ov002_02165b24.SetUnkAC_00, (u32)0x16), new MTimer(120));
    mState = data_ov002_02165aec.State02162cec_28;
}

extern "C" struct {
    void (DinoCommonLive::*SetUnkAC_00)(u32); // 0x00
} data_ov002_02165b1c, data_ov002_02165b54;

static inline void AddSerial(DinoCleaning *process, MTask *a, MTask *b, MTask *c) {
    MSerialTaskSet *serial = &process->mSerial;
    serial->AddTail(a);
    serial->AddTail(b);
    serial->AddTail(c);
}

// The cleaning passed: the picture zooms in, with the score, then shakes and sways, less and less.
void DinoCleaning::State02161f9c() {
    mUnkE4->SetUnkA0(7, TRUE);
    if (mUnkA0 != NULL) {
        ((u32 *)mUnkA0)[3] = 1;
        if (((u32 *)mUnkA0)[0] == 0) {
            ((u32 *)mUnkA0)[0] = MFx32ToInt(mStatus->GetScore()->GetUnk80());
        }
    }
    mResultPicture = new MExtAffinePicture(std::string(data_ov002_021659ec.mPassPath), TRUE, false);
    mParallel.AddTail(mResultPicture);
    mResultPicture->Show();
    mResultPicture->GetScreen()->SetCenter(0x80, 0x60);
    mSerial.AddTail(DinoCleaningPar(mResultPicture->CreateCoordAnime(MPoint(0, 192), MPoint(0, -32), 60, -2),
                                    mResultPicture->GetScreen()->CreateScaleAnime(
                                        MFx32Point(MFx32::FromRaw(0x1800), MFx32::FromRaw(0x1800)),
                                        MFx32Point(MFx32(1), MFx32(1)), 60, 0)));
    if (mUnkCA) {
        u32 score = MFx32ToInt(mStatus->GetScore()->GetUnk80());
        MMotion *motion = new MMotion(std::string(data_ov002_021659ec.mScorePath), (MSmartOam *)mUnkD4, 0, 1);
        motion->Show();
        motion->GetUnit()->SetMode(1);
        motion->SetPosition(0x80, 0x60);
        mUnk13C->AddTail(motion);
        mSerial.AddTail(motion->CreatePlayTransfer(1, 0));
        motion = new MMotion(std::string(data_ov002_021659ec.mTensPath), (MSmartOam *)mUnkD4, 0, 1);
        motion->Show();
        motion->GetUnit()->SetMode(1);
        motion->SetPosition(0x80, 0x60);
        mUnk13C->AddTail(motion);
        mSerial.AddTail(motion->CreatePlayTransfer(score / 10 + 1, 0));
        motion = new MMotion(std::string(data_ov002_021659ec.mOnesPath), (MSmartOam *)mUnkD4, 0, 1);
        motion->Show();
        motion->GetUnit()->SetMode(1);
        motion->SetPosition(0x80, 0x60);
        mUnk13C->AddTail(motion);
        mSerial.AddTail(motion->CreatePlayTransfer(score % 10 + 1, 0));
    }
    AddSerial(this, mResultPicture->CreateCoordAnime(MPoint(0, -32), MPoint(0, 0), 10, 2),
              DinoSoundCreatePlaySeTask(0x3D, 0));
    for (int i = 20; i >= 0; i--) {
        MFx32 amp = MFx32(i) / MFx32(2);
        AddSerial(this,
                  mResultPicture->CreateCoordTransfer(MFx32ToInt(amp * mRandom->NextFraction() - amp / MFx32(2)),
                                                         MFx32ToInt(amp * mRandom->NextFraction() - amp / MFx32(2))),
                  mResultPicture->GetScreen()->CreateRotTransfer(
                      MFx32ToInt(amp * mRandom->NextFraction() * MFx32(512) - amp * MFx32(512) / MFx32(2))),
                  new MTimer(1));
    }
    AddSerial(this, MCreateCallTask((DinoCleaningLive *)mUnkE4, data_ov002_02165b1c.SetUnkAC_00, (u32)0x16),
              new MTimer(100));
    mState = data_ov002_02165aec.State02162cec_40;
}

// The best cleaning: paper falls, the picture drops in, zooms and shakes, then the score counts in.
// NONMATCHING: 99.996%, the drop's first point stores its x after the second point (State02161f9c's identical call
// stores it before).
void DinoCleaning::State02162544() {
    mUnkE4->SetUnkA0(8, TRUE);
    if (mUnkA0 != NULL) {
        if (mUnkA4) {
            ((u32 *)mUnkA0)[3] = 1;
            if (((u32 *)mUnkA0)[0] == 0) {
                ((u32 *)mUnkA0)[0] = MFx32ToInt(mStatus->GetScore()->GetUnk80());
            }
        } else {
            ((u32 *)mUnkA0)[3] = 2;
            if (((u32 *)mUnkA0)[0] == 0) {
                ((u32 *)mUnkA0)[0] = MFx32ToInt(mStatus->GetScore()->GetUnk80());
            }
        }
    }
    MSmartOam *oam = new MSmartOam(gLcd->GetTopDisplay(), 100, 50, TRUE);
    oam->mOffset.x = 0;
    oam->mOffset.y = -192;
    mPaper = new DinoCleaningPaper(oam, (MSmartOam *)mUnkD4, mRandom);
    mParallel.AddHead(mPaper);
    mParallel.AddTail(oam);
    mResultPicture = new MExtAffinePicture(std::string(data_ov002_021659ec.mBestPath), TRUE, false);
    mParallel.AddTail(mResultPicture);
    mResultPicture->Show();
    mResultPicture->GetScreen()->SetCenter(0x80, 0x60);
    AddSerial(this,
              DinoCleaningPar(mResultPicture->CreateCoordAnime(MPoint(0, 192), MPoint(0, -32), 60, -2),
                              mResultPicture->GetScreen()->CreateScaleAnime(
                                  MFx32Point(MFx32::FromRaw(0x1800), MFx32::FromRaw(0x1800)),
                                  MFx32Point(MFx32(1), MFx32(1)), 60, 0)),
              mResultPicture->CreateCoordAnime(MPoint(0, -32), MPoint(0, 0), 10, 2), DinoSoundCreatePlaySeTask(0x3E, 0));
    for (int i = 20; i >= 0; i--) {
        MFx32 amp = MFx32(i) / MFx32(2);
        AddSerial(this,
                  mResultPicture->CreateCoordTransfer(MFx32ToInt(amp * mRandom->NextFraction() - amp / MFx32(2)),
                                                         MFx32ToInt(amp * mRandom->NextFraction() - amp / MFx32(2))),
                  mResultPicture->GetScreen()->CreateRotTransfer(
                      MFx32ToInt(amp * mRandom->NextFraction() * MFx32(512) - amp * MFx32(512) / MFx32(2))),
                  new MTimer(1));
    }
    if (mUnkCA) {
        MParallelTaskSet *set = new MParallelTaskSet(TRUE);
        DinoSeMotion *motion = new DinoSeMotion(std::string(data_ov002_021659ec.mBestScorePath), (MSmartOam *)mUnkD4, 0, 1);
        motion->AddSe(0x3C);
        motion->GetUnit()->SetMode(1);
        motion->Show();
        motion->SetAnchor(1);
        motion->SetPosition(0x80, 0x60);
        mUnk13C->AddTail(motion);
        mSerial.AddTail(motion->CreatePlayTransfer(1, 0));
        set->AddTail(motion->CreateWaitStopTask());
        u32 score = ((u32 *)mUnkA0)[0];
        if (score >= 100) {
            motion = new DinoSeMotion(std::string(data_ov002_021659ec.mBestHundredsPath), (MSmartOam *)mUnkD4, 0, 1);
            motion->AddSe(0x3C);
            motion->GetUnit()->SetMode(1);
            motion->Show();
            motion->SetAnchor(1);
            motion->SetPosition(0x80, 0x60);
            mUnk13C->AddTail(motion);
            mSerial.AddTail(motion->CreatePlayTransfer(1, 0));
            set->AddTail(motion->CreateWaitStopTask());
        }
        if (score >= 10) {
            motion = new DinoSeMotion(std::string(data_ov002_021659ec.mBestTensPath), (MSmartOam *)mUnkD4, 0, 1);
            motion->AddSe(0x3C);
            motion->GetUnit()->SetMode(1);
            motion->Show();
            motion->SetAnchor(1);
            motion->SetPosition(0x80, 0x60);
            mUnk13C->AddTail(motion);
            mSerial.AddTail(motion->CreatePlayTransfer(score / 10 % 10 + 1, 0));
            set->AddTail(motion->CreateWaitStopTask());
        }
        motion = new DinoSeMotion(std::string(data_ov002_021659ec.mBestOnesPath), (MSmartOam *)mUnkD4, 0, 1);
        motion->AddSe(0x3C);
        motion->GetUnit()->SetMode(1);
        motion->Show();
        motion->SetAnchor(1);
        motion->SetPosition(0x80, 0x60);
        mUnk13C->AddTail(motion);
        mSerial.AddTail(motion->CreatePlayTransfer(score % 10 + 1, 0));
        set->AddTail(motion->CreateWaitStopTask());
        mSerial.AddTail(set);
    }
    AddSerial(this, MCreateCallTask((DinoCleaningLive *)mUnkE4, data_ov002_02165b54.SetUnkAC_00, (u32)0x16),
              new MTimer(100));
    mState = data_ov002_02165b6c.State02162cec_08;
}

void DinoCleaning::StateEnableToolMenu() {
    mUnkE4->SetUnkA0(0x15, TRUE);
    mToolMenu->EnableItem();
    mState = data_ov002_02165aec.StateStopTool_60;
}

void DinoCleaning::StateStopTool() {
    mTools[mTool]->Unk18();
    mUnk124 = 0;
    mState = data_ov002_021659ec.StatePlay_78;
}

// Whether the rock is clean (only without a target).
inline BOOL DinoCleaning::IsRockClean() {
    BOOL done = FALSE;
    if (!mUnkC9) {
        if (mKasekiRock->GetRatio() == MFx32(100)) {
            done = TRUE;
        }
    }
    return done;
}

// Whether the target is reached (only with one).
inline BOOL DinoCleaning::IsTargetReached() {
    BOOL done = FALSE;
    if (mUnkC9) {
        if (mStatus->GetScore()->GetUnk80() >= MFx32(mUnk130)) {
            done = TRUE;
        }
    }
    return done;
}

// Whether too much was broken or the time is up.
inline BOOL DinoCleaning::IsOver() {
    BOOL done = TRUE;
    if (!(mStatus->GetScore()->GetUnk84() > MFx32(100 - mUnk130))) {
        if (!mStatus->GetTimer()->IsAtStart()) {
            done = FALSE;
        }
    }
    return done;
}

// NONMATCHING: the ratios' MFx32ToInt copies and the checks' temporaries take each other's stack slots.
// The cleaning itself: updates the commentary's numbers, switches tools when asked, and ends once the rock is clean,
// the target is reached or the time is up (else runs the tool, the stop confirmation or sleep).
void DinoCleaning::StatePlay() {
    if (mUnkDC->mButtons & 0x3FFF) {
        mUnk124 = 0;
    } else {
        mUnk124++;
    }
    mUnkE4->SetUnkB8(4, (void *)((int)mUnk124 / 60));
    mUnkE4->SetUnkB8(6, (void *)MFx32ToInt(mKasekiRock->GetDirtRatio()));
    mUnkE4->SetUnkB8(5, (void *)MFx32ToInt(mKasekiRock->GetRock90()->GetRatio()));
    mUnkE4->SetUnkB8(11, (void *)MFx32ToInt(mKasekiRock->GetRatio(2)));
    mUnkE4->SetUnkB8(12, (void *)MFx32ToInt(mKasekiRock->GetRatio(1)));
    if (mTool != mNextTool) {
        mState = data_ov002_02165aec.StateSwitchTool_50;
        return;
    }
    if (IsRockClean()) {
        DinoSoundPlaySe(0x1F, 0);
        mStatus->StatePause();
        mTools[mTool]->Unk1C();
        mState = data_ov002_02165aec.State0216177c_48;
        return;
    }
    if (IsTargetReached()) {
        DinoSoundPlaySe(0x1F, 0);
        mStatus->StatePause();
        mTools[mTool]->Unk1C();
        mState = data_ov002_021659ec.State0216177c_68;
        return;
    }
    if (IsOver()) {
        DinoSoundPlaySe(0x1F, 0);
        mStatus->StatePause();
        mTools[mTool]->Unk1C();
        mState = data_ov002_02165a6c.StateCheckDone_18;
        return;
    }
    mTools[mTool]->Unk20();
    if ((mUnkDC->mButtons & 8) && mMode != 2 && gLcd->GetBottomDisplay()->mBright->GetFactor() == 0) {
        StateStopConfirm();
        return;
    }
    if (mSleepTimer != 0) {
        StateSleep();
    }
}

void DinoCleaning::StateSwitchTool() {
    mTools[mTool]->Unk1C();
    mTool = mNextTool;
    mState = data_ov002_02165aec.StateStopTool_10;
}

void DinoCleaning::StateCheckDone() {
    DinoHelpBase::Disable();
    if (!mKasekiRock->IsViewUnkD9E0Zero()) {
        DinoSoundFunc_0201feec(0x2C);
        func_ov002_0215fdec(mKasekiRock);
        return;
    }
    mState = data_ov002_02165a6c.State021618c8_40;
}

void DinoCleaning::StateSleep() {
    mUnk128 = TRUE;
    DinoSoundPause();
    mUnk144->AddTail(new DinoCleaningSleep(this, &mSleepTimer));
    mState = data_ov002_02165a6c.StateWake_70;
}

void DinoCleaning::StateWake() {
    DinoSoundResume();
    mState = data_ov002_02165a6c.StatePlay_68;
}

void DinoCleaning::StateStopConfirm() {
    mUnk128 = TRUE;
    DinoSoundPause();
    mUnkE4->SetUnkA0(0x19, TRUE);
    mUnk144->AddTail(new DinoCleaningStopConfirm(this, &mStop));
    mState = data_ov002_02165aec.StateStopAnswered_20;
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask2<DinoCleaningLive, DinoCommonLive, u32, bool>;
template MTask *MCreateCallTask(DinoCleaningLive *obj, void (DinoCommonLive::*func)(u32, bool), u32 a0, bool a1);
template class MCallTask1<DinoCleaningLive, DinoCommonLive, u32>;
template MTask *MCreateCallTask(DinoCleaningLive *obj, void (DinoCommonLive::*func)(u32), u32 a0);
template class MCallTask0<DinoCleaning, DinoHelpBase>;
template MTask *MCreateCallTask(DinoCleaning *obj, void (DinoHelpBase::*func)());
template class MBgCallTask0<DinoCleaning>;
template MTask *MCreateBgCallTask(DinoCleaning *obj, void (DinoCleaning::*func)(), int thread);

// The answer to "stop cleaning?": no goes back to it; yes finishes the rock and stops the tools.
void DinoCleaning::StateStopAnswered() {
    DinoSoundResume();
    if (!mStop) {
        mUnkE4->SetUnkA0(0x19, FALSE);
        mState = data_ov002_02165a6c.StatePlay_28;
        return;
    }
    DinoSoundPlaySe(0x1F, 0);
    mUnkE4->SetUnkA0(0x18, TRUE);
    mUnkE4->SetUnkA0(1, FALSE);
    mUnkE4->SetUnkA0(2, TRUE);
    mUnk10C->Exit();
    mStatus->StatePause();
    mToolMenu->DisableAll();
    mKasekiRock->Finish();
    mToolMenu->Deactivate();
    gCurrentDisplay->mBlend->Reset();
    gCurrentDisplay->mWin->Reset();
    DinoHelpBase::Disable();
    mTools[mTool]->Unk1C();
    mSerial.AddTail(new MTimer(60));
    DinoSoundPlaySe(0x37, 0);
    mState = data_ov002_02165aec.State02161d48_08;
}

// Quits: fades both screens and the music out.
// What the cleaning reports back.
struct DinoCleaningResult {
    u32 score;      // 0x00: in percent
    u32 bonus;      // 0x04: the time bonus
    u32 frames;     // 0x08: left on the timer
    u32 unk0C;
};

extern "C" const u8 data_0208a230;    // whether the time bonus always counts

// Fills in the result (with the time bonus for a full score or always with data_0208a230), then fades out to the
// report (mode 1) or straight out.
void DinoCleaning::State02162cec() {
    if (mUnkA0 != NULL) {
        ((DinoCleaningResult *)mUnkA0)->frames = mStatus->GetTimer()->GetRemainingFrames();
        ((DinoCleaningResult *)mUnkA0)->bonus = 0;
        if ((((DinoCleaningResult *)mUnkA0)->score == 100 || data_0208a230) && mUnkA4 == 0 && mMode != 0) {
            u32 seconds = mStatus->GetTimer()->GetSeconds();
            if (seconds > mUnk12C) {
                u32 bonus = seconds - mUnk12C;
                ((DinoCleaningResult *)mUnkA0)->bonus = std::min(data_ov002_021659ec.mMaxBonus, bonus);
                ((DinoCleaningResult *)mUnkA0)->score += ((DinoCleaningResult *)mUnkA0)->bonus;
            }
        }
        if (mUnkA8 == 1) {
            ((DinoCleaningResult *)mUnkA0)->score += 25;
        }
    }
    if (mPaper != NULL) {
        mPaper->SetDone();
    }
    if (mMode != 1) {
        MTask *fadeTop;
        MTask *fadeBottom;
        MTask *exit;
        exit = CreateExitTransfer();
        fadeBottom = gLcd->GetBottomDisplay()->mBright->CreateFadeOutToBlack(60, 0);
        fadeTop = gLcd->GetTopDisplay()->mBright->CreateFadeOutToBlack(60, 0);
        MParallelTaskSet *fades = new MParallelTaskSet(TRUE);
        fades->AddTail(fadeTop);
        fades->AddTail(fadeBottom);
        MTaskSet &serial = mSerial;
        serial.AddTail(fades);
        serial.AddTail(exit);
        return;
    }
    MTask *empty;
    MTask *pictureOut;
    MTask *exitPicture;
    MTask *timer;
    MTask *report;
    MTask *stopBgm;
    MParallelTaskSet *fades;
    MTask *fadeTop;
    MTask *fadeBottom;
    MTask *exit;
    MBlend *blend = gLcd->GetBottomDisplay()->mBlend;
    exit = CreateExitTransfer();
    fadeBottom = gLcd->GetBottomDisplay()->mBright->CreateFadeOutToBlack(60, 0);
    fadeTop = gLcd->GetTopDisplay()->mBright->CreateFadeOutToBlack(60, 0);
    fades = new MParallelTaskSet(TRUE);
    fades->AddTail(fadeTop);
    fades->AddTail(fadeBottom);
    stopBgm = DinoSoundCreateStopBgmTask(-1, 60);
    report = new DinoCleaningReport((u32)this);
    timer = new MTimer(30);
    exitPicture = MTaskExitTransfer::Create(mResultPicture);
    pictureOut = blend->CreateCrossFadeOutTask(mResultPicture->GetPlaneMask(), 30, 0);
    empty = MTaskSetMakeEmptyTask::Create(mUnk13C);
    MTask *fadeOut = blend->CreateCrossFadeOutTask(0, 30, 0);
    MTaskSet &serial = mSerial;
    serial.AddTail(fadeOut);
    serial.AddTail(empty);
    serial.AddTail(pictureOut);
    serial.AddTail(exitPicture);
    serial.AddTail(timer);
    serial.AddTail(report);
    serial.AddTail(stopBgm);
    serial.AddTail(fades);
    serial.AddTail(exit);
}

void DinoCleaning::StateQuit() {
    MTask *fadeTop;
    MTask *fadeBottom;
    MTask *exit;
    exit = CreateExitTransfer();
    fadeBottom = gLcd->GetBottomDisplay()->mBright->CreateFadeOutToBlack(120, 0);
    fadeTop = gLcd->GetTopDisplay()->mBright->CreateFadeOutToBlack(120, 0);
    MParallelTaskSet *fades = new MParallelTaskSet(TRUE);
    fades->AddTail(fadeTop);
    fades->AddTail(fadeBottom);
    MTask *stopBgm = DinoSoundCreateStopBgmTask(-1, 120);
    MTaskSet &serial = mSerial;
    serial.AddTail(stopBgm);
    serial.AddTail(fades);
    serial.AddTail(exit);
}

// The file's static initialization (see dino_static_init.h): the common layout constants and the chips' power.
// NONMATCHING (80%): the game stores the screen rect first and keeps the sizes as constants; the dead stack
// temporaries differ.
#pragma always_inline on
static struct DinoCleaningStaticInit {
    DinoCleaningStaticInit() {
        DINO_POOL(MRect, data_ov002_021659ec, 0x1f0) = DinoSizeRect(MPoint(256, 192));
        DINO_POOL(MPoint, data_ov002_021659ec, 0x50) = MPoint(256, 192);
        DINO_POOL(MPoint, data_ov002_021659ec, 0xc8) = MPoint(2, 2);
        DINO_POOL(MPoint, data_ov002_021659ec, 0xb0) = MPoint(36, 28);
        DINO_POOL(MRect, data_ov002_021659ec, 0x200) = DinoSizeRect(MPoint(36, 28));
        DINO_POOL(MPoint, data_ov002_021659ec, 0x210) = MPoint(0, 24);
        DINO_POOL(MPoint, data_ov002_021659ec, 0x218) = MPoint(256, 192);
        DINO_POOL(MRect, data_ov002_021659ec, 0x220) = MRect(0, 0, 256, 24);
        DINO_POOL(MFx32, data_ov002_021659ec, 0x4) = MFx32::FromRaw(0x333);
    }
} sDinoCleaningStaticInit;
