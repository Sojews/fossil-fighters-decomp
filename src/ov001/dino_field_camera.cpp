// DinoFieldCamera.

#include "game/dino_field.h"
#include "game/dino_field_camera_hook.h"
#include "m/anime.h"

// Turns the camera from one rotation and scale to another.
class DinoFieldCameraMoveAnime : public MAnime {
public:
    // Each angle turns the short way round.
    DinoFieldCameraMoveAnime(DinoFieldCamera *camera, u16 fromX, u16 fromY, u16 fromZ, u32 fromScale, u16 toX, u16 toY,
                             u16 toZ, u32 toScale, u32 duration, int ease) {
        mCamera = camera;
        mSteps = 0x100;
        SetDuration(duration);
        mEase = ease;
        mFromX = fromX;
        u16 d = toX - fromX;
        if (d < 0x8000) {
            mToX = mFromX + d;
        } else {
            mToX = mFromX - (0x10000 - d);
        }
        mFromY = fromY;
        d = toY - fromY;
        if (d < 0x8000) {
            mToY = mFromY + d;
        } else {
            mToY = mFromY - (0x10000 - d);
        }
        mFromZ = fromZ;
        d = toZ - fromZ;
        if (d < 0x8000) {
            mToZ = mFromZ + d;
        } else {
            mToZ = mFromZ - (0x10000 - d);
        }
        mFromScale.v = fromScale;
        mToScale.v = toScale;
    }
    virtual ~DinoFieldCameraMoveAnime();
    virtual void Apply(int step);

    DinoFieldCamera *mCamera;   // 0x30
    u32 mFromX;                 // 0x34
    u32 mToX;                   // 0x38
    u32 mFromY;                 // 0x3C
    u32 mToY;                   // 0x40
    u32 mFromZ;                 // 0x44
    u32 mToZ;                   // 0x48
    MFx32 mFromScale;           // 0x4C
    MFx32 mToScale;             // 0x50
};

DinoFieldCamera::~DinoFieldCamera() {
}

void DinoFieldCamera::SetFocus(u32 chara) {
    mFocus = chara;
}

void DinoFieldCamera::SetTracking(u8 tracking) {
    mTracking = tracking;
    if (mTracking) {
        mUnkEC = -1;
    }
}

void DinoFieldCamera::ResetHeight() {
    mHeightSet = FALSE;
}

#pragma auto_inline off

u32 DinoFieldCamera::GetFocus() {
    return mFocus;
}

u16 DinoFieldCamera::GetUnkD8() {
    return mUnkD8;
}

void DinoFieldCamera::AddHook(DinoFieldCameraHook *hook) {
    mHooks.push_back(hook);
}

void DinoFieldCamera::SetUnk15C(u8 value) {
    mHooksMuted = value;
}

void DinoFieldCamera::NotifyHooks0() {
    if (!mHooksMuted) {
        u32 map = mField->mMap->GetInfo()->id;
        for (std::vector<DinoFieldCameraHook *>::iterator it = mHooks.begin(); it != mHooks.end(); it++) {
            (*it)->CameraUnk0(map);
        }
    }
}

void DinoFieldCamera::NotifyHooks1() {
    if (!mHooksMuted) {
        u32 map = mField->mMap->GetInfo()->id;
        for (std::vector<DinoFieldCameraHook *>::iterator it = mHooks.begin(); it != mHooks.end(); it++) {
            (*it)->CameraUnk1(map);
        }
    }
}

void DinoFieldCamera::SetUnk94(u32 a, u32 b, u32 c) {
    mUnk94 = a;
    mUnkD0 = b;
    mUnk9C = c;
}

void DinoFieldCamera::SetUnkD4(u16 a, u16 b, u16 c, MFx32 d) {
    mUnkD4 = a;
    mUnkD8 = c;
    mUnkD6 = b;
    mUnkDC = d.v;
}

MFx32 DinoFieldCamera::GetUnk148() {
    return mUnk148;
}

void DinoFieldCamera::Read(const void *buf) {
    MSerializer::Read(buf);
    if (mSavedSteps != 0) {
        mUnkD4 = mSavedD4;
        mUnkD6 = mSavedD6;
        mUnkD8 = mSavedD8;
        mUnkDC = mSavedDC;
        StartRotate(mSavedRotate[0], mSavedRotate[1], mSavedRotate[2], mSavedRotateArgs[0], mSavedRotateArgs[1],
                    mSavedRotateArgs[2]);
        for (s32 i = mSavedSteps; i >= 0; i--) {
            if (mUnkE8->mState == MTask::STATE_RUN) {
                mUnkE8->Execute();
            }
        }
    }
    NotifyHooks1();
}

void DinoFieldCamera::Idle() {
    if (mFocus == 0) {
        return;
    }
    DinoCharaState state;
    gDinoBackup->GetChara(mFocus, &state);
    if (!state.unk0) {
        return;
    }
    BOOL changed = state.map != mField->mMap->GetInfo()->id;
    if (changed) {
        NotifyHooks0();
        mField->mMap->GetInfo()->id = state.map;
        mField->ReloadMap();
        NotifyHooks1();
        MResource<DinoMapMData> map = gDinoResource->LoadMapM(state.map);
        s32 area = map.Get()->area;
        gDinoBackup->SetUnk180(area);
        mNamePanel->SetMode(area, 1);
    }
    DinoFieldChara *chara = mField->GetChara(mFocus);
    if (chara == NULL) {
        return;
    }
    DinoFieldCharaModel *model = chara->GetModel();
    mFocusPos.Set(model->pos.x, model->pos.y);
    SetUnk94(mFocusPos.x.v, 0x1000, mFocusPos.y.v);
    if (changed) {
        mUnkFC = 0;
    }
    IdleUnk0();
    if (changed) {
        IdleUnk1();
    } else {
        IdleUnk2();
    }
    UnkV14();
}

void DinoFieldCamera::SaveRotation(u16 fromX, u16 fromY, u16 fromZ, MFx32 fromScale, u16 toX, u16 toY, u16 toZ,
                                   MFx32 toScale, u32 duration, int ease, u32 frame) {
    mSavedD4 = fromX;
    mSavedD6 = fromY;
    mSavedD8 = fromZ;
    mSavedDC = fromScale.v;
    mSavedRotate[0] = toX;
    mSavedRotate[1] = toY;
    mSavedRotate[2] = toZ;
    mSavedRotateArgs[0] = toScale.v;
    mSavedRotateArgs[1] = duration;
    mSavedRotateArgs[2] = ease;
    mSavedSteps = frame;
}

DinoFieldCameraMoveAnime::~DinoFieldCameraMoveAnime() {
    mCamera->SaveRotation(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

void DinoFieldCameraMoveAnime::Apply(int step) {
    int n = mSteps - 1;
    int rest = n - step;
    u16 x = (mFromX * rest + mToX * step) / n;
    u16 y = (mFromY * rest + mToY * step) / n;
    u16 z = (mFromZ * rest + mToZ * step) / n;
    MFx32 scale = (mFromScale * MFx32(rest) + mToScale * MFx32(step)) / MFx32(n);
    mCamera->SetUnkD4(x, y, z, scale);
    mCamera->SaveRotation(mFromX, mFromY, mFromZ, mFromScale, mToX, mToY, mToZ, mToScale, mDuration, mEase,
                          mFrame);
}

// NONMATCHING: register allocation (the game keeps x in a register and spills the scale).
void DinoFieldCamera::StartRotate(u16 x, u16 y, u16 z, u32 scale, u32 duration, u32 ease) {
    mUnkE8->Clear();
    mUnkE8->AddTail(new DinoFieldCameraMoveAnime(this, mUnkD4, mUnkD6, mUnkD8, mUnkDC, x, y, z, scale, duration, ease));
}
