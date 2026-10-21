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
#include "nitro/g2.h"

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
    u32 level2;                 // 0x3C
    const char *redMarkPath;    // 0x40: characters whose data's unk28 is 0
};
#define sRadarPool (*(DinoFieldRadarPool *)data_ov001_02169e80)

extern "C" u32 data_0208ada0;
extern "C" u32 data_0208ada4;
extern "C" u32 data_0208ada8;
extern "C" u8 data_0208a20c;
// The pool's level values (std::max takes them by address).
extern "C" u32 data_ov001_02169e84;
extern "C" u32 data_ov001_02169ebc;

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

// A vector the radar keeps y first.
struct DinoRadarVec {
    DinoRadarVec() {}
    DinoRadarVec(MFx32 y_, MFx32 x_) {
        y = y_;
        x = x_;
    }

    MFx32 y;
    MFx32 x;
};

// The normal of the line at the angle.
static inline DinoRadarVec DinoRadarNormal(u16 angle) {
    return DinoRadarVec(MFx32::FromRaw(FX_CosIdx(angle)), -MFx32::FromRaw(FX_SinIdx(angle)));
}

// The sweep while signalling: between two lines through the player.
struct DinoRadarSweep {
    DinoRadarVec origin;    // 0x00
    DinoRadarVec n1;        // 0x08
    DinoRadarVec n2;        // 0x10
};

static inline MFx32 DinoRadarDot(MFx32Point a, DinoRadarVec b) {
    return a.x * b.x + a.y * b.y;
}

// Whether the sweep passes the point (and it is in range): its mark flashes and the radar beeps.
static inline BOOL DinoRadarSwept(DinoFieldRadar *radar, const DinoRadarSweep &sweep, const MFx32Point &p, MMotion *mark) {
    BOOL ahead = FALSE;
    if (radar->mSignalling && mark != NULL) {
        if ((p.x - sweep.origin.x) * sweep.n1.x + (p.y - sweep.origin.y) * sweep.n1.y > MFx32(0)) {
            ahead = TRUE;
        }
    }
    BOOL inside = FALSE;
    if (ahead) {
        if ((p.x - sweep.origin.x) * sweep.n2.x + (p.y - sweep.origin.y) * sweep.n2.y < MFx32(0)) {
            inside = TRUE;
        }
    }
    BOOL inRange = FALSE;
    if (inside) {
        MFx32Point d = p - radar->mPlayerPos;
        if (d.x * d.x + d.y * d.y <= radar->mRangeSq) {
            inRange = TRUE;
        }
    }
    return inRange;
}

// Whether the offset lies between the two normals' lines.
static inline BOOL DinoRadarBetween(MFx32Point d, DinoRadarVec n1, DinoRadarVec n2) {
    BOOL r = FALSE;
    if (DinoRadarDot(d, n1) > MFx32(0)) {
        if (DinoRadarDot(d, n2) < MFx32(0)) {
            r = TRUE;
        }
    }
    return r;
}

// Turns the player's mark; while signalling, sweeps; refreshes the rocks' and characters' marks (flashing those the
// sweep passes) and the list of their positions; then from the list: whether one is right in front of the player
// (the dig mark), how close the nearest is (the distance gauge) and whether one lies ahead (the direction gauge).
// NONMATCHING: same calls in order, but the inline math leaves ~20 more temporaries on the stack than the game's
// (0x660 against 0x5DC), which moves much of it past sp+0x3FC.
void DinoFieldRadar::Idle() {
    DinoRadarSweep sweep;
    sweep.n2.x = 0;
    sweep.n2.y = 0;
    sweep.n1.x = 0;
    sweep.n1.y = 0;
    sweep.origin.x = 0;
    sweep.origin.y = 0;
    MtxFx22 mtx;
    u16 rot = mPlayerAngle + 0x4000;
    MTX_Rot22_(&mtx, FX_SinIdx(rot), FX_CosIdx(rot));
    GXOamAffine *affine = (GXOamAffine *)mAffine->addr;
    affine->PA = mtx._00 >> 4;
    affine->PB = mtx._01 >> 4;
    affine->PC = mtx._10 >> 4;
    affine->PD = mtx._11 >> 4;
    mEntries.clear();
    if (mSignalling) {
        mSignalTime++;
        if (mSignalTime >= 0x40) {
            mSignalling = FALSE;
        }
        u16 a = ((u16)mSignalTime << 10) - 0x4000;
        sweep.n2.x = -MFx32::FromRaw(FX_SinIdx(a));
        sweep.n2.y = MFx32::FromRaw(FX_CosIdx(a));
        u16 b = a - 0x800;
        sweep.n1.x = -MFx32::FromRaw(FX_SinIdx(b));
        sweep.n1.y = MFx32::FromRaw(FX_CosIdx(b));
        sweep.origin.x = mPlayerPos.x;
        sweep.origin.y = mPlayerPos.y;
    }
    MPoint gauge = GetContainerGaugePos();
    if (gauge.x != mGaugePos.x || gauge.y != mGaugePos.y) {
        if (mUnk5A8) {
            if (gDinoBackup->IsUnk98Full()) {
                DinoSoundPlaySe(0x8C, 0);
            } else {
                DinoSoundPlaySe(0x8B, 0);
            }
        }
        mGaugePos = gauge;
        mUnkA4->Clear();
        mUnkA4->AddTail(mContainerGauge->CreateCoordAnime(mContainerGauge->GetPosition(), mGaugePos, 30, -2));
    }
    for (u32 i = 0; i < 200; i++) {
        if (mRocks[i] == NULL) {
            continue;
        }
        DinoBackupEntry entry;
        gDinoBackup->GetUnk134(i, &entry);
        if (entry.map == 0) {
            mRocks[i]->Exit();
            mRocks[i] = NULL;
        }
        if (mRocks[i] != NULL) {
            mEntries.push_back(*(DinoFieldRadarEntry *)&entry);
        }
        MFx32Point p = *(MFx32Point *)&entry;
        if (DinoRadarSwept(this, sweep, p, mRocks[i])) {
            mRocks[i]->Play(3, 1);
            DinoSoundPlaySe(0x88, 0);
        }
    }
    for (u32 i = 0; i < 100; i++) {
        DinoFieldCharaModel *model = mField->mCharas[i]->GetModel();
        if (mUnk3D8[i] == NULL) {
            if (model->id != 0 && model->unk05[1] != 0) {
                MMotion *mark = (MMotion *)(mUnk3D8[i] = (u32)new MMotion(
                    std::string(mField->mCharas[i]->mData.Get()->unk28 == 0 ? sRadarPool.redMarkPath : sRadarPool.rockPath),
                    mOam, 0, 1));
                mark->GetUnit()->SetMode(1);
                mark->SetPosition(DinoRadarToPoint(model->pos * mScale));
                mark->SetPriority(3);
                mark->Show();
                mRockTasks->AddTail(mark);
            }
        } else if (model->id == 0 || model->unk05[1] == 0) {
            ((MMotion *)mUnk3D8[i])->Exit();
            mUnk3D8[i] = 0;
        } else {
            ((MMotion *)mUnk3D8[i])->SetPosition(DinoRadarToPoint(model->pos * mScale));
        }
        if (mUnk3D8[i] != 0) {
            mEntries.push_back(*(DinoFieldRadarEntry *)&model->pos);
        }
        if (DinoRadarSwept(this, sweep, model->pos, (MMotion *)mUnk3D8[i])) {
            ((MMotion *)mUnk3D8[i])->Play(3, 1);
            DinoSoundPlaySe(0x88, 0);
        }
    }
    if (!mUnk5A8) {
        if (mDigMark->GetAnime() == 1) {
            mDigMark->Play(2, 0);
        }
        return;
    }
    u32 distance = 0;
    u32 direction = 0;
    BOOL dig = FALSE;
    MFx32Point front = mPlayerPos + MFx32Point(MFx32::FromRaw(FX_CosIdx(mPlayerAngle)),
                                               MFx32::FromRaw(FX_SinIdx(mPlayerAngle))) * (mPlayerHeight + MFx32(1));
    u16 angle = mPlayerAngle;
    DinoRadarVec near1 = DinoRadarNormal(angle + ((u32)mUnk580 >> 1));
    DinoRadarVec near2 = DinoRadarNormal(angle - ((u32)mUnk580 >> 1));
    DinoRadarVec far1 = DinoRadarNormal(angle + ((u32)mUnk582 >> 1));
    DinoRadarVec far2 = DinoRadarNormal(angle - ((u32)mUnk582 >> 1));
    for (u32 j = 0; j < mEntries.size(); j++) {
        MFx32Point e = *(MFx32Point *)&mEntries[j];
        MFx32Point d = e - mPlayerPos;
        MFx32 dist = d.x * d.x + d.y * d.y;
        BOOL atFront = FALSE;
        if (!dig) {
            MFx32Point f = e - front;
            if (f.x * f.x + f.y * f.y <= MFx32(4)) {
                atFront = TRUE;
            }
        }
        if (atFront) {
            dig = TRUE;
        }
        if (dist <= mUnk574) {
            distance = std::max(distance, data_ov001_02169ebc);
        } else if (dist <= mUnk578) {
            distance = std::max(distance, data_ov001_02169e84);
        }
        if (dist <= mUnk57C) {
            BOOL ahead = (direction < 2 && DinoRadarBetween(d, near2, near1));
            if (ahead) {
                direction = 2;
            } else {
                BOOL side = (direction < 1 && DinoRadarBetween(d, far2, far1));
                if (side) {
                    direction = 1;
                }
            }
        }
    }
    mMotion90->Play(distance + 3, 0);
    mMotion94->Play(direction + 2, 0);
    if (dig) {
        mDigMark->Play(1, 0);
        return;
    }
    if (mDigMark->GetAnime() == 1) {
        mDigMark->Play(2, 0);
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
