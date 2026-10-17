// DinoCleaning: the fossil cleaning scene, with DinoCleaningKasekiRock and DinoCleaningLive.

#include "game/dino_cleaning.h"
#include "game/dino_cleaning_kaseki_rock.h"
#include "game/dino_cleaning_rock.h"
#include "game/dino_cleaning_image.h"
#include "game/dino_cleaning_image_view.h"
#include "m/display.h"
#include "m/blend.h"
#include "m/bright.h"
#include "game/dino_cleaning_kaseki_rock_xray_blend_manager.h"
#include "game/help.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "types.h"

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
    mUnk94->Exit();
    mUnk88->Exit();
    mUnk88 = NULL;
    mView->DestroyOam();
    mView->LoadPalettes();
    GetDisplay()->mBlend->Reset();
    GetDisplay()->mBright->Reset();
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
DinoCleaning::~DinoCleaning() {
}

MInput *DinoCleaning::GetUnkDC() {
    return mUnkDC;
}

u32 DinoCleaning::GetUnkE0() {
    return mUnkE0;
}

u32 DinoCleaning::GetUnkD4() {
    return mUnkD4;
}

u32 DinoCleaning::GetUnkD8() {
    return mUnkD8;
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

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
class DinoCommonLive {};
class DinoCleaningLive : public DinoCommonLive {};
template class MCallTask2<DinoCleaningLive, DinoCommonLive, u32, bool>;
template MTask *MCreateCallTask(DinoCleaningLive *obj, void (DinoCommonLive::*func)(u32, bool), u32 a0, bool a1);
template class MCallTask1<DinoCleaningLive, DinoCommonLive, u32>;
template MTask *MCreateCallTask(DinoCleaningLive *obj, void (DinoCommonLive::*func)(u32), u32 a0);
template class MCallTask0<DinoCleaning, DinoHelpBase>;
template MTask *MCreateCallTask(DinoCleaning *obj, void (DinoHelpBase::*func)());
template class MBgCallTask0<DinoCleaning>;
template MTask *MCreateBgCallTask(DinoCleaning *obj, void (DinoCleaning::*func)(), int thread);
