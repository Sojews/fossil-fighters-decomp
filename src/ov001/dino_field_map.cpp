// DinoFieldMap.

#pragma ipa file

#include "m/anm_model.h"
#include "m/3d_model.h"
#include "game/dino_field.h"
#include "game/dino_gradation_bg.h"
#include "m/lcd.h"
#include "m/3d_resource.h"
#include "game/dino_resource.h"
#include "game/dino_field_raster.h"
#include "nitro/mi.h"
#include "nitro/os.h"

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

// The TU's pooled data: GetCameraIndex's lowest cell and the model path format.
extern "C" struct {
    u32 unk0;
    int minY;               // 0x4
    const char *mModelPath; // 0x8: "/model/fieldmap/%s"
    int minX;               // 0xC
} data_ov001_0216a134;
// The fog table, and its density.
extern "C" u32 data_ov001_0216a18c[8];
extern const u32 data_0208a228;
extern "C" void G3X_SetFog(BOOL enable, int mode, int slope, int offset);
extern "C" void G3X_SetFogTable(const u32 *table);
// Rolls the map's kaseki rocks.
extern "C" void func_ov001_0215f900(DinoFieldMap *map);

static inline const DinoFieldMapData *DinoMapData(DinoFieldMap *map) {
    return (const DinoFieldMapData *)map->mData.Get();
}

// Loads the map mInfo names: its data, gradation, raster and fog, grids, rocks and model.
void DinoFieldMap::Load() {
    if (mModel != NULL) {
        mParallel.Remove(mModel);
        delete mModel;
        mModel = NULL;
        mData = MResource<u8>();
        mGrid = MResource<DinoFieldMapGrid>();
        mCameraGrid = MResource<DinoFieldMapGrid>();
        if (mUnkD0 != 0) {
            mParallel.Remove((MTask *)mUnkD0);
            delete (MTask *)mUnkD0;
            mUnkD0 = 0;
        }
    }
    if (mInfo.id == 0) {
        return;
    }
    mData = gDinoResource->LoadMapM(mInfo.id);
    const u8 *top0 = DinoMapData(this)->GetGradTop();
    const u8 *top1 = DinoMapData(this)->GetGradTop();
    const u8 *top2 = DinoMapData(this)->GetGradTop();
    const u8 *bottom0 = DinoMapData(this)->GetGradBottom();
    const u8 *bottom1 = DinoMapData(this)->GetGradBottom();
    const u8 *bottom2 = DinoMapData(this)->GetGradBottom();
    mGradation->SetGradation(top0[0], top1[1], top2[2], bottom0[0], bottom1[1], bottom2[2]);
    if (DinoMapData(this)->mFog || DinoMapData(this)->mRaster) {
        mUnkD0 = (u32) new DinoFieldRaster(0);
        mParallel.AddTail((MTask *)mUnkD0);
    }
    if (DinoMapData(this)->mFog) {
        u32 density = data_0208a228;
        for (u32 i = 0; i < 8; i++) {
            data_ov001_0216a18c[i] = density | (density << 8) | (density << 16) | (density << 24);
        }
        DC_FlushRange(data_ov001_0216a18c, sizeof(data_ov001_0216a18c));
        G3X_SetFog(TRUE, 0, 0, 0);
        *(vu32 *)0x4000358 = 0x1F7D08;
        G3X_SetFogTable(data_ov001_0216a18c);
    } else {
        G3X_SetFog(FALSE, 0, 0, 0);
    }
    if (DinoMapData(this)->mHasGrid != 0) {
        mGrid = gDinoResource->LoadMapR(mInfo.id);
    }
    if (DinoMapData(this)->mCameraCount > 1) {
        mCameraGrid = gDinoResource->LoadMapC(mInfo.id);
    }
    if (mUnk90 != DinoMapData(this)->mRockSet) {
        mUnk90 = DinoMapData(this)->mRockSet;
        if (mUnk90 != 0) {
            func_ov001_0215f900(this);
        }
    }
    char path[0x100];
    const DinoFieldMapData *data = DinoMapData(this);
    OS_SPrintf(path, data_ov001_0216a134.mModelPath, (const char *)data + data->mModelName);
    mModel = new MAnmModel((M3DScene *)mUnkC4);
    mModel->unk100 = 0x24;
    mModel->SetFrames(0, 0x40000000);
    mModel->Load(path, 0);
    mModel->Run();
    mModel->mUnkF4 = 1;
    mModel->mFollowParent = TRUE;
    mParallel.AddTail(mModel);
    data = DinoMapData(this);
    OS_SPrintf(path, data_ov001_0216a134.mModelPath, (const char *)data + data->mGridName);
    M3DModelResource grid((std::string(path)));
    mModel->mModel->LoadGrid(grid.mModel);
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
