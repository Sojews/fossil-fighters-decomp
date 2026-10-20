// DinoFieldHole.

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

MFx32Point DinoFieldHole::GetPos() {
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

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x4];
    void (DinoFieldHole::*ChangeToHole_04)();
    void (DinoFieldHole::*StateWait_0C)();
    void (DinoFieldHole::*ChangeToHole_14)();
    u8 unk1C[0x24 - 0x1C];
    void (DinoFieldHole::*StateAppear_24)();
    void (DinoFieldHole::*StateCountdown_2C)();
} data_ov001_02169ddc;
extern "C" const char data_ov001_02169e50[];  // the hole's model

DinoFieldHole::DinoFieldHole(DinoField *field, u32 unk80, MFx32Point pos, u32 unk90, u8 unk118)
    : MProcess(NULL), mField(field), mUnk80(unk80), mPos(pos), mUnk90(unk90) {
    mUnk110 = 0;
    mUnk118 = unk118;
    mState = data_ov001_02169ddc.StateWait_0C;
}

void DinoFieldHole::StartChangeToHole() {
    mState = data_ov001_02169ddc.ChangeToHole_14;
}

void DinoFieldHole::StateWait() {
    mSerial.AddTail(new MTimer(30));
    mState = data_ov001_02169ddc.StateAppear_24;
}

// Makes the hole's model and starts the countdown.
void DinoFieldHole::StateAppear() {
    mUnk110 = TRUE;
    mUnk84 = new MAnmModel((M3DScene *)mUnk80);
    mUnk84->unk100 = 0x22;
    mUnk84->SetFrames(0, 0x40000000);
    mUnk84->Load(data_ov001_02169e50, FALSE);
    mUnk84->Run();
    mUnk84->mUnkF4 = 0;
    mUnk84->mFollowParent = TRUE;
    if (mUnk84->mState == MTask::STATE_RUN) {
        mUnk84->Execute();
    }
    mModelPos.x = mPos.x.v;
    mModelPos.z = mPos.y.v;
    mModelPos.y = mUnk90;
    mUnk84->SetPosRef(&mModelPos);
    mModelRot.x = mModelRot.y = mModelRot.z = 0;
    mUnk84->SetRotRef(&mModelRot);
    mParallel.AddTail(mUnk84);
    mUnk114 = 0x93;
    mState = data_ov001_02169ddc.StateCountdown_2C;
}

void DinoFieldHole::StateCountdown() {
    if (!mUnk118) {
        mUnk114--;
        if (mUnk114 == 0) {
            mUnk110 = 0;
            mState = data_ov001_02169ddc.ChangeToHole_04;
        }
    }
}
