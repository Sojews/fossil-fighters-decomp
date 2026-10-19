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
    u8 unk00[0x14];
    void (DinoBattleView3DMove::*State_14)();
    u8 unk1C[0x34 - 0x1C];
    void (DinoBattleView3DMove::*Update_34)();
    u8 unk3C[0x4C - 0x3C];
    void (DinoBattleView3DCamera::*Run_4C)();
    void (DinoBattleView3DMove::*StateIdle_54)();
} data_ov000_02196cd0;
// The default camera data.
extern "C" u8 data_ov000_021903dc[];

DinoBattleView3DMove::DinoBattleView3DMove(const DinoBattleView3DMoveParams *params, u32 unk74, Dino3DLoadCsv *model,
                                           Dino3DLoadCsv *camera, const void *unk164, u8 unk160)
    : MProcess(NULL) {
    mState = data_ov000_02196cd0.State_14;
    mUnkBC = 0;
    mUnk74 = unk74;
    mModel = model;
    mUnkEC = params->unk0;
    mUnkF0 = params->unk4;
    mUnkFC = params->unk10;
    mUnkF6 = params->unkA;
    mUnkF4 = params->unk8;
    mUnkF8 = params->unkC;
    mUnkFA = params->unkE;
    mState = data_ov000_02196cd0.StateIdle_54;
    mCamera = camera;
    mUnk164 = unk164;
    mUnkC8 = 0;
    mUnk160 = unk160;
}

DinoBattleView3DMove::~DinoBattleView3DMove() {
}

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

// Moves the camera a step (slowing over the last steps), then runs the state.
void DinoBattleView3DMove::Idle() {
    if (mUnkC8 && mUnk160) {
        mUnkCC++;
        int n = mUnkD0;
        if (n >= 1) {
            if (n > mUnk158 - 1 && mUnkFA != 0) {
                n = mUnk158 - n;
            } else {
                n = 1;
            }
        }
        VecFx32 step;
        step.x = mStep.x * n;
        step.y = mStep.y * n;
        step.z = mStep.z * n;
        if (mCamera != NULL) {
            mCamera->PushVec0(&step);
        }
    }
    (this->*mState)();
}
