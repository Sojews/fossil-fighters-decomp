// DinoBattleView3dCamera.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_battle_view_3d_camera.h"
#include "types.h"
#include "game/dino_3d_load_csv.h"
#include "game/dino_battle_view_3d_move.h"

DinoBattleView3DCamera::~DinoBattleView3DCamera() {
    mCamera->Push18();
}

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x34];
    void (DinoBattleView3DMove::*Update_34)();
    u8 unk3C[0x4C - 0x3C];
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

#pragma auto_inline off

void DinoBattleView3DMove::StateIdle() {
}

void DinoBattleView3DMove::Start() {
    mState = data_ov000_02196cd0.Update_34;
    mUnkD0 = 0;
    mUnkCC = 0;
}

void DinoBattleView3DMove::StateEnd() {
    Exit();
    mUnkD0++;
}
