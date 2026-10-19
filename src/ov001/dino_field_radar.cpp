// DinoFieldRadar.
// Only the template task instances it emits are here so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#pragma ipa file

#include "m/motion.h"
#include "game/dino_sound.h"
#include "game/dino_field_radar_menu.h"
#include "game/dino_field.h"
#include "m/bg_new_task.h"
#include "m/display.h"
#include "m/lcd.h"
#include "m/obj.h"
#include "m/smart_oam.h"
#include "m/bright.h"
#include "m/call_task.h"
#include "game/dino_backup.h"
#include "game/dino_resource.h"

DinoFieldRadar::~DinoFieldRadar() {
    GetDisplay()->mObj->mAfn->Free(mAffine);
}

DinoFieldRadarManager::~DinoFieldRadarManager() {
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask1<DinoFieldRadar, DinoFieldRadar, bool>;
template MTask *MCreateCallTask(DinoFieldRadar *obj, void (DinoFieldRadar::*func)(bool), bool a0);

#pragma auto_inline off

void DinoFieldRadar::Signal() {
    if (mMode != 1 && !mSignalling) {
        DinoSoundPlaySe(0x87, 0);
        mSignalling = TRUE;
        mSignalTime = 0;
        mMarker->Play(1, 1);
    }
}

MPoint DinoFieldRadar::GetContainerGaugePos() {
    if (mMode == 1) {
        return MPoint(0x80, 0xC2);
    }
    DinoPlayerData player;
    gDinoBackup->GetPlayer(&player, 0);
    u32 count = 0;
    for (u32 i = 0; i < 64; i++) {
        DinoBackupEntry entry;
        gDinoBackup->GetUnk98(i, &entry);
        if (entry.id) {
            count++;
        }
    }
    return MPoint(0x80, 0xC2 - 0x62 * count / (player.containerNum * 8));
}

void DinoFieldRadar::SetUnk5A8(u8 value) {
    mUnk5A8 = value;
}

DinoFieldRadarManager::DinoFieldRadarManager(DinoField *field) : MProcess(gLcd->GetTopDisplay()) {
    mField = field;
    mRadar = NULL;
}

// NONMATCHING: the game keeps more MFx32 temporaries on the stack (stack layout and scheduling differ).
void DinoFieldRadar::SetPlayer(MFx32Point pos, u16 angle, MFx32 height) {
    mPlayerPos = pos;
    mPlayerAngle = angle;
    mPlayerHeight = height;
    MFx32Point p(pos.x * mScale, pos.y * mScale);
    mOam->SetOffset(MPoint(MFx32ToInt(p.x) - 128, MFx32ToInt(p.y) - 96));
}

void DinoFieldRadarManager::Idle() {
    if (mRadar) {
        u32 focus = mField->GetCamera()->GetFocus();
        if (focus) {
            DinoFieldChara *chara = mField->GetChara(focus);
            if (chara) {
                DinoFieldCharaModel *model = chara->GetModel();
                DinoFieldCharaModel *model2 = chara->GetModel();
                mRadar->SetPlayer(model->pos, model2->angle, chara->GetHeight());
            }
        }
    }
}

void DinoFieldRadarManager::CameraUnk1(u32 map) {
    DinoPlayerData player;
    gDinoBackup->GetPlayer(&player, 0);
    if (player.radarMode && gDinoResource->GetMapExcavateMatch(map)) {
        mRadar = new DinoFieldRadar(mField, map);
        mParallel.AddTail(mRadar);
    }
}

#pragma auto_inline on

void DinoFieldRadarManager::CameraUnk0(u32 map) {
    if (mRadar) {
        mParallel.Remove(mRadar);
        delete mRadar;
        mRadar = NULL;
    }
}

// The radar list's vector members the game emits in this file.
#pragma auto_inline off
template class std::__vec_deleter<DinoFieldRadarEntry, std::allocator<DinoFieldRadarEntry> >;

MTask *DinoFieldRadarManager::CreateModeTask(u32 from, u32 to, u32 frames, u8 b) {
    if (from == 0) {
        CameraUnk1(mField->GetMap()->GetInfo()->id);
        if (mRadar != NULL) {
            return GetDisplay()->mBright->CreateFadeInFromBlack(frames, 0);
        }
    } else if (to == 0) {
        if (mRadar != NULL) {
            MSerialTaskSet *tasks = new MSerialTaskSet(TRUE);
            MTask *clear = GetDisplay()->mBright->CreateClearTransfer();
            MTask *exit = MTaskExitTransfer::Create(mRadar);
            tasks->AddTail(GetDisplay()->mBright->CreateFadeOutToBlack(frames, 0));
            tasks->AddTail(exit);
            tasks->AddTail(clear);
            mRadar = NULL;
            return tasks;
        }
    } else if (mRadar != NULL) {
        return mRadar->CreateModeTask(to, frames, b);
    }
    return new MSerialTaskSet(TRUE);
}
