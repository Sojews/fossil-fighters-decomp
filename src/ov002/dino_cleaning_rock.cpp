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


// NONMATCHING: 99.7%, temporaries take stack slots in the opposite order (incl. the MFx32 operators' copies).
MFx32 DinoCleaningRock::Hit(int row, int col, MFx32 power) {
    if (IsCellClear(row, col)) {
        return power;
    }
    int layer = mImage->mGrid[row][col];
    if (layer != 0) {
        do {
            if (mImage->GetLayer(layer)->mHard[row][col] == 0) {
                mImage->SetCellLayer(row, col, layer);
                if (mHardness[row][col] == MFx32(0)) {
                    if (mImage->mGrid[row][col] != 0) {
                        mHardness[row][col] =
                            mScale * MFx32::FromRaw(mImage->GetLayer(mImage->mGrid[row][col])->mGrid[row][col]);
                    } else {
                        mHardness[row][col] = MFx32(0);
                    }
                }
                if (mHardness[row][col] > power) {
                    mImage->SetCellLayer(row, col, layer);
                    MFx32 &hardness = mHardness[row][col];
                    hardness = hardness - power;
                    return MFx32(0);
                }
                power = power - mHardness[row][col];
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
