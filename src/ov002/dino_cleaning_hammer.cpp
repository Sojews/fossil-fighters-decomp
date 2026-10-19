// DinoCleaningHammer, DinoCleaningHammerEffect, DinoCleaningHammerTask, DinoCleaningHammerBaseHit.
// Only the destructors are decompiled so far.

#include "game/dino_cleaning_hammer.h"
#include "game/dino_cleaning_hammer_effect.h"
#include "game/dino_cleaning_hammer_task.h"
#include "game/dino_cleaning_hammer_base_hit.h"
#include "game/dino_cleaning_drill.h"
#include "game/dino_cleaning_kaseki_rock.h"
#include "game/dino_cleaning_rock.h"
#include <algorithm>
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "m/display.h"
#include "nitro/mi.h"
#include "m/raster.h"
#include "m/window.h"

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
    u8 unk14[0x30 - 0x14];
    void (DinoCleaningHammerEffect::*StateEnd_30)();
    u8 unk38[0x40 - 0x38];
    void (DinoCleaningHammerEffect::*State0215caf8_40)(); // 0x40
    u8 unk48[0x50 - 0x48];
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

// The cell next to pos in a direction (0 right, then counterclockwise), for even and odd columns.
static inline MPoint GetNeighbour(MPoint pos, u32 dir) {
    static const MPoint sOffsets[2][6] = {
        {MPoint(1, 0), MPoint(0, 1), MPoint(-1, 1), MPoint(-1, 0), MPoint(-1, -1), MPoint(0, -1)},
        {MPoint(1, 0), MPoint(1, 1), MPoint(0, 1), MPoint(-1, 0), MPoint(0, -1), MPoint(1, -1)},
    };
    const MPoint &d = sOffsets[pos.y & 1][dir];
    return MPoint(pos.x + d.x, pos.y + d.y);
}

// NONMATCHING: one stack slot more in the game (std::max's operator< argument copies) and the neighbour sums'
// registers.
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
