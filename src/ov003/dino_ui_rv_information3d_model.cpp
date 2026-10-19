// DinoUiRvInformation3DModel, DinoUiRvInformation3DModelRotate.
// Only the destructors are decompiled so far.

#pragma ipa file

#include "game/dino_ui_rv_information3d_model.h"
#include "game/dino_ui_rv_information3d_model_rotate.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "game/dino_ui_rv_information.h"
#include "m/picture.h"
#include "m/display.h"
#include "m/lcd.h"

DinoUiRvInformation3DModel::~DinoUiRvInformation3DModel() {
}

DinoUiRvInformation3DModelRotate::~DinoUiRvInformation3DModelRotate() {
}

DinoUiRvInformation::~DinoUiRvInformation() {
    if (mHasTenPictures) {
        for (int i = 0; i < 10; i++) {
            delete mPictures[i];
        }
    } else {
        for (int i = 0; i < 7; i++) {
            delete mPictures[i];
        }
    }
}

void DinoUiRvInformation::SetUnk1B1(u8 value) {
    mUnk1B1 = value;
}

void DinoUiRvInformation::SetUnk1AC(u32 value) {
    mUnk1AC = value;
}

MTask *DinoUiRvInformation::CreateWaitModelTask() {
    return MTaskSetWaitEmptyTask::Create(&mModel->mSerial);
}

void DinoUiRvInformation::Idle() {
}

extern "C" void func_ov003_02155a34(DinoUiRvInformation3DModel *model, int arg);

// The TU's pooled pointer-to-member constants.
extern "C" struct {
    void (DinoUiRvInformation::*CreateSubPicture_00)();
} data_ov003_021606d4;
extern "C" struct {
    void (DinoUiRvInformation::*Unk02156800_00)();
} data_ov003_02160784;
extern "C" struct {
    void (DinoUiRvInformation::*Unk021571bc_00)();
} data_ov003_0216069c;
extern "C" struct {
    void (DinoUiRvInformation::*Unk02156ee8_00)(DinoCreatureInfo info);
} data_ov003_02160774;

void DinoUiRvInformation::SetPage(u8 page) {
    if (page > 3 || mPage == page) {
        return;
    }
    mPage = page;
    gCurrentDisplay = gLcd->mMain;
    mPictureTasks->Clear();
    mUnk16C->Clear();
    if (!mUnk1B0 && mUnk195 && page == 3) {
        mPagePicture = new MTextPicture(*mPictures[mUnk198 + 3], 0, false);
        mPictureTasks->AddTail(mPagePicture);
    } else {
        mPagePicture = new MTextPicture(*mPictures[page], 0, false);
        mPictureTasks->AddTail(mPagePicture);
    }
    mPagePicture->Show();
    mPagePicture->SetPriority(3);
    if (mHasTenPictures && page < 3) {
        GetSerial()->AddTail(new MTimer(1), MCreateCallTask(this, data_ov003_021606d4.CreateSubPicture_00));
    }
    if (mPage) {
        func_ov003_02155a34(mModel, 0);
    }
    GetSerial()->AddTail(new MTimer(1), MCreateCallTask(this, data_ov003_02160784.Unk02156800_00));
    if (mUnk1B0) {
        GetSerial()->AddTail(new MTimer(1), MCreateCallTask(this, data_ov003_0216069c.Unk021571bc_00));
    } else {
        GetSerial()->AddTail(new MTimer(1), MCreateCallTask(this, data_ov003_02160774.Unk02156ee8_00, mInfo));
    }
}

void DinoUiRvInformation::CreateSubPicture() {
    mSubPicture = new MTextPicture(*mPictures[mPage + 7], 1, false);
    mPictureTasks->AddTail(mSubPicture);
    mSubPicture->Hide();
    mSubPicture->SetPriority(2);
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask0<DinoUiRvInformation, DinoUiRvInformation>;
template MTask *MCreateCallTask(DinoUiRvInformation *obj, void (DinoUiRvInformation::*func)());
template class MCallTask1<DinoUiRvInformation, DinoUiRvInformation, u8>;
template MTask *MCreateCallTask(DinoUiRvInformation *obj, void (DinoUiRvInformation::*func)(u8), u8 a0);
// NONMATCHING (this creator only): we copy the DinoCreatureInfo argument to a stack temporary before constructing;
// the game copies it straight from the argument (a const A1 & constructor does that but breaks the MPoint instances).
template class MCallTask1<DinoUiRvInformation, DinoUiRvInformation, DinoCreatureInfo>;
template MTask *MCreateCallTask(DinoUiRvInformation *obj, void (DinoUiRvInformation::*func)(DinoCreatureInfo), DinoCreatureInfo a0);
template class MBgCallTask0<DinoUiRvInformation3DModel>;
template MTask *MCreateBgCallTask(DinoUiRvInformation3DModel *obj, void (DinoUiRvInformation3DModel::*func)(), int thread);

#pragma auto_inline off

extern "C" void func_ov003_02156eac(DinoUiRvInformation3DModel *model, MFx32 angle) {
    BOOL out = angle < MFx32(0) || MFx32(100) <= angle;
    if (!out) {
        model->mAngle = angle;
    }
}
