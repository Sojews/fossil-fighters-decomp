// DinoFieldHole.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_field_hole.h"
#include "types.h"
#include "m/anm_model.h"
#include "m/3d_model.h"
#include "m/transfer.h"

DinoFieldHole::~DinoFieldHole() {
}

void DinoFieldHole::Idle() {
    (this->*mState)();
}

u8 DinoFieldHole::GetUnk110() {
    return mUnk110;
}

void DinoFieldHole::SetUnk114() {
    mUnk114 = 0xB1;
}

MPoint DinoFieldHole::GetPos() {
    return mPos;
}

void DinoFieldHole::ChangeToHole() {
    mModel = M3DModelResource("/model/fieldchar/ana03");
    MAnmModelBase *anm = mUnk84;
    anm->mModel->Unk08(mModel.mModel, mModel.mAnim, TRUE);
    anm = mUnk84;
    anm->mFrame = 0;
    anm->mLoopEnd = 0;
    anm->mUnk11B = TRUE;
    mUnk84->ClearTransformTracks();
    MTask *exit = MTaskExitTransfer::Create(this);
    MTask *timer = new MTimer(27);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(timer);
    serial->AddTail(exit);
}
