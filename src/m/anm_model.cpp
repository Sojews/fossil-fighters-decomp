// MAnmModelBase and the animated models (MAnmModel, MAnmEmit, MAnmTargetCamera, MAnmModelSh).
// Not decompiled yet: GetValue, TintPalette and the Unk0C updates.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "m/anm_model.h"
#include "m/3d_model.h"
#include "m/3d_resource.h"
#include "m/3d_scene.h"
#include "game/dino_sound.h"
#include "nitro/mi.h"

// Keeps a scene from drawing for the rest of the scope.
class M3DSceneDisable {
public:
    M3DSceneDisable(M3DScene *scene) {
        mScene = scene;
        u8 enabled = scene->mEnabled;
        scene->mEnabled = FALSE;
        mWasEnabled = enabled;
    }
    ~M3DSceneDisable() { mScene->mEnabled = mWasEnabled; }

    M3DScene *mScene;
    u8 mWasEnabled;
};

void MAnmModelBase::Init() {
    mFrame = 0;
    mUnkF4 = 0;
    mModel = NULL;
    mUnk18C = NULL;
    mUnk190 = NULL;
    mUnk194 = NULL;
    mLoaded = FALSE;
    mFrame = 0;
    mParent = NULL;
    mLoopFrame = 0x40000000;
    mFollowParent = TRUE;
    mActive = TRUE;
    mOwnsResource = FALSE;
    mUnk120 = 0;
    mUnk124 = 0;
    mUnk134 = 0x1000;
    mUnk144 = 0;
    mUnk138 = 0x7FFF;
    mUnk1A4 = 0;
    mJoint = -1;
    mUnk117 = TRUE;
    mName = "noname";
    mResource = NULL;
    mVisible = TRUE;
    mUnkF4 = 0;
    mSpeed = 0x10;
    mUnk145 = 0;
    mUnk11C = 0;
    mUnk13C = 0;
    mUnk11A = TRUE;
    mUnk11B = TRUE;
    mUnk140 = 0;
}

void MAnmModelBase::Load(const char *path, u8 keepPalette) {
    if (!M3DModelExists(path)) {
        M3DSceneDisable disable(mScene);
        Unload();
        mLoaded = FALSE;
        mUnk144 = 0;
        mOwnsResource = FALSE;
        return;
    }
    M3DSceneDisable disable(mScene);
    if (mResource != NULL) {
        delete mResource;
    }
    mResource = new M3DModelResource(path);
    mModel = new M3DModel();
    mScene->AddModel(mModel);
    mResource->Setup(mModel, keepPalette);
    mModel->SetUnk1C(mUnk134);
    mModel->SetUnk10(0);
    mLoaded = TRUE;
    mUnk144 = 1;
    mOwnsResource = TRUE;
}

void MAnmModelBase::Unload() {
    mLoaded = FALSE;
    if (mOwnsResource) {
        if (mResource != NULL) {
            delete mResource;
        }
        mResource = NULL;
        return;
    }
    if (mUnk18C != NULL) {
        delete[] mUnk18C;
    }
    if (mUnk190 != NULL) {
        delete[] mUnk190;
    }
    if (mUnk194 != NULL) {
        delete[] mUnk194;
    }
    mUnk18C = NULL;
    mUnk190 = NULL;
    mUnk194 = NULL;
}

void MAnmModelBase::Execute() {
    if (!mActive) {
        return;
    }
    mFrame += mSpeed;
    if (mFrame > 0x40000000) {
        mFrame = 0x40000000;
    }
    if (mFrame > mEndFrame) {
        if (mEndFrame > mLoopFrame) {
            mFrame = mLoopFrame;
            if (mLoopEnd >= mEndFrame) {
                mLoopEnd -= mEndFrame - mLoopFrame;
            }
        } else if (mEndFrame != 0x40000000) {
            Unk10();
            Suspend();
            return;
        } else {
            mFrame = mEndFrame;
        }
    }
    if (mFollowParent && mParent != NULL) {
        MI_Copy64B(mParent->mModel->GetMatrix(), &mMatrix);
        MtxFx44 joint;
        if (mJoint >= 0 && mParent->mModel->GetJointMatrix(mJoint, &joint, mUnk145)) {
            MTX_Concat44(&joint, &mMatrix, &mMatrix);
        }
        if (mModel != NULL) {
            mModel->mUnkC4 = &mMatrix;
        }
    } else {
        MTX_Identity44_(&mMatrix);
        if (mModel != NULL) {
            mModel->mUnkC4 = NULL;
        }
    }
    Unk0C();
}

void MAnmModelBase::Unk10() {
}

void MAnmModelBase::Unk0C() {
}

void MAnmModelBase::PlaySounds(int frame) {
    if (mSounds.size() != 0) {
        for (u32 i = 0; i < mSounds.size(); i++) {
            if (frame + 0x10 == mSounds[i].mFrame) {
                MAnmKey *key = &mSounds[i];
                DinoSoundFunc_0202016c(key->mMin, key->mMax, key->mUnkC >> 16, (s16) key->mUnkC);
            }
        }
    }
}

void MAnmModelBase::ClearTransformTracks() {
    mTracks[6].clear();
    mTracks[7].clear();
    mTracks[8].clear();
    mTracks[3].clear();
    mTracks[4].clear();
    mTracks[5].clear();
    mTracks[0].clear();
    mTracks[1].clear();
    mTracks[2].clear();
}

void MAnmModelSh::Unk10() {
    if (mModel != NULL) {
        mModel->SetUnk10(mVisible);
    }
}

MAnmModelBase::~MAnmModelBase() {
    if (mModel != NULL) {
        mScene->RemoveModel(mModel);
        delete mModel;
    }
    Unload();
}

MAnmEmit::~MAnmEmit() {
    for (int i = 0; i < 32; i++) {
        mScene->RemoveModel(mParticles[i]);
        delete mParticles[i];
    }
}

void MAnmEmit::Unk10() {
    for (int i = 0; i < 32; i++) {
        mParticles[i]->SetUnk10(0);
    }
}

void MAnmModel::Unk10() {
    if (mModel != NULL) {
        mModel->SetUnk10(mVisible);
    }
}

MAnmModel::~MAnmModel() {
}

MAnmTargetCamera::~MAnmTargetCamera() {
}

MAnmModelSh::~MAnmModelSh() {
}
