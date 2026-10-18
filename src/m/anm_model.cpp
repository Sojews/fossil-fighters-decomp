// The animated 3D objects (MAnmModelBase and its kinds) and MPolygonChar, which plays them from a scene script.
// Not decompiled yet: GetValue, TintPalette, the Unk0C updates, MPolygonChar::Load (the script reader) and the
// node data reader.

#include "m/anm_model.h"
#include "m/3d_model.h"
#include "m/3d_resource.h"
#include "m/3d_scene.h"
#include "m/3d_ryusen.h"
#include "m/polygon_char.h"
#include "m/process_pause.h"
#include "m/heap.h"
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
    mPosRef = NULL;
    mRotRef = NULL;
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

void MAnmModelBase::TintPalette(u16 color, fx32 amount) {
    if (mResource == NULL) {
        return;
    }
    u32 plttKey;
    u32 size;
    u8 *palette;
    mResource->GetPalette(&plttKey, &palette, &size);
    if (palette == NULL) {
        return;
    }
    u16 *tinted = new u16[size / 2];
    int r = (color & 0x1F) * amount;
    int g = ((color >> 5) & 0x1F) * amount;
    int b = ((color >> 10) & 0x1F) * amount;
    fx32 keep = 0x1000 - amount;
    for (u32 i = 0; i < size / 2; i++) {
        u16 c = ((u16 *) palette)[i];
        int cr = c & 0x1F;
        int cg = (c >> 5) & 0x1F;
        int cb = (c >> 10) & 0x1F;
        cr = (cr * keep + r) >> 12;
        cg = (cg * keep + g) >> 12;
        cb = (cb * keep + b) >> 12;
        tinted[i] = cr | (cg << 5) | (cb << 10);
    }
    mScene->LoadPltt(plttKey, tinted);
    mScene->QueueE8(NULL, (u32) tinted);
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

// The kinds' destructors have this one's body inlined.
#pragma auto_inline on
MAnmModelBase::~MAnmModelBase() {
    if (mModel != NULL) {
        mScene->RemoveModel(mModel);
        delete mModel;
    }
    Unload();
}
#pragma auto_inline reset

void MAnmModelSh::Unk10() {
    if (mModel != NULL) {
        mModel->SetUnk10(mVisible);
    }
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

// A script keyword and its value; tables end with -1.
struct MAnmName {
    const char *mName;
    int mValue;
};

// (Kept until MPolygonChar::Load, which uses them, is decompiled.)
#pragma force_active on

// Makes a key from a script line's values if it has a value (flags[1]), and a maximum (flags[2]).
static BOOL MAnmMakeKey(MAnmKey *key, int frame, const u8 *flags, const int *values) {
    if (flags[1] == 1) {
        key->mFrame = frame;
        key->mMin = values[1];
        key->mMax = values[1];
        key->mUnkC = 0;
        if (flags[2] == 1) {
            key->mMax = values[2];
        }
        return TRUE;
    }
    return FALSE;
}

static int MAnmStrCaseCmp(const char *s1, const char *s2) {
    while (*s1 != 0 && *s2 != 0) {
        char c1 = *s1;
        char c2 = *s2;
        if (c1 >= 'A' && c1 <= 'Z') {
            c1 += 0x20;
        }
        if (c2 >= 'A' && c2 <= 'Z') {
            c2 += 0x20;
        }
        if (c1 != c2) {
            break;
        }
        s1++;
        s2++;
    }
    return *s1 - *s2;
}

static int MAnmLookup(const char *name, const MAnmName *table) {
    while (table->mValue != -1) {
        if (MAnmStrCaseCmp(table->mName, name) == 0) {
            break;
        }
        table++;
    }
    return table->mValue;
}

#pragma force_active reset

MPolygonChar::MPolygonChar(void *unk80, u16 unkA4, void *unk84, void *unk88) : MProcess(NULL) {
    mUnk74 = NULL;
    mUnk80 = unk80;
    mUnk84 = unk84;
    mUnk88 = unk88;
    mUnkA4 = unkA4;
    OS_InitMutex(&mMutex);
    mParallel.SetUseMutex(TRUE);
    mRot.x = 0;
    mRot.y = 0;
    mRot.z = 0;
    mPos.x = 0;
    mPos.y = 0;
    mPos.z = 0;
    mUnkB0 = 0x1000;
    mUnkAC = 1;
    mUnkA6 = 0x7FFF;
    mUnk43C = 0;
    mPauseCount = 0;
    Reset();
}

void MPolygonChar::Reset() {
    MProcessPause pause(this, &mMutex, &mPauseCount);
    mUnkA8 = 0;
    mEndFrame = 0x0FFFFFFF;
    mFrame = 0;
    mUnk74 = NULL;
    mUnk78 = NULL;
    mUnk7C = NULL;
    mParallel.Clear();
}

MPolygonChar::~MPolygonChar() {
}

void MPolygonChar::Idle() {
    MScopedLock lock(&mMutex, TRUE);
    if (mPauseCount != 0) {
        return;
    }
    if (mFrame >= mEndFrame) {
        Reset();
        return;
    }
    mFrame++;
    MAnmModelBase *anm = mUnk74;
    if (anm != NULL && mUnkAC == 1 && mUnkB0 > 0x10) {
        BOOL looped;
        if (anm->mModel != NULL) {
            int frames = anm->mModel->GetFrameCount();
            looped = (anm->mFrame - anm->mStartFrame) % (frames << 4) == 0 ? TRUE : FALSE;
        } else {
            looped = FALSE;
        }
        if (looped) {
            MAnmModelBase *restart = mUnk7C;
            if (restart != NULL) {
                restart->Unk10();
                restart->mFrame = 0;
                restart->mLoopEnd = 0;
                restart->mSpeed = 0x10;
                mUnk7C->Run();
            }
        }
    }
}

MAnmRyusen::~MAnmRyusen() {
    for (int i = 0; i < 180; i++) {
        mScene->RemoveModel(&mTrails[i]);
    }
    delete[] mTrails;
}

void MAnmRyusen::Unk10() {
    for (int i = 0; i < 180; i++) {
        mTrails[i].SetUnk10(0);
    }
}

void MAnmSE::Unk0C() {
    if (mFrame >= mStartFrame) {
        PlaySounds(mFrame - mStartFrame);
    }
}

MAnmSE::~MAnmSE() {
}

#pragma auto_inline off
template class std::__vec_deleter<MAnmKey, std::allocator<MAnmKey> >;
