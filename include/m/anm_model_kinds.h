// The inline constructors of the scene script objects' kinds (the script readers make them).
#ifndef M_ANM_MODEL_KINDS_H
#define M_ANM_MODEL_KINDS_H

#include "m/anm_model.h"
#include "m/3d_model.h"
#include "m/3d_scene.h"
#include "m/3d_ryusen.h"

inline MAnmEmit::MAnmEmit(M3DScene *scene) : MAnmModelBase(scene) {
    for (int i = 0; i < 32; i++) {
        mParticles[i] = new M3DModel();
        scene->AddModel(mParticles[i]);
        mParticles[i]->SetUnk1C(0x1000);
    }
    mNext = 0;
    mLoopEnd = 0;
}

inline MAnmTargetCamera::MAnmTargetCamera(M3DScene *scene) : MAnmModelBase(scene) {
    mScene->SetFov(0x1555);
    mTarget.x = 0;
    mTarget.y = 0;
    mTarget.z = 0;
    mPrev.x = 0;
    mPrev.y = 0;
    mPrev.z = 0;
    mSwayPhase = 0;
}

inline MAnmRyusen::MAnmRyusen(M3DScene *scene, MAnmModelBase *target, int jointA, int jointB) : MAnmModelBase(scene) {
    mTarget = target;
    mJointA = jointA;
    mJointB = jointB;
    for (int i = 0; i < 180; i++) {
        mPointsA[i].y = -0xA000;
        mPointsB[i].y = -0xA000;
    }
    mTrails = new M3DRyusen[180];
    for (int i = 0; i < 180; i++) {
        mScene->AddModel(&mTrails[i]);
    }
    mHead = 0;
    mLoopEnd = 0;
}

#endif
