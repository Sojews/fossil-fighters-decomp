// DinoCleaning: the fossil cleaning scene, with DinoCleaningKasekiRock and DinoCleaningLive.

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
#include "m/bright.h"
#include "game/dino_cleaning_kaseki_rock_xray_blend_manager.h"
#include "game/help.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "types.h"
#include "game/dino_cleaning_status.h"
#include "m/picture.h"
#include "m/menu.h"
#include "m/seed_generator.h"
#include "m/window.h"
#include <stdio.h>

#pragma auto_inline off

DinoCleaningKasekiRock::~DinoCleaningKasekiRock() {
}

DinoCleaningKasekiRockXrayBlendManager::~DinoCleaningKasekiRockXrayBlendManager() {
}

DinoCleaningRock *DinoCleaningKasekiRock::GetRock8C() {
    return mRock8C;
}

DinoCleaningRock *DinoCleaningKasekiRock::GetRock90() {
    return mRock90;
}

u8 DinoCleaningKasekiRock::GetUnk4B8() {
    return mUnk4B8;
}

BOOL DinoCleaningKasekiRock::IsCellClear(int row, int col) {
    if (mRock90->IsCellClear(row, col) && mRock8C->IsCellClear(row, col)) {
        return TRUE;
    }
    return FALSE;
}

s32 DinoCleaningKasekiRock::GetCellLayer(int row, int col) {
    if (mRock90->IsCellClear(row, col)) {
        return mRock8C->GetCellLayer(row, col);
    }
    return mRock90->GetCellLayer(row, col) + 3;
}

DinoCleaningRock *DinoCleaningKasekiRock::GetTopRock(int row, int col) {
    if (!mRock90->IsCellClear(row, col)) {
        return mRock90;
    }
    if (mRock8C->IsCellClear(row, col)) {
        return NULL;
    }
    return mRock8C;
}

MFx32 DinoCleaningKasekiRock::GetRatio() {
    return MFx32(100) * MFx32(mUnk9C) / MFx32(mUnk98);
}

MFx32 DinoCleaningKasekiRock::GetRatio(int index) {
    return MFx32(100) * MFx32(mUnkA0[index]) / MFx32(mUnk98);
}

void DinoCleaningKasekiRock::Idle() {
    (this->*mState)();
}

void DinoCleaningKasekiRock::Finish() {
    mXray->Exit();
    mUnk88->Exit();
    mUnk88 = NULL;
    mView->DestroyOam();
    mView->LoadPalettes();
    GetDisplay()->mBlend->Reset();
    GetDisplay()->mBright->Reset();
}

// NONMATCHING: the game reads the size through the returned temporary's address (an inline helper?).
void DinoCleaningKasekiRock::SetOffsetAll(MPoint offset) {
    MPoint pos;
    MPoint size = mUnk80->GetSize();
    pos.x = -(size.x - 256) / 2 + offset.x;
    pos.y = -(size.y - 192) / 2 + offset.y;
    mUnk80->SetPosition(pos);
    mOam->SetOffset(MPoint(-offset.x, -offset.y));
    mUnk88->SetPosition(offset);
    mView->SetPosition(offset);
}

// NONMATCHING: as SetOffsetAll.
void DinoCleaningKasekiRock::SetOffset(MPoint offset) {
    MPoint pos;
    MPoint size = mUnk80->GetSize();
    pos.x = -(size.x - 256) / 2 + offset.x;
    pos.y = -(size.y - 192) / 2 + offset.y;
    mUnk80->SetPosition(pos);
    mOam->SetOffset(MPoint(-offset.x, -offset.y));
}

BOOL DinoCleaningKasekiRock::IsViewUnkD9E0Zero() {
    return mView->IsUnkD9E0Zero();
}

void DinoCleaningKasekiRock::CountCells() {
    u32 i;
    int col;

    mUnk9C = 0;
    for (i = 0; i < 4; i++) {
        mUnkA0[i] = 0;
    }
    for (col = 0; col < 28; col++) {
        int row;
        for (row = 0; row < 36; row++) {
            if (!mRock8C->GetImage()->GetMask(row, col) &&
                !mRock8C->GetImage()->GetLayer(1)->mCells410[row][col] && !mRock90->GetCellLayer(row, col)) {
                mUnk9C++;
                mUnkA0[mRock8C->GetCellLayer(row, col)]++;
            }
        }
    }
}

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

void DinoCleaningKasekiRockXrayBlendManager::Idle() {
    (this->*mState)();
}

// The scene's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x20];
    void (DinoCleaningKasekiRockXrayBlendManager::*StateDone_20)(); // 0x20
    void (DinoCleaningKasekiRockXrayBlendManager::*StateFade_28)(); // 0x28
    u8 unk30[0x48 - 0x30];
    void (DinoCleaningKasekiRock::*CountCells_48)(); // 0x48
    void (DinoCleaningKasekiRock::*StateDrop_50)(); // 0x50
    u8 unk58[0x68 - 0x58];
    void (DinoCleaningKasekiRock::*StateShake_68)(); // 0x68
} data_ov002_021657d4;
extern "C" struct {
    u8 unk00[0x8];
    const char *mCountFormat;   // 0x08
    u8 unk0C[0x48 - 0xC];
    void (DinoCleaning::*State02160638_48)(); // 0x48
    u8 unk50[0x78 - 0x50];
    void (DinoCleaning::*Unk02161490_78)();
} data_ov002_021659ec;
extern "C" struct {
    u8 unk00[0x10];
    void (DinoCleaning::*State02161164_10)(); // 0x10
    u8 unk18[0x28 - 0x18];
    void (DinoCleaning::*State02161490_28)(); // 0x28
    u8 unk30[0x40 - 0x30];
    void (DinoCleaning::*Unk021618c8_40)();
    u8 unk48[0x58 - 0x48];
    void (DinoCleaning::*State021618c8_58)(); // 0x58
    u8 unk60[0x68 - 0x60];
    void (DinoCleaning::*Unk02161490_68)();
    void (DinoCleaning::*StateWake_70)();
    void (DinoCleaning::*State02160f54_78)(); // 0x78
} data_ov002_02165a6c;
extern "C" struct {
    u8 unk00[0x8];
    void (DinoCleaning::*State02161d48_08)(); // 0x08
    void (DinoCleaning::*StateStopTool_10)();
    u8 unk18[0x20 - 0x18];
    void (DinoCleaning::*StateStopAnswered_20)();
    u8 unk28[0x60 - 0x28];
    void (DinoCleaning::*StateStopTool_60)();
    u8 unk68[0x80 - 0x68];
    void (DinoCommonLive::*SetUnkA0_80)(u32, bool); // 0x80 (the next block's first entry)
} data_ov002_02165aec;
extern "C" struct {
    void (DinoCommonLive::*SetUnkA0_00)(u32, bool); // 0x00 (DinoCommonLive::SetUnkA0)
    u8 unk08[0x10 - 0x8];
    void (DinoCleaning::*State02161260_10)(); // 0x10
    void (DinoHelpBase::*Enable_18)(); // 0x18
    void (DinoCleaning::*State021610b0_20)(); // 0x20
} data_ov002_02165b6c;

extern "C" void func_ov002_0215fdec(DinoCleaningKasekiRock *rock);

// Drops the rock in from above.
void DinoCleaningKasekiRock::StartDrop() {
    mDropY.v = 0xC0000;
    mDropSpeed.v = -0x15000;
    mView->SetPriority(0);
    DinoSoundPlaySe(0x43, 0);
    mState = data_ov002_021657d4.StateDrop_50;
}

// Falls until it lands, then settles.
// NONMATCHING: the two additions' argument copies swap stack slots.
void DinoCleaningKasekiRock::StateDrop() {
    mDropY = mDropY + mDropSpeed;
    mDropSpeed = mDropSpeed + MFx32::FromRaw(0xE66);
    if (mDropY < MFx32(0) || mDropSpeed < MFx32(0)) {
        mView->Move(0, MFx32ToInt(mDropY));
        return;
    }
    DinoSoundPlaySe(0x3F, 0);
    mView->Move(0, 0);
    mView->SetPriority(2);
    mUnk4B4 = 20;
    mState = data_ov002_021657d4.StateShake_68;
}

extern const u32 data_0208a23c;  // the X-ray fade's length

// Fades the X-ray image out as the timer runs down.
void DinoCleaningKasekiRockXrayBlendManager::StateFade() {
    if (!mHidden) {
        GetDisplay()->mBlend->SetAlpha(mTimer * 16 / data_0208a23c, 16);
    }
    mTimer--;
    if (mTimer <= 0) {
        mState = data_ov002_021657d4.StateDone_20;
    }
}

void DinoCleaningKasekiRockXrayBlendManager::StateDone() {
}

// Shakes the landed rock, less and less, then counts its cells.
void DinoCleaningKasekiRock::StateShake() {
    mUnk4B4--;
    MFx32 amp = MFx32((int)mUnk4B4) / MFx32(2);
    SetOffsetAll(MPoint(MFx32ToInt(amp * mRandom->NextFraction() - amp / MFx32(2)),
                        MFx32ToInt(amp * mRandom->NextFraction() - amp / MFx32(2))));
    if (mUnk4B4 == 0) {
        mState = data_ov002_021657d4.CountCells_48;
    }
}

DinoCleaning::DinoCleaning(u32 unk9C, void *unkA0, u32 unkA8, u32 unkAC, int mode, u32 unk140)
    : MProcess(NULL), MLooperPreLoopHook(FALSE, TRUE) {
    mUnk9C = unk9C;
    mUnkA0 = unkA0;
    mUnkA8 = unkA8;
    mUnkAC = unkAC;
    mMode = mode;
    mUnk128 = FALSE;
    mUnk138 = 0;
    mUnk140 = unk140;
    mState = data_ov002_021659ec.State02160638_48;
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

void DinoCleaning::StateEnableToolMenu() {
    mUnkE4->SetUnkA0(0x15, TRUE);
    mToolMenu->EnableItem();
    mState = data_ov002_02165aec.StateStopTool_60;
}

void DinoCleaning::StateStopTool() {
    mTools[mTool]->Unk18();
    mUnk124 = 0;
    mState = data_ov002_021659ec.Unk02161490_78;
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
    mState = data_ov002_02165a6c.Unk021618c8_40;
}

void DinoCleaning::StateSleep() {
    mUnk128 = TRUE;
    DinoSoundPause();
    mUnk144->AddTail(new DinoCleaningSleep(this, &mSleepTimer));
    mState = data_ov002_02165a6c.StateWake_70;
}

void DinoCleaning::StateWake() {
    DinoSoundResume();
    mState = data_ov002_02165a6c.Unk02161490_68;
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
        mState = data_ov002_02165a6c.State02161490_28;
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

#pragma auto_inline off

// Shows the x-ray image: zooming in from white, then fading out.
void DinoCleaningKasekiRock::StartXray() {
    ((DinoCleaning *)unk7C)->GetUnkE4()->SetUnkAC(11);
    DinoCleaningKasekiRockXrayBlendManager *xray = mXray;
    DinoSoundPlaySe(0x26, 0);
    xray->mTasks->Clear();
    MFx32 to;
    MFx32 from;
    from = MFx32::FromRaw(0x1B33);
    to = MFx32(1);
    MTask *scale = xray->mImage->CreateScaleAnime(from, to, 45, -1);
    MTask *fade = xray->GetDisplay()->mBright->CreateFadeInFromWhite(60, 0);
    MParallelTaskSet *set = new MParallelTaskSet(TRUE);
    set->AddTail(fade);
    set->AddTail(scale);
    xray->mTasks->AddTail(set);
    xray->mImage->Show();
    MBlend *blend = xray->GetDisplay()->mBlend;
    blend->SetMode(1);
    blend->SetAlpha(16, 16);
    blend->SetTarget1(xray->mImage->GetPlaneMask());
    blend->SetTarget2(0x3F);
    xray->mTimer = data_0208a23c;
    xray->mState = data_ov002_021657d4.StateFade_28;
    mView->CreatePalAnimes();
}

void DinoCleaningKasekiRock::StopXray() {
    DinoCleaningKasekiRockXrayBlendManager *xray = mXray;
    xray->mTasks->Clear();
    xray->GetDisplay()->mBlend->Reset();
    xray->GetDisplay()->mBright->Reset();
    xray->mImage->Hide();
    mView->LoadPalettes();
}

extern "C" void func_ov002_0215a4fc(DinoCleaningImageView *view, MPoint pos);

extern "C" void func_ov002_0215fdd4(DinoCleaningKasekiRock *rock, MPoint pos) {
    func_ov002_0215a4fc(rock->mView, pos);
}
