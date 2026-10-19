// DinoFieldMap.
// Only the destructor and GetInfo are decompiled so far.

#include "m/anm_model.h"
#include "m/3d_model.h"
#include "game/dino_field.h"
#include "game/dino_gradation_bg.h"
#include "m/lcd.h"

DinoFieldMap::DinoFieldMap(void *unkC4) : MProcess(NULL) {
    mUnkC4 = unkC4;
    if (mUnkC4 != NULL) {
        mGradation = new DinoGradationBg(gLcd->GetBottomDisplay());
        mParallel.AddTail(mGradation);
    }
    mModel = NULL;
    mUnkD0 = 0;
    MI_CpuFill8(&mInfo, 0, 8);
    Add((u8 *)&mInfo, 8);
    Close();
}

DinoFieldMap::~DinoFieldMap() {
}

DinoFieldMapInfo *DinoFieldMap::GetInfo() {
    return &mInfo;
}

void DinoFieldMap::Idle() {
}

void DinoFieldMap::Read(const void *buf) {
    MSerializer::Read(buf);
    Load();
}

fx32 DinoFieldMap::GetGroundHeight(fx32 x, fx32 y) {
    if (mModel != NULL) {
        return mModel->mModel->GetGroundHeight(x, y);
    }
    return 0;
}

BOOL DinoFieldMap::IsWalkable(fx32 x, fx32 y, fx32 *height) {
    if (mModel != NULL) {
        return mModel->mModel->IsOnGround(x, y, height);
    }
    return TRUE;
}

MFx32 DinoFieldMap::GetWalkSpeed() {
    return ((DinoFieldMapData *) mData.mFile.GetData())->mWalkSpeed;
}

void DinoFieldMap::ClearGradation() {
    mGradation->SetGradation(0, 0, 0, 0, 0, 0);
}

// NONMATCHING: the game keeps the bounds and the cell in memory (more temporaries; the rect's fields re-read).
void *DinoFieldMap::GetArea(MFx32Point pos) {
    MResource<DinoFieldMapGrid> &grid = mGrid;
    MFx32Point half = pos / MFx32(2);
    MPoint cell(MFx32ToInt(half.x), MFx32ToInt(half.y));
    MRect bounds(0, 0, grid.Get()->width, grid.Get()->height);
    if (!bounds.Contains(cell)) {
        DinoFieldMapData *data = (DinoFieldMapData *) mData.mFile.GetData();
        return (u8 *) data + data->mAreas;
    }
    u32 at = cell.y * grid.Get()->width + cell.x;
    u8 area = ((const u8 *) grid.Get() + grid.Get()->cells)[at];
    DinoFieldMapData *data = (DinoFieldMapData *) mData.mFile.GetData();
    return (u8 *) data + data->mAreas + area * 0x14;
}

void *DinoFieldMap::GetEntry44(int index) {
    DinoFieldMapData *data = (DinoFieldMapData *) mData.mFile.GetData();
    return (u8 *) data + data->mEntries44 + index * 12;
}

void *DinoFieldMap::GetEntry4C(int index) {
    DinoFieldMapData *data = (DinoFieldMapData *) mData.mFile.GetData();
    return (u8 *) data + ((u32 *) ((u8 *) data + data->mEntries4C))[index];
}
