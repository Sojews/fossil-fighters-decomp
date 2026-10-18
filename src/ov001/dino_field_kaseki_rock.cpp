// DinoFieldKasekiRock.
// Only the destructors are decompiled so far.

#include "game/dino_field_kaseki_rock.h"
#include "game/dino_backup.h"
#include "game/dino_field.h"
#include "m/anm_model.h"

static const char *sRockModels[] = {
    "/model/fieldchar/t01_iwa_def_01", "/model/fieldchar/t01_iwa_def_01", "/model/fieldchar/t01_iwa_red_01",
    "/model/fieldchar/t01_iwa_blue_01", "/model/fieldchar/t01_iwa_green_01", "/model/fieldchar/t01_iwa_yel_01",
    "/model/fieldchar/t01_iwa_white_01",
};

DinoFieldKasekiRock::DinoFieldKasekiRock(DinoField *field, M3DScene *scene, u32 index) : MProcess(NULL) {
    mField = field;
    mScene = scene;
    mIndex = index;
    DinoBackupEntry28 rock;
    gDinoBackup->GetUnk138(index, &rock);
    mModel = new MAnmModel(mScene);
    mModel->unk100 = 0x23;
    mModel->SetFrames(0, 0x40000000);
    mModel->Load(sRockModels[rock.color], 0);
    mModel->Run();
    mModel->mUnkF4 = 0;
    mModel->mFollowParent = TRUE;
    mParallel.AddTail(mModel);
    mPos.x = rock.unkC;
    mPos.z = rock.unk10;
    mPos.y = mField->GetGroundHeight(mPos.x, mPos.z) + 0.1 * 4096;
    mModel->SetPosRef(&mPos);
    mRot.x = mRot.y = mRot.z = 0;
    mModel->SetRotRef(&mRot);
}

DinoFieldKasekiRock::~DinoFieldKasekiRock() {
}

void DinoFieldKasekiRock::Idle() {
}
