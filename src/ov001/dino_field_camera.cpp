// DinoFieldCamera.

#include "game/dino_field.h"
#include "game/dino_field_camera_hook.h"

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

void DinoFieldCamera::SetUnkD4(u16 a, u16 b, u16 c, u32 d) {
    mUnkD4 = a;
    mUnkD8 = c;
    mUnkD6 = b;
    mUnkDC = d;
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
