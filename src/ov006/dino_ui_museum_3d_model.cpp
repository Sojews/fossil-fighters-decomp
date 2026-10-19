// DinoUiMuseum3dModel.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_ui_museum_3d_model.h"
#include "game/dino_ui_museum.h"
#include "m/3d_scene.h"
#include "m/3d.h"
#include "m/picture.h"
#include "m/blend.h"
#include "m/lcd.h"
#include "m/display.h"
#include "types.h"

// The 3D scene's heap (in ov003's memory), and the background's path.
extern "C" u8 data_ov003_021624cc[];
extern "C" const char data_ov006_02196e0c[];

DinoUiMuseum3DModel::DinoUiMuseum3DModel() : MProcess(NULL), mCurrent(0), mRequested(0) {
    mScene = new M3DScene((u32)(data_ov003_021624cc + 4), 0xF618);
    mScene->mUnk14 = 1;
    mScene->mUnk18 = 0x12C000;
    mScene->mM3d->mBgCntBits.priority = 3;
    VecFx32 v;
    v.x = 0;
    v.y = 5 << 12;
    v.z = 0;
    mScene->SetCameraTarget(&v);
    v.x = 4 << 12;
    v.y = 1 << 11;
    v.z = 10 << 12;
    mScene->SetCameraPos(&v);
    mTasks = new MParallelTaskSet;
    GetParallel()->AddTail(mTasks, mScene);
    DinoUiMuseumEffectLight *light = new DinoUiMuseumEffectLight;
    light->mHeight = 0x60;
    light->mPicture->Show();
    light->mPicture->Show();
    light->mTimer = 60;
    mParallel.AddTail(light);
    mBackground = new MTextPicture(gLcd->GetTopDisplay(), std::string(data_ov006_02196e0c), TRUE, false);
    mBackground->SetPriority(2);
    mBackground->Show();
    mParallel.AddTail(mBackground);
}

DinoUiMuseum3DModel::~DinoUiMuseum3DModel() {
}
