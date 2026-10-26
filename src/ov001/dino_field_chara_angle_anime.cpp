// DinoFieldCharaAngleAnime, DinoFieldCharaWaitStopTask, DinoFieldCharaAngleToTask, DinoFieldCharaMoveToTask, DinoFieldCharaShowManpuTask, DinoFieldCharaHideManpuTask, DinoFieldCharaModelMotionIdTask.
// Also DinoFieldChara's members here: models, motions, movement.

#pragma ipa file
#include "game/dino_field.h"
#include "game/dino_sound.h"
#include "m/file.h"
#include "nitro/os.h"
#include "nitro/mi.h"
#include "m/anm_model.h"
#include "m/3d_model.h"
#include "m/polygon_char.h"
#include "game/dino_field_chara_angle_anime.h"
#include "game/dino_field_chara_coord_anime.h"
#include "game/dino_field_chara_wait_stop_task.h"
#include "game/dino_field_chara_angle_to_task.h"
#include "game/dino_field_chara_move_to_task.h"
#include "game/dino_field_chara_show_manpu_task.h"
#include "game/dino_field_chara_hide_manpu_task.h"
#include "game/dino_field_chara_model_motion_id_task.h"

DinoFieldChara::DinoFieldChara(DinoField *field, M3DScene *scene)
    : MProcess(NULL), mUnk114(0), mField(field), mScene(scene) {
    mUnk310 = 0;
    mBody = NULL;
    mUnk128 = NULL;
    mHead = NULL;
    mManpu = NULL;
    mEffect = NULL;
    mUnk2D8 = MFx32Point(MFx32(0), MFx32(0));
    mUnk2E0 = 0;
    ResetState();
    Add((u8 *) &mModel, 0x2C);
    Add(&mUnk114, 1);
    Close();
    mAngleTasks = new MSerialTaskSet;
    GetParallel()->AddTail(mAngleTasks);
    mMoveTasks = new MSerialTaskSet;
    GetParallel()->AddTail(mMoveTasks);
    mTaskSet = new MSerialTaskSet;
    GetParallel()->AddTail(mTaskSet);
}

DinoFieldCharaAngleAnime::~DinoFieldCharaAngleAnime() {
}

DinoFieldCharaWaitStopTask::~DinoFieldCharaWaitStopTask() {
}

void DinoFieldCharaAngleAnime::Apply(int step) {
    int last = mSteps - 1;
    mChara->SetDirection((mFrom * (last - step) + mTo * step) / last);
}

DinoFieldCharaAngleToTask::~DinoFieldCharaAngleToTask() {
}

DinoFieldCharaMoveToTask::~DinoFieldCharaMoveToTask() {
}

DinoFieldCharaShowManpuTask::~DinoFieldCharaShowManpuTask() {
}

DinoFieldCharaHideManpuTask::~DinoFieldCharaHideManpuTask() {
}

DinoFieldCharaModelMotionIdTask::~DinoFieldCharaModelMotionIdTask() {
}

DinoFieldCharaCoordAnime::~DinoFieldCharaCoordAnime() {
    if (!mUnk4C) {
        mChara->SetMotion(1, 1, MFx32(1));
    }
}

DinoFieldChara::~DinoFieldChara() {
    mParallel.Clear();
}

DinoFieldCharaModel *DinoFieldChara::GetModel() {
    return &mModel;
}

// The chara file's header.
struct DinoFieldCharaData {
    u32 unk00;
    u32 mBodyModelCount;    // 0x04
    u32 mBodyModelNames;    // 0x08: offset of the offsets of their names
    u32 mHeadModelCount;    // 0x0C
    u32 mHeadModelNames;    // 0x10
    MFx32 mMoveSpeed;   // 0x14: -1 = the default
    MFx32 mHeight;      // 0x18
    MFx32 mNearRange;   // 0x1C: how close the camera's focus counts as near
    u32 mUnk20;         // 0x20
    u32 mUnk24;         // 0x24
    u8 unk28[0x2C - 0x28];
    u8 mUnk2C;          // 0x2C
    u8 mUnk2D;          // 0x2D
    u8 unk2E;
    u8 mUnk2F;          // 0x2F
    MFx32Rect mRect30;  // 0x30: the collision box, around the position
    MFx32Rect mRect40;  // 0x40
    MFx32 mUnk50;       // 0x50: -10 = the default
    u32 mWalkStepCount; // 0x54: the frames of the walk motion with a footstep
    u32 mWalkSteps;     // 0x58: offset of them (u16s)
    u32 mRunStepCount;  // 0x5C: the same for the run motion
    u32 mRunSteps;      // 0x60
};

// Defaults in main's data.
extern "C" const MFx32 data_020b5ab8;   // move speed
extern "C" const MFx32 data_020b5abc;   // mUnk50
extern "C" const MFx32 data_020b5ac8;   // move speed while following
extern const char *data_ov001_0216a20c[];   // the file's constants; [1] is the motion model path format

void DinoFieldChara::ResetState() {
    MI_CpuFill8(&mModel, 0, 0x2C);
    mMotion = 1;
    mUnk114 = 0;
    MI_CpuFill8(&mTask, 0, sizeof(DinoCharaTask));
}

// Forgets the models and tasks, then loads the character's data and models again (or stops, with no character).
void DinoFieldChara::Refresh() {
    mMoveTasks->Clear();
    mTaskSet->Clear();
    mBody = NULL;
    mUnk128 = NULL;
    mHead = NULL;
    mManpu = NULL;
    mEffect = NULL;
    mHasFootSe = FALSE;
    mMoveSe = mMoveSe2 = 0;
    mMotionModels.clear();
    mMotionModelsAlt.clear();
    mModelResource = M3DModelResource();
    mData = MResource<DinoCharaData>();
    mUnk2D8 = MFx32Point(MFx32(0), MFx32(0));
    mUnk2E0 = 0;
    mBeholdTarget = 0;
    mBodyPos.x = mBodyPos.y = mBodyPos.z = 0x80000000;
    if (mModel.id != 0) {
        Run();
        mData = gDinoResource->LoadChara(mModel.id);
        mModel.unk05[1] = ((DinoFieldCharaData *) mData.mFile.GetData())->mUnk2F;
        ApplyBodyModel();
        ApplyHeadModel();
        ApplyMotion();
    } else {
        Suspend();
    }
}

MFx32 DinoFieldChara::GetMoveSpeed() {
    if (mTask.unk4 == 1) {
        return data_020b5ac8;
    }
    MFx32 speed = ((DinoFieldCharaData *) mData.mFile.GetData())->mMoveSpeed;
    if (!(speed != MFx32(-1))) {
        return data_020b5ab8;
    }
    return speed;
}

u8 DinoFieldChara::GetUnk2C() {
    return ((DinoFieldCharaData *) mData.mFile.GetData())->mUnk2C;
}

MFx32Rect DinoFieldChara::GetRect30() {
    const DinoFieldCharaData *data = (const DinoFieldCharaData *) mData.mFile.GetData();
    return data->mRect30;
}

u8 DinoFieldChara::GetUnk2D() {
    return ((DinoFieldCharaData *) mData.mFile.GetData())->mUnk2D;
}

MFx32Rect DinoFieldChara::GetRect40() {
    const DinoFieldCharaData *data = (const DinoFieldCharaData *) mData.mFile.GetData();
    return data->mRect40;
}

MFx32 DinoFieldChara::GetUnk50() {
    MFx32 v = ((DinoFieldCharaData *) mData.mFile.GetData())->mUnk50;
    if (!(v != MFx32(-10))) {
        return data_020b5abc;
    }
    return v;
}

void DinoFieldChara::SetUnk2D8(MFx32Point pos, u32 a, MFx32 b) {
    mUnk2D8 = pos;
    mUnk2E0 = a;
    mUnk2E4 = b;
}

void DinoFieldChara::ClearUnk2E0() {
    mUnk2E0 = 0;
}

void DinoFieldChara::StopWalking() {
    if (!mModel.unk05[0]) {
        SetMotion(1, 1, mField->GetMap()->GetWalkSpeed());
    }
    mUnk2D8 = MFx32Point(MFx32(0), MFx32(0));
    mUnk2E0 = 0;
}

BOOL DinoFieldChara::IsUnk20Two() {
    return ((DinoFieldCharaData *) mData.mFile.GetData())->mUnk20 == 2 ? TRUE : FALSE;
}

BOOL DinoFieldChara::IsUnkA1Motion() {
    return mModel.unk05[0] && (mMotion == 2 || mMotion == 3);
}

// Where the chara is on the screen.
BOOL DinoFieldCharaGetScreenPos(MPoint *pos, DinoFieldChara *chara) {
    return chara->mBody->mModel->GetJointScreenPos(-1, pos);
}

// One of the shared motions (1-based), or none.
void DinoFieldChara::SetCommonMotion(u32 motion) {
    if (motion == 0) {
        mBody->ClearTransformTracks();
        return;
    }
    char path[64];
    OS_SPrintf(path, data_ov001_0216a20c[3], motion - 1);
    MAnmLoadTracks(path, mBody);
    mBody->SetFrame(0);
}

// When the field's unk258 mode ends (StopWalking when it starts).
void DinoFieldChara::LeaveUnk258() {
    HideManpu();
    mMotionModelsAlt.clear();
    if (mEffect != NULL) {
        mEffect->Reset();
    }
    mBeholdTarget = 0;
}

static inline BOOL IsAnimDone(MAnmModelBase *model) {
    if (model->mModel != NULL) {
        return model->mModel->IsAnimDone();
    }
    return FALSE;
}

// Once the motion is done: motion 4 (motion 3 outside unk A1), with a sound by the character's kind.
void DinoFieldChara::PlayReaction() {
    if (mBody != NULL && IsAnimDone(mBody)) {
        if (IsUnkA1Motion()) {
            SetMotion(4, 0, MFx32(1));
            if (((DinoFieldCharaData *) mData.mFile.GetData())->mUnk20 == 2) {
                DinoSoundPlaySe(0xBF, 0);
            } else if (((DinoFieldCharaData *) mData.mFile.GetData())->mUnk24 == 2) {
                DinoSoundPlaySe(0xC1, 0);
            } else {
                DinoSoundPlaySe(0xBD, 0);
            }
        } else {
            SetMotion(3, 0, MFx32(1));
            if (((DinoFieldCharaData *) mData.mFile.GetData())->mUnk20 == 2) {
                DinoSoundPlaySe(0xBE, 0);
            } else if (((DinoFieldCharaData *) mData.mFile.GetData())->mUnk24 == 2) {
                DinoSoundPlaySe(0xC0, 0);
            } else {
                DinoSoundPlaySe(0xBB, 0);
            }
        }
    }
}

static inline BOOL IsUnk20One(DinoFieldChara *chara) {
    return chara->mBody != NULL && ((DinoFieldCharaData *) chara->mData.mFile.GetData())->mUnk20 == 1;
}

static inline BOOL IsUnk24Kind(DinoFieldChara *chara) {
    return IsUnk20One(chara) && (((DinoFieldCharaData *) chara->mData.mFile.GetData())->mUnk24 == 1 ||
                                 ((DinoFieldCharaData *) chara->mData.mFile.GetData())->mUnk24 == 2);
}

BOOL DinoFieldChara::IsUnk24Reacting() {
    return IsUnk24Kind(this) && IsUnkA1Motion();
}

// Whether the camera's focus stands within the chara's range (for a chara of kind 2, in an A1 motion).
BOOL DinoFieldChara::IsFocusNear() {
    if (mBody != NULL && ((DinoFieldCharaData *) mData.mFile.GetData())->mUnk20 == 2 && IsUnkA1Motion()) {
        u32 focus = mField->GetCamera()->GetFocus();
        if (focus == 0) {
            return FALSE;
        }
        DinoFieldChara *chara = mField->GetChara(focus);
        if (chara == NULL) {
            return FALSE;
        }
        MFx32 len;
        MFx32 range;
        len = (chara->mModel.pos - mModel.pos).Length();
        range = ((DinoFieldCharaData *) mData.mFile.GetData())->mNearRange;
        if (len <= range) {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL DinoFieldChara::IsMoving() {
    return !IsAnimDone(mBody);
}

MFx32 DinoFieldChara::GetHeight() {
    if (IsUnkA1Motion()) {
        MFx32 zero;
        zero.v = 0;
        return zero;
    }
    return ((DinoFieldCharaData *) mData.mFile.GetData())->mHeight;
}

inline DinoFieldCharaAngleAnime::DinoFieldCharaAngleAnime(DinoFieldChara *chara, u32 angle, u32 frames, int ease) {
    mChara = chara;
    mSteps = 256;
    SetDuration(frames);
    mEase = ease;
    u16 from = mChara->GetModel()->angle;
    u16 diff = angle - from;
    mFrom = from;
    if (diff < 0x8000) {
        mTo = from + diff;
    } else {
        mTo = from - (0x10000 - diff);
    }
}

void DinoFieldChara::SetAngle(u32 angle, u32 frames, u32 ease) {
    mAngleTasks->Clear();
    mAngleTasks->AddTail(new DinoFieldCharaAngleAnime(this, angle, frames, ease));
}

inline DinoFieldCharaCoordAnime::DinoFieldCharaCoordAnime(DinoFieldChara *chara, MFx32Point to, u32 frames, int ease,
                                                          u8 silent, u32 se)
    : MFx32PointAnime(chara->GetModel()->pos, to) {
    mChara = chara;
    mUnk4C = silent;
    mUnk50 = se;
    mUnk54 = 0;
    SetDuration(frames);
    mEase = ease;
}

void DinoFieldChara::MoveTo(MFx32Point coord, u32 frames, u32 ease, u8 silent) {
    u32 se = mMoveSe;
    if (silent) {
        se = 0;
    }
    if (mHasFootSe) {
        mHasFootSe = FALSE;
        se = mFootSe;
    }
    if (mBody == NULL) {
        se = 0;
    }
    mMoveTasks->Clear();
    mMoveTasks->AddTail(new DinoFieldCharaCoordAnime(this, coord, frames, ease, silent, se));
}

void DinoFieldChara::SetTask(DinoCharaTask task) {
    mTask = task;
    switch (task.unk4) {
    case 0: {
        DinoCharaState state;
        gDinoBackup->GetChara(mModel.id, &state);
        mHome.x = state.x;
        mHome.y = state.y;
        break;
    }
    case 2:
        mUnkC0 = 1;
        break;
    case 3:
        mUnkC0 = 1;
        mUnkC4 = 0;
        break;
    }
    mModel.unk10 = 0;
}

void DinoFieldChara::Read(const void *buf) {
    MSerializer::Read(buf);
    if (mModel.id) {
        gDinoBackup->GetCharaTask(mModel.id, &mTask);
    }
}

MTask *DinoFieldChara::CreateWaitMoveTask() {
    MTask *move;
    MTask *angle;
    angle = MTaskSetWaitEmptyTask::Create(mAngleTasks);
    move = MTaskSetWaitEmptyTask::Create(mMoveTasks);
    MParallelTaskSet *set = new MParallelTaskSet(TRUE);
    set->AddTail(move);
    set->AddTail(angle);
    return set;
}

MTask *DinoFieldChara::CreateWaitStopTask() {
    return new DinoFieldCharaWaitStopTask(this);
}

MTask *DinoFieldChara::CreateAngleToTask(u32 angle, u32 frames, u32 ease) {
    return new DinoFieldCharaAngleToTask(this, angle, frames, ease);
}

MTask *DinoFieldChara::CreateMoveToTask(MFx32Point coord, u32 frames, u32 ease, u8 silent) {
    return new DinoFieldCharaMoveToTask(this, coord, frames, ease, silent);
}

MTask *DinoFieldChara::CreateShowManpuTask(u32 manpu, MPoint offset) {
    return new DinoFieldCharaShowManpuTask(this, manpu, offset);
}

MTask *DinoFieldChara::CreateHideManpuTask() {
    return new DinoFieldCharaHideManpuTask(this);
}

MTask *DinoFieldChara::CreateSetMotionTask(u32 motion, u8 loop, MFx32 speed) {
    return new DinoFieldCharaModelMotionIdTask(this, motion, loop, speed, FALSE);
}

MTask *DinoFieldChara::CreateChangeMotionTask(u32 motion, u8 loop, MFx32 speed) {
    return new DinoFieldCharaModelMotionIdTask(this, motion, loop, speed, TRUE);
}

// A manpu entry: its motion's name is at the offset in its first word.
static inline const char *GetManpuName(const DinoExcavateDefs *defs, u32 manpu) {
    const u8 *entry = (const u8 *)defs + ((const u32 *)((const u8 *)defs + defs->manpus))[manpu];
    return (const char *)entry + *(const u32 *)entry;
}

// Shows the emotion bubble over the chara's head, at offset.
void DinoFieldChara::ShowManpu(u32 manpu, MPoint offset) {
    HideManpu();
    MResource<DinoExcavateDefs> defs = gDinoResource->GetExcavateDefs();
    M3DModel *model = mBody->mModel;
    MPoint head;
    model->GetJointScreenPos(model->FindJoint("head"), &head);
    mUnk2FC[0] = offset.x;
    mUnk2FC[1] = offset.y;
    mManpu = new MMotion(std::string("/motion/event/") + GetManpuName(defs.Get(), manpu) + ".bin", mField->GetOam(), 0, 1);
    mManpu->Play(1, 0);
    mManpu->SetPosition(MPoint(head.x + offset.x, head.y + offset.y));
    mManpu->SetPriority(3);
    mManpu->Show();
    mTaskSet->AddTail(mManpu);
}

void DinoFieldChara::HideManpu() {
    if (mManpu) {
        mTaskSet->Remove(mManpu);
        delete mManpu;
        mManpu = NULL;
    }
}

void DinoFieldChara::UpdateBodyModel() {
    ApplyBodyModel();
    ApplyHeadModel();
}

void DinoFieldChara::UpdateHeadModel() {
    ApplyHeadModel();
}

void DinoFieldChara::SetMotion(u32 motion, u8 loop, MFx32 speed) {
    if (mMotion != motion) {
        ChangeMotion(motion, loop, speed);
    }
}

// Plays a motion: its model comes from the caches or is loaded (and cached, but for motions 0x15-0x1A), the head
// goes back on the body's head joint, and the speed goes into the body's speed track.
void DinoFieldChara::ChangeMotion(u32 motion, u8 loop, MFx32 speed) {
    if (mBody == NULL) {
        return;
    }
    mMotion = motion;
    mUnk314 = 0;
    std::map<u32, M3DModelResource>::iterator alt = mMotionModelsAlt.find(motion);
    if (alt != mMotionModelsAlt.end()) {
        mModelResource = mMotionModelsAlt[motion];
    } else {
        std::map<u32, M3DModelResource>::iterator it = mMotionModels.find(motion);
        if (it != mMotionModels.end()) {
            mModelResource = mMotionModels[motion];
        } else {
            const char *path = GetMotionModelPath(mMotion);
            if (path == NULL) {
                return;
            }
            mModelResource = M3DModelResource(std::string(path));
            if (mMotion < 0x15 || mMotion > 0x1A) {
                if (mField->IsUnk258()) {
                    mMotionModelsAlt[mMotion] = mModelResource;
                } else {
                    mMotionModels[mMotion] = mModelResource;
                }
            }
        }
    }
    mBody->mUnkF4 = loop;
    mModelResource.Setup(mBody->mModel, 0);
    mBody->SetFrame(0);
    mBody->ClearTransformTracks();
    if (mHead != NULL) {
        int joint = mBody->mModel->FindJoint("head");
        if (joint != -1) {
            mHead->SetParent(mBody, joint);
        }
    }
    mBody->SetSpeed(speed.v);
}

// Puts the models where the character stands: on the ground (when it moved), facing its angle.
void DinoFieldChara::ApplyMotion() {
    if (mBody == NULL) {
        return;
    }
    if (mBodyPos.x != mModel.pos.x.v || mBodyPos.y != mModel.pos.y.v) {
        fx32 height = mField->GetGroundHeight(mModel.pos.x.v, mModel.pos.y.v);
        if (height != (fx32)0x80000000) {
            mUnk114 = height;
        }
        fx32 top = mField->GetMaxGroundHeight(mModel.pos);
        if (top != (fx32)0x80000000) {
            mUnk130.y = std::min((fx32)mUnk114 + FX32_ONE * 0.4, top + FX32_ONE * 0.05);
        } else {
            mUnk130.y = (fx32)mUnk114 + FX32_ONE * 0.15;
        }
    }
    fx32 x = mModel.pos.x.v;
    mBodyPos.x = x;
    mUnk130.x = x;
    mBodyPos.y = mUnk114;
    mBodyPos.z = mModel.pos.y.v;
    mUnk130.z = mBodyPos.z;
    mBody->SetPosRef(&mBodyPos);
    int angle = (u16)(0x4000 - mModel.angle);
    mBodyRot.x = 0;
    mBodyRot.y = ((u16)angle * 360) >> 4;
    mBodyRot.z = 0;
    mBody->SetRotRef(&mBodyRot);
    if (mUnk128 != NULL) {
        mUnk128->SetPosRef(&mUnk130);
    }
    if (mEffect != NULL) {
        fx32 z = mModel.pos.y.v;
        fx32 y = mUnk114;
        MPolygonChar *e = mEffect;
        e->SetPosition(mModel.pos.x.v, y, z);
        for (MAnmModelBase *anm = (MAnmModelBase *) e->mParallel.mHead; anm != NULL;
             anm = (MAnmModelBase *) anm->mNext) {
            anm->mPosRef = &e->mPos;
            anm->mUnk11B = TRUE;
        }
        e = mEffect;
        e->SetAngle(angle);
        for (MAnmModelBase *anm = (MAnmModelBase *) e->mParallel.mHead; anm != NULL;
             anm = (MAnmModelBase *) anm->mNext) {
            anm->mRotRef = &e->mRot;
            anm->mUnk11B = TRUE;
        }
    }
}

void DinoFieldChara::StartBehold(u32 target) {
    mBeholdTarget = target;
}

void DinoFieldChara::FinishBehold() {
    mBeholdTarget = 0;
}

// Each frame: the footstep sound on the motion's step frames, the manpu over the head, turning to the character
// beheld; then (outside the field's unk258 mode) the reaction to the camera's focus or the task's step, and the walk.
void DinoFieldChara::Idle() {
    if (mModel.id == 0) {
        return;
    }
    if (mUnk310 != 0) {
        if (mBody != NULL) {
            u32 frame = mBody->mUnk140 >> 4;
            if (frame != mUnk314) {
                mUnk314 = frame;
                switch (mMotion) {
                case 2:
                    for (u32 i = 0; i < ((DinoFieldCharaData *) mData.mFile.GetData())->mWalkStepCount; i++) {
                        DinoFieldCharaData *data = (DinoFieldCharaData *) mData.mFile.GetData();
                        if (frame == ((u16 *) ((u8 *) data + data->mWalkSteps))[i]) {
                            DinoSoundPlaySe(mUnk310, 0);
                            break;
                        }
                    }
                    break;
                case 3:
                    for (u32 i = 0; i < ((DinoFieldCharaData *) mData.mFile.GetData())->mRunStepCount; i++) {
                        DinoFieldCharaData *data = (DinoFieldCharaData *) mData.mFile.GetData();
                        if (frame == ((u16 *) ((u8 *) data + data->mRunSteps))[i]) {
                            DinoSoundPlaySe(mUnk310, 0);
                            break;
                        }
                    }
                    break;
                }
            }
        }
        mUnk310 = 0;
    }
    if (mManpu != NULL) {
        M3DModel *model = mBody->mModel;
        MPoint head;
        model->GetJointScreenPos(model->FindJoint("head"), &head);
        mManpu->SetPosition(MPoint(head.x + mUnk2FC[0], head.y + mUnk2FC[1]));
    }
    if (mBeholdTarget != 0) {
        DinoFieldChara *chara = mField->GetChara(mBeholdTarget);
        if (chara != NULL) {
            mModel.angle = MFx32PointAngle(chara->mModel.pos - mModel.pos);
            ApplyMotion();
        }
    }
    if (mField->IsUnk258()) {
        return;
    }
    if (((DinoFieldCharaData *) mData.mFile.GetData())->mUnk20 == 1) {
        u32 focus = mField->GetCamera()->GetFocus();
        if (focus == 0) {
            return;
        }
        DinoFieldChara *chara = mField->GetChara(focus);
        if (chara == NULL) {
            return;
        }
        DinoFieldCharaData *data = (DinoFieldCharaData *) mData.mFile.GetData();
        MFx32Point d = chara->mModel.pos - mModel.pos;
        BOOL near = d.x * d.x + d.y * d.y <= MFx32::FromRaw(FX_Mul(data->mNearRange.v, data->mNearRange.v));
        if (near != IsUnkA1Motion()) {
            PlayReaction();
        }
        ApplyMotion();
        return;
    }
    if (((DinoFieldCharaData *) mData.mFile.GetData())->mUnk20 == 2) {
        ApplyMotion();
        return;
    }
    switch (mTask.unk4) {
    case 0:
        IdleNoTask();
        break;
    case 1:
        IdleFollow();
        break;
    case 2:
        IdleWander();
        break;
    case 3:
        IdlePatrol();
        break;
    }
    if (mUnk2E0 != 0) {
        SetMotion(mUnk2E0, 1, mUnk2E4);
        mUnk2E0 = 0;
        if (mUnk2D8.x != MFx32(0) || mUnk2D8.y != MFx32(0)) {
            mModel.angle = MFx32Atan2(mUnk2D8.y, mUnk2D8.x);
            mModel.pos = mField->ResolveMove(this, mUnk2D8);
        }
        ApplyMotion();
    }
}

// The following settings in the excavation settings' block at offset78.
struct DinoFollowDefs {
    u8 unk00[0x2C];
    MFx32 trot;     // 0x2C: (times the walk speed) the speed from which it trots
    MFx32 run;      // 0x30: and runs
    MFx32 start;    // 0x34: how far the target must be to start following
    MFx32 keep;     // 0x38: and to keep following
    MFx32 factor;   // 0x3C: the speed per distance

    MFx32 GetTrot() const { return trot; }
    MFx32 GetRun() const { return run; }
    MFx32 GetStart() const { return start; }
    MFx32 GetKeep() const { return keep; }
    MFx32 GetFactor() const { return factor; }
};

// The point's length, squaring x after y.
static inline MFx32 DinoLength(const MFx32Point &p) {
    MFx32 r;
    s64 xx = p.x.v;
    s64 yy = p.y.v;
    *(vu16 *) 0x040002B0 = 1;
    *(u64 *) 0x040002B8 = xx * xx + (yy * yy + 0x1000);
    while (*(vu16 *) 0x040002B0 & 0x8000) {
    }
    r.v = *(vu32 *) 0x040002B4;
    return r;
}

// Watching (mTask.unk28): turns to the camera's focus when it comes within range (w[0] around its height),
// follows it at w[2] per second until it strays w[1] from home, waits w[4] frames, then walks home at w[3].
// NONMATCHING: 95.6, the frame is 32 bytes smaller (the game keeps more copies of the products) and registers differ.
// Whether d is shorter than limit.
static inline bool DinoWithin(const MFx32Point &d, MFx32 limit) {
    return d.x * d.x + d.y * d.y < limit * limit;
}

BOOL DinoFieldChara::Unk02162154() {
    if (!mTask.unk28.active) {
        return FALSE;
    }
    u32 focus = mField->GetCamera()->GetFocus();
    if (focus == 0) {
        return FALSE;
    }
    DinoFieldChara *chara = mField->GetChara(focus);
    if (chara == NULL) {
        return FALSE;
    }
    switch (mModel.unk10) {
    case 0: {
        MFx32Point d = chara->GetModel()->pos - mModel.pos;
        MFx32 range;
        range.v = mTask.unk28.w[0];
        MFx32 height = chara->GetHeight();
        MFx32 limit = height + range;
        if (DinoWithin(d, limit)) {
            mModel.unk10 = 1;
        } else {
            return FALSE;
        }
        break;
    }
    case 1: {
        MFx32Point d = chara->GetModel()->pos - mModel.pos;
        MFx32 speed = MFx32::FromRaw((s32)mTask.unk28.w[2] / 60);
        SetUnk2D8(d / d.Length() * speed, 2, MFx32(1));
        MFx32Point away = mModel.pos - mHome;
        MFx32 limit = MFx32::FromRaw(mTask.unk28.w[1]);
        if (away.x * away.x + away.y * away.y > limit * limit) {
            mModel.unk10 = 2;
            *(u16 *) mModel.unk14 = 0;
        }
        break;
    }
    case 2: {
        if (++*(u16 *) mModel.unk14 >= mTask.unk28.w[4]) {
            MFx32Point d = chara->GetModel()->pos - mModel.pos;
            MFx32 range;
            range.v = mTask.unk28.w[0];
            MFx32 height = chara->GetHeight();
            MFx32 limit = height + range;
            if (DinoWithin(d, limit)) {
                mModel.unk10 = 1;
                break;
            }
        }
        MFx32Point d = mHome - mModel.pos;
        MFx32 speed = MFx32::FromRaw((s32)mTask.unk28.w[3] / 60);
        MFx32 len = d.Length();
        if (len <= speed.Times(MFx32(10))) {
            SetMotion(1, 1, MFx32(1));
            mModel.unk10 = 0;
        } else {
            SetUnk2D8(d / len * speed, 2, MFx32(1));
        }
        break;
    }
    }
    return TRUE;
}

void DinoFieldChara::IdleNoTask() {
    if (!mField->IsUnk258()) {
        Unk02162154();
    }
}

// Following (mTask.unk8.fellow): walks after its target, faster the further it is (and only once it's far enough,
// or a little nearer while already walking).
// NONMATCHING: 95%, register allocation (the game keeps the squared length's high word in r4, the defs in r6).
void DinoFieldChara::IdleFollow() {
    if (mField->IsUnk258()) {
        return;
    }
    DinoFieldChara *chara = mField->GetChara(mTask.unk8.fellow.target);
    if (chara == NULL) {
        return;
    }
    MFx32Point d = chara->GetModel()->pos - mModel.pos;
    MFx32 len = DinoLength(d);
    MFx32 speed;
    MFx32 walk = mField->GetMap()->GetWalkSpeed();
    const DinoFollowDefs *defs = (const DinoFollowDefs *)gDinoResource->GetExcavate78();
    u32 mode;
    BOOL follow = FALSE;
    if (len >= defs->GetStart()) {
        follow = TRUE;
    } else if (len >= defs->GetKeep() && (mUnk2D8.x != MFx32(0) || mUnk2D8.y != MFx32(0))) {
        follow = TRUE;
    }
    if (follow) {
        speed = len * defs->GetFactor() * walk;
        if (speed >= defs->GetRun() * walk) {
            mode = 3;
        } else if (speed >= defs->GetTrot() * walk) {
            mode = 2;
        } else {
            mode = 1;
        }
    } else {
        speed = 0;
        mode = 1;
    }
    SetUnk2D8(d / d.Length() * speed, mode, walk);
}

static inline MFx32Point AngleToDir(u16 angle) {
    return MFx32Point(MFx32::FromRaw(FX_SinCosTable_[(angle >> 4) * 2 + 1]), MFx32::FromRaw(FX_SinCosTable_[(angle >> 4) * 2]));
}

// Wandering (mTask.unk8.wander): waits w[0] to w[1] frames, then walks at w[2] per second to a random point within
// w[3] of where the backup has the character.
// NONMATCHING: the game keeps the random distance in its own stack slot across the Next call.
void DinoFieldChara::IdleWander() {
    if (mField->IsUnk258() || Unk02162154()) {
        return;
    }
    if (mUnkC0 != 0) {
        if (--mUnkC0 != 0) {
            return;
        }
        DinoCharaState state;
        gDinoBackup->GetChara(mModel.id, &state);
        MFx32Point offset = AngleToDir(gDinoResource->GetRandom()->Next(0x10000)) *
                            (gDinoResource->GetRandom()->NextFraction() * MFx32::FromRaw(mTask.unk8.wander.w[3]));
        MFx32Point target(state.x + offset.x, state.y + offset.y);
        mHome = target;
    }
    MFx32Point d = mHome - mModel.pos;
    MFx32 speed = MFx32::FromRaw((s32)mTask.unk8.wander.w[2] / 60);
    MFx32 len = d.Length();
    if (len <= speed.Times(MFx32(10))) {
        SetMotion(1, 1, MFx32(1));
        mUnkC0 = mTask.unk8.wander.w[0] + MFx32ToInt(MFx32(mTask.unk8.wander.w[1] - mTask.unk8.wander.w[0]) *
                                                     gDinoResource->GetRandom()->NextFraction());
    } else {
        SetUnk2D8(d / len * speed, 2, MFx32(1));
    }
}

static inline MFx32Point RegionCenterFx(DinoRegionCenter p) {
    return MFx32Point(MFx32(p.unk00), MFx32(p.unk04));
}

// Patrolling (mTask.unk8.patrol): waits w[0] to w[1] frames, then walks at w[2] per second to the center of the next
// region in its list.
// NONMATCHING: 98.8, the squared length's sum is kept in other registers.
void DinoFieldChara::IdlePatrol() {
    if (mField->IsUnk258() || Unk02162154()) {
        return;
    }
    if (mUnkC0 != 0) {
        if (--mUnkC0 != 0) {
            return;
        }
        u32 i;
        for (i = 0; i < mTask.unk8.patrol.count; i++) {
            if (mUnkC4 == mTask.unk8.patrol.points[i]) {
                i++;
                break;
            }
        }
        i %= mTask.unk8.patrol.count;
        mUnkC4 = mTask.unk8.patrol.points[i];
        mHome = RegionCenterFx(gDinoResource->GetRegionCenter(mUnkC4));
    }
    MFx32Point d = mHome - mModel.pos;
    MFx32 speed = MFx32::FromRaw((s32)mTask.unk8.patrol.w[2] / 60);
    if (DinoLength(d) <= speed.Times(MFx32(10))) {
        SetMotion(1, 1, MFx32(1));
        mUnkC0 = mTask.unk8.patrol.w[0] + MFx32ToInt(MFx32(mTask.unk8.patrol.w[1] - mTask.unk8.patrol.w[0]) *
                                                     gDinoResource->GetRandom()->NextFraction());
    } else {
        SetUnk2D8(d / d.Length() * speed, 2, MFx32(1));
    }
}

void DinoFieldChara::SetFootSe(u32 se) {
    mHasFootSe = TRUE;
    mFootSe = se;
}

void DinoFieldChara::SetDirection(u16 angle) {
    if (mBeholdTarget == 0) {
        mModel.angle = angle;
        ApplyMotion();
    }
}

void DinoFieldCharaAngleToTask::Transfer() {
    mChara->SetAngle(mAngle, mA, mB);
}

void DinoFieldCharaShowManpuTask::Transfer() {
    mChara->ShowManpu(mManpu, mOffset);
}

void DinoFieldCharaHideManpuTask::Transfer() {
    mChara->HideManpu();
}

void DinoFieldCharaMoveToTask::Transfer() {
    mChara->MoveTo(mCoord, mA, mB, mC);
}

void DinoFieldCharaModelMotionIdTask::Transfer() {
    if (mForce) {
        mChara->ChangeMotion(mMotion, mLoop, mSpeed);
    } else {
        mChara->SetMotion(mMotion, mLoop, mSpeed);
    }
}

void DinoFieldCharaWaitStopTask::Execute() {
    if (!mChara->IsMoving()) {
        Exit();
    }
}

void DinoFieldCharaCoordAnime::Apply(int step) {
    MFx32PointAnime::Apply(step);
    if (!mUnk4C) {
        mChara->SetMotion(2, 1, mChara->mField->GetMap()->GetWalkSpeed());
    }
    fx32 x = mValue.x.v;
    fx32 y = mValue.y.v;
    MFx32Point *pos = &mChara->GetModel()->pos;
    pos->x.v = x;
    pos->y.v = y;
    mChara->ApplyMotion();
}

void DinoFieldCharaCoordAnime::Execute() {
    MFx32PointAnime::Execute();
    if (mUnk50 && !mUnk4C) {
        mChara->mUnk310 = mUnk50;
    }
}

// The tree of the character's motion model caches: lookups the game emits in this file.
typedef std::__red_black_tree<u32, std::pair<const u32, M3DModelResource>,
                              std::__select_first<std::pair<const u32, M3DModelResource> >, std::less<u32>,
                              std::allocator<std::pair<const u32, M3DModelResource> > >
    DinoFieldCharaCacheTree;
static inline const DinoHeadMaskDef *GetHeadMask(DinoFieldChara *chara, u32 id) {
    const u8 *defs = (const u8 *) chara->mHeadMasks.Get();
    return (const DinoHeadMaskDef *) (defs + ((const u32 *) (defs + ((const DinoHeadMaskDefs *) defs)->tableOffset))[id]);
}

// The head model, with its footstep sounds: the player's head mask, if one is on.
const char *DinoFieldChara::GetHeadModelPath() {
    if (mModel.id == 1) {
        mMoveSe = 0x4E8;
        mMoveSe2 = 0x4FC;
    } else {
        mMoveSe = 0xC5;
        mMoveSe2 = 0xC6;
    }
    DinoCharaState state;
    gDinoBackup->GetChara(mModel.id, &state);
    if (state.headModel >= ((DinoFieldCharaData *) mData.mFile.GetData())->mHeadModelCount) {
        return NULL;
    }
    if (mModel.id == 1 && gDinoBackup->GetSystemVar(4) != 0) {
        mHeadMasks = gDinoResource->LoadHeadMaskDefs();
        const DinoHeadMaskDef *mask = GetHeadMask(this, gDinoBackup->GetSystemVar(4));
        OS_SPrintf(mMotionModelPath, data_ov001_0216a20c[2], (const char *) mask + mask->modelName);
        mMoveSe = GetHeadMask(this, gDinoBackup->GetSystemVar(4))->moveSe;
        mMoveSe2 = GetHeadMask(this, gDinoBackup->GetSystemVar(4))->moveSe2;
    } else {
        u32 head = state.headModel;
        const u8 *data = (const u8 *) mData.mFile.GetData();
        const u32 *names = (const u32 *) (data + ((const DinoFieldCharaData *) data)->mHeadModelNames);
        OS_SPrintf(mMotionModelPath, data_ov001_0216a20c[2], data + names[head]);
    }
    if (!MFileExists(std::string(mMotionModelPath))) {
        return NULL;
    }
    return mMotionModelPath;
}

// The model a motion of the character's body model uses, if it has its own.
const char *DinoFieldChara::GetMotionModelPath(u32 motion) {
    DinoCharaState state;
    gDinoBackup->GetChara(mModel.id, &state);
    u32 count = ((DinoFieldCharaData *) mData.mFile.GetData())->mBodyModelCount;
    u32 body = state.bodyModel;
    if (body >= count) {
        return NULL;
    }
    const u8 *data = (const u8 *) mData.mFile.GetData();
    const u32 *names = (const u32 *) (data + ((const DinoFieldCharaData *) data)->mBodyModelNames);
    OS_SPrintf(mMotionModelPath, data_ov001_0216a20c[1], data + names[body], motion);
    if (!MFileExists(std::string(mMotionModelPath))) {
        return NULL;
    }
    return mMotionModelPath;
}

void DinoFieldChara::CacheMotion(u32 motion) {
    const char *path = GetMotionModelPath(motion);
    if (path != NULL) {
        if (mField->IsUnk258()) {
            mMotionModelsAlt[mMotion] = M3DModelResource(std::string(path));
        } else {
            mMotionModels[mMotion] = M3DModelResource(std::string(path));
        }
    }
}

void DinoFieldChara::ClearMotionCache() {
    mMotionModelsAlt.clear();
}

// Makes the body model again (and the shadow, if the data has one), first choosing the motion: for kinds 1 and 2,
// standing near the camera's focus switches to motion 2 (kind 1, with a sound) or 3.
void DinoFieldChara::ApplyBodyModel() {
    if (mBody != NULL) {
        mTaskSet->Remove(mBody);
        delete mBody;
        mBody = NULL;
        if (mUnk128 != NULL) {
            mTaskSet->Remove(mUnk128);
        }
        delete mUnk128;
        mUnk128 = NULL;
        mMotionModels.clear();
        mMotionModelsAlt.clear();
    }
    u32 loop = TRUE;
    switch (((DinoFieldCharaData *) mData.mFile.GetData())->mUnk20) {
    case 0:
        mMotion = 1;
        CacheMotion(1);
        break;
    case 1:
    case 2:
        if (mModel.unk05[0]) {
            loop = FALSE;
            if (IsUnkA1Motion()) {
                mMotion = 2;
            } else {
                mMotion = 1;
            }
        } else {
            mModel.unk05[0] = TRUE;
            u32 focus = mField->GetCamera()->GetFocus();
            if (focus == 0) {
                break;
            }
            DinoFieldChara *chara = mField->GetChara(focus);
            if (chara == NULL) {
                break;
            }
            MFx32 len;
            MFx32 range;
            len = (chara->mModel.pos - mModel.pos).Length();
            range = ((DinoFieldCharaData *) mData.mFile.GetData())->mNearRange;
            if (len > range) {
                mMotion = 1;
            } else if (((DinoFieldCharaData *) mData.mFile.GetData())->mUnk20 == 1) {
                mMotion = 2;
                if (((DinoFieldCharaData *) mData.mFile.GetData())->mUnk24 == 1 ||
                    ((DinoFieldCharaData *) mData.mFile.GetData())->mUnk24 == 2) {
                    GetSerial()->AddTail(new MTimer(1), DinoSoundCreatePlaySeTask(0xBC, 0));
                }
            } else {
                mMotion = 3;
                DinoSoundPlaySe(0xBE, 0);
            }
            loop = FALSE;
        }
        break;
    }
    const char *path = GetMotionModelPath(mMotion);
    if (path == NULL) {
        return;
    }
    mMotionModels[mMotion] = M3DModelResource(std::string(path));
    mBody = new MAnmModel(mScene);
    mBody->unk100 = 0x20;
    mBody->SetFrames(0, 0x40000000);
    mBody->Load(path, 0);
    mBody->Run();
    mBody->mUnkF4 = loop;
    mBody->mFollowParent = TRUE;
    mTaskSet->AddTail(mBody);
    if (((DinoFieldCharaData *) mData.mFile.GetData())->unk2E) {
        mUnk128 = new MAnmModel(mScene);
        mUnk128->unk100 = 0x21;
        mUnk128->SetFrames(0, 0x40000000);
        mUnk128->Load("/model/fieldchar/cha_shadow", 0);
        mUnk128->Run();
        mUnk128->mUnkF4 = 0;
        mUnk128->mFollowParent = TRUE;
        mTaskSet->AddTail(mUnk128);
    }
}

void DinoFieldChara::ApplyHeadModel() {
    if (mHead != NULL) {
        mTaskSet->Remove(mHead);
        delete mHead;
        mHead = NULL;
    }
    if (mBody != NULL) {
        int joint = mBody->mModel->FindJoint("head");
        if (joint != -1) {
            const char *path = GetHeadModelPath();
            if (path != NULL) {
                mHead = new MAnmModel(mScene);
                mHead->unk100 = 0x20;
                mHead->SetFrames(0, 0x40000000);
                mHead->Load(path, 0);
                mHead->SetParent(mBody, joint);
                mHead->Run();
                mHead->mUnkF4 = 1;
                mHead->mFollowParent = TRUE;
                mHead->SetUnk145();
                mTaskSet->AddTail(mHead);
            }
        }
    }
}

void DinoFieldChara::SetEffect(u32 effect) {
    if (mEffect == NULL) {
        mEffect = new MPolygonChar(mScene, 0x7FFF, NULL, NULL);
        mTaskSet->AddTail(mEffect);
        int angle = (u16) (0x4000 - mModel.angle);
        fx32 z = mModel.pos.y.v;
        fx32 y = mUnk114;
        MPolygonChar *e = mEffect;
        e->SetPosition(mModel.pos.x.v, y, z);
        for (MAnmModelBase *anm = (MAnmModelBase *) e->mParallel.mHead; anm != NULL;
             anm = (MAnmModelBase *) anm->mNext) {
            anm->mPosRef = &e->mPos;
            anm->mUnk11B = TRUE;
        }
        e = mEffect;
        e->SetAngle(angle);
        for (MAnmModelBase *anm = (MAnmModelBase *) e->mParallel.mHead; anm != NULL;
             anm = (MAnmModelBase *) anm->mNext) {
            anm->mRotRef = &e->mRot;
            anm->mUnk11B = TRUE;
        }
    }
    if (effect == 0) {
        mEffect->Reset();
        return;
    }
    char path[64];
    OS_SPrintf(path, "/model/fieldchar/effect%02d.csv", effect);
    mEffect->Load(path, NULL);
    MPolygonChar *e = mEffect;
    e->mUnkAC = 1;
    for (MAnmModelBase *anm = (MAnmModelBase *) e->mParallel.mHead; anm != NULL; anm = (MAnmModelBase *) anm->mNext) {
        anm->mFollowParent = TRUE;
    }
    for (MAnmModelBase *anm = (MAnmModelBase *) mEffect->mParallel.mHead; anm != NULL;
         anm = (MAnmModelBase *) anm->mNext) {
        anm->mActive = TRUE;
    }
}

#pragma auto_inline off
// (clear and insert_node_at go ahead of destroy, as in the game.)
template void DinoFieldCharaCacheTree::clear();
template DinoFieldCharaCacheTree::node_base *DinoFieldCharaCacheTree::insert_node_at(node_base *parent, bool left,
                                                                                  bool leftmost,
                                                                                  const value_type &v);
template DinoFieldCharaCacheTree::node_base *DinoFieldCharaCacheTree::find_insert_position(const u32 &k,
                                                                                        node_base *&last_not_greater,
                                                                                        bool &left, bool &leftmost);
template DinoFieldCharaCacheTree::iterator DinoFieldCharaCacheTree::find(const u32 &k);
template DinoFieldCharaCacheTree::node_base *DinoFieldCharaCacheTree::lower_bound(const u32 &k, node_base *x,
                                                                               node_base *y);
