// The animated 3D objects (MAnmModelBase and its kinds) and MPolygonChar, which plays them from a scene script.
// Not decompiled yet: TintPalette, most of the Unk0C updates, MPolygonChar::Load (the script reader) and the
// node data reader.

#pragma ipa file

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

#pragma opt_strength_reduction off
// A track's value at a frame: random between the keys' bounds, interpolated between the keys around the frame
// (the last key's bounds after the last key, the first key's before the first).
// NONMATCHING: the game reloads the track's size in the search loop and recomputes the track's address at each use,
// and keeps the 64-bit operands on the stack.
int MAnmModelBase::GetValue(int track, int frame) {
    u32 count = mTracks[track].size();
    if (count == 0) {
        return 0;
    }
    u32 i;
    for (i = 0; i < mTracks[track].size(); i++) {
        if (frame + 0x10 < mTracks[track][i].mFrame) {
            break;
        }
    }
    if (i == count || count == 1) {
        MAnmKey *key = &mTracks[track][mTracks[track].size() - 1];
        return key->mMin + mRandom.Next(key->mMax - key->mMin);
    }
    if (i == 0) {
        MAnmKey *key = &mTracks[track][0];
        return key->mMin + mRandom.Next(key->mMax - key->mMin);
    }
    MAnmKey *a = &mTracks[track][i - 1];
    MAnmKey *b = &mTracks[track][i];
    s64 span = (s64)b->mFrame - a->mFrame;
    frame += 0x10;
    s64 t = (s64)frame - a->mFrame;
    s64 u = span - t;
    if (span == 0) {
        return a->mMin + mRandom.Next(a->mMax - a->mMin);
    }
    int lo = ((s64)a->mMin * u + (s64)b->mMin * t) / span;
    int hi = ((s64)a->mMax * u + (s64)b->mMax * t) / span;
    return lo + mRandom.Next(hi - lo);
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
#pragma opt_strength_reduction reset

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

// Sets the model's animation frame (in 1/16ths), held at its last frame.
static inline void MAnmSetModelFrame(M3DModel *model, u32 frame) {
    if (frame >= ((const u32 *)model->mModelRes)[8] * 16) {
        frame = (((const u32 *)model->mModelRes)[8] - 1) * 16;
    }
    model->mFrame = frame;
}

// Raises last to the track's last key's frame.
static inline void MAnmLastFrame(std::vector<MAnmKey> &track, u32 &last) {
    if (track.size() != 0) {
        u32 frame = track[track.size() - 1].mFrame;
        if (frame > last) {
            last = frame;
        }
    }
}

// Plays the model's animation and follows the transform, color and speed tracks (only while they change, or when
// something was set).
void MAnmModel::Unk0C() {
    if (!mLoaded) {
        return;
    }
    if (mFrame >= mStartFrame) {
        int frames = mModel->GetFrameCount() << 4;
        if (mUnkF4 == 0 && frames <= mFrame - mStartFrame) {
            MAnmSetModelFrame(mModel, mFrame - mStartFrame);
            mModel->SetUnk10(mVisible);
            if (!mVisible) {
                Suspend();
                return;
            }
            mFrame = mStartFrame + frames - 16;
        }
        if (frames > 0) {
            mUnk140 = (u32)(mFrame - mStartFrame) % frames;
            MAnmSetModelFrame(mModel, mUnk140);
        }
        mModel->SetUnk10(mFollowParent && mUnk134);
        mModel->SetPolygonId(unk100);
        PlaySounds(mFrame - mStartFrame);
        if (mUnk11A) {
            mUnk13C = 0;
            MAnmLastFrame(mTracks[0], mUnk13C);
            MAnmLastFrame(mTracks[1], mUnk13C);
            MAnmLastFrame(mTracks[2], mUnk13C);
            MAnmLastFrame(mTracks[3], mUnk13C);
            MAnmLastFrame(mTracks[4], mUnk13C);
            MAnmLastFrame(mTracks[5], mUnk13C);
            MAnmLastFrame(mTracks[6], mUnk13C);
            MAnmLastFrame(mTracks[7], mUnk13C);
            MAnmLastFrame(mTracks[8], mUnk13C);
            MAnmLastFrame(mTracks[9], mUnk13C);
            MAnmLastFrame(mTracks[10], mUnk13C);
        }
        if (mUnk11A || mUnk11B || mFrame <= mUnk13C + mStartFrame) {
            mPos.x = mPos.y = mPos.z = 0;
            if (mTracks[0].size() != 0) {
                mPos.x = GetValue(0, mFrame - mStartFrame);
            }
            if (mTracks[1].size() != 0) {
                mPos.y = GetValue(1, mFrame - mStartFrame);
            }
            if (mTracks[2].size() != 0) {
                mPos.z = GetValue(2, mFrame - mStartFrame);
            }
            if (mPosRef != NULL) {
                mPos.x += mPosRef->x;
                mPos.y += mPosRef->y;
                mPos.z += mPosRef->z;
            }
            mModel->SetTranslation(&mPos);
            VecFx32 v;
            v.x = v.y = v.z = 0;
            if (mTracks[3].size() != 0) {
                v.x = (u16)(GetValue(3, mFrame - mStartFrame) * 16 / 360);
            }
            if (mTracks[4].size() != 0) {
                v.y = (u16)(GetValue(4, mFrame - mStartFrame) * 16 / 360);
            }
            if (mTracks[5].size() != 0) {
                v.z = (u16)(GetValue(5, mFrame - mStartFrame) * 16 / 360);
            }
            if (mRotRef != NULL) {
                v.x += mRotRef->x * 16 / 360;
                v.y += mRotRef->y * 16 / 360;
                v.z += mRotRef->z * 16 / 360;
                v.x = (u16)v.x;
                v.y = (u16)v.y;
                v.z = (u16)v.z;
            }
            mModel->SetRotation(&v);
            v.x = v.y = v.z = FX32_ONE;
            if (mTracks[6].size() != 0) {
                v.x = GetValue(6, mFrame - mStartFrame);
            }
            if (mTracks[7].size() != 0) {
                v.y = GetValue(7, mFrame - mStartFrame);
            }
            if (mTracks[8].size() != 0) {
                v.z = GetValue(8, mFrame - mStartFrame);
            }
            mModel->SetScale(&v);
            u32 alpha;
            if (mTracks[9].size() != 0) {
                alpha = GetValue(9, mFrame - mStartFrame) * (int)mUnk134 >> FX32_SHIFT;
            } else {
                alpha = mUnk134;
            }
            u16 color = mUnk138;
            mModel->SetUnk1C(alpha);
            mModel->SetUnk2F4(color);
            mSpeed = 16;
            if (mTracks[10].size() != 0) {
                mSpeed = GetValue(10, mFrame - mStartFrame) >> 8;
            }
        }
        mUnk11A = mUnk11B = FALSE;
    }
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

// Emits a particle (a copy of the model, placed by the transform tracks) every so often, and moves the live ones
// along their z axes.
// NONMATCHING: the game loads a particle's speed before the particle (r1/r0 swapped).
void MAnmEmit::Unk0C() {
    if (!mLoaded) {
        return;
    }
    if (mFrame >= mStartFrame) {
        int emitted = -1;
        PlaySounds(mFrame - mStartFrame);
        mUnk140 = (u32)(mFrame - mStartFrame) % (mModel->GetFrameCount() << 4);
        if (mLoopEnd <= mFrame) {
            emitted = mNext & 31;
            mNext++;
            M3DModel *model = mModel;
            M3DModel *particle = mParticles[emitted];
            particle->mUnkDC = model->mUnkDC;
            particle->mTexKey = model->mTexKey;
            particle->mPlttKey = model->mPlttKey;
            particle->Unk08((void *)model->mModelRes, (void *)model->mAnimRes, TRUE);
            mParticles[emitted]->SetUnk10(mFollowParent);
            mBirthFrames[emitted] = mFrame;
            mPos.x = mPos.y = mPos.z = 0;
            if (mTracks[0].size() != 0) {
                mPos.x = GetValue(0, mFrame - mStartFrame);
            }
            if (mTracks[1].size() != 0) {
                mPos.y = GetValue(1, mFrame - mStartFrame);
            }
            if (mTracks[2].size() != 0) {
                mPos.z = GetValue(2, mFrame - mStartFrame);
            }
            if (mPosRef != NULL) {
                mPos.x += mPosRef->x;
                mPos.y += mPosRef->y;
                mPos.z += mPosRef->z;
            }
            mModel->SetTranslation(&mPos);
            VecFx32 v;
            v.x = v.y = v.z = 0;
            if (mTracks[3].size() != 0) {
                v.x = (u16)(GetValue(3, mFrame - mStartFrame) * 16 / 360);
            }
            if (mTracks[4].size() != 0) {
                v.y = (u16)(GetValue(4, mFrame - mStartFrame) * 16 / 360);
            }
            if (mTracks[5].size() != 0) {
                v.z = (u16)(GetValue(5, mFrame - mStartFrame) * 16 / 360);
            }
            mParticles[emitted]->SetRotation(&v);
            mModel->SetRotation(&v);
            v.x = v.y = v.z = FX32_ONE;
            mParticles[emitted]->SetScale(&v);
            mModel->SetScale(&v);
            mPos.x = mModel->GetMatrix()->_30;
            mPos.y = mModel->GetMatrix()->_31;
            mPos.z = mModel->GetMatrix()->_32;
            mParticles[emitted]->SetTranslation(&mPos);
            MtxFx44 *matrix = &mMatrices[emitted];
            MI_CpuCopy32(&mMatrix, matrix, sizeof(MtxFx44));
            mParticles[emitted]->mUnkC4 = matrix;
            if (mTracks[6].size() != 0) {
                mParticles[emitted]->SetUnk1C(GetValue(6, mFrame - mStartFrame));
            }
            mSpeeds[emitted] = 0x100;
            if (mTracks[8].size() != 0) {
                mSpeeds[emitted] = GetValue(8, mFrame - mStartFrame);
            }
            mParticles[emitted]->SetPolygonId(unk100 + emitted);
            mLoopEnd = mFrame + 16;
            if (mTracks[7].size() != 0) {
                mLoopEnd = mFrame + (GetValue(7, mFrame - mStartFrame) >> 8);
            }
            MAnmSetModelFrame(mParticles[emitted], 0);
        }
        for (int i = 0; i < 32; i++) {
            if (mParticles[i]->GetUnk10()) {
                mParticles[i]->SetUnk10(mFollowParent);
                if (emitted != i) {
                    fx32 speed = mSpeeds[i];
                    M3DModel *particle = mParticles[i];
                    particle->mTrans.x += particle->mMatrix._20 * speed >> FX32_SHIFT;
                    particle->mTrans.y += particle->mMatrix._21 * speed >> FX32_SHIFT;
                    particle->mTrans.z += particle->mMatrix._22 * speed >> FX32_SHIFT;
                    particle->mMatrixValid = FALSE;
                    particle = mParticles[i];
                    particle->mFrame += 16;
                    if (particle->IsAnimDone()) {
                        particle->mFrame = (((const u32 *)particle->mModelRes)[8] - 1) * 16;
                    }
                }
                if (mParticles[i]->IsAnimDone()) {
                    mParticles[i]->SetUnk10(mVisible);
                }
            }
        }
    }
}

// NONMATCHING: the game keeps the sine table's address in a register and works out both angles' indices first.
// Aims the scene's camera: orbits the target (tracks 3, 4: angles, 6: distance), smoothed (11), with the field of
// view (7) and a sway (8, 9: amounts, 10: period).
void MAnmTargetCamera::Unk0C() {
    if (mFrame < mStartFrame) {
        return;
    }
    int pitch = (u16)(GetValue(3, mFrame - mStartFrame) * 16 / 360);
    int yaw = (u16)(GetValue(4, mFrame - mStartFrame) * 16 / 360);
    fx32 distance = GetValue(6, mFrame - mStartFrame);
    mPos.x = mPos.y = mPos.z = 0;
    if (mTracks[0].size() != 0) {
        mPos.x = GetValue(0, mFrame - mStartFrame);
    }
    if (mTracks[1].size() != 0) {
        mPos.y = GetValue(1, mFrame - mStartFrame);
    }
    if (mTracks[2].size() != 0) {
        mPos.z = GetValue(2, mFrame - mStartFrame);
    }
    mPos.x += mTarget.x + mMatrix._30;
    mPos.y += mTarget.y + mMatrix._31;
    mPos.z += mTarget.z + mMatrix._32;
    int smooth = 0;
    if (mTracks[11].size() != 0) {
        smooth = GetValue(11, mFrame - mStartFrame);
    }
    smooth >>= 4;
    if (smooth > 0) {
        int x = (mPos.x * (0x100 - smooth) + mPrev.x * smooth) >> 8;
        int y = (mPos.y * (0x100 - smooth) + mPrev.y * smooth) >> 8;
        int z = (mPos.z * (0x100 - smooth) + mPrev.z * smooth) >> 8;
        if (x != mPrev.x) {
            mPos.x = x;
        }
        if (y != mPrev.y) {
            mPos.y = y;
        }
        if (z != mPrev.z) {
            mPos.z = z;
        }
    }
    mPrev.x = mPos.x;
    mPrev.y = mPos.y;
    mPrev.z = mPos.z;
    const fx16 *table = FX_SinCosTable_;
    int p = ((u16)pitch >> 4) * 2;
    int y = ((u16)yaw >> 4) * 2;
    VecFx32 v;
    v.x = distance * table[p + 1] >> FX32_SHIFT;
    v.y = distance * table[p] >> FX32_SHIFT;
    v.z = v.x;
    fx16 sinYaw = table[y];
    v.x = mPos.x + (sinYaw * v.z >> FX32_SHIFT);
    v.y += mPos.y;
    fx16 cosYaw = table[y + 1];
    v.z = mPos.z + (v.z * cosYaw >> FX32_SHIFT);
    mScene->SetCameraPos(&v);
    mScene->SetCameraTarget(&mPos);
    fx16 sin0 = table[0];
    v.x = FX_Mul(sin0, cosYaw);
    v.y = table[1];
    v.z = -FX_Mul(sin0, sinYaw);
    mScene->SetCameraUp(&v);
    if (mTracks[7].size() != 0) {
        int fov = GetValue(7, mFrame - mStartFrame) * 16 / 360;
        mScene->SetFov((u16)(fov / 2));
    }
    mPos.x = mPos.y = mPos.z = 0;
    if (mTracks[8].size() != 0) {
        mPos.x = GetValue(8, mFrame - mStartFrame);
    }
    if (mTracks[9].size() != 0) {
        mPos.y = GetValue(9, mFrame - mStartFrame);
    }
    int period;
    if (mTracks[10].size() != 0) {
        period = GetValue(10, mFrame - mStartFrame);
    }
    mPos.x >>= FX32_SHIFT;
    mPos.y >>= FX32_SHIFT;
    period >>= FX32_SHIFT;
    if (mPos.x == 0 && mPos.y == 0) {
        mSwayPhase = 0;
    } else if (period > 0) {
        int wave;
        switch ((mSwayPhase >> 10) & 3) {
        case 0:
            wave = mSwayPhase & 0x3FF;
            break;
        case 1:
            wave = 0x400 - (mSwayPhase & 0x3FF);
            break;
        case 2:
            wave = -(mSwayPhase & 0x3FF);
            break;
        case 3:
            wave = -(0x400 - (mSwayPhase & 0x3FF));
            break;
        }
        mScene->SetUnk164((mPos.x * wave >> 10 << 12) / 256, (mPos.y * wave >> 10 << 12) / 0xC0);
        mSwayPhase += 0x1000 / period;
    }
    PlaySounds(mFrame - mStartFrame);
}

// The shadow: plays the model's animation and follows the transform tracks, on the ground.
void MAnmModelSh::Unk0C() {
    if (!mLoaded) {
        return;
    }
    if (mFrame >= mStartFrame) {
        int frames = mModel->GetFrameCount() << 4;
        if (mUnkF4 == 0 && frames <= mFrame - mStartFrame) {
            MAnmSetModelFrame(mModel, mFrame - mStartFrame);
            mModel->SetUnk10(mVisible);
            if (!mVisible) {
                Suspend();
                return;
            }
            mFrame = mStartFrame + frames - 16;
        }
        if (frames > 0) {
            MAnmSetModelFrame(mModel, (u32)(mFrame - mStartFrame) % frames);
        }
        mModel->SetUnk10(mFollowParent && mUnk134);
        mModel->SetPolygonId(unk100);
        PlaySounds(mFrame - mStartFrame);
        mPos.x = mPos.y = mPos.z = 0;
        if (mTracks[0].size() != 0) {
            mPos.x = GetValue(0, mFrame - mStartFrame);
        }
        if (mTracks[1].size() != 0) {
            mPos.y = GetValue(1, mFrame - mStartFrame);
        }
        if (mTracks[2].size() != 0) {
            mPos.z = GetValue(2, mFrame - mStartFrame);
        }
        if (mPosRef != NULL) {
            mPos.x += mPosRef->x;
            mPos.y += mPosRef->y;
            mPos.z += mPosRef->z;
        }
        mPos.y = 0x51 - mMatrix._31;
        mModel->SetTranslation(&mPos);
        VecFx32 v;
        v.x = v.y = v.z = 0;
        if (mTracks[3].size() != 0) {
            v.x = (u16)(GetValue(3, mFrame - mStartFrame) * 16 / 360);
        }
        if (mTracks[4].size() != 0) {
            v.y = (u16)(GetValue(4, mFrame - mStartFrame) * 16 / 360);
        }
        if (mTracks[5].size() != 0) {
            v.z = (u16)(GetValue(5, mFrame - mStartFrame) * 16 / 360);
        }
        if (mRotRef != NULL) {
            v.x += mRotRef->x * 16 / 360;
            v.y += mRotRef->y * 16 / 360;
            v.z += mRotRef->z * 16 / 360;
            v.x = (u16)v.x;
            v.y = (u16)v.y;
            v.z = (u16)v.z;
        }
        mModel->SetRotation(&v);
        v.x = v.y = v.z = FX32_ONE;
        if (mTracks[6].size() != 0) {
            v.x = GetValue(6, mFrame - mStartFrame);
        }
        if (mTracks[7].size() != 0) {
            v.y = GetValue(7, mFrame - mStartFrame);
        }
        if (mTracks[8].size() != 0) {
            v.z = GetValue(8, mFrame - mStartFrame);
        }
        mModel->SetScale(&v);
    }
}

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
BOOL MAnmMakeKey(MAnmKey *key, int frame, const u8 *flags, const int *values) {
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

int MAnmStrCaseCmp(const char *s1, const char *s2) {
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

int MAnmLookup(const char *name, const MAnmName *table) {
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

#pragma opt_propagation off
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
#pragma opt_propagation reset

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

// Adds the target's joints' positions to the trail and lays the trail's quads along it, fading out to its end.
void MAnmRyusen::Unk0C() {
    MtxFx44 joint;
    MtxFx43 mtx;
    int length;
    fx32 red, green, blue, alpha;
    if (!mTarget->mModel->GetUnk10()) {
        Unk10();
        return;
    }
    if (mFrame >= mStartFrame) {
        PlaySounds(mFrame - mStartFrame);
        MTX_Copy44To43_(mTarget->mModel->GetMatrix(), &mtx);
        if (mJointA >= 0 && mTarget->mModel->GetJointMatrix(mJointA, &joint, 0)) {
            MTX_MultVec43((const VecFx32 *)&joint._30, &mtx, &mPointsA[mHead]);
        }
        if (mJointB >= 0 && mTarget->mModel->GetJointMatrix(mJointB, &joint, 0)) {
            MTX_MultVec43((const VecFx32 *)&joint._30, &mtx, &mPointsB[mHead]);
        }
        alpha = FX32_ONE;
        blue = FX32_ONE;
        green = FX32_ONE;
        red = FX32_ONE;
        if (mTracks[0].size() != 0) {
            red = GetValue(0, mFrame - mStartFrame);
        }
        if (mTracks[1].size() != 0) {
            green = GetValue(1, mFrame - mStartFrame);
        }
        if (mTracks[2].size() != 0) {
            blue = GetValue(2, mFrame - mStartFrame);
        }
        if (mTracks[3].size() != 0) {
            alpha = GetValue(3, mFrame - mStartFrame);
        }
        length = 60;
        if (mTracks[4].size() != 0) {
            length = GetValue(4, mFrame - mStartFrame) * 60 >> FX32_SHIFT;
        }
        mAge++;
        int i;
        for (i = 0; i < length; i++) {
            int back = (mHead + 179 - i) % 180;
            int front = (mHead + 180 - i) % 180;
            mTrails[i].SetQuad(&mPointsA[front], &mPointsB[front], &mPointsA[back], &mPointsB[back]);
            if (mAge > i) {
                mTrails[i].SetUnk10(TRUE);
            } else {
                mTrails[i].SetUnk10(FALSE);
            }
            mTrails[i].SetUnk2F4((u32)(blue * 31) >> FX32_SHIFT << 10 |
                                 ((u32)(red * 31) >> FX32_SHIFT | (u32)(green * 31) >> FX32_SHIFT << 5));
            mTrails[i].SetUnk1C((u32)((alpha * 31) * (length - i)) / length >> FX32_SHIFT);
        }
        for (; i < 180; i++) {
            mTrails[i].SetUnk10(FALSE);
        }
        mHead = (mHead + 1) % 180;
    } else {
        mAge = 0;
    }
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
template std::__vec_deleter<MAnmKey, std::allocator<MAnmKey> &>::~__vec_deleter();
// (The script reader makes arrays of key vectors and trail models; until it is decompiled, this keeps the
// constructors it uses out of line.)
#pragma dont_inline on
void MAnmMakeArrays() {
    delete[] new std::vector<MAnmKey>[1];
    delete[] new M3DRyusen[1];
}
#pragma dont_inline reset

template std::string::size_type std::string::find(const char *s, std::string::size_type pos,
                                                  std::string::size_type n) const;
// (Calling find here instantiates it with the name the game's callers use.)
#pragma dont_inline on
std::string::size_type MAnmModelFindAnchor(const std::string &s, const char *text) {
    return s.find(text, 0);
}
std::string::size_type MAnmModelFind3Anchor(const std::string &s, const char *text) {
    return s.find(text, 0, 0);
}
#pragma dont_inline reset

// (The reallocation helper's allocate goes ahead of its deleter's destructor, as in the game.)
template void std::__vec_deleter<MAnmKey, std::allocator<MAnmKey> &>::allocate(size_t n, Metrowerks::int2type<true>);
