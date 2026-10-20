// DinoUiRvInformation3DModel, DinoUiRvInformation3DModelRotate.
// Only some functions are decompiled so far.

#pragma ipa file

#include "game/dino_ui_rv_information3d_model.h"
#include "game/dino_ui_rv_information3d_model_rotate.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "game/dino_ui_rv_information.h"
#include <stdio.h>
#include "m/indicator.h"
#include "m/motion.h"
#include "game/dino_ui_revive_status.h"
#include "game/dino_ui_field_menu_number3.h"
#include "game/dino_resource.h"
#include "m/picture.h"
#include "m/display.h"
#include "m/lcd.h"
#include "m/3d_model.h"
#include "m/3d_scene.h"
#include "m/3d.h"
#include "m/input.h"
#include "m/window.h"
#include "m/irq.h"
#include "m/anm_model.h"

extern "C" u8 data_ov003_021624d0[];  // the scene's heap

DinoUiRvInformation3DModel::DinoUiRvInformation3DModel() : MProcess(NULL) {
    mUnk80 = 0;
    mUnk84 = 0;
    mUnk88 = 0;
    mScene = new M3DScene((u32)data_ov003_021624d0, 0xF618);
    mScene->mUnk14 = 1;
    mScene->mUnk18 = 300 << FX32_SHIFT;
    mScene->mM3d->mBgCntBits.priority = 1;
    mScene->mM3d->Hide();
    gCurrentDisplay->mWin->GetWindow0()->Hide();
    gCurrentDisplay->mWin->GetWindow1()->Hide();
    VecFx32 v;
    v.x = 0;
    v.y = 0;
    v.z = 0;
    mScene->SetCameraTarget(&v);
    v.x = 0;
    v.y = 0;
    v.z = 19 << FX32_SHIFT;
    mScene->SetCameraPos(&v);
    mScene->SetUnk80(-80, 0);
    mUnk8C = mUnk90 = mUnk94 = 0;
    mTasks = new MParallelTaskSet(FALSE);
    mUnk9C = 0;
    MSingleInputHub *hub = new MSingleInputHub(1);
    hub->EnableTouch();
    mInput = hub->GetInput(0);
    mParallel.AddTail(hub);
    MParallelTaskSet *parallel = GetParallel();
    M3DScene *scene = mScene;
    parallel->AddTail(mTasks);
    parallel->AddTail(scene);
}

// Hides the scene's 3D screen when the value changes.
void DinoUiRvInformation3DModel::SetUnk84(u32 value) {
    if (mUnk84 != value) {
        mUnk84 = value;
        mScene->mM3d->Hide();
    }
}

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
        mModel->SetUnk84(0);
    }
    GetSerial()->AddTail(new MTimer(1), MCreateCallTask(this, data_ov003_02160784.Unk02156800_00));
    if (mUnk1B0) {
        GetSerial()->AddTail(new MTimer(1), MCreateCallTask(this, data_ov003_0216069c.Unk021571bc_00));
    } else {
        GetSerial()->AddTail(new MTimer(1), MCreateCallTask(this, data_ov003_02160774.Unk02156ee8_00, mInfo));
    }
}

extern "C" struct {
    void (DinoUiRvInformation::*SetPage_00)(u8 page);
} data_ov003_02160794;
// The creature type's mark: the first entry with the type (or -1) gives the anime.
extern "C" struct {
    int type;
    int anime;
} data_ov003_021601e0[6];
extern "C" const char data_ov003_02160c24[];   // the number's format
extern "C" void func_ov003_02157364(DinoUiRvInformation *info);
extern "C" void func_ov003_021576fc(DinoUiRvInformation *info);

// Shows a creature: its number, type and name, and the page's details (reloading the page when the picture
// changes).
void DinoUiRvInformation::Unk02156ee8(DinoCreatureInfo info) {
    u32 index = info.state.index;
    if (index == 0 || index > gDinoResource->GetCreatureCount()) {
        return;
    }
    mInfo.state = info.state;
    mUnk1B0 = 0;
    u32 old = mUnk198;
    u32 id = (mInfo.state.index - 1) * 4 + 1;
    {
        const u8 *data = (const u8 *)mUnk19C.Get();
        mUnk195 = (data + ((const u32 *)(data + ((const u32 *)data)[2]))[id])[4];
    }
    {
        const u8 *data = (const u8 *)mUnk19C.Get();
        mUnk198 = *(const u32 *)(data + ((const u32 *)(data + ((const u32 *)data)[2]))[id] + 0x38);
    }
    const DinoCreatureDef *def = gDinoResource->GetCreatureDef(index);
    {
        char number[4];
        sprintf(number, data_ov003_02160c24, def->voice);
        ((MIndicator *)mViewsAC[0])->SetText(std::string(number));
    }
    u8 type = def->unk0D;
    for (u32 i = 0; i < 6; i++) {
        int t = data_ov003_021601e0[i].type;
        if (t == type || t == -1) {
            ((MMotion *)mViewsAC[1])->Play(data_ov003_021601e0[i].anime, 0);
            break;
        }
    }
    ((MIndicator *)mViewsAC[2])->SetText(std::string(gDinoResource->GetCreatureText08(index)));
    mViewsAC[0]->Show();
    mViewsAC[1]->Show();
    mViewsAC[2]->Show();
    if (mUnk198 != old && mPage == 3) {
        GetSerial()->AddTail(new MTimer(1), MCreateCallTask(this, data_ov003_02160794.SetPage_00, mPage));
        mPage = 0xFF;
        return;
    }
    if (mHasTenPictures && mPage < 3) {
        if (info.state.parts[0].level != 0) {
            mPagePicture->Show();
            mSubPicture->Hide();
        } else {
            mPagePicture->Hide();
            mSubPicture->Show();
        }
    }
    switch (mPage) {
    case 0:
        func_ov003_02157364(this);
        return;
    case 1:
    case 2:
        func_ov003_021576fc(this);
        return;
    case 3:
        mUnk16C->Clear();
        mUnk164 = new DinoUiReviveStatus(gLcd->mMain, mInfo, 0);
        mUnk16C->AddTail(mUnk164);
        break;
    }
}

// Hides the page's parts (on the last page, puts its picture back up).
void DinoUiRvInformation::Unk021571bc() {
    mUnk1B0 = 1;
    mViewsAC[0]->Hide();
    mViewsAC[1]->Hide();
    mViewsAC[2]->Hide();
    if (mHasTenPictures && mPage < 3) {
        mPagePicture->Show();
        mSubPicture->Hide();
    }
    switch (mPage) {
    case 0:
        mViewD4->Hide();
        mViewEC->Hide();
        mViewD8->Hide();
        for (int i = 0; i < 4; i++) {
            mViewsDC[i]->Hide();
        }
        for (int i = 0; i < 6; i++) {
            mNumbersBC[i]->Hide();
        }
        mModel->SetUnk84(0);
        break;
    case 1:
    case 2:
        for (int i = 0; i < 3; i++) {
            mViewsF0[i]->Hide();
            mViews104[i]->Hide();
            mViews118[i]->Hide();
            mNumbers12C[i]->Hide();
            mNumbers140[i]->Hide();
        }
        break;
    case 3:
        mUnk16C->Clear();
        if (mUnk195) {
            gCurrentDisplay = gLcd->mMain;
            mPictureTasks->Clear();
            mPagePicture = new MTextPicture(*mPictures[mPage], FALSE, false);
            mPictureTasks->AddTail(mPagePicture);
            mPagePicture->Show();
            mPagePicture->SetPriority(3);
        }
        break;
    }
    mUnk195 = 0;
    mUnk198 = 0;
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

extern "C" const char data_ov003_02160be0[];   // the model's path
extern "C" const u32 data_02089ecc[];          // by creature
extern "C" const fx32 data_02089ce8[];         // by creature: the height the model turns around

void DinoUiRvInformation3DModel::LoadModel() {
    char path[0x100];
    OS_SPrintf(path, data_ov003_02160be0, mUnk80);
    mAnm = new MAnmModel(mScene);
    mAnm->SetFrames(0, 0x40000000);
    mAnm->Load(path, 0);
    mAnm->Run();
    mAnm->mUnkF4 = 1;
    mAnm->mFollowParent = TRUE;
    mUnk8C = mUnk94 = 0;
    mUnk90 = data_02089ecc[mUnk80];
    mUnk94 = 0xA000;
    mUnk9C = 0;
    mUnkA8 = mUnkAC = 0;
    mRotate = new DinoUiRvInformation3DModelRotate(mAnm, data_02089ce8[mUnk80]);
    MIrqLock lock;
    mTasks->AddTail(mAnm, mRotate);
}

#pragma auto_inline off

// Whether the attack's first value is 8 to 11, or its second 2 or 9 to 15.
BOOL DinoUiRvAttackFlag(u32 id) {
    const DinoAttackDef *def = gDinoResource->GetAttackDef(id);
    if ((1 << *DinoOffsetPtr<u8>(def, def->unk10)) & 0xF00) {
        return TRUE;
    }
    switch (*DinoOffsetPtr<u8>(def, def->unk14)) {
    case 2:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
        return TRUE;
    }
    return FALSE;
}

extern "C" void func_ov003_02156eac(DinoUiRvInformation3DModel *model, MFx32 angle) {
    BOOL out = angle < MFx32(0) || MFx32(100) <= angle;
    if (!out) {
        model->mAngle = angle;
    }
}

// The angle's index into the sine table.
static inline int DinoDegToIdx(fx32 deg) {
    return (u16)((deg << 4) / 360);
}

// Turns the model around its pivot by the angles.
void DinoUiRvInformation3DModelRotate::Execute() {
    MTX_Identity44(&mMtx);
    MTX_TransApply44(&mMtx, &mMtx, 0, mPivotY, 0);
    MtxFx44 rot;
    MTX_RotX44(&rot, FX_SinIdx(DinoDegToIdx(mRotX)), FX_CosIdx(DinoDegToIdx(mRotX)));
    MTX_Concat44(&rot, &mMtx, &mMtx);
    MTX_RotY44(&rot, FX_SinIdx(DinoDegToIdx(mRotY)), FX_CosIdx(DinoDegToIdx(mRotY)));
    MTX_Concat44(&rot, &mMtx, &mMtx);
    MTX_RotZ44(&rot, FX_SinIdx(DinoDegToIdx(mRotZ)), FX_CosIdx(DinoDegToIdx(mRotZ)));
    MTX_Concat44(&rot, &mMtx, &mMtx);
    MTX_TransApply44(&mMtx, &mMtx, 0, -mPivotY, 0);
    mOwner->mModel->mUnkC4 = &mMtx;
}
