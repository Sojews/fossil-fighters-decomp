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
