// DinoCleaningHammer, DinoCleaningHammerEffect, DinoCleaningHammerTask, DinoCleaningHammerBaseHit.

// (The game's -ipa file build: Unk20 inlines the tasks' constructors and what they inline.)
#pragma ipa file

#include "game/dino_cleaning_neighbour.h"
#include "game/dino_cleaning_hammer.h"
#include "game/dino_cleaning_hammer_effect.h"
#include "game/dino_cleaning_hammer_task.h"
#include "game/dino_cleaning_hammer_base_hit.h"
#include "game/dino_cleaning_drill.h"
#include "game/dino_cleaning_kaseki_rock.h"
#include "game/dino_cleaning_rock.h"
#include "game/dino_cleaning.h"
#include <algorithm>
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "m/display.h"
#include "nitro/mi.h"
#include "m/raster.h"
#include "m/window.h"
#include "m/input.h"
#include "game/dino_common_live.h"
#include "game/dino_sound.h"
#include "game/dino_static_init.h"
#include <typeinfo>

// Done: frees the window and resets the display's windows.
void DinoCleaningHammerEffect::StateEnd() {
    delete mWindow;
    gCurrentDisplay->mWin->Reset();
    Exit();
}

DinoCleaningHammer::DinoCleaningHammer(DinoCleaning *cleaning, u8 arg1) : DinoCleaningTool(cleaning) {
    mUnk80 = arg1;
}

void DinoCleaningHammer::Unk18() {
}

void DinoCleaningHammer::Unk1C() {
}

DinoCleaningHammer::~DinoCleaningHammer() {
}

DinoCleaningHammerEffect::~DinoCleaningHammerEffect() {
}

DinoCleaningHammerTask::~DinoCleaningHammerTask() {
}

DinoCleaningHammerBaseHit::~DinoCleaningHammerBaseHit() {
}

void DinoCleaningHammerBaseHit::Idle() {
    (this->*mState)();
}

void DinoCleaningHammerTask::Idle() {
    (this->*mState)();
}

void DinoCleaningHammerEffect::Idle() {
    (this->*mState)();
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MBgCallTask0<DinoCleaningDrill>;
template MTask *MCreateBgCallTask(DinoCleaningDrill *obj, void (DinoCleaningDrill::*func)(), int thread);

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x4];
    MFx32 mShakeDecay;      // 0x04: how much the base hit's shake dies down by each frame
    MFx32 mRadius;          // 0x08: the ring's radius
    u8 unk0C[0x10 - 0xC];
    MFx32 mBigRadius;       // 0x10
    u8 unk14[0x1C - 0x14];
    int mInnerMin;          // 0x1C: the ring's inner edge lags this many frames at least
    void (DinoCleaningHammerEffect::*StateClear_20)(); // 0x20
    u8 unk28[0x30 - 0x28];
    void (DinoCleaningHammerEffect::*StateEnd_30)();
    MPoint mCellOrigin;     // 0x38: the rock's first cell, in cells from the screen's corner
    void (DinoCleaningHammerEffect::*State0215caf8_40)(); // 0x40
    void (DinoCleaningHammerEffect::*StateStart_48)(); // 0x48
    void (DinoCleaningHammerTask::*StateStart_50)(); // 0x50
    u8 unk58[0x60 - 0x58];
    void (DinoCleaningHammerBaseHit::*StateStart_60)(); // 0x60
    void (DinoCleaningHammerBaseHit::*State0215d0c4_68)(); // 0x68
    u8 unk70[0x78 - 0x70];
    void (DinoCleaningHammerTask::*State0215ccfc_78)(); // 0x78
} data_ov002_02164fb8;

// A window ring, through which the hit shows, grows to the radius.
void DinoCleaningHammerEffect::StateStart() {
    mWindow = new MRasterWindow(-1, NULL, TRUE);
    mWindow->SetTarget(0);
    mWindow->SetObjTarget(0x3F);
    mWindow->SetBlendEnabled(TRUE);
    mWindow->SetObjBlendEnabled(TRUE);
    mWindow->Show();
    mFrame = 0;
    mFrames = MFx32ToInt((mBig ? data_ov002_02164fb8.mBigRadius : data_ov002_02164fb8.mRadius) / mSpeed) - 1;
    mState = data_ov002_02164fb8.State0215caf8_40;
}

extern "C" fx32 FX_Sqrt(fx32 x);
extern "C" void func_02068df4(MRasterWindow *window, MPoint center, int radius);  // cuts a disc out of the window

static inline MFx32 DinoFxSqrt(MFx32 x) {
    return MFx32::FromRaw(FX_Sqrt(x.v));
}

// The ring grows (fast, then slower): the window is the disc less the inner disc and the right edge.
// NONMATCHING: 74%, the game divides first and passes the raw quotient to FX_Sqrt after making the multipliers.
void DinoCleaningHammerEffect::State0215caf8() {
    MFx32 ratio = MFx32(std::min(mFrame, mFrames - 1)) / MFx32(mFrames);
    int outer = MFx32ToInt(DinoFxSqrt(ratio) * MFx32(mFrames) * MFx32(8));
    ratio = MFx32(std::max(mFrame - 2, data_ov002_02164fb8.mInnerMin)) / MFx32(mFrames);
    MFx32 inner = DinoFxSqrt(ratio) * MFx32(mFrames) * MFx32(8);
    mWindow->SetCircle(mCenter, outer);
    func_02068df4(mWindow, mCenter, MFx32ToInt(inner));
    mWindow->SubtractRect(MRect(255, 0, 256, 192));
    if (mFrame++ > mFrames + 2) {
        mState = data_ov002_02164fb8.StateClear_20;
    }
}

void DinoCleaningHammerEffect::StateClear() {
    mWindow->Clear();
    mState = data_ov002_02164fb8.StateEnd_30;
}

// Copies the rock's cells, unless there's nothing at the hit.
void DinoCleaningHammerTask::StateStart() {
    if (mRock->GetCellLayer(mRow, mCol) == 0) {
        Exit();
        return;
    }
    for (u32 col = 0; col < 0x1C; col++) {
        for (u32 row = 0; row < 0x24; row++) {
            mCells[row][col] = mRock->GetCell(row, col);
            mLayers[row][col] = mRock->GetCellLayer(row, col);
            mHit[row][col] = mLayers[row][col] != 0 ? 1 : 0;
        }
    }
    mHit[mRow][mCol] = 2;
    mRadius = data_ov002_02164fb8.mRadius;
    if (mBig) {
        mRadius = data_ov002_02164fb8.mBigRadius;
    }
    mRadius2 = mRadius;
    mState = data_ov002_02164fb8.State0215ccfc_78;
}

// NONMATCHING: 98.8%, one stack slot more in the game (std::max's operator< argument copies).
// Spreads the hit to the joined cells around it (no deeper ones), breaking what it reached last frame, and shakes the
// rock, less and less.
void DinoCleaningHammerTask::State0215ccfc() {
    MI_CpuCopy8(mHit, mPrevHit, sizeof(mHit));
    MFx32 radius = mRadius;
    for (u32 x = 2; x < 0x22; x++) {
        for (u32 y = 2; y < 0x1A; y++) {
            switch (mHit[x][y]) {
            case 0:
                break;
            case 2:
                mKasekiRock->Hit(x, y, radius, MFx32(1), mDecay);
                mHit[x][y] = 0;
                break;
            case 1:
                MPoint pos(x, y);
                for (u32 i = 0; i < 6; i++) {
                    MPoint n = GetNeighbour(pos, i);
                    if (mPrevHit[n.x][n.y] == 2 && mLayers[n.x][n.y] >= mLayers[x][y] && (mCells[x][y] & (1 << i)) &&
                        (mCells[n.x][n.y] & (1 << (i + 3) % 6))) {
                        mHit[x][y] = 2;
                        break;
                    }
                }
                break;
            }
        }
    }
    mRadius = mRadius - mDecay;
    MFx32 amp = std::max(MFx32(0), mRadius * MFx32(1));
    mKasekiRock->SetOffsetAll(MPoint(MFx32ToInt(amp * mRandom->NextFraction() - amp / MFx32(2)),
                                     MFx32ToInt(amp * mRandom->NextFraction() - amp / MFx32(2))));
    if (mRadius <= MFx32(0)) {
        Exit();
    }
}

void DinoCleaningHammerBaseHit::StateStart() {
    mRadius = mBig ? data_ov002_02164fb8.mBigRadius : data_ov002_02164fb8.mRadius;
    mState = data_ov002_02164fb8.State0215d0c4_68;
}

extern "C" void func_ov002_0215fdec(DinoCleaningKasekiRock *rock);

// Shakes the rock, less and less.
// NONMATCHING: the fixed-point temporaries sit in other stack slots.
void DinoCleaningHammerBaseHit::State0215d0c4() {
    mRadius = mRadius - data_ov002_02164fb8.mShakeDecay;
    func_ov002_0215fdec(mRock);
    MFx32 amp = std::max(MFx32(0), mRadius * MFx32(3));
    mRock->SetOffset(MPoint(MFx32ToInt(amp * mUnk80->NextFraction() - amp / MFx32(2)),
                            MFx32ToInt(amp * mUnk80->NextFraction() - amp / MFx32(2))));
    if (mRadius <= MFx32(0)) {
        Exit();
    }
}

// The TU's pooled pointer-to-member constants.
extern "C" struct {
    void (DinoCleaningDrill::*LoadGuides_00)();
} data_ov002_0216523c;

DinoCleaningDrill::DinoCleaningDrill(DinoCleaning *cleaning, u8 arg1) : DinoCleaningTool(cleaning) {
    MTaskSet *parallel = &mParallel;
    mUnk87C = arg1;
    parallel->mUseMutex = TRUE;
}

// Loads the guides in the background.
void DinoCleaningDrill::Unk18() {
    MTask *task = MCreateBgCallTask(this, data_ov002_0216523c.LoadGuides_00, 1);
    GetCleaning()->GetSerial()->AddTail(task);
}

extern "C" const MFx32 data_ov002_02164fbc;     // the ring's speed
extern "C" const MFx32 data_ov002_02164fd0;     // (the strong hit's)

// The file's .data pool starts with the task's typeinfo; the hammer's touch area is pooled after it.
#define DINO_HAMMER_POOL(T, off) (*(const T *)((const u8 *)&typeid(DinoCleaningHammerTask) + (off)))

// The cell under a touch point (every other row is half a cell along).
static inline MPoint DinoHammerTouchCell(MPoint pos) {
    return MPoint((pos.x - ((pos.y & 8) >> 1)) >> 3, pos.y >> 3) + data_ov002_02164fb8.mCellOrigin;
}

// The screen point of a cell's corner.
static inline MPoint DinoHammerCellPos(MPoint cell) {
    cell.x -= data_ov002_02164fb8.mCellOrigin.x;
    cell.y -= data_ov002_02164fb8.mCellOrigin.y;
    return MPoint(cell.x * 8 + (cell.y & 1) * 4, cell.y * 8);
}

inline DinoCleaningHammerBaseHit::DinoCleaningHammerBaseHit(DinoCleaningKasekiRock *rock, MRandom *random, u8 big)
    : MProcess(NULL) {
    mRock = rock;
    mUnk80 = random;
    mBig = big;
    mState = data_ov002_02164fb8.StateStart_60;
}

inline DinoCleaningHammerEffect::DinoCleaningHammerEffect(DinoCleaningKasekiRock *rock, MPoint center, u8 big)
    : MProcess(NULL) {
    mState = data_ov002_02164fb8.StateStart_48;
    mRock = rock;
    mCenter = center;
    mBig = big;
    mSpeed = mBig ? data_ov002_02164fd0 : data_ov002_02164fbc;
}

// Starts at the deepest of the cell and its neighbours, after a few frames.
inline DinoCleaningHammerTask::DinoCleaningHammerTask(DinoCleaningKasekiRock *kasekiRock, DinoCleaningRock *rock,
                                                      MRandom *random, MPoint cell, u8 big)
    : MProcess(NULL) {
    mState = data_ov002_02164fb8.StateStart_50;
    mKasekiRock = kasekiRock;
    mRock = rock;
    mRandom = random;
    mBig = big;
    mDecay = mBig ? data_ov002_02164fd0 : data_ov002_02164fbc;
    u32 deepest = kasekiRock->GetCellLayer(cell.x, cell.y);
    MPoint at = cell;
    for (u32 i = 0; i < 6; i++) {
        MPoint n = GetNeighbour(cell, i);
        u32 layer = kasekiRock->GetCellLayer(n.x, n.y);
        if (layer > deepest) {
            at = n;
            deepest = layer;
        }
    }
    mRow = at.x;
    mCol = at.y;
    mSerial.AddTail(new MTimer(4));
}

// A tap on the rock: a hit on a cleared cell knocks the base; otherwise the ring goes out and the hit breaks cells.
void DinoCleaningHammer::Unk20() {
    MInput *input = GetCleaning()->GetUnkDC();
    if (!(input->GetPressed() & 0x1000)) {
        return;
    }
    if (!DINO_HAMMER_POOL(MRect, 0x68).Contains(input->GetTouchPos())) {
        return;
    }
    MPoint cell = DinoHammerTouchCell(input->GetTouchPos());
    u8 big = mUnk80;
    if (GetCleaning()->GetKasekiRock()->IsCellClear(cell.x, cell.y)) {
        GetCleaning()->GetUnkE4()->SetUnkAC(0x11);
        DinoSoundPlaySe(0x24, 0);
        DinoCleaningHammerBaseHit *hit =
            new DinoCleaningHammerBaseHit(GetCleaning()->GetKasekiRock(), GetCleaning()->GetRandom(), big);
        GetCleaning()->GetSerial()->AddTail(hit);
        return;
    }
    GetCleaning()->GetUnkE4()->SetUnkAC(0xF);
    if (GetCleaning()->GetKasekiRock()->GetUnk4B8()) {
        DinoSoundPlaySe(0x25, 0);
    } else if (mUnk80) {
        DinoSoundPlaySe(0x23, 0);
    } else {
        DinoSoundPlaySe(0x22, 0);
    }
    DinoCleaningHammerEffect *effect =
        new DinoCleaningHammerEffect(GetCleaning()->GetKasekiRock(), DinoHammerCellPos(cell), big);
    DinoCleaningHammerTask *task =
        new DinoCleaningHammerTask(GetCleaning()->GetKasekiRock(), GetCleaning()->GetKasekiRock()->GetTopRock(cell.x, cell.y),
                                   GetCleaning()->GetRandom(), cell, big);
    MParallelTaskSet *set = new MParallelTaskSet(TRUE);
    set->AddTail(task);
    set->AddTail(effect);
    GetCleaning()->GetSerial()->AddTail(set);
}
