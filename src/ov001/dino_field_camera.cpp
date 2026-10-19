// DinoFieldCamera.

#include "game/dino_field.h"
#include "game/dino_field_area_panel.h"
#include "game/dino_field_camera_hook.h"
#include "m/anime.h"

// The angle t of the way back from `to` to `from`, turning the short way round.
static u16 LerpAngle(u16 from, u16 to, MFx32 t) {
    int d = to - from;
    int target;
    if (d < 0x8000) {
        target = from + d;
    } else {
        target = from - (0x10000 - d);
    }
    return MFx32ToInt(MFx32(from) * t + MFx32(target) * (MFx32(1) - t));
}

DinoFieldCamera::DinoFieldCamera(DinoField *field, u32 namePanel)
    : MProcess(NULL), mField(field), mUnk90(namePanel), mFocus(0), mTracking(1), mUnk140(0), mHeightSet(0),
      mHooksMuted(0) {
    mUnkD6 = mUnkD8 = 0;
    mUnkDC.v = 0x4A000;
    mUnkD4 = 0x3FF;
    mUnkD6 = 0x1B53;
    mUnkE0 = 0;
    mUnkFC = 0;
    mUnk98 = FX32_ONE;
    mUnkD0 = FX32_ONE;
    MI_CpuFill8(&mUnkA0, 0, sizeof(mUnkA0));
    MI_CpuFill8(&mUnkC4, 0, 12);
    mSavedSteps = 0;
    mUnkE8 = new MParallelTaskSet;
    mParallel.AddTail(mUnkE8);
    Add((s32 *)&mFocus, 1);
    Add(&mTracking, 1);
    Add((u8 *)&mUnkD4, 12);
    Add((u8 *)&mUnk94, 12);
    Add((u8 *)&mUnkAC, 12);
    Add((u8 *)&mUnkA0, 12);
    Add((u8 *)&mUnkB8, 12);
    Add((u8 *)&mUnkC4, 12);
    Add((u8 *)&mUnkD0, 4);
    Add((u8 *)&mFocusPos, 8);
    Add((u8 *)&mUnk114, 8);
    Add((s32 *)&mUnkFC, 1);
    Add((u8 *)&mSavedD4, 12);
    Add((u8 *)mSavedRotate, 12);
    Add((s32 *)&mSavedRotateArgs[1], 1);
    Add(&mSavedRotateArgs[2], 1);
    Add(&mSavedSteps, 1);
    Close();
    if (mUnk90 != 0) {
        mNamePanel = new DinoFieldAreaPanel;
        mParallel.AddTail(mNamePanel);
    }
}

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
    mUnkDC = d;
}

MFx32 DinoFieldCamera::GetUnk148() {
    return mUnk148;
}

// After a map change: puts the camera on the ground under the focus, and sets the map's rotation and scale for where
// the focus stands (or the camera's own when it doesn't track).
void DinoFieldCamera::IdleUnk1() {
    fx32 height = mField->GetGroundHeight(mUnk94, mUnk9C);
    if (height != (fx32) 0x80000000) {
        mUnk98 = height + mUnkD0;
    }
    mUnkB8.x = mUnk94;
    mUnkB8.y = mUnk98;
    mUnkB8.z = mUnk9C;
    {
        const VecFx32 &src = mUnkB8;
        mUnkA0.x = src.x;
        mUnkA0.y = src.y;
        mUnkA0.z = src.z;
    }
    {
        const VecFx32 &src = mUnkA0;
        mUnkAC.x = src.x;
        mUnkAC.y = src.y;
        mUnkAC.z = src.z;
    }
    mUnkC4 = mUnkC8 = mUnkCC = 0;
    if (mTracking) {
        mUnkE8->Clear();
        mUnkEC = mField->mMap->GetCameraIndex(mFocusPos);
        if (mUnkEC != 0) {
            DinoFieldMapCamera *camera = mField->GetMap()->GetCamera(mUnkEC);
            MFx32 scale = camera->scale;
            SetUnkD4(camera->x, camera->y, camera->z, scale);
        } else {
            SetUnkD4(mUnk100, mUnk102, mUnk104, mUnk108);
        }
    } else {
        mUnkE8->Clear();
        SetUnkD4(mUnkD4, mUnkD6, mUnkD8, mUnkDC);
    }
    mUnk114 = mFocusPos;
}

// The excavation settings' block at offset78 (only the camera's part is known).
struct DinoExcavate78 {
    u8 unk00[0x10];
    MFx32 cameraEase;   // 0x10: how much of the camera's setting stays each frame as it eases to the default
};

// (The game passes the value through a copy.)
static inline u32 MFx32ToRaw(MFx32 x) {
    return x.v;
}

static inline const DinoExcavate78 *GetExcavate78(const DinoExcavateDefs *defs) {
    if (defs->offset78 == 0) {
        return NULL;
    }
    return (const DinoExcavate78 *)((const u8 *)defs + defs->offset78);
}

// Turns to the map's camera setting where the focus stands when it changes; while nothing turns, eases toward the
// default setting as the focus moves.
// NONMATCHING: stack slots only (the game puts the setting's scale above the easing's temporaries, and the position
// test's below them).
void DinoFieldCamera::IdleUnk2() {
    if (mTracking) {
        s32 old = mUnkEC;
        mUnkEC = mField->GetMap()->GetCameraIndex(mFocusPos);
        s32 index = mUnkEC;
        if (old != index) {
            if (index != 0) {
                DinoFieldMapCamera *camera = mField->GetMap()->GetCamera(index);
                StartRotate(camera->x, camera->y, camera->z, MFx32ToRaw(camera->scale), 60, 12);
            } else {
                StartRotate(mUnk100, mUnk102, mUnk104, mUnk108.v, 60, 12);
            }
        }
        if (mUnkE8->IsEmpty() && mUnkEC == 0 && !(mUnk114 == mFocusPos)) {
            const MFx32 t = GetExcavate78(gDinoResource->GetExcavateDefs().Get())->cameraEase;
            DinoFieldMapCamera camera;
            camera.x = LerpAngle(mUnkD4, mUnk100, t);
            camera.y = LerpAngle(mUnkD6, mUnk102, t);
            camera.z = LerpAngle(mUnkD8, mUnk104, t);
            camera.scale = mUnkDC * t + mUnk108 * (MFx32(1) - t);
            SetUnkD4(camera.x, camera.y, camera.z, camera.scale);
        }
    }
    mUnk114 = mFocusPos;
}

void DinoFieldCamera::Read(const void *buf) {
    MSerializer::Read(buf);
    if (mSavedSteps != 0) {
        mUnkD4 = mSavedD4;
        mUnkD6 = mSavedD6;
        mUnkD8 = mSavedD8;
        mUnkDC.v = mSavedDC;
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
    mUnkE8->AddTail(new DinoFieldCameraMoveAnime(this, mUnkD4, mUnkD6, mUnkD8, mUnkDC.v, x, y, z, scale, duration, ease));
}
