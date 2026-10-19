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

// The point's whole part.
static inline MPoint ToPoint(MFx32Point p) {
    return MPoint(MFx32ToInt(p.x), MFx32ToInt(p.y));
}

// NONMATCHING: stack slots of the division's temporaries only (as GetCameraIndex).
void *DinoFieldMap::GetArea(MFx32Point pos) {
    MResource<DinoFieldMapGrid> &grid = mGrid;
    MPoint cell = ToPoint(pos / MFx32(2));
    if (!MRect(0, 0, grid.Get()->width, grid.Get()->height).Contains(cell)) {
        DinoFieldMapData *data = (DinoFieldMapData *) mData.mFile.GetData();
        return (u8 *) data + data->mAreas;
    }
    u32 at = cell.x + cell.y * grid.Get()->width;
    u8 area = grid.Get()->GetCells()[at];
    DinoFieldMapData *data = (DinoFieldMapData *) mData.mFile.GetData();
    return (u8 *) data + data->mAreas + area * 0x14;
}

// GetCameraIndex's lowest cell, in this file's data (what is around it is not known).
extern "C" struct {
    u32 unk0;
    int minY;   // 0x4
    u32 unk8;
    int minX;   // 0xC
} data_ov001_0216a134;

// NONMATCHING: stack slots of the division's temporaries only (the game's go the other way round).
int DinoFieldMap::GetCameraIndex(MFx32Point pos) {
    DinoFieldMapData *data = (DinoFieldMapData *) mData.mFile.GetData();
    if (data->mCameraCount <= 1) {
        return 0;
    }
    MResource<DinoFieldMapGrid> &grid = mCameraGrid;
    MPoint cell = ToPoint(pos / MFx32(2));
    cell.x = std::min((int) grid.Get()->width - 1, std::max(data_ov001_0216a134.minX, cell.x));
    cell.y = std::min((int) grid.Get()->height - 1, std::max(data_ov001_0216a134.minY, cell.y));
    u32 at = cell.x + cell.y * grid.Get()->width;
    return grid.Get()->GetCells()[at];
}

DinoFieldMapCamera *DinoFieldMap::GetCamera(int index) {
    DinoFieldMapData *data = (DinoFieldMapData *) mData.mFile.GetData();
    return (DinoFieldMapCamera *) ((u8 *) data + data->mCameras) + index;
}

// The point in fixed point.
static inline MFx32Point ToFx32Point(MPoint p) {
    return MFx32Point(MFx32(p.x), MFx32(p.y));
}

// NONMATCHING: stack slots only (the game's square root result is above the centre).
int DinoFieldMap::FindCircle(MFx32Point pos) {
    for (u32 i = 1; i < ((DinoFieldMapData *) mData.mFile.GetData())->mCircleCount; i++) {
        DinoFieldMapData *data = (DinoFieldMapData *) mData.mFile.GetData();
        DinoFieldMapCircle *circle =
            (DinoFieldMapCircle *) ((u8 *) data + ((u32 *) ((u8 *) data + data->mCircles))[i]);
        MFx32Point center = ToFx32Point(MPoint(circle->x, circle->y));
        if ((center - pos).Length() <= MFx32(circle->radius)) {
            return i;
        }
    }
    return 0;
}

DinoFieldMapCircle *DinoFieldMap::GetCircle(int index) {
    DinoFieldMapData *data = (DinoFieldMapData *) mData.mFile.GetData();
    return (DinoFieldMapCircle *) ((u8 *) data + ((u32 *) ((u8 *) data + data->mCircles))[index]);
}
