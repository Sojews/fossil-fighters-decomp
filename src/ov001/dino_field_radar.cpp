// DinoFieldRadar: the excavation radar on the top screen.

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
#include "m/blend.h"
#include "m/call_task.h"
#include "game/dino_backup.h"
#include "game/dino_resource.h"
#include "game/dino_static_init.h"

#include "game/dino_se_motion.h"
#include "m/picture.h"

// The file's pooled data: the radar's paths (and the letters its face's file name is made of).
extern "C" u32 data_ov001_02169e80[];
struct DinoFieldRadarPool {
    u32 unk00;
    u32 unk04;
    s32 unk08;
    const char *levelDigits;    // 0x0C: "123", by system var 0x20
    const char *digMarkPath;    // 0x10
    const char *backFormat;     // 0x14: the face, by size, range and level
    const char *markerPath;     // 0x18
    const char *rangeLetters;   // 0x1C: "abc", by system var 0x1F
    const char *backFramePath;  // 0x20
    const char *playerPath;     // 0x24
    const char *sizeLetters;    // 0x28: "sml", by system var 0x1E
    const char *rockPath;       // 0x2C
    const char *distancePath;   // 0x30
    const char *directionPath;  // 0x34
    const char *containerPath;  // 0x38
};
#define sRadarPool (*(DinoFieldRadarPool *)data_ov001_02169e80)

extern "C" u32 data_0208ada0;
extern "C" u32 data_0208ada4;
extern "C" u32 data_0208ada8;
extern "C" u8 data_0208a20c;

// The point a field position comes to on the radar.
static inline MPoint DinoRadarToPoint(MFx32Point p) {
    return MPoint(MFx32ToInt(p.x), MFx32ToInt(p.y));
}

// The help areas; the scale and ranges from the excavate defs (the range shortened by the radar's size); the blend;
// the face; a mark per rock on this map; the marker, distance and direction gauges, the dig mark, the player's mark
// (turned with an affine) and the container gauge.
DinoFieldRadar::DinoFieldRadar(DinoField *field, u32 map)
    : MProcess(gLcd->GetTopDisplay()), mField(field), mSignalling(FALSE) {
    MDisplayScope scope(GetDisplay());
    DinoHelpDummyItem *radarHelp = new DinoHelpDummyItem(gCurrentDisplay, MRect(0x3F, 0x1F, 0xBF, 0x9F),
                                                         std::string(gDinoResource->GetText(data_0208ada8)));
    DinoHelpDummyItem *containerHelp = new DinoHelpDummyItem(gCurrentDisplay, MRect(0xE0, 0x45, 0xF8, 0xB7),
                                                             std::string(gDinoResource->GetText(data_0208ada0)));
    mParallel.AddTail(radarHelp, containerHelp);
    if (gDinoBackup->GetSystemVar(0x20) >= 3) {
        DinoHelpDummyItem *digHelp = new DinoHelpDummyItem(gCurrentDisplay, MRect(8, 0x98, 0x28, 0xB8),
                                                           std::string(gDinoResource->GetText(data_0208ada4)));
        mParallel.AddTail(digHelp);
        mField->AddEntry((u32)radarHelp, 3, (u32)digHelp);
    }
    mField->AddEntry((u32)radarHelp, 2, (u32)mField->mUnk2C0);
    mEntries.reserve(300);
    MResource<DinoExcavateDefs> defs = gDinoResource->GetExcavateDefs();
    mScale = MFx32(1) / (MFx32::FromRaw(defs.Get()->radarRange) / MFx32(80));
    MFx32 range = MFx32::FromRaw(defs.Get()->radarRange);
    if (!data_0208a20c) {
        if (gDinoBackup->GetSystemVar(0x1E) == 1) {
            range *= MFx32::FromRaw(0x800);
        } else if (gDinoBackup->GetSystemVar(0x1E) == 2) {
            range *= MFx32::FromRaw(0xB00);
        }
    }
    mRangeSq = range * range;
    mUnk574 = MFx32::FromRaw(defs.Get()->unk10) * MFx32::FromRaw(defs.Get()->unk10);
    mUnk578 = MFx32::FromRaw(defs.Get()->unk14) * MFx32::FromRaw(defs.Get()->unk14);
    mUnk57C = MFx32::FromRaw(defs.Get()->unk18) * MFx32::FromRaw(defs.Get()->unk18);
    mUnk580 = defs.Get()->unk1C;
    mUnk582 = defs.Get()->unk1E;
    DinoPlayerData player;
    gDinoBackup->GetPlayer(&player, 0);
    mMode = player.radarMode;
    gCurrentDisplay->mBlend->Reset();
    gCurrentDisplay->mBlend->SetMode(3);
    gCurrentDisplay->mBlend->SetTarget1(0x3F);
    gCurrentDisplay->mBlend->SetTarget2(0x3F);
    if (mMode == 1) {
        gCurrentDisplay->mBlend->SetBrightness(10);
        gCurrentDisplay->mBlend->SetAlpha(16, 0);
    } else {
        gCurrentDisplay->mBlend->SetBrightness(0);
        gCurrentDisplay->mBlend->SetAlpha(16, 16);
    }
    int size = gDinoBackup->GetSystemVar(0x1E);
    int reach = gDinoBackup->GetSystemVar(0x1F);
    int level = gDinoBackup->GetSystemVar(0x20);
    char path[256];
    OS_SPrintf(path, sRadarPool.backFormat, *(sRadarPool.sizeLetters + size - 1), *(sRadarPool.rangeLetters + reach - 1),
               *(sRadarPool.levelDigits + level - 1));
    mBack = new MTextPicture(std::string(path), TRUE, true);
    mBack->SetPriority(2);
    mBack->Show();
    mBackFrame = new MTextPicture(std::string(sRadarPool.backFramePath), TRUE, true);
    mBackFrame->SetPriority(3);
    mBackFrame->Show();
    u32 rocks = 0;
    for (u32 i = 0; i < 200; i++) {
        DinoBackupEntry entry;
        gDinoBackup->GetUnk134(i, &entry);
        if (entry.map == map) {
            rocks++;
        }
    }
    mRockTasks = new MParallelTaskSet();
    static const u32 sMinRockOams = 100;
    mOam = new MSmartOam(std::max(rocks + 10, sMinRockOams), 0x28, TRUE);
    for (u32 i = 0; i < 200; i++) {
        DinoBackupEntry entry;
        gDinoBackup->GetUnk134(i, &entry);
        if (entry.map != map) {
            mRocks[i] = NULL;
            continue;
        }
        MMotion *rock = mRocks[i] = new MMotion(std::string(sRadarPool.rockPath), mOam, 0, 1);
        rock->GetUnit()->SetMode(1);
        rock->SetPosition(DinoRadarToPoint(*(MFx32Point *)&entry * mScale));
        rock->SetPriority(3);
        rock->Show();
        mRockTasks->AddTail(rock);
    }
    for (u32 i = 0; i < 100; i++) {
        mUnk3D8[i] = 0;
    }
    mOam88 = new MSmartOam(0x14, 0, TRUE);
    mMarker = new MMotion(std::string(sRadarPool.markerPath), mOam88, 0, 1);
    mMarker->GetUnit()->SetMode(1);
    mMarker->SetPriority(3);
    mMarker->SetPosition(0x80, 0x60);
    mMarker->Show();
    mParallel.AddTail(mMarker);
    mMotion90 = new MMotion(std::string(sRadarPool.distancePath), mOam88, 2, 1);
    mMotion90->GetUnit()->SetMode(1);
    mMotion90->SetPriority(2);
    mMotion90->SetPosition(0x80, 0x60);
    if (gDinoBackup->GetSystemVar(0x20) >= 2) {
        mMotion90->Show();
    }
    mParallel.AddTail(mMotion90);
    mMotion94 = new MMotion(std::string(sRadarPool.directionPath), mOam88, 2, 1);
    mMotion94->GetUnit()->SetMode(1);
    mMotion94->SetPriority(2);
    mMotion94->SetPosition(0x80, 0x60);
    if (gDinoBackup->GetSystemVar(0x20) >= 2) {
        mMotion94->Show();
    }
    mParallel.AddTail(mMotion94);
    DinoSeMotion *dig = new DinoSeMotion(std::string(sRadarPool.digMarkPath), mOam88, 2, 1);
    if (gDinoBackup->GetSystemVar(0x20) >= 3) {
        dig->AddSe(0x8A);
    }
    mDigMark = dig;
    mDigMark->GetUnit()->SetMode(1);
    mDigMark->SetPriority(2);
    mDigMark->SetPosition(0x80, 0x60);
    if (gDinoBackup->GetSystemVar(0x20) >= 3) {
        mDigMark->Show();
    }
    mParallel.AddTail(mDigMark);
    mAffine = gCurrentDisplay->mObj->mAfn->Alloc(1, TRUE);
    mPlayerMark = new MMotion(std::string(sRadarPool.playerPath), mOam88, 0, 1);
    mPlayerMark->GetUnit()->SetMode(1);
    mPlayerMark->SetPriority(2);
    mPlayerMark->Play(1, 0);
    mPlayerMark->Show();
    mPlayerMark->SetPosition(0x80, 0x60);
    mPlayerMark->mEntries[mPlayerMark->mCells[0].entry].affineParam = mAffine->index;
    MSmartOamCell *cell = &mPlayerMark->mEntries[mPlayerMark->mCells[0].entry];
    cell->objMode = 3;
    int shape = cell->GetShape();
    cell->width = gObjSizes[0][shape].x * 2;
    cell->height = gObjSizes[0][shape].y * 2;
    mParallel.AddTail(mPlayerMark);
    mUnkA4 = new MParallelTaskSet();
    mContainerGauge = new MMotion(std::string(sRadarPool.containerPath), mOam88, 0, 1);
    mContainerGauge->SetPriority(3);
    mGaugePos = GetContainerGaugePos();
    mContainerGauge->SetPosition(mGaugePos);
    mContainerGauge->Play(1, 0);
    mContainerGauge->Show();
    mParallel.AddTail(mUnkA4, mContainerGauge);
    mParallel.AddTail(mBack, mBackFrame, mRockTasks, mOam, mOam88);
    if (mMode == 1) {
        mOam88->mHidden = TRUE;
        mOam->mHidden = TRUE;
        mUnk5A8 = FALSE;
    } else {
        mUnk5A8 = TRUE;
    }
}

DinoFieldRadar::~DinoFieldRadar() {
    GetDisplay()->mObj->mAfn->Free(mAffine);
}

DinoFieldRadarManager::~DinoFieldRadarManager() {
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask1<DinoFieldRadar, DinoFieldRadar, bool>;

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

void DinoFieldRadar::SetUnk5A8(bool value) {
    mUnk5A8 = value;
}

// Switches the radar to the mode: 1 hides it (fading the blend out), anything else shows it again.
MTask *DinoFieldRadar::CreateModeTask(u32 mode, u32 frames, u8 b) {
    mMode = mode;
    if (mode == 1) {
        return (new MSerialTaskSet(TRUE))
            ->Add(b ? DinoSoundCreatePlaySeTask(0x86, 0) : (MTask *) new MTimer(0))
            ->Add(MCreateCallTask(this, &DinoFieldRadar::SetUnk5A8, false))
            ->Add(mMotion90->CreatePlayTransfer(2, 1))
            ->Add(mMotion94->CreatePlayTransfer(1, 1))
            ->Add((new MParallelTaskSet(TRUE))
                      ->Add(GetDisplay()->mBlend->CreateFadeRatioAnime(0, 10, frames, 0))
                      ->Add(GetDisplay()->mBlend->CreateAlphaRatioAnime(16, 0, 16, 16, frames, 0)))
            ->Add(GetDisplay()->mBlend->CreateAlphaTransfer(16, 0))
            ->Add(mOam->CreateHideTransfer())
            ->Add(mOam88->CreateHideTransfer());
    }
    return (new MSerialTaskSet(TRUE))
        ->Add(b ? DinoSoundCreatePlaySeTask(0x85, 0) : (MTask *) new MTimer(0))
        ->Add(mOam->CreateShowTransfer())
        ->Add(mOam88->CreateShowTransfer())
        ->Add(GetDisplay()->mBlend->CreateAlphaTransfer(16, 16))
        ->Add(mMotion90->CreatePlayTransfer(1, 1))
        ->Add(mMotion94->CreatePlayTransfer(1, 1))
        ->Add((new MParallelTaskSet(TRUE))
                  ->Add(GetDisplay()->mBlend->CreateFadeRatioAnime(10, 0, frames, 0))
                  ->Add(GetDisplay()->mBlend->CreateAlphaRatioAnime(0, 16, 16, 16, 60, 0)))
        ->Add(MCreateCallTask(this, &DinoFieldRadar::SetUnk5A8, true));
}

// (After auto-inlining is off: the mode task calls it.)
template MTask *MCreateCallTask(DinoFieldRadar *obj, void (DinoFieldRadar::*func)(bool), bool a0);

DinoFieldRadarManager::DinoFieldRadarManager(DinoField *field) : MProcess(gLcd->GetTopDisplay()) {
    mField = field;
    mRadar = NULL;
}

// Centers the radar's sprites on the player.
void DinoFieldRadar::SetPlayer(MFx32Point pos, u16 angle, MFx32 height) {
    mPlayerPos = pos;
    mPlayerAngle = angle;
    mPlayerHeight = height;
    MPoint offset;
    MPoint q = DinoRadarToPoint(pos * mScale);
    offset = MPoint(q.x - 128, q.y - 96);
    mOam->SetOffset(offset);
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

// The file's static initialization (see dino_static_init.h).
static struct DinoFieldRadarStaticInit {
    DinoFieldRadarStaticInit() {
        DINO_POOL_WORD(data_ov001_02169e80, 0x8) = 8192;
    }
} sDinoFieldRadarStaticInit;
