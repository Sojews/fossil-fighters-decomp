// DinoUiRvInformation3DModel, DinoUiRvInformation3DModelRotate.
// Only the destructors are decompiled so far.

#pragma ipa file

#include "game/dino_ui_rv_information3d_model.h"
#include "game/dino_ui_rv_information3d_model_rotate.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "game/dino_ui_rv_information.h"

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
