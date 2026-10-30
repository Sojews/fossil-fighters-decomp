// DinoBattleView3d.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#pragma ipa file

#include "game/dino_sound.h"
#include <printf.h>
#include "game/dino_battle_view_3d.h"
#include "game/dino_3d_load_csv.h"
#include "game/dino_battle_view_class.h"
#include "types.h"
#include "game/dino_battle_view_3d_camera.h"
#include "m/input.h"
#include "game/camera_wipe.h"
#include "game/b3d_status_view_task.h"
#include "game/dino_battle_view_3d2.h"
#include "game/black_in_task.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "game/dino_battle_view_3d_move.h"
#include "game/camera_load_task.h"
#include "game/dino_battle_view_kaseki_point_menu.h"
#include "m/3d_scene.h"
#include "m/lcd.h"
#include "m/display.h"
#include "game/dino_battle_view_field_player_bg.h"
#include "game/dino_resource.h"
#include "m/3d.h"
#include "m/smart_oam.h"
#include "m/file.h"
#include "game/dino_static_init.h"
#include "game/dino_message.h"

DinoBattleView3D::~DinoBattleView3D() {
    CancelQueue();
    mUnkC0->Clear();
    if (mUnkE4 != NULL) {
        mParallel.Remove(mUnkE4);
        delete mUnkE4;
        mUnkE4 = NULL;
    }
    if (mUnkE8 != NULL) {
        mParallel.Remove(mUnkE8);
        delete mUnkE8;
        mUnkE8 = NULL;
    }
    mUnkCC->Clear();
}

B3DStatusViewTask::~B3DStatusViewTask() {
    mSerial.Clear();
}

LoadWinnerLoad::~LoadWinnerLoad() {
    mSerial.Clear();
}

extern "C" const char data_ov000_021965c0[];   // "din%03d_%02d"
extern "C" const VecFx32 data_ov000_02190334, data_ov000_0219034c;

// Loads the slot's i-th creature's winner model into the view's i-th model.
void LoadWinnerLoad::LoadModel(int i) {
    for (int j = 0; j < 3; j++) {
        mView->mModels[j]->Reset();
    }
    char name[64];
    sprintf(name, data_ov000_021965c0, mView->mCreatureIds[mSlot * 3 + i], 4);
    mView->mModels[i]->LoadByName(name, TRUE, 0x0FFFFFFF);
    mView->mModels[i]->PushVec3(&data_ov000_02190334);
    mView->mModels[i]->PushVec2(&data_ov000_0219034c);
    if (mView->mUnk14A[mSlot * 3 + i] == 0) {
        mView->mModels[i]->Push11();
    } else {
        mView->mModels[i]->Push12();
    }
    mView->mModels[i]->Push7(0x1000);
}

void B3DStatusViewTask::Idle() {
    (this->*mState)();
}

// The TU's pooled pointer-to-member constants (set up at startup).
extern "C" struct {
    u8 unk00[0x8];
    const char *mDamageTwo_08;      // the damage cameras' name formats, by hits
    const char *mDamageThree_0C;
    u8 unk10[0x18 - 0x10];
    const char *mHitMotion2_18;     // the hit marks' motions
    const char *mDamageOne_1C;
    const char *mHitMotion0_20;
    const char *mMarkMotion_24;     // an extra mark's
    u8 unk28[0x30 - 0x28];
    const char *mLineMotion_30;     // the ability's lines
    u8 unk34[0x40 - 0x34];
    const char *mHitMotion1_40;
    u8 unk44[0x48 - 0x44];
    const char *mDamageMiss_48;
    void (B3DStatusViewTask::*State_4C)();
    u8 unk54[0x5C - 0x54];
    void (B3DStatusViewTask::*State_5C)();
    u8 unk64[0x6C - 0x64];
    void (B3DStatusViewTask::*State_6C)();
    void (B3DStatusViewTask::*State_74)();
} data_ov000_021962e8;

class DinoBattleView3D;
// The next pool symbol's.
extern "C" struct {
    u8 unk00[0x1C];
    void (DinoBattleView3D::*StateLoad_1C)();
    u8 unk24[0x2C - 0x24];
    void (B3DStatusViewTask::*StateCheck_2C)();
    u8 unk34[0x3C - 0x34];
    void (DinoBattleView3D::*RunNextQueued_3C)();
    u8 unk44[0x4C - 0x44];
    void (B3DStatusViewTask::*State_4C)();
    void (B3DStatusViewTask::*State_54)();
    u8 unk5C[0x6C - 0x5C];
    void (B3DStatusViewTask::*State_6C)();
} data_ov000_02196368;

extern "C" MTask *func_ov000_0218abd0(void *camera, int light);
extern "C" MTask *func_ov000_0218ac04(void *camera, void *arg1, const VecFx32 *arg2, BOOL arg3);
extern "C" MTask *func_ov000_0218ac40(void *camera, u8 pause);
extern "C" MTask *func_ov000_0218ac74(void *camera, Dino3DLoadCsv *parent);

// Points the camera at the status's creature and shows it.
void B3DStatusViewTask::StateShow() {
    u32 next;
    mView->GetUnk19C(&mUnk7C, &next);
    if (next == (u32)-1) {
        mSerial.AddTail(MTaskExitTransfer::Create(this));
        return;
    }
    if (next != 0 && (next >> 16) == (mUnk80 >> 16)) {
        mUnk80 = next;
    }
    mUnk84 = mView->mView->GetUnkE4(mUnk80 >> 16)->GetUnkAC();
    int i = (u16)mUnk80 - 1;
    MTask *unmask, *hide, *pause, *change, *parent, *light, *mask, *show;
    show = ((M3DScene *)mView->GetUnkB0())->CreateEnableTask(TRUE);
    mask = new DinoBattleView3DMask((DinoBattleView3DMask::View *)mView, 1 << i);
    light = func_ov000_0218abd0(mView->mUnkE8, -1);
    parent = func_ov000_0218ac74(mView->mUnkE8, mView->mModels[i]);
    change = func_ov000_0218ac04(mView->mUnkE8, (u8 *)mView + 0x1E0, &data_ov000_02190334, mUnk84 == 0);
    pause = func_ov000_0218ac40(mView->mUnkE8, 0);
    hide = ((M3DScene *)mView->GetUnkB0())->CreateEnableTask(FALSE);
    unmask = new DinoBattleView3DMask((DinoBattleView3DMask::View *)mView, 0);
    MSerialTaskSet *serial = GetSerial();
    serial->AddTail(unmask);
    serial->AddTail(hide);
    serial->AddTail(pause);
    serial->AddTail(change);
    serial->AddTail(parent);
    serial->AddTail(light);
    serial->AddTail(mask);
    serial->AddTail(show);
    mState = data_ov000_021962e8.State_4C;
}

// Fades the status's creature group in.
void B3DStatusViewTask::StateFade() {
    mView->GetUnk19C(&mUnk7C, &mUnk80);
    if (mUnk80 == (u32)-1) {
        mSerial.AddTail(MTaskExitTransfer::Create(this));
        return;
    }
    mUnk84 = mView->mView->GetUnkE4(mUnk80 >> 16)->GetUnkAC();
    int i = (u16)mUnk80 - 1;
    GetSerial()->AddTail(new DinoBattleView3DMask((DinoBattleView3DMask::View *)mView, 1 << i),
                         func_ov000_0218ac04(mView->mUnkE8, (u8 *)mView + 0x1E0, &data_ov000_02190334, mUnk84 == 0),
                         new DinoBattleView3DGroupFade(mView->mUnkE8, (DinoBattleView3DGroupFade::Target *)mView,
                                                       mUnk84, 0, i));
    mState = data_ov000_021962e8.State_5C;
}

// Goes on to the next status: within the same group the cameras just resume, else the group fades.
// NONMATCHING: 98%, the fade's duration is loaded later than in the game (scheduling).
void B3DStatusViewTask::StateNext() {
    mView->GetUnk19C(&mUnk7C, &mUnk80);
    if (mUnk80 == (u32)-1) {
        mSerial.AddTail(MTaskExitTransfer::Create(this));
        return;
    }
    if ((mUnk80 >> 16) == (mUnk7C >> 16)) {
        GetSerial()->AddTail(func_ov000_0218ac40(mView->mUnkE8, 1), func_ov000_0218ac40(mView->mUnkE4, 1));
        mState = data_ov000_02196368.State_6C;
        return;
    }
    mUnk84 = mView->mView->GetUnkE4(mUnk80 >> 16)->GetUnkAC();
    GetSerial()->AddTail(new DinoBattleView3DGroupFade(mView->mUnkE4, (DinoBattleView3DGroupFade::Target *)mView,
                                                       mUnk84 == 0, 1, -1),
                         func_ov000_0218ac40(mView->mUnkE4, 1), func_ov000_0218ac40(mView->mUnkE8, 0));
    mState = data_ov000_02196368.State_4C;
}

// Shows the view's current status, then goes on to the next (fading in between different ones).
void B3DStatusViewTask::StateAdvance() {
    mView->SetUnk19C(mUnk80);
    mView->GetUnk19C(&mUnk7C, &mUnk80);
    if (mUnk80 == (u32)-1) {
        mSerial.AddTail(MTaskExitTransfer::Create(this));
        return;
    }
    if (mUnk7C != mUnk80) {
        mSerial.AddTail(new BlackInTask());
        mState = data_ov000_021962e8.State_6C;
        return;
    }
    Exit();
}

void B3DStatusViewTask::StateCheck() {
    mView->GetUnk19C(&mUnk7C, &mUnk80);
    if (mUnk80 == (u32)-1) {
        mSerial.AddTail(MTaskExitTransfer::Create(this));
        return;
    }
    if (mUnk7C == mUnk80) {
        mState = data_ov000_02196368.State_54;
        return;
    }
    mSerial.AddTail(new BlackInTask());
    mState = data_ov000_021962e8.State_74;
}


DinoBattleView3DDamage::~DinoBattleView3DDamage() {
    mSerial.Clear();
    if (mModel->GetUnk180()) {
        DinoSoundFunc_0201feb4(0);
    }
}

DinoBattleView3DAction::~DinoBattleView3DAction() {
    mSerial.Clear();
    if (mModel->GetUnk180()) {
        DinoSoundFunc_0201feb4(0);
    }
}

// Loads the action's motion.
void DinoBattleView3DAction::LoadCsv() {
    mCsv = new Dino3DLoadCsv(mUnk8C + mUnk88 * 10, mUnk78, mUnk98, (void *)5, mUnk84);
    mParallel.AddTail(mCsv);
}


DinoBattleView3DMask::~DinoBattleView3DMask() {
}

DinoBattleView3DGroupFade::~DinoBattleView3DGroupFade() {
    mSerial.Clear();
    mTarget->mUnk13C = mSaved;
}

BlackInTask::~BlackInTask() {
}

BlackOutTask::~BlackOutTask() {
}

MValChange::~MValChange() {
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
#pragma auto_inline off
template class MBgCallTask0<DinoBattleView3DAction>;
template MTask *MCreateBgCallTask(DinoBattleView3DAction *obj, void (DinoBattleView3DAction::*func)(), int thread);
template class MBgCallTask1<DinoBattleView3DGroupFade, int>;
template MTask *MCreateBgCallTask(DinoBattleView3DGroupFade *obj, void (DinoBattleView3DGroupFade::*func)(int), int a0, int thread);
template class MBgCallTask1<DinoBattleView3DDamage, int>;
template MTask *MCreateBgCallTask(DinoBattleView3DDamage *obj, void (DinoBattleView3DDamage::*func)(int), int a0, int thread);
template class MBgCallTask1<LoadWinnerLoad, int>;
template MTask *MCreateBgCallTask(LoadWinnerLoad *obj, void (LoadWinnerLoad::*func)(int), int a0, int thread);
#pragma auto_inline reset

extern "C" struct {
    void (DinoBattleView3DAction::*LoadCsv_00)();
} data_ov000_0219634c;
extern "C" const u32 data_0208a0d8;  // the action's fade length
extern "C" MTask *func_ov000_0218ae50(u32 arg);
extern "C" MTask *func_ov000_0218addc(u32 arg);

// The TU's pooled pointer-to-member constants for the hit's model loads.
extern "C" struct {
    void (DinoBattleView3DDamage::*LoadPath_00)(int i);
} data_ov000_02196364;
extern "C" struct {
    void (DinoBattleView3DDamage::*LoadName_00)(int i);
} data_ov000_0219633c;
extern "C" const char data_ov000_021965d0[];    // a hit model's name format
extern "C" const char data_ov000_02196800[], data_ov000_021967e4[];
extern "C" MTask *func_ov000_0218ab8c(DinoBattleView3DCamera *camera, const char *name, int light);
extern "C" const int data_0208a1d0;     // the held motion's fade-in length
extern "C" const u32 data_020b5a9c;     // (in the shared battle pool at data_020b5a90)

// Loads the hit's models, then plays them one step at a time with the hit marks (a button skips to the end), and
// wipes or fades out.
// NONMATCHING: 99.98%, the wipe-in mode's branches are the other way round, and the extra marks' x is spilled to
// the last stack slot instead of the first.
void DinoBattleView3DDamage::Idle() {
    switch (mStep) {
    case 0:
        mModel->SetUnk180(0);
        ((u8 *)mOwner)[0x144] = 0;
        mStep++;
        mMotionCount = 0;
        break;
    case 1:
        for (u32 i = 0; i < mDefs->mCount; i++) {
            sprintf(mNames[i], data_ov000_021965d0, mDefs->mUnkB8, mDefs->mUnkBC[i]);
            mCsvs[i] = new Dino3DLoadCsv(mOwner, 0x7FFF, (void *)0x81, (void *)mSide);
            mFiles[i] = NULL;
            if (i == mDefs->mCount - 1) {
                if (mDefs->mUnkA0) {
                    mFiles[i] = data_ov000_02196800;
                }
                if (mDefs->mUnkA4) {
                    mFiles[i] = data_ov000_021967e4;
                }
            }
            if (mDefs->mPaths[i] != NULL) {
                mSerial.AddTail(MCreateBgCallTask(this, data_ov000_02196364.LoadPath_00, (int)i, 1));
            } else {
                mSerial.AddTail(MCreateBgCallTask(this, data_ov000_0219633c.LoadName_00, (int)i, 1));
            }
        }
        mFrame = 0;
        mStep = 11;
        break;
    case 11:
        ((u8 *)mOwner)[0x144] = 1;
        mParallel.AddTail(new CameraWipe((CameraWipe::Owner *)mOwner, mSide ? 1 : 0, 15));
        mStep = 2;
        break;
    case 2:
        if (mFrame == 0) {
            mParallel.AddTail(func_ov000_0218ab8c(mCamera, mName, mSide));
        }
        mStep++;
        mFrame = 0;
        if (mShowMarks && mDefs->mType < 15) {
            mMotionCount = mDefs->mMarks[mIndex] + 1;
            switch (mDefs->mType) {
            case 0:
                mMotions[0] = new MMotion(std::string(data_ov000_021962e8.mHitMotion0_20), mDefs->mOam, 0, 1);
                mMarkDelay = 0;
                break;
            case 1:
                mMotions[0] = new MMotion(std::string(data_ov000_021962e8.mHitMotion1_40), mDefs->mOam, 0, 1);
                mMarkDelay = 1;
                break;
            case 2:
                mMotions[0] = new MMotion(std::string(data_ov000_021962e8.mHitMotion2_18), mDefs->mOam, 0, 1);
                mMarkDelay = 2;
                break;
            }
            const DinoBattleAdjustDefs *adjust = gDinoResource->GetBattleAdjustDefs();
            MPoint pos(adjust->damagePos.x, adjust->damagePos.y);
            int dx = mSide ? -40 : 40;
            pos.x += dx + (gDinoResource->GetRandom()->Next(80) - 40);
            pos.y += gDinoResource->GetRandom()->Next(56) + 8;
            mMotions[0]->SetPosition(pos);
            mMotions[0]->SetPriority(2);
            mMotions[0]->SetAnchor(2);
            mMotions[0]->Play(1, 0);
            mMotions[0]->Run();
            mMotions[0]->Hide();
            mDefs->mTasks->AddTail(mMotions[0]);
            mShown = 1;
            for (u32 i = 1; i < mMotionCount; i++) {
                mMotions[i] = new MMotion(std::string(data_ov000_021962e8.mMarkMotion_24), mDefs->mOam, 0, 1);
                mMotions[i]->SetPosition(pos.x + mDefs->mOffsets[(int)i - 1 + mIndex * 4], pos.y);
                mMotions[i]->SetPriority(2);
                mMotions[i]->SetAnchor(4);
                mDefs->mTasks->AddTail(mMotions[i]);
            }
        }
        if (mIndex != 0 && mDefs->mSes[mIndex - 1] != 0) {
            DinoSoundStopSe(mDefs->mSes[mIndex - 1], 0);
        }
        if (mDefs->mSes[mIndex] != 0) {
            DinoSoundPlaySe(mDefs->mSes[mIndex], 0);
        }
        if (mDefs->mSes2[mIndex] != 0) {
            DinoSoundPlaySe(mDefs->mSes2[mIndex], 0);
        }
        if (mIndex == mDefs->mCount - 1 && mDefs->mUnkA4) {
            DinoSoundPlaySe(0x2DF, 0);
        }
        if (mIndex != 0) {
            mCsvs[mIndex - 1]->Push11();
        }
        mCsvs[mIndex]->Push12();
        break;
    case 3: {
        mCamera->mCamera->SetUnk464(mCsvs[mIndex]);
        mCamera->mCamera->SetParent(mCsvs[mIndex]);
        if ((mInputHub->GetInput(0)->mButtons & 3) || (mInputHub->GetInput(0)->mButtons & 0x1000)) {
            mModel->SetUnk180(1);
        }
        if (mIndex != 0) {
            mCsvs[mIndex - 1]->Reset();
        }
        mCamera->mCamera->Push19(0x4B00);
        mMotions[0]->Show();
        Dino3DLoadCsv *csv = mCsvs[mIndex];
        if (csv != NULL && csv->mState == MTask::STATE_RUN) {
            csv->Execute();
        }
        if (mFrame > mStepFrames - 2 + (mIndex + 1 == mDefs->mCount) * 20 || (mModel->GetUnk180() && mFrame > 15)) {
            mIndex++;
            for (u32 i = 0; i < mMotionCount; i++) {
                mMotions[i]->Exit();
            }
            if (mIndex == mDefs->mCount || mModel->GetUnk180()) {
                if (mHold == 0 || mModel->GetUnk180()) {
                    if (mFade) {
                        mSerial.AddTail(func_ov000_0218addc(15));
                    } else {
                        mSerial.AddTail(new CameraWipe((CameraWipe::Owner *)mOwner, mSide ? 2 : 3, 15));
                    }
                    mStep++;
                    return;
                }
                mStep = 10;
                mFrame = 0;
                return;
            }
            if (mIndex < 3) {
                mCsvs[mIndex]->Push12();
            }
            mStep = 2;
            return;
        }
        if (mShown < mMotionCount) {
            if (mMarkDelay <= 0) {
                mMotions[mShown]->Show();
                mMotions[mShown]->Run();
                mMotions[mShown]->Play(mDefs->mAnimes[mShown - 1 + mIndex * 4], 0);
                mMarkDelay = 5;
                mShown++;
            }
            mMarkDelay--;
        }
        mFrame++;
        break;
    }
    case 4:
        for (u32 i = 0; i < mDefs->mCount; i++) {
            if (mCsvs[i] != NULL) {
                mCsvs[i]->Push11();
                if (mCsvs[i]->mState == MTask::STATE_RUN) {
                    mCsvs[i]->Execute();
                }
                mCsvs[i]->Reset();
                delete mCsvs[i];
            }
        }
        mCamera->mCamera->SetParent(NULL);
        Exit();
        break;
    case 10:
        if ((mInputHub->GetInput(0)->mButtons & 3) || (mInputHub->GetInput(0)->mButtons & 0x1000)) {
            mModel->SetUnk180(1);
        }
        mFrame++;
        if (mFrame > mHold || mModel->GetUnk180()) {
            if (mFade) {
                mSerial.AddTail(func_ov000_0218addc(15));
            } else {
                mSerial.AddTail(new MTimer(15), new CameraWipe((CameraWipe::Owner *)mOwner, mSide ? 2 : 3, 15));
            }
            mStep = 4;
            return;
        }
        u32 in, out;
        if (mFrame < data_0208a1d0) {
            in = (mFrame << 12) / data_0208a1d0;
            out = 0x1000;
        } else {
            in = 0x1000;
            out = 0x1000 - ((mFrame - data_0208a1d0) << 12) / (mHold - data_0208a1d0);
        }
        if (mCsvs[mIndex - 1] != NULL) {
            mCsvs[mIndex - 1]->Push15();
            mCsvs[mIndex - 1]->Push22(data_020b5a9c, in);
            mCsvs[mIndex - 1]->Push20(0x7FFF, out);
            if (mCsvs[mIndex - 1]->mState == MTask::STATE_RUN) {
                mCsvs[mIndex - 1]->Execute();
            }
        }
        break;
    }
}

// Loads and plays the action's motion until it ends (or a button skips it), then wipes or fades it out.
#pragma auto_inline off
void DinoBattleView3DAction::Idle() {
    switch (mStep) {
    case 0:
        ((u8 *)mUnk78)[0x144] = 0;
        mModel->SetUnk180(0);
        mStep++;
        break;
    case 1:
        if (mUnk88 == 0) {
            Exit();
            return;
        }
        mSerial.AddTail(MCreateBgCallTask(this, data_ov000_0219634c.LoadCsv_00, 1));
        mFrame = 0;
        mStep++;
        break;
    case 2:
        mCsv->Push19(0x4B00);
        mParallel.AddTail(func_ov000_0218ae50(data_0208a0d8));
        mStep++;
        break;
    case 3:
        ((u8 *)mUnk78)[0x144] = 1;
        if ((mInputHub->GetInput(0)->mButtons & 3) || (mInputHub->GetInput(0)->mButtons & 0x1000)) {
            mModel->SetUnk180(1);
        }
        if (mCsv->IsStopped() || mFrame > 300 || mCsv->IsUnk74Done() || mModel->GetUnk180()) {
            mStep++;
            if (mUnkE4) {
                GetSerial()->AddTail(new MTimer(15), func_ov000_0218addc(15));
            } else {
                GetSerial()->AddTail(new MTimer(15), new CameraWipe((CameraWipe::Owner *)mUnk78, mUnk84 ? 2 : 3, 15));
            }
        }
        mFrame++;
        break;
    case 4:
        mCsv->Push11();
        mCamera->Resume();
        mCsv->Reset();
        Exit();
        break;
    }
}
#pragma auto_inline reset

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.

// The TU's pooled pointer-to-member constant for the group's model loads.
extern "C" struct {
    void (DinoBattleView3DGroupFade::*LoadModel_00)(int i);
} data_ov000_0219637c;
extern "C" const char data_ov000_021965a0[];    // a creature's model name format
extern "C" const VecFx32 data_ov000_02190364[];  // the slot positions
extern "C" Dino3DLoadCsv *func_ov000_02187e68(DinoBattleView3D *view, int slot);

// Fades a side's group of three creatures out (unk7C 1) or in with the given models (unk7C 0), the camera
// following the group.
void DinoBattleView3DGroupFade::Idle() {
    switch (mUnk84) {
    case 0:
        if (mSaved != 0 && mSaved != 1) {
            Exit();
            return;
        }
        if (mUnk7C == 0) {
            for (int i = 0; i < 3; i++) {
                GetView()->mModels[i]->Push11();
            }
        }
        mUnk84++;
        break;
    case 1:
        if (mUnk7C == 0) {
            mSerial.AddTail(((M3DScene *)GetView()->mUnkB0)->CreateEnableTask(0));
            MSerialTaskSet *serial = &mSerial;
            int i = 0;
            char *name = mNames[0];
            for (; i < 3; i++) {
                sprintf(name, data_ov000_021965a0, GetView()->mCreatureIds[mSaved * 3 + i], 7);
                mUnk150[i] = GetView()->mModels[i]->GetUnkB0();
                serial->AddTail(MCreateBgCallTask(this, data_ov000_0219637c.LoadModel_00, i, 1));
                name += 64;
            }
        } else {
            mSerial.AddTail(func_ov000_0218addc(mDuration));
        }
        mUnk84 = 4;
        break;
    case 4:
        GetView()->SetGroupShown(mSaved, mUnk7C == 0);
        if (mUnk7C == 0) {
            for (int i = 0; i < 3; i++) {
                if (GetView()->mUnk14A[mSaved * 3 + i] == 0) {
                    GetView()->mModels[i]->Push11();
                } else {
                    GetView()->mModels[i]->Push12();
                }
                GetView()->mModels[i]->PushVec3(&data_ov000_02190364[(u32)GetView()->mSlotModels[mSaved * 3 + i]]);
                GetView()->mModels[i]->PushVec2(&data_ov000_0219034c);
                if (GetView()->mUnk150[mSaved * 3 + i] & 4) {
                    GetView()->mModels[i]->Push8(0);
                } else {
                    GetView()->mModels[i]->Push8(1);
                }
                GetView()->mModels[i]->Push7(mUnk150[i]);
            }
            if (mSlot == -1) {
                GetCamera()->Follow(func_ov000_02187e68(GetView(), mSaved));
            } else {
                GetCamera()->Follow(GetView()->mModels[mSlot]);
            }
            mSerial.AddTail(((M3DScene *)GetView()->mUnkB0)->CreateEnableTask(1));
            mSerial.AddTail(func_ov000_0218ae50(mDuration));
            ((M3DScene *)GetView()->mUnkB0)->mM3d->Show();
        } else {
            GetCamera()->Follow(NULL);
            for (int i = 0; i < 3; i++) {
                GetView()->mModels[i]->Push11();
            }
        }
        mUnk84++;
        break;
    case 5:
        if (mUnk7C == 1) {
            GetView()->mModels[0]->Reset();
            GetView()->mModels[1]->Reset();
            GetView()->mModels[2]->Reset();
            GetCamera()->Follow(NULL);
        }
        Exit();
        break;
    }
}

// The TU's pooled pointer-to-member constant for the winner's model load.
extern "C" struct {
    void (LoadWinnerLoad::*LoadModel_00)(int i);
} data_ov000_0219636c;
extern "C" const u32 data_0208a1d8;    // the pose's length

// Finds the winning side's creature with the slot model being looked for, loads its winner model and has the
// camera show it posing (with its cry) until the pose ends or the view is skipped.
void LoadWinnerLoad::Idle() {
    switch (mStep) {
    case 0:
        mWinner = -1;
        for (int i = 0; i < 3; i++) {
            if (mUnk84 == (int)mView->mSlotModels[mSlot * 3 + i] && mView->mUnk14A[mSlot * 3 + i] == 1) {
                mWinner = i;
                mStep++;
            }
        }
        mUnk84++;
        if (mUnk84 > 4) {
            mStep = 7;
            mSerial.AddTail(new CameraWipe((CameraWipe::Owner *)mView->mUnkB0, 0x10000 | (mSlot ? 2 : 3), 15));
        } else if (mWinner != -1) {
            mSerial.AddTail(new CameraWipe((CameraWipe::Owner *)mView->mUnkB0, 0x10000 | (mSlot ? 2 : 3), 15));
            mSerial.AddTail(func_ov000_0218ac74(mView->mUnkE4, NULL));
            mSerial.AddTail(((M3DScene *)mView->mUnkB0)->CreateEnableTask(0));
        }
        if (mSlot) {
            gLcd->mMain->SetBackdrop(0x8000);
        } else {
            gLcd->mMain->SetBackdrop(0xFFFF);
        }
        break;
    case 1:
        mStep++;
        break;
    case 2:
        mSerial.AddTail(MCreateBgCallTask(this, data_ov000_0219636c.LoadModel_00, mWinner, 1));
        mStep++;
        break;
    case 3: {
        MTask *follow;
        MTask *light;
        MTask *enable;
        ((DinoBattleView3DCamera *)mView->mUnkE4)->SetLight(mSlot == 0);
        mView->mModels[mWinner]->Push4(0);
        enable = ((M3DScene *)mView->mUnkB0)->CreateEnableTask(1);
        light = func_ov000_0218abd0(mView->mUnkE4, -1);
        follow = func_ov000_0218ac74(mView->mUnkE4, mView->mModels[mWinner]);
        GetSerial()->AddTail(func_ov000_0218ac04(mView->mUnkE4, &mView->mUnk1A4[1], &data_ov000_02190334, mSlot == 0),
                             follow, light, enable);
        mSerial.AddTail(new CameraWipe((CameraWipe::Owner *)mView->mUnkB0, 0x10000 | (mSlot ? 1 : 0), 15));
        mStep++;
        break;
    }
    case 4:
        mStep++;
        break;
    case 5: {
        MTask *cry = DinoSoundCreatePlaySeTask(
            (gDinoResource->GetCreatureDef(mView->mCreatureIds[mSlot * 3 + mWinner])->voice - 1) * 3 + 0x14F, 0);
        GetSerial()->AddTail(new MTimer(15), cry);
        mStep++;
        mTimer = data_0208a1d8 - 45;
        mView->SetUnk180(0);
        break;
    }
    case 6:
        if (--mTimer <= 0) {
            mStep = 0;
            return;
        }
        if (mView->GetUnk180()) {
            DinoSoundFunc_0201feb4(0);
            mStep = 0;
            mUnk84 = 3;
        }
        break;
    case 7:
        Exit();
        break;
    }
}

void MValChange::Execute() {
    *mDst = mValue;
    Exit();
}

void BlackInTask::Idle() {
    MTask *exit = MTaskExitTransfer::Create(this);
    MTask *task = func_ov000_0218ae50(mUnk74);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(task);
    serial->AddTail(exit);
}

void BlackOutTask::Idle() {
    MTask *exit = MTaskExitTransfer::Create(this);
    MTask *task = func_ov000_0218addc(mUnk74);
    MSerialTaskSet *serial = &mSerial;
    serial->AddTail(task);
    serial->AddTail(exit);
}

void DinoBattleView3D::Idle() {
    MMessageObserver::Dispatch();
    (this->*mState)();
}

void DinoBattleView3DMask::Execute() {
    for (int i = 0; i < 3; i++) {
        if (mMask & (1 << i)) {
            mView->mModels[i]->Push7(0x1000);
        } else {
            mView->mModels[i]->Push7(0);
        }
    }
    Exit();
}

#pragma auto_inline off

void DinoBattleView3D::GetUnk19C(u32 *a, u32 *b) {
    *a = mUnk19C;
    *b = mUnk1A0;
}

void DinoBattleView3D::SetUnk19C(u32 value) {
    mUnk19C = value;
}

u32 DinoBattleView3D::GetUnkB0() {
    return mUnkB0;
}

extern "C" u32 data_0208a0c0;
extern "C" const u32 data_0208a0e8;    // the ability's group fade length
extern "C" const u32 data_0208a118;    // how long a hit that knocks down is held
extern "C" const u32 data_0208a0bc;    // the turn end's length
extern "C" const u32 data_0208a0cc[2]; // [1]: its fades' length
extern "C" const u32 data_0208a0d0;    // (data_0208a0cc[1], which OnMessage reaches by its own address)
extern "C" const u32 data_0208a1e0, data_0208a0dc;  // before the winner's pose, by creatures left (1, 2; 3 is [0] above)
extern "C" const char *const data_ov000_0219654c[];  // the hit models, by hit (three each)

inline B3DStatusViewTask::B3DStatusViewTask(DinoBattleView3D *view) : MProcess(NULL) {
    mView = view;
    unk78 = 0;
    mState = data_ov000_02196368.StateCheck_2C;
}

inline LoadWinnerLoad::LoadWinnerLoad(DinoBattleView3D *view, int slot) : MProcess(NULL) {
    mView = view;
    mSlot = slot;
    mWinner = -1;
    mStep = 0;
    mUnk84 = 0;
    int shown = 0;
    for (int i = 0; i < 3; i++) {
        if (mView->mUnk14A[mSlot * 3 + i] == 1) {
            shown++;
        }
    }
    switch (shown) {
    case 1:
        mSerial.AddTail(new MTimer(data_0208a1e0));
        break;
    case 2:
        mSerial.AddTail(new MTimer(data_0208a0dc));
        break;
    case 3:
        mSerial.AddTail(new MTimer(data_0208a0cc[0]));
        break;
    }
}

// Whether the hit at the attack type's index is its last (the third, or the next is none).
static inline BOOL IsLastHit(int at, int i) {
    return i == 2 || gDinoResource->LoadBattleAttackType().Get()->GetHit(at + 1) == 0;
}

typedef DinoBattleView3DDamage::Defs DinoBattleView3DDamageDefs;
#define GROUP_FADE_TARGET (DinoBattleView3DGroupFade::Target *)this

// Queues the 3D scene's response to each battle message (abilities and team attacks with their damage
// cameras and marks, turns, status views, the winner's pose) and answers the sender when it asks.
// NONMATCHING (94%): the game's message dispatch tree and the hit sound switch (a jump table for 0-9) have other
// shapes, and in the ability's hit loop the game keeps &paths[i] in a register (our temporaries' flags take it).
void DinoBattleView3D::OnMessage(MMessage *msg) {
    switch (msg->mId) {
    case 0x10007: {
        DinoBattleViewCreatureBeginSignal *m = (DinoBattleViewCreatureBeginSignal *)msg;
        int i = (u16)m->mUnk10 - 1;
        int slot = mView->GetUnkE4(m->mUnk0C)->GetUnkAC();
        mCreatureIds[i + slot * 3] = (u8)*(u32 *)m->mUnk18;
        mUnk104[i + slot * 3] = (u8)*(u32 *)m->mUnk18;
        mSlotModels[i + slot * 3] = (void *)m->mUnk14;
        (mUnk14A + i)[slot * 3] = TRUE;
        if (mUnk174 != 0) {
            mUnkC0->AddTail(func_ov000_0218ac04(mUnkE4, &mUnk1A4[0], &data_ov000_02190334, -1));
        }
        mUnk174 = 0;
        break;
    }
    case 0x1000B: {
        DinoBattleViewEffectTurnBeginRequest *m = (DinoBattleViewEffectTurnBeginRequest *)msg;
        u32 side = mView->GetUnkE4(m->mUnk10)->GetUnkAC();
        mSlot = mUnk138 = side;
        CancelQueue();
        if (mUnk174 == 0) {
            mUnkC0->AddTail(func_ov000_0218addc(30));
        }
        mUnkC0->AddTail(new MValChange((int *)&mUnk140, 1),
                        func_ov000_0218ac04(mUnkE4, &mUnk1A4[2], &data_ov000_02190334, side == 0),
                        new DinoBattleView3DGroupFade(mUnkE4, GROUP_FADE_TARGET, side, 0, -1, -1),
                        new MValChange((int *)&mUnk140, 0));
        mUnk174 = 1;
        break;
    }
    case 0x1001D:
        break;
    case 0x10021: {
        DinoBattleViewCreatureMoveRequest *m = (DinoBattleViewCreatureMoveRequest *)msg;
        int i = (u16)m->mUnk14 - 1 + mView->GetUnkE4((s32)m->mUnk14 >> 16)->GetUnkAC() * 3;
        mUnkC0->AddTail(CreateMove(m->mUnk18, m->mUnk1C, i));
        if (m->mUnk18 != 3 || (m->mUnk1C != 1 && m->mUnk1C != 2)) {
            mUnkC0->AddTail(func_ov000_0218ac04(mUnkE4, &mUnk1A4[3], &data_ov000_02190334, mSlot == 0));
        }
        if (mUnk174 == 3) {
            mUnkC0->AddTail(func_ov000_0218ae50(data_0208a0c0));
        }
        mUnk174 = 3;
        break;
    }
    case 0x1002B: {
        DinoBattleViewCreatureAbility3DRequest *m = (DinoBattleViewCreatureAbility3DRequest *)msg;
        MMotion *line = NULL;
        int attacker = (u16)m->mUnk14 - 1 + mView->GetUnkE4((s32)m->mUnk14 >> 16)->GetUnkAC() * 3;
        int target = (u16)m->mUnk1C - 1 + mView->GetUnkE4((s32)m->mUnk1C >> 16)->GetUnkAC() * 3;
        SetUnk180(0);
        CancelQueue();
        if (mUnk174 == 3) {
            mUnkC0->AddTail(func_ov000_0218ae50(data_0208a0c0));
        }
        u32 kind = mUnk104[attacker];
        mUnk104[attacker] = mCreatureIds[attacker];
        u32 targetId = mCreatureIds[target];
        u32 number = ((m->mUnk18 - 1) & 7) + 1;
        mView->GetUnkE4((s32)m->mUnk14 >> 16)->GetUnkAC();
        mView->GetUnkE4((s32)m->mUnk1C >> 16)->GetUnkAC();
        BOOL marks = TRUE;
        u32 hits = 0;
        DinoBattleView3DDamageDefs *defs = new DinoBattleView3DDamageDefs;
        u32 camera = 0;
        defs->mType = 0xFFFF;
        defs->mCount = 0;
        int i = 0;
        defs->mPaths[0] = defs->mPaths[1] = defs->mPaths[2] = defs->mPaths[3] = NULL;
        defs->mUnkB8 = targetId;
        const char *paths[4];
        paths[0] = paths[1] = paths[2] = paths[3] = NULL;
        if ((u16)m->mUnk14 != 0) {
            int base = (kind - 1) * 20 + (number - 1) * 3 + 2;
            for (i = 0; i < 3; i++) {
                s8 hit = gDinoResource->LoadBattleAttackType().Get()->GetHit(base + i);
                if (hit == 0) {
                    break;
                }
                hits++;
                defs->mCount++;
                defs->mSes[i] = 0;
                defs->mSes2[i] = 0;
                int h = (hit - 1) * 3;
                if (m->mUnk20 == 0) {
                    camera = 7;
                    defs->mType = 0;
                    defs->mPaths[i] = paths[i];
                    defs->mUnkBC[i] = camera;
                    defs->mSes2[i] = 0x2DD;
                    break;
                }
                defs->mSes[i] = (gDinoResource->GetCreatureDef(targetId)->voice - 1) * 3 + 0x150;
                if (m->mUnk38 && IsLastHit(base + i, i)) {
                    defs->mSes[i] = (gDinoResource->GetCreatureDef(targetId)->voice - 1) * 3 + 0x151;
                }
                switch (m->mUnk20) {
                case 1:
                    camera = 5;
                    defs->mSes2[i] = 0x2D7;
                    paths[i] = m->mUnk38 ? data_ov000_0219654c[h + 2] : data_ov000_0219654c[h];
                    defs->mType = 1;
                    if (m->mUnk38 && IsLastHit(base + i, i)) {
                        camera = 6;
                    }
                    break;
                case 2:
                    camera = 5;
                    defs->mSes2[i] = 0x2D6;
                    paths[i] = data_ov000_0219654c[h + 1];
                    defs->mType = 1;
                    if (m->mUnk38 && IsLastHit(base + i, i)) {
                        camera = 6;
                    }
                    break;
                case 3:
                    if (IsLastHit(base + i, i)) {
                        camera = 6;
                        defs->mSes2[i] = 0x2D5;
                        paths[i] = data_ov000_0219654c[h + 2];
                        defs->mType = 2;
                    } else {
                        camera = 5;
                        defs->mSes2[i] = 0x2D6;
                        paths[i] = data_ov000_0219654c[h + 1];
                        defs->mType = 1;
                    }
                    break;
                case 5:
                    camera = 5;
                    defs->mSes2[i] = 0x2D6;
                    defs->mType = 1;
                    break;
                case 4:
                    camera = 5;
                    defs->mSes2[i] = 0x2D6;
                    marks = FALSE;
                    break;
                case 6:
                    camera = 0;
                    marks = FALSE;
                    break;
                default:
                    marks = FALSE;
                    break;
                }
                defs->mPaths[i] = paths[i];
                defs->mUnkBC[i] = camera;
                switch (h) {
                case 0:
                case 3:
                    break;
                case 6:
                    defs->mSes2[i] = 0x2D9;
                    break;
                case 9:
                    defs->mSes2[i] = 0x2DB;
                    break;
                case 12:
                    defs->mSes2[i] = 0x2DC;
                    break;
                case 15:
                    defs->mSes2[i] = 0x2DA;
                    break;
                case 18:
                    defs->mSes2[i] = 0x2D8;
                    break;
                }
            }
        }
        defs->mOam = mOam;
        defs->mTasks = mUnkBC;
        defs->mUnkA0 = 0;
        defs->mUnkA4 = 0;
        if (marks) {
            // (A hit of no damage takes one from another.)
            switch (hits) {
            case 1:
                break;
            case 2:
                if (m->mUnk2C[1] == 0 && m->mUnk2C[0] != 0) {
                    m->mUnk2C[1]++;
                    m->mUnk2C[0]--;
                }
                break;
            case 3:
                if (m->mUnk2C[1] == 0 && m->mUnk2C[0] != 0) {
                    m->mUnk2C[1]++;
                    m->mUnk2C[0]--;
                }
                if (m->mUnk2C[2] == 0) {
                    if (m->mUnk2C[0] != 0) {
                        m->mUnk2C[2]++;
                        m->mUnk2C[0]--;
                    } else if (m->mUnk2C[1] != 0) {
                        m->mUnk2C[2]++;
                        m->mUnk2C[1]--;
                    }
                }
                break;
            }
            // Each hit's damage digits, lowest first.
            for (u32 j = 0; j < hits; j++) {
                u32 damage = m->mUnk2C[j];
                defs->mMarks[j] = 4;
                for (int d = 0; d < 4; d++) {
                    if (damage == 0) {
                        defs->mMarks[j] = d;
                        break;
                    }
                    defs->mAnimes[j * 4 + d] = damage % 10 + 1;
                    damage /= 10;
                }
                switch (defs->mMarks[j]) {
                case 0:
                    defs->mMarks[j] = 1;
                    defs->mAnimes[j * 4] = 1;
                    defs->mOffsets[j * 4] = 0;
                    break;
                case 1:
                    defs->mOffsets[j * 4] = 0;
                    break;
                case 2:
                    defs->mOffsets[j * 4] = 8;
                    defs->mOffsets[j * 4 + 1] = -8;
                    break;
                case 3:
                    defs->mOffsets[j * 4] = 16;
                    defs->mOffsets[j * 4 + 1] = 0;
                    defs->mOffsets[j * 4 + 2] = -16;
                    break;
                case 4:
                    defs->mOffsets[j * 4] = 24;
                    defs->mOffsets[j * 4 + 1] = 8;
                    defs->mOffsets[j * 4 + 2] = -8;
                    defs->mOffsets[j * 4 + 3] = -24;
                    break;
                }
            }
        }
        if (m->mUnk45) {
            if (m->mUnk4C == 1) {
                line = new MMotion(std::string(data_ov000_021962e8.mLineMotion_30), mOam, 0, 1);
                line->SetPosition(128, 96);
                line->Play(1, 0);
                line->Hide();
                line->Suspend();
                mUnkBC->AddTail(line);
                defs->mUnkA0 = 1;
            }
            if (m->mUnk50 > 1) {
                defs->mUnkA4 = 1;
            }
            mUnkC0->AddTail(((M3DScene *)mUnkB0)->mM3d->CreateShowTransfer(),
                            new DinoBattleView3DGroupFade(mUnkE4, GROUP_FADE_TARGET, mSlot, 1, -1, data_0208a0e8),
                            func_ov000_0218ac40(mUnkE4, 1));
            if (m->mUnk4C == 1) {
                mUnkC0->AddTail(MTaskRunTransfer::Create(line), line->CreateShowTransfer());
            }
            mUnkC0->AddTail(new DinoBattleView3DAction(this, (void *)mUnkB0, mSlot, mUnkE4, camera == 0, kind, number));
        }
        if (camera != 0) {
            s8 type = gDinoResource->LoadBattleAttackType().Get()->GetHit((targetId - 1) * 20);
            char name[64];
            if (m->mUnk20 == 0) {
                sprintf(name, data_ov000_021962e8.mDamageMiss_48, type);
                defs->mSes[0] = 0;
                defs->mSes2[0] = 0x2DD;
                defs->mMarks[0] = 0;
            } else {
                switch (hits) {
                case 0:
                    sprintf(name, data_ov000_021962e8.mDamageMiss_48, type);
                    break;
                case 1:
                    sprintf(name, data_ov000_021962e8.mDamageOne_1C, type);
                    break;
                case 2:
                    sprintf(name, data_ov000_021962e8.mDamageTwo_08, type);
                    break;
                case 3:
                    sprintf(name, data_ov000_021962e8.mDamageThree_0C, type);
                    break;
                }
            }
            mUnkC0->AddTail(new DinoBattleView3DDamage(this, (void *)mUnkB0, mSlot, mUnkE4, defs, name, TRUE,
                                                       m->mUnk38 ? data_0208a118 : 0),
                            new DelDmgMotionTask(defs));
        } else {
            delete defs;
        }
        if (line != NULL) {
            mUnkC0->AddTail(MTaskExitTransfer::Create(line));
        }
        mUnkC0->AddTail(((MMessageObserver *)m->mUnk0C)->CreatePostTask(new DinoBattleViewCreatureAbility3DResult()));
        mUnkC0->AddTail(func_ov000_0218ac40(mUnkE4, 0));
        mUnkC0->AddTail(func_ov000_0218abd0(mUnkE4, mSlot == 0),
                        new DinoBattleView3DGroupFade(mUnkE4, GROUP_FADE_TARGET, mSlot, 0, -1, -1));
        mUnk174 = 4;
        break;
    }
    case 0x1002D: {
        DinoBattleView3DTeamAttackRequest *m = (DinoBattleView3DTeamAttackRequest *)msg;
        SetUnk180(0);
        CancelQueue();
        if (mUnk174 == 3) {
            mUnkC0->AddTail(func_ov000_0218ae50(data_0208a0c0));
        }
        int attacker = (u16)m->mUnk14 - 1 + mView->GetUnkE4((s32)m->mUnk14 >> 16)->GetUnkAC() * 3;
        u32 kind = mUnk104[attacker];
        mUnk104[attacker] = mCreatureIds[attacker];
        u32 number = ((m->mUnk18 - 1) & 7) + 1;
        mUnkC0->AddTail(((M3DScene *)mUnkB0)->mM3d->CreateShowTransfer(),
                        new DinoBattleView3DGroupFade(mUnkE4, GROUP_FADE_TARGET, mSlot, 1, -1, data_0208a0e8),
                        func_ov000_0218ac40(mUnkE4, 1),
                        new DinoBattleView3DAction(this, (void *)mUnkB0, mSlot, mUnkE4, FALSE, kind, number));
        for (int t = 0; t < (int)m->mUnk1C; t++) {
            int hit = gDinoResource->LoadBattleAttackType().Get()->GetHit((kind - 1) * 20 + (number - 1) * 3 + 2);
            u32 target = m->mUnk20[t];
            u32 targetId = mCreatureIds[(u16)target - 1 + mView->GetUnkE4((s32)target >> 16)->GetUnkAC() * 3];
            if (hit == 0) {
                hit++;
            }
            int h = (hit - 1) * 3;
            DinoBattleView3DDamageDefs *defs = new DinoBattleView3DDamageDefs;
            defs->mSes[0] = 0;
            defs->mSes2[0] = 0;
            const char *path = NULL;
            u32 camera = 0;
            defs->mType = 0xFFFF;
            defs->mOam = mOam;
            BOOL marks = TRUE;
            defs->mTasks = mUnkBC;
            if (m->mUnk44[t]) {
                defs->mSes[0] = (gDinoResource->GetCreatureDef(targetId)->voice - 1) * 3 + 0x151;
            } else {
                defs->mSes[0] = (gDinoResource->GetCreatureDef(targetId)->voice - 1) * 3 + 0x150;
            }
            switch (m->mUnk2C[t]) {
            case 0:
                camera = 7;
                defs->mType = 0;
                defs->mSes2[0] = 0x2DD;
                break;
            case 1:
                camera = 5;
                path = data_ov000_0219654c[h];
                defs->mType = 1;
                defs->mSes2[0] = 0x2D7;
                break;
            case 2:
                camera = 5;
                path = data_ov000_0219654c[h + 1];
                defs->mType = 1;
                defs->mSes2[0] = 0x2D6;
                break;
            case 3:
                camera = 6;
                path = data_ov000_0219654c[h + 2];
                defs->mType = 2;
                defs->mSes2[0] = 0x2D5;
                break;
            case 5:
                camera = 5;
                defs->mType = 1;
                defs->mSes2[0] = 0x2D6;
                break;
            case 4:
                camera = 5;
                defs->mSes2[0] = 0x2D6;
                marks = FALSE;
                break;
            case 6:
                marks = FALSE;
                camera = 0;
                break;
            default:
                marks = FALSE;
                break;
            }
            switch (h) {
            case 0:
            case 3:
                break;
            case 6:
                defs->mSes2[0] = 0x2D9;
                break;
            case 9:
                defs->mSes2[0] = 0x2DB;
                break;
            case 12:
                defs->mSes2[0] = 0x2DC;
                break;
            case 15:
                defs->mSes2[0] = 0x2DA;
                break;
            case 18:
                defs->mSes2[0] = 0x2D8;
                break;
            }
            if (m->mUnk2C[t] == 0) {
                defs->mSes[0] = 0;
                defs->mSes2[0] = 0x2DD;
            }
            if (marks) {
                u32 damage = m->mUnk38[t];
                defs->mMarks[0] = 4;
                for (int d = 0; d < 4; d++) {
                    if (damage == 0) {
                        defs->mMarks[0] = d;
                        break;
                    }
                    defs->mAnimes[d] = damage % 10 + 1;
                    damage /= 10;
                }
                switch (defs->mMarks[0]) {
                case 0:
                    defs->mMarks[0] = 1;
                    defs->mAnimes[0] = 1;
                    defs->mOffsets[0] = 0;
                    break;
                case 1:
                    defs->mOffsets[0] = 0;
                    break;
                case 2:
                    defs->mOffsets[0] = 8;
                    defs->mOffsets[1] = -8;
                    break;
                case 3:
                    defs->mOffsets[0] = 16;
                    defs->mOffsets[1] = 0;
                    defs->mOffsets[2] = -16;
                    break;
                case 4:
                    defs->mOffsets[0] = 24;
                    defs->mOffsets[1] = 8;
                    defs->mOffsets[2] = -8;
                    defs->mOffsets[3] = -24;
                    break;
                }
            }
            if (camera != 0) {
                defs->mUnkB8 = targetId;
                defs->mUnkBC[0] = camera;
                s8 type = gDinoResource->LoadBattleAttackType().Get()->GetHit((targetId - 1) * 20);
                char name[64];
                if (m->mUnk2C[t] == 0) {
                    sprintf(name, data_ov000_021962e8.mDamageMiss_48, type);
                    defs->mMarks[0] = 0;
                } else {
                    sprintf(name, data_ov000_021962e8.mDamageOne_1C, type);
                }
                defs->mPaths[0] = path;
                defs->mCount = 1;
                defs->mUnkA0 = 0;
                defs->mUnkA4 = 0;
                mUnkC0->AddTail(new DinoBattleView3DDamage(this, (void *)mUnkB0, mSlot, mUnkE4, defs, name,
                                                           t == m->mUnk1C - 1, m->mUnk44[t] ? data_0208a118 : 0),
                                new DelDmgMotionTask(defs));
            } else {
                delete defs;
            }
        }
        mUnkC0->AddTail(((MMessageObserver *)m->mUnk0C)->CreatePostTask(new DinoBattleView3DTeamAttackResult()));
        mUnkC0->AddTail(func_ov000_0218ac40(mUnkE4, 0));
        mUnkC0->AddTail(func_ov000_0218abd0(mUnkE4, mSlot == 0),
                        new DinoBattleView3DGroupFade(mUnkE4, GROUP_FADE_TARGET, mSlot, 0, -1, -1));
        mUnk174 = 4;
        break;
    }
    case 0x10033: {
        DinoBattleViewCreatureEndRequest *m = (DinoBattleViewCreatureEndRequest *)msg;
        CancelQueue();
        mUnk14A[(u16)m->mUnk14 - 1 + mView->GetUnkE4((s32)m->mUnk14 >> 16)->GetUnkAC() * 3] = FALSE;
        if (mSlot != mView->GetUnkE4((s32)m->mUnk14 >> 16)->GetUnkAC()) {
            break;
        }
        mUnkC0->AddTail(new BlackOutTask(15), new DinoBattleView3DGroupFade(mUnkE4, GROUP_FADE_TARGET, mSlot, 0, -1, -1));
        break;
    }
    case 0x1002F: {
        DinoBattleViewCreatureUpdateRequest *m = (DinoBattleViewCreatureUpdateRequest *)msg;
        mUnk150[(u16)m->mUnk10 - 1 + mView->GetUnkE4((s32)m->mUnk10 >> 16)->GetUnkAC() * 3] = m->mUnk38;
        break;
    }
    case 0x10010:
        if (mUnk174 != 2 && mUnk174 != 4 && mUnk174 != 3 && mUnk174 != 1 && mUnk174 != 5) {
            mUnkC0->AddTail(func_ov000_0218ac04(mUnkE4, &mUnk1A4[2], &data_ov000_02190334, -1));
        }
        if (mUnk174 == 5) {
            CancelQueue();
            mUnkC0->AddTail(new DinoBattleView3DGroupFade(mUnkE8, GROUP_FADE_TARGET, mUnk138, 1, -1, -1));
            mUnkC0->AddTail(func_ov000_0218ac40(mUnkE4, 0),
                            new DinoBattleView3DMask((DinoBattleView3DMask::View *)this, 7),
                            func_ov000_0218ac40(mUnkE8, 1));
            mUnkC0->AddTail(new DinoBattleView3DGroupFade(mUnkE4, GROUP_FADE_TARGET, mSlot, 0, -1, -1));
            mUnk138 = mSlot;
        }
        mUnkC0->AddTail(func_ov000_0218ae50(data_0208a0c0));
        mUnk174 = 2;
        break;
    case 0x10077: {
        DinoBattleView3DPoisonDamageRequest *m = (DinoBattleView3DPoisonDamageRequest *)msg;
        mUnkC0->AddTail(new MTimer(10));
        if (m->mUnk0C == 0) {
            break;
        }
        mUnkC0->AddTail(((MMessageObserver *)m->mUnk0C)->CreatePostTask(new DinoBattleView3DPoisonDamageResult()));
        break;
    }
    case 0x1007B: {
        DinoBattleView3DTurnEndRequest *m = (DinoBattleView3DTurnEndRequest *)msg;
        int side = mView->GetUnkE4(m->mUnk10)->GetUnkAC();
        FlushQueue();
        if (mUnk140 == 0) {
            mUnkC0->AddTail(func_ov000_0218ae50(data_0208a0c0));
            mUnkC0->AddTail(new BlackOutTask(data_0208a0d0),
                            func_ov000_0218ac04(mUnkE4, &mUnk1A4[4], &data_ov000_02190334, side == 0),
                            new BlackInTask(data_0208a0d0), new MTimer(data_0208a0bc - data_0208a0d0));
        }
        mUnkC0->AddTail(new DinoBattleView3DGroupFade(mUnkE4, GROUP_FADE_TARGET, mSlot, 1, -1, -1));
        if (m->mUnk0C != 0) {
            mUnkC0->AddTail(((MMessageObserver *)m->mUnk0C)->CreatePostTask(new DinoBattleView3DTurnEndResult()));
        }
        break;
    }
    case 0x10073: {
        DinoBattleView3DSupportEffectRequest *m = (DinoBattleView3DSupportEffectRequest *)msg;
        if (m->mUnk0C == 0) {
            break;
        }
        mUnkC0->AddTail(((MMessageObserver *)m->mUnk0C)->CreatePostTask(new DinoBattleView3DSupportEffectResult()));
        break;
    }
    case 0x10075: {
        DinoBattleView3DWatchStatusRequest *m = (DinoBattleView3DWatchStatusRequest *)msg;
        mView->GetUnkE4((s32)m->mUnk10 >> 16)->GetUnkAC();
        if (mUnk174 != 5) {
            mUnk19C = -1;
            mUnk1A0 = -1;
        }
        mUnk1A0 = m->mUnk10;
        if (m->mUnk10 != 0) {
            if (mQueue.empty()) {
                mQueue.push_back(new B3DStatusViewTask(this));
            }
        } else {
            mUnkC0->AddTail(new BlackInTask(),
                            func_ov000_0218ac04(mUnkE4, &mUnk1A4[2], &data_ov000_02190334, mSlot == 0));
        }
        if (m->mUnk0C != 0) {
            mUnkC0->AddTail(((MMessageObserver *)m->mUnk0C)->CreatePostTask(new DinoBattleView3DWatchStatusResult()));
        }
        mUnk174 = 5;
        break;
    }
    case 0x1008B: {
        DinoBattleView3DGameSetRequest *m = (DinoBattleView3DGameSetRequest *)msg;
        int side = mView->GetUnkE4(m->mUnk10)->GetUnkAC();
        CancelQueue();
        mUnkC0->AddTail(new LoadWinnerLoad(this, side));
        if (m->mUnk0C == 0) {
            break;
        }
        mParallel.AddTail(((MMessageObserver *)m->mUnk0C)->CreatePostTask(new DinoBattleView3DGameSetResult()));
        break;
    }
    case 0x10090: {
        DinoBattleView3DRevolutionSetRequest *m = (DinoBattleView3DRevolutionSetRequest *)msg;
        int i = (u16)m->mUnk10 - 1 + mView->GetUnkE4((s32)m->mUnk10 >> 16)->GetUnkAC() * 3;
        mUnk104[i] = mCreatureIds[i];
        mCreatureIds[i] = m->mUnk14;
        if (m->mUnk0C == 0) {
            break;
        }
        mUnkC0->AddTail(((MMessageObserver *)m->mUnk0C)->CreatePostTask(new DinoBattleView3DRevolutionSetResult()));
        break;
    }
    case 0x100A2:
        SetUnk180(1);
        break;
    }
}

DelDmgMotionTask::~DelDmgMotionTask() {
    delete mUnk10;
}

void DelDmgMotionTask::Execute() {
    Exit();
}

#pragma auto_inline off

void DinoBattleView3D::SetUnk180(u8 value) {
    mUnk180 = value;
    if (value && !mView->GetUnk138()) {
        DinoSoundFunc_0201feb4(0);
    }
}

u8 DinoBattleView3D::GetUnk180() {
    if (mView->GetUnk138()) {
        return FALSE;
    }
    return mUnk180;
}

// A 16-byte-element vector the game emits members of in this file.
#pragma auto_inline off
template class std::__vec_deleter<DinoBattleView3DEntry, std::allocator<DinoBattleView3DEntry> >;

// A 52-byte-element vector the game appends to in this file.
template void std::__vec_deleter<DinoBattleView3DRecord, std::allocator<DinoBattleView3DRecord> >::append_realloc(size_t n, const DinoBattleView3DRecord &x);

// A vector of string lists (its elements are destroyed with vector<string>'s out-of-line destructor).
// NONMATCHING (both erase_at_end, callcheck only): our vector<string> destructor is inlined into a
// __vec_deleter<string> D2 call, so the bytes match but the call target differs (see the vector<string> note).
template class std::__vec_deleter<std::vector<std::string>, std::allocator<std::vector<std::string> > >;

#pragma auto_inline off

// The 3D processes' states (called through pointers to members).
extern "C" void func_ov000_02184cf0(DinoBattleView3DGroupFade *fade, int i) {
    fade->mTarget->mModels[i]->LoadByName(fade->mNames[i], TRUE, 0x0FFFFFFF);
}

extern "C" void func_ov000_02184d64(DinoBattleView3DDamage *damage, int i) {
    damage->mCsvs[i]->LoadByName(damage->mNames[i], TRUE, 0x0FFFFFFF);
    if (damage->mFiles[i] != NULL) {
        damage->mCsvs[i]->LoadFile(damage->mFiles[i], NULL, 0);
    }
}

extern "C" void func_ov000_02184da0(DinoBattleView3DDamage *damage, int i) {
    damage->mCsvs[i]->Load(damage->mDefs->mPaths[i], (u32)damage->mNames[i]);
    if (damage->mFiles[i] != NULL) {
        damage->mCsvs[i]->LoadFile(damage->mFiles[i], NULL, 0);
    }
}

// A model free in the slot (the current one for -1): the last with nothing on it.
extern "C" Dino3DLoadCsv *func_ov000_02187e68(DinoBattleView3D *view, int slot) {
    Dino3DLoadCsv *model = NULL;
    if (slot < 0) {
        slot = view->mSlot;
    }
    for (int i = 0; i < 3; i++) {
        if (view->mSlotModels[slot * 3 + i] == NULL) {
            model = view->mModels[i];
        }
    }
    return model;
}

extern "C" void func_ov000_0218ada0();
extern "C" const char *const data_ov000_02196514[6];    // each kind's motion file name format (by id, number)

// Sets up the scene and the task sets, and for each kind lists the motion files present for each id (0x61-0x67, up
// to ten numbers each).
// NONMATCHING: 99.4%, two of the inlined vector byte temporaries take each other's stack slots (the copied names'
// init_copy tag and the slow push's tag).
DinoBattleView3D::DinoBattleView3D(DinoBattleView *view) : MProcess(NULL), mView(view) {
    int i;
    func_ov000_0218ada0();
    mUnkCC = new MParallelTaskSet();
    mParallel.AddTail(mUnkCC);
    mUnkC0 = new MSerialTaskSet();
    mParallel.AddTail(mUnkC0);
    mUnkB0 = (u32)new M3DScene();
    mParallel.AddTail((M3DScene *)mUnkB0);
    ((M3DScene *)mUnkB0)->mM3d->mBgCntBits.priority = 3;
    Dino3DClearSoundNames();
    for (i = 0; i < 5; i++) {
        mModels[i] = NULL;
    }
    for (i = 0; i < 6; i++) {
        mCreatureIds[i] = 0;
        mSlotModels[i] = NULL;
    }
    for (i = 0; i < 6; i++) {
        mUnk14A[i] = 0;
        mUnk150[i] = 0;
    }
    for (i = 0; i < 2; i++) {
        mUnk148[i] = 0;
    }
    mUnkB4 = 0;
    mSlot = -1;
    mUnk138 = -1;
    mUnk13C = -1;
    mUnk16C = -1;
    mUnk144 = 0;
    mUnk170 = 0;
    mUnkE4 = NULL;
    mUnkE8 = NULL;
    mUnk140 = 0;
    mUnk19C = -1;
    mUnk1A0 = -1;
    for (int k = 0; k < 6; k++) {
        mUnk1A4[k].clear();
        for (int id = 0x61; id <= 0x67; id++) {
            char buf[0x40];
            std::vector<std::string> names;
            for (int n = 0; n < 10; n++) {
                OS_SPrintf(buf, data_ov000_02196514[k], id, n);
                std::string path(buf);
                if (MFileExists(path)) {
                    names.push_back(path);
                }
            }
            if (names.size() != 0) {
                mUnk1A4[k].push_back(names);
            }
        }
    }
    mOam = new MSmartOam(gLcd->mMain, 0x10, 0, TRUE);
    mUnkBC = new MParallelTaskSet();
    mParallel.AddTail(mUnkBC, mOam);
    mState = data_ov000_02196368.StateLoad_1C;
}

// Deletes the tasks waiting to run.
void DinoBattleView3D::CancelQueue() {
    while (!mQueue.empty()) {
        MTask *task = mQueue.front();
        mQueue.pop_front();
        delete task;
    }
    mUnk19C = -1;
    mUnk1A0 = -1;
}

extern "C" u32 data_0208a0c0;
extern "C" u8 data_ov000_02196cb4[];
extern "C" Dino3DLoadCsv *func_ov000_02187e68(DinoBattleView3D *view, int slot);

// Shows something on the slot's models: the camera follows a free model, and (on the current side) one of the
// models moves.
MTask *DinoBattleView3D::CreateMove(int unkA, u32 shown, int slot) {
    mSlotModels[slot] = (void *)shown;
    Dino3DLoadCsv *model = func_ov000_02187e68(this, -1);
    CameraSetDefParentTask::Camera *camera = (CameraSetDefParentTask::Camera *)mUnkE4;
    camera->mParent = model;
    camera->mUnk7C->SetParent(camera->mParent);
    if (slot / 3 == mSlot) {
        MSerialTaskSet *set = new MSerialTaskSet();
        DinoBattleView3DMoveParams params;
        params.unk0 = mCreatureIds[slot];
        params.unk4 = 8;
        params.unk10 = 0x7FFF;
        params.unk8 = 0;
        params.unkA = unkA;
        params.unkC = shown;
        params.unkE = 20;
        set->mExitWhenEmpty = TRUE;
        MTask *move = new DinoBattleView3DMove(&params, mUnkB0, mModels[slot % 3], NULL, data_ov000_02196cb4, 0);
        set->AddTail(func_ov000_0218addc(data_0208a0c0));
        set->AddTail(move);
        return set;
    }
    return new MTimer(1);
}

extern "C" const char data_ov000_02196c78[];   // the field's csv
extern "C" const char data_ov000_02196c94[];   // the default background's csv
extern "C" const VecFx32 data_ov000_02190340;

void DinoBattleView3D::StateLoad() {
    mState = data_ov000_02196368.RunNextQueued_3C;
    mField = new Dino3DLoadCsv(data_ov000_02196c78, (void *)mUnkB0, 0x7FFF, (void *)4, NULL, 0);
    mUnkCC->AddTail(mField);
    std::string path;
    const DinoBattleBgDefs *defs = gDinoResource->GetBattleBgDefs();
    for (u32 i = 0; i < defs->count; i++) {
        u32 battle = defs->Get(i)->battle;
        u32 current = mView->GetUnk134();
        if (current == battle) {
            const DinoBattleBgDef *def = defs->Get(i);
            path = (const char *)def + def->csvPath;
            break;
        }
    }
    if (path.empty()) {
        path = data_ov000_02196c94;
    }
    mBackground = new Dino3DLoadCsv(path.c_str(), (void *)mUnkB0, 0x7FFF, (void *)2, NULL, 0x3F);
    mBackground->Push7(0x1000);
    mBackground->PushVec3(&data_ov000_02190340);
    mBackground->Push21(0);
    mUnkCC->AddTail(mBackground);
    for (int i = 0; i < 3; i++) {
        mModels[i] = new Dino3DLoadCsv((void *)mUnkB0, 0x7FFF, (void *)1, NULL);
        mUnkCC->AddTail(mModels[i]);
    }
    SetUnk180(0);
    mUnkE4 = new DinoBattleView3DCamera((M3DScene *)mUnkB0, mField, (u32)mUnk1A4);
    mParallel.AddTail(mUnkE4);
    mUnkE8 = new DinoBattleView3DCamera((M3DScene *)mUnkB0, mField, (u32)mUnk1A4);
    mParallel.AddTail(mUnkE8);
    ((DinoBattleView3DCamera *)mUnkE8)->Stop();
    mUnk174 = 6;
    ((M3DScene *)mUnkB0)->mM3d->Show();
    ((M3DScene *)mUnkB0)->mUnk10 = 1;
}

// Starts the next waiting task once the task set is idle.
void DinoBattleView3D::RunNextQueued() {
    if (!mQueue.empty() && mUnkC0->IsEmpty()) {
        MTask *task = mQueue.front();
        mQueue.pop_front();
        mUnkC0->AddTail(task);
    }
}

// Hands the tasks waiting to run to the task set.
void DinoBattleView3D::FlushQueue() {
    while (!mQueue.empty()) {
        MTask *task = mQueue.front();
        mQueue.pop_front();
        mUnkC0->AddTail(task);
    }
}

// The queue's members the game emits in this file.
template std::__deque_deleter<MTask *, std::allocator<MTask *> >::~__deque_deleter();
template std::__deque_deleter<MTask *, std::allocator<MTask *> >::iterator std::__deque_deleter<MTask *, std::allocator<MTask *> >::make_iter(size_t n);
template void std::__deque_deleter<MTask *, std::allocator<MTask *> >::push_back(MTask *const &x, Metrowerks::int2type<true>);
template void std::__deque_deleter<MTask *, std::allocator<MTask *> >::erase_front(size_t n, Metrowerks::int2type<true>);

// The file's static initialization (see dino_static_init.h).
extern "C" u32 data_ov000_021964e8[];
extern "C" u32 data_ov000_021963a8[];
static struct DinoBattleView3dStaticInit {
    DinoBattleView3dStaticInit() {
        DINO_POOL_WORD(data_ov000_02196368, 0x5c) = 128;
        DINO_POOL_WORD(data_ov000_02196368, 0x60) = 176;
        DINO_POOL_WORD(data_ov000_02196368, 0xc) = 8;
        DINO_POOL_WORD(data_ov000_02196368, 0x10) = 150;
        DINO_POOL_WORD(data_ov000_02196368, 0x64) = 64;
        DINO_POOL_WORD(data_ov000_02196368, 0x68) = 112;
        DINO_POOL_WORD(data_ov000_021964e8, 0x2c) = DINO_POOL_WORD(data_ov000_021962e8, 0x34);
        DINO_POOL_WORD(data_ov000_021964e8, 0x30) = DINO_POOL_WORD(data_ov000_021962e8, 0x14);
        DINO_POOL_WORD(data_ov000_021964e8, 0x34) = DINO_POOL_WORD(data_ov000_021962e8, 0x10);
        DINO_POOL_WORD(data_ov000_021964e8, 0x38) = DINO_POOL_WORD(data_ov000_021962e8, 0x4);
        DINO_POOL_WORD(data_ov000_021964e8, 0x3c) = DINO_POOL_WORD(data_ov000_021962e8, 0x2c);
        DINO_POOL_WORD(data_ov000_021964e8, 0x40) = DINO_POOL_WORD(data_ov000_021962e8, 0x3c);
        ((volatile u16 *)data_ov000_021963a8)[0x34 / 2] = 0x801f;
        ((volatile u16 *)data_ov000_021963a8)[0x36 / 2] = 0xe475;
        ((volatile u16 *)data_ov000_021963a8)[0x38 / 2] = 0xfe08;
        ((volatile u16 *)data_ov000_021963a8)[0x3a / 2] = 0x83ff;
        ((volatile u16 *)data_ov000_021963a8)[0x3c / 2] = 0x83e0;
    }
} sDinoBattleView3dStaticInit;
