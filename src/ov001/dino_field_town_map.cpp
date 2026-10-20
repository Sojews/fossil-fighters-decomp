// DinoFieldTownMap, TownMapIcon.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#pragma ipa file

#include "game/dino_resource.h"
#include "m/display.h"
#include "m/blend.h"
#include "game/dino_field_town_map_menu.h"
#include "game/dino_field_town_map.h"
#include "game/town_map_icon.h"
#include "game/dino_field.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "m/fx32.h"
#include "game/dino_field_player_control.h"
#include "m/lcd.h"
#include "m/bright.h"
#include "m/motion.h"
#include "game/dino_static_init.h"
#include "game/help.h"
#include "m/picture.h"
#include "m/smart_oam.h"

// The TU's pooled strings.
extern "C" struct {
    u32 unk00;
    const char *mMapPath;   // 0x04
    const char *mIconPath;  // 0x08
} data_ov001_0216a8e4;
extern "C" const u32 data_0208ad9c;     // the town map's help text

inline TownMapIcon::TownMapIcon(MSmartOam *oam) : MProcess(NULL) {
    for (int i = 0; i < 2; i++) {
        MMotion *&motion = (mMotions[i] = new MMotion(std::string(data_ov001_0216a8e4.mIconPath), oam, 0, 1));
        motion->Show();
        mMotions[i]->SetPriority(3);
        mMotions[i]->SetAnchor(i);
        mMotions[i]->Play(i ? 4 : 2, 0);
        GetParallel()->AddTail(mMotions[i]);
    }
    mMotions[1]->GetUnit()->SetMode(1);
}

// NONMATCHING: in the icon's loop the game keeps this + i * 4 (not &mMotions[i]) across the path string's destructor.
DinoFieldTownMap::DinoFieldTownMap(DinoField *field, u32 map) : MProcess(gLcd->GetTopDisplay()) {
    mPicture = NULL;
    mField = field;
    MDisplayScope scope(GetDisplay());
    DinoHelpDummyItem *help = new DinoHelpDummyItem(gCurrentDisplay, MRect(0, 0, 256, 192).Inflate(-16),
                                                    std::string(gDinoResource->GetText(data_0208ad9c)));
    mParallel.AddTail(help);
    mField->AddEntry((u32) help, 2, (u32) mField->mUnk2C0);
    if (map != 0) {
        char path[64];
        OS_SPrintf(path, data_ov001_0216a8e4.mMapPath, map);
        mPicture = new MTextPicture(std::string(path), 1, TRUE);
        mPicture->SetPriority(3);
        mPicture->Show();
        mParallel.AddTail(mPicture);
        mIcons = new MParallelTaskSet();
        MSmartOam *oam = new MSmartOam(4, 0, 1);
        oam->SetUnk1C(1);
        mIcon = new TownMapIcon(oam);
        mIcons->AddTail(mIcon);
        mBlinks = new MParallelTaskSet();
        mParallel.AddTail(mIcons, mBlinks, oam);
        MBlend *blend = gLcd->mSub->mBlend;
        blend->SetMode(1);
        blend->SetAlpha(16, 16);
        blend->SetTarget1(0);
        blend->SetTarget2(0x3F);
        blend->SetBrightness(0);
    }
}

DinoFieldTownMap::~DinoFieldTownMap() {
}

// Blinks the sub screen's layers (fading them out and back) while it's on.
void DinoFieldTownMap::Idle() {
    if (mPicture != NULL && mBlinks->IsEmpty()) {
        MBlend *blend = gLcd->mSub->mBlend;
        mBlinks->AddTail(blend->CreateAlphaRatioAnime(0, 0x10, 0x10, 0x10, 30, 0),
                         blend->CreateAlphaRatioAnime(0x10, 0, 0x10, 0x10, 30, 0));
    }
}

// The point's whole part.
static inline MPoint ToPoint(MFx32Point p) {
    return MPoint(MFx32ToInt(p.x), MFx32ToInt(p.y));
}

#pragma auto_inline off
void DinoFieldTownMap::Update(MFx32Point pos, MPoint offset, MFx32 scale) {
    if (mPicture != NULL) {
        mIcon->mMotions[0]->Play(2, 0);
        mIcon->SetPosition(ToPoint(pos * scale) + offset);
        mIcon->Show();
    } else {
        mIcon->Hide();
    }
}
#pragma auto_inline reset

TownMapIcon::~TownMapIcon() {
}

DinoFieldTownMapManager::~DinoFieldTownMapManager() {
}

void TownMapIcon::Idle() {
}

DinoFieldDance::~DinoFieldDance() {
}

void DinoFieldDance::Idle() {
    (this->*mState)();
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask2<DinoField, DinoField, MFx32Point, long>;
template MTask *MCreateCallTask(DinoField *obj, void (DinoField::*func)(MFx32Point, long), MFx32Point a0, long a1);

void DinoFieldTownMapManager::CameraUnk0(u32 map) {
    if (mTownMap) {
        mParallel.Remove(mTownMap);
        delete mTownMap;
        mTownMap = NULL;
    }
}

#pragma auto_inline off

// The TU's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x1C];
    void (DinoFieldDance::*Start_1C)();   // 0x1C
} data_ov001_0216a814;

DinoFieldDance::DinoFieldDance(DinoField *field, u32 a, u32 b, u32 c, u32 d, u32 e)
    : MProcess(gLcd->GetBottomDisplay()), mField(field), mUnk80(a), mDance(gDinoResource->LoadDance(b)), mUnk94(c),
      mUnk98(d), mUnk9C(e) {
    mState = data_ov001_0216a814.Start_1C;
}

void DinoFieldDance::ShowTime() {
    u32 time = mTime;
    u32 frames = time % 60;
    u32 seconds = (time - frames) / 60;
    u32 hundredths = frames * 100 / 60;
    mTimeDigits[0]->Play(seconds / 10 + 1, 0);
    mTimeDigits[1]->Play(seconds % 10 + 1, 0);
    mTimeDigits[2]->Play(hundredths / 10 + 1, 0);
    mTimeDigits[3]->Play(hundredths % 10 + 1, 0);
    gDinoBackup->SetLocalVar(mUnk94, mTime);
    gDinoBackup->SetLocalVar(mUnk98, seconds);
    gDinoBackup->SetLocalVar(mUnk9C, hundredths);
}

void DinoFieldDance::Finish() {
    mUnkDC->Clear();
    mUnkE0->Clear();
    mFont->Release();
    for (u32 i = 0; i < 6; i++) {
        GetSerial()->AddTail(mOam->CreateHideTransfer(), new MTimer(10), mOam->CreateShowTransfer(), new MTimer(10));
    }
    GetSerial()->AddTail(MTaskExitTransfer::Create(this));
}

DinoFieldTownMapManager::DinoFieldTownMapManager(DinoField *field) : MProcess(gLcd->GetTopDisplay()) {
    mField = field;
    mTownMap = NULL;
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
// NONMATCHING (the creator only): we copy the MFx32 argument to a stack temporary; the game doesn't (same as the
// DinoField picture-load creators). An aggregate MFx32 fixes it but breaks MFx32 returns (hidden pointer).
template class MCallTask4<DinoFieldPlayerControl, DinoFieldPlayerControl, MFx32Point, MFx32, u16, u32>;
template MTask *MCreateCallTask(DinoFieldPlayerControl *obj, void (DinoFieldPlayerControl::*func)(MFx32Point, MFx32, u16, u32), MFx32Point a0, MFx32 a1, u16 a2, u32 a3);

// Keeps the town map on the player (or its centre).
void DinoFieldTownMapManager::Idle() {
    if (mTownMap == NULL) {
        return;
    }
    DinoFieldChara *chara = mField->GetChara(1);
    if (chara == NULL) {
        return;
    }
    if (!mCentered) {
        mTownMap->Update(chara->GetModel()->pos, mOffset, mScale);
    } else {
        mTownMap->Update(MFx32Point(MFx32(0), MFx32(0)), mOffset, MFx32(1));
    }
}

// Makes the town map the map shows, if the player has turned the town map on.
void DinoFieldTownMapManager::CameraUnk1(u32 map) {
    if (map == 0) {
        return;
    }
    DinoPlayerData player;
    gDinoBackup->GetPlayer(&player, 0);
    if (!player.townMapMode) {
        return;
    }
    MResource<DinoMapMData> data = gDinoResource->LoadMapM(map);
    u32 townMap = 0;
    for (u32 i = 0; i < data.Get()->townMapCount; i++) {
        const DinoMapMTownMap *entry = &data.Get()->GetTownMaps()[i];
        if (entry->flag == 0 && townMap == 0) {
            townMap = entry->id;
        } else if (entry->flag != 0 && gDinoBackup->GetFlag(entry->flag)) {
            townMap = entry->id;
        }
    }
    if (townMap != 0) {
        mOffset = data.Get()->GetTownMapOffset();
        mScale = data.Get()->townMapScale;
        mCentered = data.Get()->townMapCentered;
        mTownMap = new DinoFieldTownMap(mField, townMap);
        mParallel.AddTail(mTownMap);
    }
}

MTask *DinoFieldTownMapManager::CreateModeTask(u32 radarMode, u32 to, u32 frames) {
    if (to == 0) {
        if (mTownMap != NULL) {
            MSerialTaskSet *tasks = new MSerialTaskSet(TRUE);
            MTask *clear = GetDisplay()->mBright->CreateClearTransfer();
            MTask *exit = MTaskExitTransfer::Create(mTownMap);
            tasks->AddTail(GetDisplay()->mBright->CreateFadeOutToBlack(frames, 0));
            tasks->AddTail(exit);
            tasks->AddTail(clear);
            mTownMap = NULL;
            return tasks;
        }
    } else {
        CameraUnk1(mField->GetMap()->GetInfo()->id);
        if (mTownMap != NULL) {
            return GetDisplay()->mBright->CreateFadeInFromBlack(frames, 0);
        }
    }
    return new MSerialTaskSet(TRUE);
}

// The file's static initialization (see dino_static_init.h).
extern "C" u32 data_ov001_0216a868[];
extern "C" u32 data_ov001_0216a870[];
extern "C" u32 data_ov001_0216a878[];
static struct DinoFieldTownMapStaticInit {
    DinoFieldTownMapStaticInit() {
        DINO_POOL_WORD(data_ov001_0216a814, 0x4c) = -31;
        DINO_POOL_WORD(data_ov001_0216a814, 0x50) = -80;
        volatile s32 *p = (volatile s32 *) &data_ov001_0216a868;
        p[0x0 / 4] = -9;
        p[0x4 / 4] = -80;
        p = (volatile s32 *) &data_ov001_0216a870;
        p[0x0 / 4] = 17;
        p[0x4 / 4] = -80;
        p = (volatile s32 *) &data_ov001_0216a878;
        p[0x0 / 4] = 39;
        p[0x4 / 4] = -80;
    }
} sDinoFieldTownMapStaticInit;
