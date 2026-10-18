// DinoFieldMap.
// Only the destructor and GetInfo are decompiled so far.

#include "m/anm_model.h"
#include "m/3d_model.h"
#include "game/dino_field.h"
#include "game/dino_gradation_bg.h"

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
    return ((DinoFieldMapData *) mFile.GetData())->mWalkSpeed;
}

void DinoFieldMap::ClearGradation() {
    mGradation->SetGradation(0, 0, 0, 0, 0, 0);
}

void *DinoFieldMap::GetEntry44(int index) {
    DinoFieldMapData *data = (DinoFieldMapData *) mFile.GetData();
    return (u8 *) data + data->mEntries44 + index * 12;
}

void *DinoFieldMap::GetEntry4C(int index) {
    DinoFieldMapData *data = (DinoFieldMapData *) mFile.GetData();
    return (u8 *) data + ((u32 *) ((u8 *) data + data->mEntries4C))[index];
}
