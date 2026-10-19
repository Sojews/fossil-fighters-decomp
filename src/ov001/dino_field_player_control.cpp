// DinoFieldPlayerControl.
// Only the destructor is decompiled so far.

#include "game/dino_field_player_control.h"
#include "m/call_task.h"
#include "m/fx32.h"
#include "m/polygon_char.h"
#include "m/anm_model.h"
#include "game/dino_field.h"

DinoFieldPlayerControl::DinoFieldPlayerControl(DinoField *field, u32 *arg1, DinoFieldPlayerControlResult *result,
                                               u32 arg3)
    : MProcess(NULL), mField(field), mUnk78(arg1), mResult(result), mUnk8C(arg3), mUnk98(0), mUnkAC(0),
      mExcavateDefs(gDinoResource->GetExcavateDefs()) {
    mUnk80 = mField->GetUnk234();
    mUnk88 = 0;
    mUnk84 = 0;
    if (mResult->unk08) {
        mUnk90 = mResult->unk00;
        mUnk94 = mResult->unk04;
    }
    *mUnk78 = 0;
    mEffect = NULL;
}

DinoFieldPlayerControl::~DinoFieldPlayerControl() {
    StoreResult();
}

void DinoFieldPlayerControl::SetEffect(VecFx32 pos, u16 angle, u32 effect) {
    if (mEffect == NULL) {
        mEffect = new MPolygonChar(mField->mScene, 0x7FFF, NULL, NULL);
        mParallel.AddTail(mEffect);
    }
    MPolygonChar *e = mEffect;
    e->SetPosition(pos.x, pos.z, pos.y);
    for (MAnmModelBase *anm = (MAnmModelBase *) e->mParallel.mHead; anm != NULL; anm = (MAnmModelBase *) anm->mNext) {
        anm->mPosRef = &e->mPos;
        anm->mUnk11B = TRUE;
    }
    e = mEffect;
    e->mRot.x = 0;
    e->mRot.y = (u16) (0x4000 - angle) * 360 / 16;
    e->mRot.z = 0;
    for (MAnmModelBase *anm = (MAnmModelBase *) e->mParallel.mHead; anm != NULL; anm = (MAnmModelBase *) anm->mNext) {
        anm->mRotRef = &e->mRot;
        anm->mUnk11B = TRUE;
    }
    char path[64];
    OS_SPrintf(path, "/model/fieldchar/effect%02d.csv", effect);
    mEffect->Load(path, NULL);
    e = mEffect;
    e->mUnkAC = 1;
    for (MAnmModelBase *anm = (MAnmModelBase *) e->mParallel.mHead; anm != NULL; anm = (MAnmModelBase *) anm->mNext) {
        anm->mFollowParent = TRUE;
    }
    for (MAnmModelBase *anm = (MAnmModelBase *) mEffect->mParallel.mHead; anm != NULL;
         anm = (MAnmModelBase *) anm->mNext) {
        anm->mActive = TRUE;
    }
}
