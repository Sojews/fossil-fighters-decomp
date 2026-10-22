// DinoCleaningDrill.

// (The game's -ipa file build: Unk20 inlines the line, its marker and the cell helpers.)
#pragma ipa file

#include "game/dino_cleaning_drill.h"
#include "game/dino_cleaning.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "game/dino_static_init.h"
#include "m/fx32.h"
#include "game/dino_cleaning_neighbour.h"
#include "game/dino_cleaning_kaseki_rock.h"
#include <algorithm>
#include <stdlib.h>
#include "m/input.h"
#include "nitro/mi.h"
#include "game/dino_common_live.h"
#include "game/dino_sound.h"

#pragma auto_inline off
// The TU's pooled constants.
struct DinoCleaningDrillPool {
    u8 unk00[4];
    MFx32 mUnk04;           // 0x04
    const char *markPath;   // 0x08
    MFx32 mMaxDistance;     // 0x0C: the stroke length the drill's strength tops out at
    const char *rowPath;    // 0x10
    u8 unk14[4];
    MFx32 mForce;           // 0x18
    MFx32 mMinDistance;     // 0x1C
    const char *columnPath; // 0x20
    MFx32 mScale;           // 0x24
    MFx32 mUnk28;           // 0x28
    u8 unk2C[0x4C - 0x2C];
    MPoint mCellOrigin;     // 0x4C: the rock's first cell, in cells from the screen's corner
};
// (The pool from +0x80, which the game addresses through its own symbol.)
extern "C" struct {
    u8 unk00[0x20];
    MRect mArea;            // 0x20: where the drill works
} data_ov002_02165278;
extern "C" u32 data_ov002_021651f8[];
#define sDrillPool (*(DinoCleaningDrillPool *)data_ov002_021651f8)

void DinoCleaningDrill::LoadGuides() {
    mTouching = 0;
    mRowGuide = new MMotion(std::string(sDrillPool.rowPath), (MSmartOam *)GetCleaning()->GetUnkD4(), 0, 1);
    mRowGuide->Play(1, 0);
    mRowGuide->SetPriority(2);
    mColumnGuide = new MMotion(std::string(sDrillPool.columnPath), (MSmartOam *)GetCleaning()->GetUnkD4(), 0, 1);
    mColumnGuide->Play(1, 0);
    mColumnGuide->SetPriority(2);
    mMark = new MMotion(std::string(sDrillPool.markPath), (MSmartOam *)GetCleaning()->GetUnkD4(), 0, 1);
    mMark->SetPriority(2);
    mMark->SetAnchor(1);
    mMark->Play(1, 0);
    GetParallel()->AddTail(mColumnGuide, mRowGuide, mMark);
}

void DinoCleaningDrill::Unk1C() {
    mParallel.Clear();
}

void DinoCleaningDrill::ShowGuides() {
    mRowGuide->Show();
    mColumnGuide->Show();
    mMark->Show();
}

void DinoCleaningDrill::HideGuides() {
    mRowGuide->Hide();
    mColumnGuide->Hide();
    mMark->Hide();
}

void DinoCleaningDrill::SetGuidePos(MPoint pos) {
    mRowGuide->SetPosition(0, pos.y);
    mColumnGuide->SetPosition(pos.x, 0);
    mMark->SetPosition(pos);
}

void DinoCleaningDrill::MarkCell(int x, int y) {
    DinoCleaningKasekiRock *rock = GetCleaning()->GetKasekiRock();
    if (mUnk878 == 0) {
        if (rock->GetCellLayer(x, y) != 0) {
            mMarks[x][y] = 1;
        }
        return;
    }
    u32 layer = rock->GetCellLayer(x, y);
    u32 deepest = layer;
    u32 count = 0;
    for (u32 i = 0; i < 6; i++) {
        MPoint n = GetNeighbour(MPoint(x, y), i);
        u32 l = rock->GetCellLayer(n.x, n.y);
        if (l >= layer) {
            count++;
            deepest = std::max(deepest, l);
        }
    }
    if (count >= 3) {
        layer = deepest;
    }
    if (layer <= (u32)rock->GetCellLayer(x, y)) {
        mMarks[x][y] = 1;
    }
    for (u32 i = 0; i < 6; i++) {
        MPoint n = GetNeighbour(MPoint(x, y), i);
        if (layer <= (u32)rock->GetCellLayer(n.x, n.y)) {
            mMarks[n.x][n.y] = 1;
        }
    }
}
#pragma auto_inline on

extern "C" fx32 FX_Sqrt(fx32 x);
extern "C" void func_ov002_0215fdd4(DinoCleaningKasekiRock *rock, MPoint pos);

// The cell under a screen point.
static inline MPoint DinoDrillTouchCell(MPoint pos) {
    return MPoint((pos.x - ((pos.y & 8) >> 1)) >> 3, pos.y >> 3) + sDrillPool.mCellOrigin;
}

// The screen point at a cell's centre.
static inline MPoint DinoDrillCellCenter(MPoint cell) {
    cell.x -= sDrillPool.mCellOrigin.x;
    cell.y -= sDrillPool.mCellOrigin.y;
    return MPoint(cell.x * 8 + (cell.y & 1) * 4 + 4, cell.y * 8 + 4);
}

static inline MFx32 DinoDrillSqrt(MFx32 x) {
    return MFx32::FromRaw(FX_Sqrt(x.v));
}

extern "C" const MFx32 data_ov002_02165204, data_ov002_02165214;  // the pool's +0xC and +0x1C, by their own names

// std::min and std::max on a pooled constant: the value is read through the pool, the constant returned by its name.
static inline const MFx32 &DinoDrillMin(const MFx32 &a, const MFx32 &named, const MFx32 &b) {
    return b < a ? b : named;
}
static inline const MFx32 &DinoDrillMax(const MFx32 &a, const MFx32 &named, const MFx32 &b) {
    return a < b ? b : named;
}

static inline MFx32 DinoDrillSub(MFx32 a, const MFx32 &b) {
    MFx32 r;
    r.v = a.v - b.v;
    return r;
}

static inline MFx32 DinoDrillSubRef(const MFx32 &a, const MFx32 &b) {
    MFx32 r;
    r.v = a.v - b.v;
    return r;
}

// Marks each cell a line crosses, once.
struct DinoDrillLineMarker {
    DinoDrillLineMarker(DinoCleaningDrill *drill) {
        mDrill = drill;
        MI_CpuFill8(mDone, 0, sizeof(mDone));
    }
    void operator()(MPoint pos) {
        int x = (pos.x - ((pos.y & 8) >> 1)) >> 3;
        x += sDrillPool.mCellOrigin.x;
        int y = (pos.y >> 3) + sDrillPool.mCellOrigin.y;
        MPoint cell(x, y);
        if (!mDone[cell.x][cell.y]) {
            mDone[cell.x][cell.y] = 1;
            mDrill->MarkCell(cell.x, cell.y);
        }
    }

    u8 mDone[36][28];
    DinoCleaningDrill *mDrill;
    u8 unk3F4[0x400 - 0x3F4];
};

// Calls op at each point of the line (in the clip rect), from one end to the other.
template <class Op> static inline void DinoDrawLine(Op op, MRect clip, MPoint from, MPoint to) {
    int x = from.x;
    int y = from.y;
    int dx = to.x - x;
    int dy = to.y - y;
    int sx = dx < 0 ? -1 : 1;
    int sy = dy < 0 ? -1 : 1;
    int ay;
    int ax;
    ax = labs(dx * 2);
    ay = labs(dy * 2);
    if (clip.left <= x && x < clip.right && clip.top <= y && y < clip.bottom) {
        op(MPoint(x, y));
    }
    if (ax > ay) {
        int d = ay - ax / 2;
        while (x != to.x) {
            if (d >= 0) {
                y += sy;
                d -= ax;
            }
            x += sx;
            d += ay;
            if (clip.left <= x && x < clip.right && clip.top <= y && y < clip.bottom) {
                op(MPoint(x, y));
            }
        }
    } else {
        int d = ax - ay / 2;
        while (y != to.y) {
            if (d >= 0) {
                x += sx;
                d -= ay;
            }
            y += sy;
            d += ax;
            if (clip.left <= x && x < clip.right && clip.top <= y && y < clip.bottom) {
                op(MPoint(x, y));
            }
        }
    }
}

// Drills while the stylus is down: the cells along the stroke since last frame (and their neighbours, see MarkCell),
// hitting each newly marked cell.
// NONMATCHING: 99.3%, the game keeps the force product in the frame's lowest temporary slot, ours sits with the named
// locals (shifting the temporaries between by 4); tried a named local, a const reference, Times/Mul, helpers.
void DinoCleaningDrill::Unk20() {
    MInput *input = GetCleaning()->GetUnkDC();
    if ((input->mButtons & 0x1000) && data_ov002_02165278.mArea.Contains(input->GetTouchPos())) {
        ShowGuides();
        SetGuidePos(input->GetTouchPos());
        MPoint touch = input->GetTouchPos();
        MPoint cell = DinoDrillTouchCell(touch);
        if (!mTouching) {
            mTouching = 1;
            mPrevTouch = touch;
            mPrevCell = cell;
            if (GetCleaning()->GetKasekiRock()->IsCellClear(cell.x, cell.y)) {
                GetCleaning()->GetUnkE4()->SetUnkAC(0x12);
                return;
            }
            GetCleaning()->GetUnkE4()->SetUnkAC(0xD);
            return;
        }
        MFx32 scale;
        MFx32 power;
        if (mUnk87C) {
            power = MFx32(1);
            mUnk878 = 1;
            DinoSoundFunc_0201feec(0x30);
        } else {
            power = MFx32::FromRaw(0x800);
            mUnk878 = 1;
            DinoSoundFunc_0201feec(0x2F);
        }
        if (cell.x == mPrevCell.x && cell.y == mPrevCell.y) {
            MI_CpuFill8(mPrevMarks, 0, sizeof(mPrevMarks));
            MI_CpuFill8(mMarks, 0, sizeof(mMarks));
            MarkCell(cell.x, cell.y);
        } else {
            MI_CpuCopy8(mMarks, mPrevMarks, sizeof(mMarks));
            MI_CpuFill8(mMarks, 0, sizeof(mMarks));
            int dy = mPrevTouch.y - touch.y;
            int dx = mPrevTouch.x - touch.x;
            MFx32 distance = DinoDrillSqrt(MFx32(dx * dx + dy * dy));
            distance = DinoDrillMax(sDrillPool.mMinDistance, data_ov002_02165214,
                                    DinoDrillMin(sDrillPool.mMaxDistance, data_ov002_02165204, distance));
            MFx32 speed = DinoDrillSubRef(sDrillPool.mUnk04, sDrillPool.mUnk28) * DinoDrillSub(distance, sDrillPool.mMinDistance) /
                          DinoDrillSubRef(sDrillPool.mMaxDistance, sDrillPool.mMinDistance);
            DinoDrawLine(DinoDrillLineMarker(this), MRect(0, 0, 256, 192), DinoDrillCellCenter(mPrevCell),
                         DinoDrillCellCenter(cell));
        }
        mPrevTouch = touch;
        mPrevCell = cell;
        scale = sDrillPool.mForce;
        const MFx32 &force = power * scale;
        DinoCleaningKasekiRock *rock = GetCleaning()->GetKasekiRock();
        if (!rock->IsCellClear(cell.x, cell.y) && !rock->GetUnk4B8() &&
            !(GetCleaning()->mUnkAC == 1 && rock->GetCellLayer(cell.x, cell.y) == 6)) {
            func_ov002_0215fdd4(rock, touch);
        }
        for (u32 y = 0; y < 28; y++) {
            for (u32 x = 0; x < 36; x++) {
                if (mMarks[x][y] && !mPrevMarks[x][y]) {
                    if (GetCleaning()->mUnkAC == 1 && rock->GetCellLayer(x, y) == 6) {
                        continue;
                    }
                    rock->Hit(x, y, force, sDrillPool.mScale, MFx32(0));
                }
            }
        }
        if (rock->GetUnk4B8() && rock->GetCellLayer(cell.x, cell.y) != 0) {
            DinoSoundFunc_0201feec(0x2D);
            return;
        }
        switch (rock->GetCellLayer(cell.x, cell.y)) {
        case 1:
            DinoSoundFunc_0201feec(0x35);
            return;
        case 2:
            DinoSoundFunc_0201feec(0x34);
            return;
        case 3:
            DinoSoundFunc_0201feec(0x34);
            return;
        case 4:
            DinoSoundFunc_0201feec(0x33);
            return;
        case 5:
            DinoSoundFunc_0201feec(0x32);
            return;
        case 6:
            DinoSoundFunc_0201feec(0x31);
            return;
        }
        return;
    }
    mTouching = 0;
    HideGuides();
}

DinoCleaningDrill::~DinoCleaningDrill() {
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MBgCallTask0<DinoCleaningDrill>;
template MTask *MCreateBgCallTask(DinoCleaningDrill *obj, void (DinoCleaningDrill::*func)(), int thread);

// The file's static initialization (see dino_static_init.h), with the drill's fixed-point constants.
// NONMATCHING: the screen rect sits past the +0x80 anchor and the game hoists its zero half.
#pragma always_inline on
static struct DinoCleaningDrillStaticInit {
    DinoCleaningDrillStaticInit() {
        DINO_COMMON_LAYOUT_INIT(data_ov002_021651f8, 0x3c, 0xc0, 0x4c, 0x34, 0x90, 0xa0, 0xa8, 0xb0);
        DINO_POOL(MFx32, data_ov002_021651f8, 0x4) = MFx32::FromRaw(0x666);
        DINO_POOL(MFx32, data_ov002_021651f8, 0x28) = MFx32::FromRaw(0x333);
        DINO_POOL(MFx32, data_ov002_021651f8, 0xc) = MFx32(24);
        DINO_POOL(MFx32, data_ov002_021651f8, 0x1c) = MFx32(8);
        DINO_POOL(MFx32, data_ov002_021651f8, 0x18) = MFx32::FromRaw(0x333);
        DINO_POOL(MFx32, data_ov002_021651f8, 0x14) = MFx32(6);
        DINO_POOL(MFx32, data_ov002_021651f8, 0x24) = MFx32::FromRaw(0x800);
    }
} sDinoCleaningDrillStaticInit;
