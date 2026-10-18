// DinoBattleView3dCamera.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_battle_view_3d_camera.h"
#include "types.h"
#include "game/dino_3d_load_csv.h"

DinoBattleView3DCamera::~DinoBattleView3DCamera() {
    mCamera->Push18();
}

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x4C];
    void (DinoBattleView3DCamera::*Run_4C)();
} data_ov000_02196cd0;
// The default camera data.
extern "C" u8 data_ov000_021903dc[];

DinoBattleView3DCamera::DinoBattleView3DCamera(M3DScene *scene, Dino3DLoadCsv *camera, u32 unk88)
    : MProcess(NULL), mCamera(camera), mScene(scene) {
    mState = data_ov000_02196cd0.Run_4C;
    mUnk9C = data_ov000_021903dc;
    mUnk88 = unk88;
    Reset();
    mLight = -1;
    unk80 = 0;
}
