// DinoFieldCharaAngleAnime, DinoFieldCharaWaitStopTask, DinoFieldCharaAngleToTask, DinoFieldCharaMoveToTask, DinoFieldCharaShowManpuTask, DinoFieldCharaHideManpuTask, DinoFieldCharaModelMotionIdTask.
// Also DinoFieldChara's members here: models, motions, movement.

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
    MFx32Rect mRect30;  // 0x30: the collision box, around the position
    MRect mRect40;      // 0x40
    MFx32 mUnk50;       // 0x50: -10 = the default
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

MFx32 DinoFieldChara::GetMoveSpeed() {
    if (mTask.unk4 == 1) {
        return data_020b5ac8;
    }
    MFx32 speed = ((DinoFieldCharaData *) mFile.GetData())->mMoveSpeed;
    if (!(speed != MFx32(-1))) {
        return data_020b5ab8;
    }
    return speed;
}

u8 DinoFieldChara::GetUnk2C() {
    return ((DinoFieldCharaData *) mFile.GetData())->mUnk2C;
}

MFx32Rect DinoFieldChara::GetRect30() {
    const DinoFieldCharaData *data = (const DinoFieldCharaData *) mFile.GetData();
    return data->mRect30;
}

u8 DinoFieldChara::GetUnk2D() {
    return ((DinoFieldCharaData *) mFile.GetData())->mUnk2D;
}

MRect DinoFieldChara::GetRect40() {
    const DinoFieldCharaData *data = (const DinoFieldCharaData *) mFile.GetData();
    return data->mRect40;
}

MFx32 DinoFieldChara::GetUnk50() {
    MFx32 v = ((DinoFieldCharaData *) mFile.GetData())->mUnk50;
    if (!(v != MFx32(-10))) {
        return data_020b5abc;
    }
    return v;
}

void DinoFieldChara::SetUnk2D8(MPoint pos, u32 a, u32 b) {
    mUnk2D8 = pos;
    mUnk2E0 = a;
    mUnk2E4 = b;
}

void DinoFieldChara::ClearUnk2E0() {
    mUnk2E0 = 0;
}

// NONMATCHING: two dead stores to the MPoint temporaries are in a different order.
void DinoFieldChara::StopWalking() {
    if (!mModel.unk05[0]) {
        SetMotion(1, 1, mField->GetMap()->GetWalkSpeed());
    }
    SetUnk2D8Pos(MPoint(0, 0));
    mUnk2E0 = 0;
}

BOOL DinoFieldChara::IsUnk20Two() {
    return ((DinoFieldCharaData *) mFile.GetData())->mUnk20 == 2 ? TRUE : FALSE;
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
            if (((DinoFieldCharaData *) mFile.GetData())->mUnk20 == 2) {
                DinoSoundPlaySe(0xBF, 0);
            } else if (((DinoFieldCharaData *) mFile.GetData())->mUnk24 == 2) {
                DinoSoundPlaySe(0xC1, 0);
            } else {
                DinoSoundPlaySe(0xBD, 0);
            }
        } else {
            SetMotion(3, 0, MFx32(1));
            if (((DinoFieldCharaData *) mFile.GetData())->mUnk20 == 2) {
                DinoSoundPlaySe(0xBE, 0);
            } else if (((DinoFieldCharaData *) mFile.GetData())->mUnk24 == 2) {
                DinoSoundPlaySe(0xC0, 0);
            } else {
                DinoSoundPlaySe(0xBB, 0);
            }
        }
    }
}

static inline BOOL IsUnk20One(DinoFieldChara *chara) {
    return chara->mBody != NULL && ((DinoFieldCharaData *) chara->mFile.GetData())->mUnk20 == 1;
}

static inline BOOL IsUnk24Kind(DinoFieldChara *chara) {
    return IsUnk20One(chara) && (((DinoFieldCharaData *) chara->mFile.GetData())->mUnk24 == 1 ||
                                 ((DinoFieldCharaData *) chara->mFile.GetData())->mUnk24 == 2);
}

BOOL DinoFieldChara::IsUnk24Reacting() {
    return IsUnk24Kind(this) && IsUnkA1Motion();
}

// Whether the camera's focus stands within the chara's range (for a chara of kind 2, in an A1 motion).
// NONMATCHING: stack slots of the subtraction's temporaries only (the game's y temporaries sit above the x ones).
BOOL DinoFieldChara::IsFocusNear() {
    if (mBody != NULL && ((DinoFieldCharaData *) mFile.GetData())->mUnk20 == 2 && IsUnkA1Motion()) {
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
        range = ((DinoFieldCharaData *) mFile.GetData())->mNearRange;
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
    return ((DinoFieldCharaData *) mFile.GetData())->mHeight;
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
        mHomeX = state.x;
        mHomeY = state.y;
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

void DinoFieldChara::StartBehold(u32 target) {
    mBeholdTarget = target;
}

void DinoFieldChara::FinishBehold() {
    mBeholdTarget = 0;
}

void DinoFieldChara::IdleNoTask() {
    if (!mField->IsUnk258()) {
        Unk02162154();
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
    if (state.headModel >= ((DinoFieldCharaData *) mFile.GetData())->mHeadModelCount) {
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
        const u8 *data = (const u8 *) mFile.GetData();
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
    u32 count = ((DinoFieldCharaData *) mFile.GetData())->mBodyModelCount;
    u32 body = state.bodyModel;
    if (body >= count) {
        return NULL;
    }
    const u8 *data = (const u8 *) mFile.GetData();
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
template DinoFieldCharaCacheTree::node_base *DinoFieldCharaCacheTree::find_insert_position(const u32 &k,
                                                                                        node_base *&last_not_greater,
                                                                                        bool &left, bool &leftmost);
template DinoFieldCharaCacheTree::iterator DinoFieldCharaCacheTree::find(const u32 &k);
template DinoFieldCharaCacheTree::node_base *DinoFieldCharaCacheTree::lower_bound(const u32 &k, node_base *x,
                                                                               node_base *y);
