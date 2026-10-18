// DinoFieldCamera.
// Only the destructor is decompiled so far.

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
