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

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x4];
    void (DinoFieldHole::*ChangeToHole_04)();
    void (DinoFieldHole::*StateWait_0C)();
    void (DinoFieldHole::*ChangeToHole_14)();
    u8 unk1C[0x24 - 0x1C];
    void (DinoFieldHole::*Unk0215c418_24)();
} data_ov001_02169ddc;

DinoFieldHole::DinoFieldHole(DinoField *field, u32 unk80, MPoint pos, u32 unk90, u8 unk118)
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
    mState = data_ov001_02169ddc.Unk0215c418_24;
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
