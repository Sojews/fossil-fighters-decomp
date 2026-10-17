// DinoCleaningRock.

#include "game/dino_cleaning_rock.h"
#include "game/dino_cleaning_image.h"

DinoCleaningRock::DinoCleaningRock(u32 arg0, DinoCleaningImage *image, u32 arg2, fx32 scale) : MProcess(NULL) {
    mUnk74 = arg0;
    mImage = image;
    mUnk7C = arg2;
    mScale = scale;
}

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
