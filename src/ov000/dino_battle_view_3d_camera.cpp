// DinoBattleView3dCamera.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_battle_view_3d_camera.h"
#include "types.h"

extern "C" void func_0202b520(void *camera);

DinoBattleView3DCamera::~DinoBattleView3DCamera() {
    func_0202b520(mCamera);
}
