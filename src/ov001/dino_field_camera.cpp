// DinoFieldCamera.

#include "game/dino_field.h"
#include "game/dino_field_area_panel.h"
#include "game/dino_field_camera_hook.h"
#include "m/anime.h"
#include "m/3d_scene.h"
#include <stdlib.h>

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
    DinoFieldCameraMoveAnime(DinoFieldCamera *camera, u16 fromX, u16 fromY, u16 fromZ, MFx32 fromScale, u16 toX, u16 toY,
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
        mFromScale = fromScale;
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

// The camera's follow settings (in the excavation defs).
struct DinoFieldCameraSettings {
    fx32 ease;      // 0x00: how much of its velocity the target keeps moving by each frame
    fx32 spring;    // 0x04
    fx32 reach;     // 0x08: how far (times the camera's tangent) the target may lag the focus
    fx32 damping;   // 0x0C: how much of the target's distance to the focus goes each frame
};

// d less the part damping takes, rounded away from zero (0 stays 0).
static inline fx32 DinoCameraDamp(fx32 d, fx32 damping) {
    if (d > 0) {
        return d - FX_Mul(d, damping) - 1;
    }
    if (d < 0) {
        return d - FX_Mul(d, damping) + 1;
    }
    return 0;
}

// The camera's place: distance from the target (at least 1) at the pitch (kept off straight down and up) and yaw.
static inline void DinoCameraOrbit(VecFx32 *pos, const VecFx32 &target, u16 yaw, u16 pitch, fx32 distance) {
    if (distance < FX32_ONE) {
        distance = FX32_ONE;
    }
    if (pitch <= 0xC0FF && pitch > 0x8000) {
        pitch = 0xC0FF;
    } else if (pitch >= 0x3F00 && pitch < 0x8000) {
        pitch = 0x3F00;
    }
    fx32 h = (FX_CosIdx(pitch) * distance) >> FX32_SHIFT;
    fx32 y = (FX_SinIdx(pitch) * distance) >> FX32_SHIFT;
    pos->x = h;
    pos->y = y;
    pos->z = h;
    pos->x = target.x + ((FX_SinIdx(yaw) * h) >> FX32_SHIFT);
    pos->y = y + target.y;
    pos->z = target.z + ((FX_CosIdx(yaw) * h) >> FX32_SHIFT);
}

// Moves the camera: the focus height follows the ground; the target follows the focus (dragged along within reach while
// tracking, else easing in), and the camera sits at the target's distance and angles from it.
// NONMATCHING: 90%, the game truncates the yaw and pitch to u16 twice more around the orbit (more inline levels?), and
// the drag's length and angle temporaries are scheduled differently.
void DinoFieldCamera::UnkV14() {
    const DinoFieldCameraSettings *settings = (const DinoFieldCameraSettings *)gDinoResource->GetExcavate78();
    if ((!mField->IsUnk258() && mTracking) || !mHeightSet) {
        fx32 height = mField->GetGroundHeight(mUnk94, mUnk9C);
        if (height != (fx32)0x80000000) {
            height += mUnkD0;
            fx32 eased = ((s32)mUnk98 * 30 + height * 2) >> 5;
            if ((s32)mUnk98 != eased) {
                mUnk98 = eased;
            } else {
                mUnk98 = height;
            }
        }
    }
    mUnk148 = mUnkDC * MFx32::FromRaw(FX_SinIdx(mUnkD4)) / MFx32::FromRaw(FX_CosIdx(mUnkD4));
    MFx32 reach = mUnk148 * MFx32::FromRaw(settings->reach);
    if (!mField->IsUnk258() && mTracking) {
        mHeightSet = FALSE;
        MFx32Point d(MFx32::FromRaw(mUnk94 - mUnkAC.x), MFx32::FromRaw(mUnk9C - mUnkAC.z));
        MFx32 length = d.Length();
        if (length > reach) {
            u16 angle = FX_Atan2Idx(d.y.v, d.x.v);
            MFx32 over = length - reach;
            mUnkAC.x += (MFx32::FromRaw(FX_CosIdx(angle)) * over).v;
            mUnkAC.z += (MFx32::FromRaw(FX_SinIdx(angle)) * over).v;
        }
        mUnkAC.y = mUnk98;
    } else if (mHeightSet) {
        mUnkAC.x = mUnk94;
        mUnkAC.y = mUnk98;
        mUnkAC.z = mUnk9C;
    } else {
        mUnkAC.x += FX_Mul(mUnkC4, settings->ease);
        mUnkAC.y += FX_Mul(mUnkC8, settings->ease);
        mUnkAC.z += FX_Mul(mUnkCC, settings->ease);
        mUnkC4 = mUnkC8 = mUnkCC = 0;
        mUnkAC.x = mUnk94 + DinoCameraDamp(mUnkAC.x - mUnk94, settings->damping);
        mUnkAC.y = mUnk98 + DinoCameraDamp(mUnkAC.y - mUnk98, settings->damping);
        mUnkAC.z = mUnk9C + DinoCameraDamp(mUnkAC.z - mUnk9C, settings->damping);
        if (labs(mUnkAC.x - mUnk94) < 40 && labs(mUnkAC.y - mUnk98) < 40 && labs(mUnkAC.z - mUnk9C) < 40) {
            mHeightSet = TRUE;
        }
    }
    if (!mField->IsUnk258() && mTracking) {
        fx32 dx = mUnkAC.x - mUnkA0.x;
        fx32 dy = mUnkAC.y - mUnkA0.y;
        fx32 dz = mUnkAC.z - mUnkA0.z;
        mUnkA0.x = mUnkAC.x;
        mUnkA0.y = mUnkAC.y;
        mUnkA0.z = mUnkAC.z;
        fx32 k = (MFx32::FromRaw(settings->spring) / reach).v;
        fx32 vx = DinoCameraDamp(mUnkC4 + dx, k);
        fx32 vy = DinoCameraDamp(mUnkC8 + dy, k);
        fx32 vz = DinoCameraDamp(mUnkCC + dz, k);
        mUnkAC.x += FX_Mul(mUnkC4, settings->ease) - FX_Mul(vx, settings->ease);
        mUnkAC.y += FX_Mul(mUnkC8, settings->ease) - FX_Mul(vy, settings->ease);
        mUnkAC.z += FX_Mul(mUnkCC, settings->ease) - FX_Mul(vz, settings->ease);
        mUnkC4 = vx;
        mUnkC8 = vy;
        mUnkCC = vz;
    } else {
        mUnkA0.x = mUnkAC.x;
        mUnkA0.y = mUnkAC.y;
        mUnkA0.z = mUnkAC.z;
    }
    mUnkB8.x = mUnkAC.x + FX_Mul(mUnkC4, settings->ease);
    mUnkB8.y = mUnkAC.y + FX_Mul(mUnkC8, settings->ease);
    mUnkB8.z = mUnkAC.z + FX_Mul(mUnkCC, settings->ease);
    ((M3DScene *)mUnk90)->SetCameraTarget(&mUnkB8);
    VecFx32 pos;
    DinoCameraOrbit(&pos, mUnkB8, mUnkD8, mUnkD6, mUnkDC.v);
    ((M3DScene *)mUnk90)->SetCameraPos(&pos);
    ((M3DScene *)mUnk90)->SetFov(mUnkD4);
}

// The point in fixed point.
static inline MFx32Point ToFx32Point(MPoint p) {
    return MFx32Point(MFx32(p.x), MFx32(p.y));
}

// The camera setting a circle of the map has, if any.
static inline DinoFieldMapCamera *GetCircleCamera(DinoFieldMapCircle *circle) {
    if (circle->unk00 == 0) {
        return NULL;
    }
    return (DinoFieldMapCamera *) ((u8 *) circle + circle->unk00);
}

// Without tracking: the map's default camera setting, or the setting of the circle the focus is in, blended out to the
// default between the circle's inner and outer radius.
// NONMATCHING: the game's frame has 32 more bytes of temporaries, and reads the first distance after the radius.
void DinoFieldCamera::IdleUnk0() {
    DinoFieldMapCamera *first = mField->GetMap()->GetCamera(0);
    DinoFieldMapCamera def;
    def.x = first->x;
    def.y = first->y;
    def.z = first->z;
    def.scale = first->scale;
    MFx32 outer;
    MFx32 inner;
    MFx32Point center;
    DinoFieldMapCamera camera;
    MFx32 len;
    if (mUnkFC != 0) {
        DinoFieldMapCircle *circle = mField->GetMap()->GetCircle(mUnkFC);
        MFx32Point c = ToFx32Point(MPoint(circle->x, circle->y));
        MFx32 d = (mFocusPos - c).Length();
        if (d.v > MFx32(circle->radius).v) {
            mUnkFC = mField->GetMap()->FindCircle(mFocusPos);
        }
    } else {
        mUnkFC = mField->GetMap()->FindCircle(mFocusPos);
    }
    if (mUnkFC == 0) {
        mUnk100 = def.x;
        mUnk102 = def.y;
        mUnk104 = def.z;
        mUnk108 = def.scale;
        return;
    }
    DinoFieldMapCircle *circle = mField->GetMap()->GetCircle(mUnkFC);
    camera.x = GetCircleCamera(circle)->x;
    camera.y = GetCircleCamera(circle)->y;
    camera.z = GetCircleCamera(circle)->z;
    camera.scale = GetCircleCamera(circle)->scale;
    center = ToFx32Point(MPoint(circle->x, circle->y));
    inner = MFx32((int) circle->unk0C);
    outer = MFx32(circle->radius);
    len = (mFocusPos - center).Length();
    if (len <= inner) {
        mUnk100 = camera.x;
        mUnk102 = camera.y;
        mUnk104 = camera.z;
        mUnk108 = camera.scale;
        return;
    }
    if (len > outer) {
        mUnk100 = def.x;
        mUnk102 = def.y;
        mUnk104 = def.z;
        mUnk108 = def.scale;
        return;
    }
    MFx32 s = MFx32(1) - (len - inner) / (outer - inner);
    MFx32 t = MFx32(1) - s * s;
    mUnk100 = LerpAngle(def.x, camera.x, t);
    mUnk102 = LerpAngle(def.y, camera.y, t);
    mUnk104 = LerpAngle(def.z, camera.z, t);
    mUnk108 = camera.scale * (MFx32(1) - t) + def.scale * t;
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
    MFx32Point &pos = model->pos;
    mFocusPos.x = pos.x;
    mFocusPos.y = pos.y;
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

void DinoFieldCamera::StartRotate(u16 x, u16 y, u16 z, u32 scale, u32 duration, u32 ease) {
    mUnkE8->Clear();
    mUnkE8->AddTail(new DinoFieldCameraMoveAnime(this, mUnkD4, mUnkD6, mUnkD8, mUnkDC, x, y, z, scale, duration, ease));
}
