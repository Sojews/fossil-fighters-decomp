// DinoCleaningRock.

#include "game/dino_cleaning_rock.h"
#include "game/dino_cleaning_image.h"
#include "game/dino_cleaning_image_layer.h"
#include "m/fx32.h"
#include "game/dino_static_init.h"

DinoCleaningRock::DinoCleaningRock(u32 arg0, DinoCleaningImage *image, u32 arg2, fx32 scale) : MProcess(NULL) {
    mUnk74 = arg0;
    mImage = image;
    mUnk7C = arg2;
    mScale.v = scale;
}

// NONMATCHING: the column offset and the output row pointer swap registers (the game keeps the offset on the
// stack). The game doesn't strength-reduce the row index.
#pragma opt_strength_reduction off
void DinoCleaningRock::RefreshHardness() {
    for (u32 col = 0; col < 28; col++) {
        for (u32 row = 0; row < 36; row++) {
            if (mImage->mGrid[row][col] != 0) {
                mHardness[row][col] =
                    mScale * MFx32::FromRaw(mImage->GetLayer(mImage->mGrid[row][col])->mGrid[row][col]);
            } else {
                mHardness[row][col] = MFx32(0);
            }
        }
    }
}
#pragma opt_strength_reduction reset

DinoCleaningRock::~DinoCleaningRock() {
}

void DinoCleaningRock::Idle() {
}

DinoCleaningImage *DinoCleaningRock::GetImage() {
    return mImage;
}

MFx32 DinoCleaningRock::GetRatio() {
    return mImage->GetRatio();
}

BOOL DinoCleaningRock::IsCellClear(int row, int col) {
    if (mImage->mGrid[row][col] == 0) {
        return TRUE;
    }
    return FALSE;
}

s32 DinoCleaningRock::GetCellLayer(int row, int col) {
    return mImage->mGrid[row][col];
}

u8 DinoCleaningRock::GetCell(int row, int col) {
    return mImage->GetCell(row, col);
}


// Hit's MFx32 temporaries take stack slots by inline depth (shallower = higher), so the operations sit at the depths
// that give the game's slot order: the refresh one level down, the compare/subtractions two or more.
static inline void RefreshCell(DinoCleaningRock *rock, int row, int col) {
    if (rock->mImage->mGrid[row][col] != 0) {
        rock->mHardness[row][col] =
            rock->mScale * MFx32::FromRaw(rock->mImage->GetLayer(rock->mImage->mGrid[row][col])->mGrid[row][col]);
    } else {
        rock->mHardness[row][col] = MFx32(0);
    }
}

static inline void SubFrom(MFx32 &p, const MFx32 &h) { p -= h; }
static inline void SubFrom2(MFx32 &p, MFx32 h) { SubFrom(p, h); }
static inline void SubFrom3(MFx32 &p, const MFx32 &h) { SubFrom2(p, h); }
static inline bool Gt(const MFx32 &a, const MFx32 &b) { return a > b; }
static inline void Sub1(MFx32 &h, const MFx32 &p) { h -= p; }

// NONMATCHING: 99.8%, the refresh's FromRaw copy and the * operator's b copy swap stack slots, and the hardness
// subtraction swaps r1/r2.
MFx32 DinoCleaningRock::Hit(int row, int col, MFx32 power) {
    if (IsCellClear(row, col)) {
        return power;
    }
    u32 layer = mImage->mGrid[row][col];
    if (layer != 0) {
        do {
            if (mImage->GetLayer(layer)->mHard[row][col] == 0) {
                mImage->SetCellLayer(row, col, layer);
                if (mHardness[row][col] == MFx32(0)) {
                    RefreshCell(this, row, col);
                }
                if (Gt(mHardness[row][col], power)) {
                    mImage->SetCellLayer(row, col, layer);
                    MFx32 &hardness = mHardness[row][col];
                    Sub1(hardness, power);
                    return MFx32(0);
                }
                SubFrom3(power, mHardness[row][col]);
                mHardness[row][col] = MFx32(0);
            }
        } while (--layer != 0);
    }
    mImage->SetCellLayer(row, col, 0);
    mHardness[row][col] = MFx32(0);
    return power;
}

// The file's static initialization (see dino_static_init.h).
extern "C" u32 data_ov002_02165730[];
#pragma always_inline on
static struct DinoCleaningRockStaticInit {
    DinoCleaningRockStaticInit() {
        DINO_COMMON_LAYOUT_INIT(data_ov002_02165730, 0x0, 0x38, 0x18, 0x10, 0x48, 0x58, 0x60, 0x68);
    }
} sDinoCleaningRockStaticInit;
