// DinoCleaning: the fossil cleaning scene, with DinoCleaningKasekiRock and DinoCleaningLive.

#include <algorithm>
#include "m/lcd.h"
#include "game/dino_cleaning.h"
#include "game/dino_cleaning_kaseki_rock.h"
#include "game/dino_cleaning_dummy_item_menu.h"
#include "game/dino_cleaning_live_task.h"
#include "game/dino_cleaning_stop_confirm_menu_item_menu.h"
#include "game/dino_cleaning_tool.h"
#include "game/dino_common_live.h"
#include "game/dino_sound.h"
#include "game/dino_cleaning_rock.h"
#include "game/dino_cleaning_image.h"
#include "game/dino_cleaning_image_view.h"
#include "m/display.h"
#include "m/smart_oam.h"
#include "m/blend.h"
#include "game/dino_cleaning_paper.h"
#include "game/dino_cleaning_report.h"
#include "m/bright.h"
#include "game/dino_cleaning_kaseki_rock_xray_blend_manager.h"
#include "game/help.h"
#include "m/bg_new_task.h"
#include "m/call_task.h"
#include "types.h"
#include "game/dino_cleaning_status.h"
#include "game/dino_cleaning_score.h"
#include "game/dino_cleaning_timer.h"
#include "m/input.h"
#include "game/dino_cleaning_neighbour.h"
#include "m/ext_affine.h"
#include "m/picture.h"
#include "m/menu.h"
#include "m/seed_generator.h"
#include "m/window.h"
#include <stdio.h>
#include "game/dino_static_init.h"
#include "game/dino_cleaning_drill.h"
#include "game/dino_cleaning_hammer.h"
#include "game/dino_cleaning_xray.h"
#include "game/dino_cleaning_blower.h"
#include "m/bg.h"
#include "m/obj.h"

#pragma auto_inline off

DinoCleaningKasekiRock::~DinoCleaningKasekiRock() {
}

DinoCleaningKasekiRockXrayBlendManager::~DinoCleaningKasekiRockXrayBlendManager() {
}

DinoCleaningRock *DinoCleaningKasekiRock::GetRock8C() {
    return mRock8C;
}

DinoCleaningRock *DinoCleaningKasekiRock::GetRock90() {
    return mRock90;
}

u8 DinoCleaningKasekiRock::GetUnk4B8() {
    return mUnk4B8;
}

BOOL DinoCleaningKasekiRock::IsCellClear(int row, int col) {
    if (mRock90->IsCellClear(row, col) && mRock8C->IsCellClear(row, col)) {
        return TRUE;
    }
    return FALSE;
}

s32 DinoCleaningKasekiRock::GetCellLayer(int row, int col) {
    if (mRock90->IsCellClear(row, col)) {
        return mRock8C->GetCellLayer(row, col);
    }
    return mRock90->GetCellLayer(row, col) + 3;
}

DinoCleaningRock *DinoCleaningKasekiRock::GetTopRock(int row, int col) {
    if (!mRock90->IsCellClear(row, col)) {
        return mRock90;
    }
    if (mRock8C->IsCellClear(row, col)) {
        return NULL;
    }
    return mRock8C;
}

MFx32 DinoCleaningKasekiRock::GetRatio() {
    return MFx32(100) * MFx32(mUnk9C) / MFx32(mUnk98);
}

MFx32 DinoCleaningKasekiRock::GetRatio(int index) {
    return MFx32(100) * MFx32(mUnkA0[index]) / MFx32(mUnk98);
}

void DinoCleaningKasekiRock::Idle() {
    (this->*mState)();
}

void DinoCleaningKasekiRock::Finish() {
    mXray->Exit();
    mUnk88->Exit();
    mUnk88 = NULL;
    mView->DestroyOam();
    mView->LoadPalettes();
    GetDisplay()->mBlend->Reset();
    GetDisplay()->mBright->Reset();
}

// NONMATCHING: the game reads the size through the returned temporary's address (an inline helper?).
void DinoCleaningKasekiRock::SetOffsetAll(MPoint offset) {
    MPoint pos;
    MPoint size = mUnk80->GetSize();
    pos.x = -(size.x - 256) / 2 + offset.x;
    pos.y = -(size.y - 192) / 2 + offset.y;
    mUnk80->SetPosition(pos);
    mOam->SetOffset(MPoint(-offset.x, -offset.y));
    mUnk88->SetPosition(offset);
    mView->SetPosition(offset);
}

// NONMATCHING: as SetOffsetAll.
void DinoCleaningKasekiRock::SetOffset(MPoint offset) {
    MPoint pos;
    MPoint size = mUnk80->GetSize();
    pos.x = -(size.x - 256) / 2 + offset.x;
    pos.y = -(size.y - 192) / 2 + offset.y;
    mUnk80->SetPosition(pos);
    mOam->SetOffset(MPoint(-offset.x, -offset.y));
}

BOOL DinoCleaningKasekiRock::IsViewUnkD9E0Zero() {
    return mView->IsUnkD9E0Zero();
}

// The rock's playing area.
extern "C" struct {
    u8 unk00[0x44];
    MRect mBounds;      // 0x44
    MFx32 mChipsScales[2][3];   // 0x54: the ctor's function-local static, set on the first call
} data_ov002_02165854;
extern "C" void func_ov002_0215a7b4(DinoCleaningImageView *view);

// Knocks out the top rock's cells left with no top rock around them (where either rock is through already), then
// refreshes the view.
// NONMATCHING: 99.97%, the game's second test branches past the "i = 0" it shares with the first.
extern "C" void func_ov002_0215fdec(DinoCleaningKasekiRock *rock) {
    for (int col = data_ov002_02165854.mBounds.left; col < data_ov002_02165854.mBounds.right; col++) {
        for (int row = data_ov002_02165854.mBounds.top; row < data_ov002_02165854.mBounds.bottom; row++) {
            if (rock->mRock90->GetCellLayer(col, row) == 0 || rock->mRock8C->GetCellLayer(col, row) == 0) {
                for (u32 i = 0; i < 6; i++) {
                    MPoint n = GetNeighbour(MPoint(col, row), i);
                    if (rock->mRock90->GetCellLayer(n.x, n.y) != 0) {
                        goto next;
                    }
                }
                rock->mRock90->Hit(col, row, MFx32(1));
                ((DinoCleaningImageView *)rock->mUnk88)->GetImage(1)->SetCellLayer(col, row,
                                                                                    rock->mRock90->GetCellLayer(col, row));
            }
        next:;
        }
    }
    func_ov002_0215a7b4(rock->mView);
}

MFx32 DinoCleaningKasekiRock::GetDirtRatio() {
    return mView->GetDirtRatio();
}


// Hits the cell: the top rock takes the power, and what breaks through (less the decay, scaled) hits the rock below;
// then the images show the new layers.
// NONMATCHING: 97%, the game's frame has one more temporary (a copy of what's left after the first hit).
void DinoCleaningKasekiRock::Hit(int row, int col, MFx32 power, MFx32 scale, MFx32 decay) {
    if (mUnk4B8) {
        return;
    }
    MFx32 left = mRock90->Hit(row, col, power);
    if (left != power) {
        left = std::max(MFx32(0), left - decay);
    }
    power = left;
    mRock8C->Hit(row, col, power * scale);
    if (mUnk4A0 != 1) {
        ((DinoCleaningImageView *)mUnk88)->GetImage(0)->SetCellLayer(row, col, mRock8C->GetCellLayer(row, col));
        return;
    }
    ((DinoCleaningImageView *)mUnk88)->GetImage(0)->SetCellLayer(row, col, mRock8C->GetCellLayer(row, col));
    ((DinoCleaningImageView *)mUnk88)->GetImage(1)->SetCellLayer(row, col, mRock90->GetCellLayer(row, col));
}

void DinoCleaningKasekiRock::CountCells() {
    u32 i;
    int col;

    mUnk9C = 0;
    for (i = 0; i < 4; i++) {
        mUnkA0[i] = 0;
    }
    for (col = 0; col < 28; col++) {
        int row;
        for (row = 0; row < 36; row++) {
            if (!mRock8C->GetImage()->GetMask(row, col) &&
                !mRock8C->GetImage()->GetLayer(1)->mCells410[row][col] && !mRock90->GetCellLayer(row, col)) {
                mUnk9C++;
                mUnkA0[mRock8C->GetCellLayer(row, col)]++;
            }
        }
    }
}

// NONMATCHING: the members' destructors are missing (the member layout isn't known yet).
// Puts both screens back the way the field wants them.
DinoCleaning::~DinoCleaning() {
    Unregister();
    mParallel.Clear();
    delete mRandom;
    gLcd->mMain->ResetHooks();
    gLcd->mSub->ResetHooks();
    gLcd->mMain->SetBackdrop(0x8000);
    gLcd->mMain->Setup(5, 4, 0, 0);
    gLcd->mSub->Setup(5, 4, 0, 0);
    gLcd->VSyncNow();
    gLcd->SetBottomDisplay(gLcd->mSub);
    gCurrentDisplay = gLcd->GetBottomDisplay();
}

MInput *DinoCleaning::GetUnkDC() {
    return mUnkDC;
}

MRandom *DinoCleaning::GetRandom() {
    return mRandom;
}

u32 DinoCleaning::GetUnkD4() {
    return mUnkD4;
}

DinoCleaningKasekiRock *DinoCleaning::GetKasekiRock() {
    return mKasekiRock;
}

DinoCommonLive *DinoCleaning::GetUnkE4() {
    return mUnkE4;
}

void DinoCleaning::SetUnk128(BOOL value) {
    mUnk128 = value;
}

void DinoCleaning::Execute() {
    if (!mUnk128) {
        MProcess::Execute();
    } else if (mUnk144->mState == STATE_RUN) {
        mUnk144->Execute();
    }
}

void DinoCleaning::Idle() {
    (this->*mState)();
}

void DinoCleaning::OnSleep() {
    mSleepTimer = 180;
}

void DinoCleaning::OnWake() {
}

void DinoCleaning::OnPreLoop() {
    if (mSleepTimer != 0) {
        mSleepTimer--;
    }
}

void DinoCleaningKasekiRockXrayBlendManager::Idle() {
    (this->*mState)();
}

// The scene's pooled pointer-to-member constants (see menu.cpp).
extern "C" struct {
    u8 unk00[0x4];
    const char *mBlankLayerPath;    // 0x04
    u8 unk08[0x10 - 0x8];
    const char *mKasekiLayerFormat; // 0x10: the fossil's layers (number, layer, variant)
    const char *mChipsLayerFormat;  // 0x14: the rock's layers
    u8 unk18[0x20 - 0x18];
    void (DinoCleaningKasekiRockXrayBlendManager::*StateDone_20)(); // 0x20
    void (DinoCleaningKasekiRockXrayBlendManager::*StateFade_28)(); // 0x28
    void (DinoCleaningKasekiRockXrayBlendManager::*StateDone_30)(); // 0x30
    void (DinoCleaningKasekiRock::*CountCells_38)(); // 0x38
    u8 unk40[0x48 - 0x40];
    void (DinoCleaningKasekiRock::*CountCells_48)(); // 0x48
    void (DinoCleaningKasekiRock::*StateDrop_50)(); // 0x50
    u8 unk58[0x68 - 0x58];
    void (DinoCleaningKasekiRock::*StateShake_68)(); // 0x68
} data_ov002_021657d4;
extern "C" struct {
    u8 unk00[0x4];
    MFx32 mChipPower;           // 0x04: what each cell takes when the rock is cleared at once
    const char *mCountFormat;   // 0x08
    u8 unk0C[0x10 - 0xC];
    const char *mFailPath;      // 0x10: the picture of a failed cleaning
    const char *mCleanPath;     // 0x14: the picture shown when the rock comes clean
    u8 unk18[0x24 - 0x18];
    const char *mTitlePath;     // 0x24: the title (the first mode's)
    u32 mMaxBonus;              // 0x28: the most the time bonus gives
    int mZero_2C;               // 0x2C: (the constant the count's shake is clamped to)
    u8 unk30[0x44 - 0x30];
    const char *mTitlePath2;    // 0x44: the other modes' title
    void (DinoCleaning::*State02160638_48)(); // 0x48
    u8 unk50[0x58 - 0x50];
    void (DinoCleaning::*State02160de8_58)(); // 0x58
    u8 unk60[0x68 - 0x60];
    void (DinoCleaning::*State0216177c_68)(); // 0x68
    void (DinoCleaning::*State02161d48_70)(); // 0x70
    void (DinoCleaning::*StatePlay_78)(); // 0x78
} data_ov002_021659ec;
extern "C" struct {
    void (DinoCleaning::*StateEnableToolMenu_00)(); // 0x00
    u8 unk08[0x10 - 0x8];
    void (DinoCleaning::*State02161164_10)(); // 0x10
    void (DinoCleaning::*StateCheckDone_18)(); // 0x18
    void (DinoCleaning::*StateQuit_20)(); // 0x20
    void (DinoCleaning::*StatePlay_28)(); // 0x28
    u8 unk30[0x38 - 0x30];
    void (DinoCleaning::*State02162544_38)(); // 0x38
    void (DinoCleaning::*State021618c8_40)(); // 0x40
    u8 unk48[0x50 - 0x48];
    void (DinoCleaning::*State02161d48_50)(); // 0x50
    void (DinoCleaning::*StateCheckDone_58)(); // 0x58
    void (DinoCleaning::*State02161f9c_60)(); // 0x60
    void (DinoCleaning::*StatePlay_68)(); // 0x68
    void (DinoCleaning::*StateWake_70)();
    void (DinoCleaning::*State02160f54_78)(); // 0x78
} data_ov002_02165a6c;
extern "C" struct {
    u8 unk00[0x8];
    void (DinoCleaning::*State02161d48_08)(); // 0x08
    void (DinoCleaning::*StateStopTool_10)();
    u8 unk18[0x20 - 0x18];
    void (DinoCleaning::*StateStopAnswered_20)();
    void (DinoCleaning::*State02162cec_28)(); // 0x28
    u8 unk30[0x48 - 0x30];
    void (DinoCleaning::*State0216177c_48)(); // 0x48
    void (DinoCleaning::*StateSwitchTool_50)(); // 0x50
    void (DinoCleaning::*State02160de8_58)(); // 0x58
    void (DinoCleaning::*StateStopTool_60)();
    u8 unk68[0x78 - 0x68];
    void (DinoCleaning::*State021610b0_78)();
    void (DinoCommonLive::*SetUnkA0_80)(u32, bool); // 0x80 (the next block's first entry)
} data_ov002_02165aec;
extern "C" struct {
    void (DinoCommonLive::*SetUnkA0_00)(u32, bool); // 0x00 (DinoCommonLive::SetUnkA0)
    u8 unk08[0x10 - 0x8];
    void (DinoCleaning::*State02161260_10)(); // 0x10
    void (DinoHelpBase::*Enable_18)(); // 0x18
    void (DinoCleaning::*State021610b0_20)(); // 0x20
} data_ov002_02165b6c;


// Drops the rock in from above.
void DinoCleaningKasekiRock::StartDrop() {
    mDropY.v = 0xC0000;
    mDropSpeed.v = -0x15000;
    mView->SetPriority(0);
    DinoSoundPlaySe(0x43, 0);
    mState = data_ov002_021657d4.StateDrop_50;
}

// Falls until it lands, then settles.
// NONMATCHING: the two additions' argument copies swap stack slots.
void DinoCleaningKasekiRock::StateDrop() {
    mDropY = mDropY + mDropSpeed;
    mDropSpeed = mDropSpeed + MFx32::FromRaw(0xE66);
    if (mDropY < MFx32(0) || mDropSpeed < MFx32(0)) {
        mView->Move(0, MFx32ToInt(mDropY));
        return;
    }
    DinoSoundPlaySe(0x3F, 0);
    mView->Move(0, 0);
    mView->SetPriority(2);
    mUnk4B4 = 20;
    mState = data_ov002_021657d4.StateShake_68;
}

extern const u32 data_0208a23c;  // the X-ray fade's length

// Fades the X-ray image out as the timer runs down.
void DinoCleaningKasekiRockXrayBlendManager::StateFade() {
    if (!mHidden) {
        GetDisplay()->mBlend->SetAlpha(mTimer * 16 / data_0208a23c, 16);
    }
    mTimer--;
    if (mTimer <= 0) {
        mState = data_ov002_021657d4.StateDone_20;
    }
}

void DinoCleaningKasekiRockXrayBlendManager::StateDone() {
}

extern "C" const u32 data_0208ad2c;  // the help text over the rock
extern "C" u32 data_ov002_02165fa4;  // the guard of the ctor's static
extern "C" MFx32 data_ov002_021658a8[2][3];

inline DinoCleaningKasekiRockXrayBlendManager::DinoCleaningKasekiRockXrayBlendManager(DinoCleaningImageView *image)
    : MProcess(NULL) {
    mState = data_ov002_021657d4.StateDone_30;
    mImage = image;
    mTimer = 0;
    mHidden = FALSE;
    unk88 = new MParallelTaskSet;
    mTasks = new MParallelTaskSet;
    mParallel.AddTail(unk88, mTasks);
}

// Builds the rock: the fossil's image (a blank layer and three fossil layers) over the chips' image (a blank layer
// and three rock layers), a rock for each, the view and its x-ray copy, then counts the fossil's visible cells.
// NONMATCHING: 99.45%, the per-iteration scale table sits below the layer table (the game has it above), and the
// loop counter's store is scheduled after the table copies.
DinoCleaningKasekiRock::DinoCleaningKasekiRock(DinoCleaning *cleaning, u32 view, u32 number, u32 kasekiScale,
                                               u32 kasekiVariant, u8 unk14, u32 chipsNumber, u32 chipsScale,
                                               u32 chipsVariant, u8 unk4B8, BOOL unk19C, u32 scale0, u32 scale1)
    : MProcess(NULL) {
    unk7C = (u32)cleaning;
    mUnk4A0 = chipsVariant;
    mOam = (MSmartOam *)cleaning->GetUnkD4();
    mRandom = cleaning->GetRandom();
    mUnk4B8 = unk4B8;
    mParallel.AddTail(new DinoHelpDummyItem(gLcd->GetBottomDisplay(), MRect(0x1c, 0x25, 0xe5, 0xb0),
                                            std::string(gDinoResource->GetText(data_0208ad2c))));
    mUnk80 = (MView *)view;
    MIndexBitmap *bitmap = gCurrentDisplay->mBg->CreateIndexBitmap(1, TRUE);
    mView = new DinoCleaningImageView;
    mView->CreateOam(cleaning->GetRandom());

    char path[0x80];
    DinoCleaningImage *kaseki = new DinoCleaningImage(cleaning);
    kaseki->AddLayer(new DinoCleaningImageLayer(0, std::string(data_ov002_021657d4.mBlankLayerPath), 1, 0, MFx32(1)));
    u32 sounds[2][3] = {{0x2a, 0x2a, 0x2b}, {0, 0x2a, 0x2b}};
    u32 layers[3] = {1, 3, 2};
    fx32 s0 = scale0;
    fx32 s1 = scale1;
    for (u32 i = 3; i > 0; i--) {
        MFx32 scales[2][3] = {{MFx32::FromRaw(s0), MFx32::FromRaw(s1), MFx32(1)},
                              {MFx32::FromRaw(s0), MFx32::FromRaw(s1), MFx32(20)}};
        OS_SPrintf(path, data_ov002_021657d4.mKasekiLayerFormat, number, layers[i - 1],
                   (i < 3 ? kasekiVariant : 0) + 'a');
        DinoCleaningImageLayer *layer = new DinoCleaningImageLayer((u32)cleaning->GetRandom(), std::string(path), 0,
                                                                   sounds[unk19C == 0 ? 1 : 0][i - 1],
                                                                   scales[unk19C == 0 ? 1 : 0][i - 1]);
        layer->SetUnk14(unk14);
        kaseki->AddLayer(layer);
    }
    mRock8C = new DinoCleaningRock((u32)cleaning, kaseki, (u32)cleaning->GetRandom(), kasekiScale);
    mView->AddImage(kaseki);

    DinoCleaningImage *chips = new DinoCleaningImage(cleaning);
    chips->AddLayer(new DinoCleaningImageLayer(0, std::string(data_ov002_021657d4.mBlankLayerPath), 1, 0, MFx32(1)));
    static const u32 chipsSounds[3] = {0x27, 0x28, 0x29};
    for (u32 i = 3; i > 0; i--) {
        if (!(data_ov002_02165fa4 & 1)) {
            data_ov002_02165854.mChipsScales[0][0] = MFx32(1);
            data_ov002_02165854.mChipsScales[0][1] = MFx32(1);
            data_ov002_02165854.mChipsScales[0][2] = MFx32(1);
            data_ov002_02165854.mChipsScales[1][0] = MFx32(1);
            data_ov002_02165854.mChipsScales[1][1] = MFx32(2);
            data_ov002_02165854.mChipsScales[1][2] = MFx32(1);
            data_ov002_02165fa4 |= 1;
        }
        OS_SPrintf(path, data_ov002_021657d4.mChipsLayerFormat, chipsNumber, i, (i == 1 ? chipsVariant : 0) + 'a');
        chips->AddLayer(new DinoCleaningImageLayer((u32)cleaning->GetRandom(), std::string(path), 0,
                                                   chipsSounds[i - 1],
                                                   data_ov002_021658a8[chipsVariant == 1 ? 1 : 0][i - 1]));
    }
    mRock90 = new DinoCleaningRock((u32)cleaning, chips, (u32)cleaning->GetRandom(), chipsScale);
    mView->AddImage(chips);
    mView->CreateScreen();
    mRock8C->RefreshHardness();
    mRock90->RefreshHardness();
    gCurrentDisplay->mBg->DeleteIndexBitmap(bitmap);

    mUnk88 = new DinoCleaningImageView(*mView);
    if (mUnk4A0 != 1) {
        ((DinoCleaningImageView *)mUnk88)->GetImage(1)->ClearGrid();
    }
    mView->Move(0, 0xc0);
    mView->SetPriority(2);
    mView->Show();
    mUnk88->SetPriority(2);
    mXray = new DinoCleaningKasekiRockXrayBlendManager((DinoCleaningImageView *)mUnk88);
    mParallel.AddTail(mXray, mRock8C, mRock90, mView, mUnk88);

    mUnk98 = 0;
    mUnk9C = 0;
    for (u32 i = 0; i < 4; i++) {
        mUnkA0[i] = 0;
    }
    for (int col = 0; col < 28; col++) {
        for (int row = 0; row < 36; row++) {
            if (mRock8C->GetImage()->GetMask(row, col) == 0 &&
                mRock8C->GetImage()->GetLayer(1)->mCells410[row][col] == 0) {
                mUnk98++;
            }
        }
    }
    mState = data_ov002_021657d4.CountCells_38;
}

// Shakes the landed rock, less and less, then counts its cells.
void DinoCleaningKasekiRock::StateShake() {
    mUnk4B4--;
    MFx32 amp = MFx32((int)mUnk4B4) / MFx32(2);
    SetOffsetAll(MPoint(MFx32ToInt(amp * mRandom->NextFraction() - amp / MFx32(2)),
                        MFx32ToInt(amp * mRandom->NextFraction() - amp / MFx32(2))));
    if (mUnk4B4 == 0) {
        mState = data_ov002_021657d4.CountCells_48;
    }
}

DinoCleaning::DinoCleaning(u32 unk9C, void *unkA0, u32 unkA8, u32 unkAC, int mode, u32 unk140)
    : MProcess(NULL), MLooperPreLoopHook(FALSE, TRUE) {
    mUnk9C = unk9C;
    mUnkA0 = unkA0;
    mUnkA8 = unkA8;
    mUnkAC = unkAC;
    mMode = mode;
    mUnk128 = FALSE;
    mPaper = NULL;
    mUnk140 = unk140;
    mState = data_ov002_021659ec.State02160638_48;
}

// Chips at every cell left of the top rock and shakes the rock; once nothing is left, settles it and checks the result.
void DinoCleaning::State0216177c() {
    BOOL left = FALSE;
    DinoHelpBase::Disable();
    for (int col = 0; col < 28; col++) {
        for (int row = 0; row < 36; row++) {
            if (!mKasekiRock->GetRock90()->IsCellClear(row, col)) {
                left = TRUE;
                mKasekiRock->GetRock90()->Hit(row, col, data_ov002_021659ec.mChipPower);
            }
        }
    }
    MFx32 amp = MFx32(6);
    mKasekiRock->SetOffsetAll(MPoint(MFx32ToInt(amp * mRandom->NextFraction() - amp / MFx32(2)),
                                     MFx32ToInt(amp * mRandom->NextFraction() - amp / MFx32(2))));
    if (!left) {
        mKasekiRock->SetOffsetAll(MPoint(0, 0));
        mState = data_ov002_02165a6c.StateCheckDone_58;
    }
}

extern "C" const int data_0208a238;    // the score below which the result is poor

// Stops everything and judges the cleaning by the score.
void DinoCleaning::State021618c8() {
    mUnkE4->SetUnkA0(1, FALSE);
    mUnkE4->SetUnkA0(2, TRUE);
    DinoHelpBase::Disable();
    mUnk10C->Exit();
    mStatus->StatePause();
    mToolMenu->DisableAll();
    mKasekiRock->Finish();
    mToolMenu->Deactivate();
    gCurrentDisplay->mBlend->Reset();
    gCurrentDisplay->mWin->Reset();
    DinoHelpBase::Disable();
    mSerial.AddTail(new MTimer(60));
    if (mUnkC8) {
        mState = data_ov002_02165a6c.StateQuit_20;
        return;
    }
    if (mStatus->GetScore()->GetUnk80() < MFx32(mUnk130)) {
        DinoSoundPlaySe(0x37, 0);
        mState = data_ov002_021659ec.State02161d48_70;
        return;
    }
    if (mStatus->GetScore()->GetUnk84() > MFx32(100 - mUnk130)) {
        DinoSoundPlaySe(0x37, 0);
        mState = data_ov002_02165a6c.State02161d48_50;
        return;
    }
    if (mStatus->GetScore()->GetUnk80() < MFx32(data_0208a238)) {
        DinoSoundPlaySe(0x39, 0);
        mState = data_ov002_02165a6c.State02161f9c_60;
        return;
    }
    DinoSoundPlaySe(0x3A, 0);
    mState = data_ov002_02165a6c.State02162544_38;
}

extern "C" void (DinoCleaning::*const data_ov002_02165a44)();  // (pooled: State02160de8)
extern "C" const char data_ov002_02165f58[];    // the live's script, by mode
extern "C" const char data_ov002_02165f6c[];
extern "C" const char data_ov002_02165f84[];

static inline MPoint DinoHalfBack(const MPoint &d) {
    return MPoint(-d.x / 2, -d.y / 2);
}

static inline MParallelTaskSet *DinoCleaningPar(MTask *a, MTask *b) {
    MParallelTaskSet *set = new MParallelTaskSet(TRUE);
    set->AddTail(a);
    set->AddTail(b);
    return set;
}

// Sets the cleaning up: its fossil's numbers, the screens, the input, the live, the tools and the menus.
// NONMATCHING: 99.7%, the game loads the title size's y before its x when centring it (scheduling).
void DinoCleaning::State02160638() {
    DinoSoundLoadScene(0xD);
    DinoHelpBase::Disable();
    if (mUnkA0 != NULL) {
        ((u32 *)mUnkA0)[0] = 0;
        ((u32 *)mUnkA0)[1] = 0;
    }
    mUnk128 = 0;
    mSleepTimer = 0;
    MSleepHook::Register();
    MResource<DinoKasekiDefs> defs = gDinoResource->LoadKasekiDefs();
    const u8 *def = (const u8 *)defs.Get()->GetDef(mUnk9C);
    mUnkA4 = ((const u32 *)def)[0x24 / 4];
    mUnkB4 = ((const u32 *)def)[0x8 / 4];
    mUnkB8 = ((const u32 *)def)[0xC / 4];
    mUnkC0 = (MFx32::FromRaw(0x800) * MFx32::FromRaw(((const u32 *)def)[0x10 / 4])).v;
    mUnkC4 = (MFx32::FromRaw(0x800) * MFx32::FromRaw(((const u32 *)def)[0x14 / 4])).v;
    mUnkBC = def[1];
    mUnk12C = ((const u32 *)def)[0x2C / 4] - 1;
    mUnk130 = ((const u32 *)def)[0x34 / 4];
    mUnkC8 = def[2];
    mUnkC9 = def[3];
    mUnkCA = def[5];
    mUnkCC = *(const u32 *)(def + ((const u32 *)def)[0x40 / 4]);
    mUnkD0 = ((const u32 *)(def + ((const u32 *)def)[0x40 / 4]))[1];
    gLcd->mMain->ResetHooks();
    gLcd->mSub->ResetHooks();
    gLcd->mSub->Setup(5, 2, 4, 1, 0, 0);
    gLcd->mMain->Setup(6, 1, 3, 1, 0, 0);
    gLcd->mMain->mObj->Show();
    gLcd->mSub->mObj->Show();
    gLcd->mMain->SetBackdrop(0xFFFF);
    gLcd->SetBottomDisplay(gLcd->mMain);
    gCurrentDisplay = gLcd->mMain;
    gLcd->VSyncNow();
    SetDisplay(gCurrentDisplay);
    gCurrentDisplay->mBg->SetMode(5);
    mRandom = new MRandom();
    MSingleInputHub *hub = new MSingleInputHub(1);
    hub->EnableTouch();
    mUnkDC = hub->GetInput(0);
    mParallel.AddTail(hub);
    std::string script;
    switch (mMode) {
    case 0:
        script = data_ov002_02165f58;
        break;
    case 3:
        script = data_ov002_02165f58;
        break;
    case 1:
        script = data_ov002_02165f6c;
        break;
    case 2:
        script = data_ov002_02165f84;
        break;
    }
    mUnk144 = new MParallelTaskSet();
    mUnkE4 = new DinoCleaningLive((u32)this, std::string(script));
    mUnkE4->SetUnkB8(7, (void *)mUnkAC);
    mUnkE4->SetUnkB8(10, (void *)mUnkA8);
    mUnkE4->SetUnkB8(8, (void *)((const u32 *)def)[0x24 / 4]);
    mUnkE4->SetUnkB8(9, (void *)((mUnk9C - 1) & 3));
    mUnkE4->SetUnkB8(5, (void *)100);
    mUnkE4->SetUnkA0(0, 1);
    mUnkE4->SetUnkA0(0x17, mUnkAC == 0 && ((const u32 *)def)[0x24 / 4] == 1);
    mUnk144->AddTail(mUnkE4);
    mParallel.AddTail(mUnk144);
    mUnk13C = new MParallelTaskSet();
    mParallel.AddTail(mUnk13C);
    mUnkD4 = (u32) new MSmartOam(100, 50, TRUE);
    ((MSmartOam *)mUnkD4)->SetUnk1C(7);
    mTools[0] = new DinoCleaningDrill(this, 0);
    mParallel.AddTail(mTools[0]);
    mTools[1] = new DinoCleaningDrill(this, 1);
    mParallel.AddTail(mTools[1]);
    mTools[2] = new DinoCleaningHammer(this, 0);
    mParallel.AddTail(mTools[2]);
    mTools[3] = new DinoCleaningHammer(this, 1);
    mParallel.AddTail(mTools[3]);
    mTools[4] = new DinoCleaningXray(this);
    mParallel.AddTail(mTools[4]);
    mToolMenu = new DinoCleaningToolMenu((u32)this, mUnkD4, (u32)&mNextTool);
    mToolMenu->Deactivate();
    mParallel.AddTail(mToolMenu);
    mUnk10C = new DinoCleaningBlower(this);
    mParallel.AddTail(mUnk10C);
    mUnk120 = (u32) new MTextPicture(
        std::string(mMode == 0 ? data_ov002_021659ec.mTitlePath : data_ov002_021659ec.mTitlePath2), 1, true);
    ((MTextPicture *)mUnk120)->SetPosition(DinoHalfBack(((MTextPicture *)mUnk120)->GetSize() - MPoint(256, 192)));
    ((MTextPicture *)mUnk120)->SetPriority(3);
    ((MTextPicture *)mUnk120)->Show();
    mKasekiRock = NULL;
    mUnk11C = new MParallelTaskSet();
    mParallel.AddTail(mUnk11C);
    mUnk11C->AddTail(MCreateBgCallTask(this, data_ov002_02165a44, 1));
    mParallel.AddTail((MTextPicture *)mUnk120);
    mParallel.AddTail((MSmartOam *)mUnkD4);
    mNextTool = 2;
    mTool = mNextTool;
    mStatus = new DinoCleaningStatus(this, ((const u32 *)def)[0x30 / 4] * 60, mUnk130, mUnkC9);
    mParallel.AddTail(mStatus);
    mSerial.AddTail(DinoCleaningPar(gLcd->mMain->mBright->CreateFadeInFromWhite(30, 0),
                                    gLcd->mSub->mBright->CreateFadeInFromWhite(30, 0)));
    mState = data_ov002_02165aec.State02160de8_58;
}

// Creates the kaseki rock (and runs it once).
void DinoCleaning::State02160de8() {
    DinoCleaningKasekiRock *rock = new DinoCleaningKasekiRock(this, mUnk120, mUnkB4, mUnkC0, mUnkA8, mUnkBC, mUnkB8,
                                                              mUnkC4, mUnkAC, mUnkC8, mUnkC9, mUnkCC, mUnkD0);
    if (rock->MTask::mState == STATE_RUN) {
        rock->Execute();
    }
    mKasekiRock = rock;
}

// Starts the status display, then waits a moment.
void DinoCleaning::State02160e78() {
    DinoSoundPlaySe(0x38, 0);
    mStatus->Start();
    MTask *timer = new MTimer(30);
    MTask *wait = MTaskSetWaitEmptyTask::Create(mUnk11C);
    MParallelTaskSet *waiting = new MParallelTaskSet(TRUE);
    waiting->AddTail(wait);
    waiting->AddTail(timer);
    mSerial.AddTail(waiting);
    mState = data_ov002_02165a6c.State02160f54_78;
}

// Shows the count (if any left) as text for a second, counting it down.
void DinoCleaning::State021610b0() {
    mToolMenu->Activate();
    if (mUnk110 == 0) {
        mState = data_ov002_02165b6c.State02161260_10;
        return;
    }
    char text[0x100];
    sprintf(text, data_ov002_021659ec.mCountFormat, mUnk110);
    mUnk118 = new MTextPicture(text, TRUE, true);
    mParallel.AddTail(mUnk118);
    mUnk114 = 60;
    mUnk110 = mUnk110 - 1;
    DinoSoundPlaySe(0x3B, 0);
    mState = data_ov002_02165a6c.State02161164_10;
}

// A random offset in [-range/2, range/2).
static inline MFx32 RandomOffset(MRandom *random, MFx32 range) {
    return range * random->NextFraction() - range / MFx32(2);
}

// Shakes the count's text, less as its second runs out, then counts on.
// NONMATCHING: the game loads mRandom just before each NextFraction call.
void DinoCleaning::State02161164() {
    MFx32 scale = MFx32(std::max((int)mUnk114 - 45, data_ov002_021659ec.mZero_2C)) / MFx32(2);
    mUnk118->Show();
    MFx32 x = RandomOffset(mRandom, scale);
    MFx32 y = RandomOffset(mRandom, scale);
    mUnk118->SetPosition(MFx32ToInt(x), MFx32ToInt(y));
    mUnk114--;
    if (mUnk114 == 0) {
        mUnk118->Exit();
        mState = data_ov002_02165aec.State021610b0_78;
    }
}



static inline void AddSerial(DinoCleaning *process, MTask *a, MTask *b, MTask *c, MTask *d) {
    MSerialTaskSet *serial = &process->mSerial;
    serial->AddTail(a);
    serial->AddTail(b);
    serial->AddTail(c);
    serial->AddTail(d);
}

// Starts the rock and, after a moment, the help and the live commentary.
void DinoCleaning::State02160f54() {
    mUnk11C->AddTail(mKasekiRock);
    mKasekiRock->StartDrop();
    AddSerial(this, new MTimer(60), MCreateCallTask((DinoCleaningLive *)mUnkE4, data_ov002_02165aec.SetUnkA0_80, (u32)3, true),
              new MTimer(2), MCreateCallTask(this, data_ov002_02165b6c.Enable_18));
    mUnk110 = 3;
    mState = data_ov002_02165b6c.State021610b0_20;
}

extern "C" struct {
    void (DinoHelpBase::*Enable_00)(); // 0x00
} data_ov002_02165b5c;

static inline void AddSerial(DinoCleaning *process, MTask *a, MTask *b, MTask *c, MTask *d, MTask *e, MTask *f) {
    MSerialTaskSet *serial = &process->mSerial;
    serial->AddTail(a);
    serial->AddTail(b);
    serial->AddTail(c);
    serial->AddTail(d);
    serial->AddTail(e);
    serial->AddTail(f);
}

// The rock is clean: stops the x-ray and the status, flashes the picture, then gives back the help.
void DinoCleaning::State02161260() {
    mUnkE4->SetUnkA0(0, FALSE);
    mUnkE4->SetUnkA0(1, TRUE);
    mUnkE4->SetUnkAC(4);
    if (mMode != 2) {
        DinoSoundPlayBgm(0x36, 0);
    }
    DinoSoundPlaySe(0x1B, 0);
    mKasekiRock->StopXray();
    mStatus->Stop();
    mToolMenu->FocusFirst();
    mToolMenu->DisableItem();
    mTool = mNextTool;
    MTextPicture *picture = new MTextPicture(std::string(data_ov002_021659ec.mCleanPath), TRUE, false);
    picture->Show();
    mParallel.AddTail(picture);
    MBlend *blend = gCurrentDisplay->mBlend;
    blend->SetMode(1);
    blend->SetTarget1(picture->GetPlaneMask());
    blend->SetTarget2(0x3F);
    blend->SetAlpha(16, 16);
    DinoHelpBase::Disable();
    AddSerial(this, new MTimer(1), blend->CreateAlphaRatioAnime(16, 0, 16, 16, 5, 0), MTaskExitTransfer::Create(picture),
              blend->CreateClearTransfer(), new MTimer(1), MCreateCallTask(this, data_ov002_02165b5c.Enable_00));
    mState = data_ov002_02165a6c.StateEnableToolMenu_00;
}

extern "C" struct {
    void (DinoCommonLive::*SetUnkAC_00)(u32); // 0x00
} data_ov002_02165b24;

static inline void AddSerial(DinoCleaning *process, MTask *a, MTask *b) {
    MSerialTaskSet *serial = &process->mSerial;
    serial->AddTail(a);
    serial->AddTail(b);
}

static inline void AddSerial(DinoCleaning *process, MTask *a, MTask *b, MTask *c, MTask *d, MTask *e) {
    MSerialTaskSet *serial = &process->mSerial;
    serial->AddTail(a);
    serial->AddTail(b);
    serial->AddTail(c);
    serial->AddTail(d);
    serial->AddTail(e);
}

// The cleaning failed: the picture rises, swaying, then spins in.
void DinoCleaning::State02161d48() {
    mUnkE4->SetUnkA0(6, TRUE);
    if (mUnkA0 != NULL) {
        ((u32 *)mUnkA0)[0] = 0;
        ((u32 *)mUnkA0)[3] = 0;
    }
    mResultPicture = new MExtAffinePicture(std::string(data_ov002_021659ec.mFailPath), TRUE, false);
    mParallel.AddTail(mResultPicture);
    mResultPicture->Show();
    mResultPicture->GetScreen()->SetCenter(0x33, 0x2E);
    for (u32 i = 0; i <= 120; i++) {
        MPoint pos;
        pos.x = MFx32ToInt(MFx32::FromRaw(FX_SinIdx((u16)(i * 0x10000 / 40))) * MFx32(30));
        pos.y = 192 - i * 192 / 120;
        AddSerial(this, mResultPicture->CreateCoordTransfer(pos), new MTimer(1));
    }
    AddSerial(this, new MTimer(60), DinoSoundCreatePlaySeTask(0x41, 0),
              mResultPicture->GetScreen()->CreateRotAnime(0, 0x1000, 20, 2),
              MCreateCallTask((DinoCleaningLive *)mUnkE4, data_ov002_02165b24.SetUnkAC_00, (u32)0x16), new MTimer(120));
    mState = data_ov002_02165aec.State02162cec_28;
}

void DinoCleaning::StateEnableToolMenu() {
    mUnkE4->SetUnkA0(0x15, TRUE);
    mToolMenu->EnableItem();
    mState = data_ov002_02165aec.StateStopTool_60;
}

void DinoCleaning::StateStopTool() {
    mTools[mTool]->Unk18();
    mUnk124 = 0;
    mState = data_ov002_021659ec.StatePlay_78;
}

// Whether the rock is clean (only without a target).
inline BOOL DinoCleaning::IsRockClean() {
    BOOL done = FALSE;
    if (!mUnkC9) {
        if (mKasekiRock->GetRatio() == MFx32(100)) {
            done = TRUE;
        }
    }
    return done;
}

// Whether the target is reached (only with one).
inline BOOL DinoCleaning::IsTargetReached() {
    BOOL done = FALSE;
    if (mUnkC9) {
        if (mStatus->GetScore()->GetUnk80() >= MFx32(mUnk130)) {
            done = TRUE;
        }
    }
    return done;
}

// Whether too much was broken or the time is up.
inline BOOL DinoCleaning::IsOver() {
    BOOL done = TRUE;
    if (!(mStatus->GetScore()->GetUnk84() > MFx32(100 - mUnk130))) {
        if (!mStatus->GetTimer()->IsAtStart()) {
            done = FALSE;
        }
    }
    return done;
}

// NONMATCHING: the ratios' MFx32ToInt copies and the checks' temporaries take each other's stack slots.
// The cleaning itself: updates the commentary's numbers, switches tools when asked, and ends once the rock is clean,
// the target is reached or the time is up (else runs the tool, the stop confirmation or sleep).
void DinoCleaning::StatePlay() {
    if (mUnkDC->mButtons & 0x3FFF) {
        mUnk124 = 0;
    } else {
        mUnk124++;
    }
    mUnkE4->SetUnkB8(4, (void *)((int)mUnk124 / 60));
    mUnkE4->SetUnkB8(6, (void *)MFx32ToInt(mKasekiRock->GetDirtRatio()));
    mUnkE4->SetUnkB8(5, (void *)MFx32ToInt(mKasekiRock->GetRock90()->GetRatio()));
    mUnkE4->SetUnkB8(11, (void *)MFx32ToInt(mKasekiRock->GetRatio(2)));
    mUnkE4->SetUnkB8(12, (void *)MFx32ToInt(mKasekiRock->GetRatio(1)));
    if (mTool != mNextTool) {
        mState = data_ov002_02165aec.StateSwitchTool_50;
        return;
    }
    if (IsRockClean()) {
        DinoSoundPlaySe(0x1F, 0);
        mStatus->StatePause();
        mTools[mTool]->Unk1C();
        mState = data_ov002_02165aec.State0216177c_48;
        return;
    }
    if (IsTargetReached()) {
        DinoSoundPlaySe(0x1F, 0);
        mStatus->StatePause();
        mTools[mTool]->Unk1C();
        mState = data_ov002_021659ec.State0216177c_68;
        return;
    }
    if (IsOver()) {
        DinoSoundPlaySe(0x1F, 0);
        mStatus->StatePause();
        mTools[mTool]->Unk1C();
        mState = data_ov002_02165a6c.StateCheckDone_18;
        return;
    }
    mTools[mTool]->Unk20();
    if ((mUnkDC->mButtons & 8) && mMode != 2 && gLcd->GetBottomDisplay()->mBright->GetFactor() == 0) {
        StateStopConfirm();
        return;
    }
    if (mSleepTimer != 0) {
        StateSleep();
    }
}

void DinoCleaning::StateSwitchTool() {
    mTools[mTool]->Unk1C();
    mTool = mNextTool;
    mState = data_ov002_02165aec.StateStopTool_10;
}

void DinoCleaning::StateCheckDone() {
    DinoHelpBase::Disable();
    if (!mKasekiRock->IsViewUnkD9E0Zero()) {
        DinoSoundFunc_0201feec(0x2C);
        func_ov002_0215fdec(mKasekiRock);
        return;
    }
    mState = data_ov002_02165a6c.State021618c8_40;
}

void DinoCleaning::StateSleep() {
    mUnk128 = TRUE;
    DinoSoundPause();
    mUnk144->AddTail(new DinoCleaningSleep(this, &mSleepTimer));
    mState = data_ov002_02165a6c.StateWake_70;
}

void DinoCleaning::StateWake() {
    DinoSoundResume();
    mState = data_ov002_02165a6c.StatePlay_68;
}

void DinoCleaning::StateStopConfirm() {
    mUnk128 = TRUE;
    DinoSoundPause();
    mUnkE4->SetUnkA0(0x19, TRUE);
    mUnk144->AddTail(new DinoCleaningStopConfirm(this, &mStop));
    mState = data_ov002_02165aec.StateStopAnswered_20;
}

// Template task instances the game emits in this file. Until the code that makes the tasks is decompiled, they are
// instantiated explicitly.
template class MCallTask2<DinoCleaningLive, DinoCommonLive, u32, bool>;
template MTask *MCreateCallTask(DinoCleaningLive *obj, void (DinoCommonLive::*func)(u32, bool), u32 a0, bool a1);
template class MCallTask1<DinoCleaningLive, DinoCommonLive, u32>;
template MTask *MCreateCallTask(DinoCleaningLive *obj, void (DinoCommonLive::*func)(u32), u32 a0);
template class MCallTask0<DinoCleaning, DinoHelpBase>;
template MTask *MCreateCallTask(DinoCleaning *obj, void (DinoHelpBase::*func)());
template class MBgCallTask0<DinoCleaning>;
template MTask *MCreateBgCallTask(DinoCleaning *obj, void (DinoCleaning::*func)(), int thread);

// The answer to "stop cleaning?": no goes back to it; yes finishes the rock and stops the tools.
void DinoCleaning::StateStopAnswered() {
    DinoSoundResume();
    if (!mStop) {
        mUnkE4->SetUnkA0(0x19, FALSE);
        mState = data_ov002_02165a6c.StatePlay_28;
        return;
    }
    DinoSoundPlaySe(0x1F, 0);
    mUnkE4->SetUnkA0(0x18, TRUE);
    mUnkE4->SetUnkA0(1, FALSE);
    mUnkE4->SetUnkA0(2, TRUE);
    mUnk10C->Exit();
    mStatus->StatePause();
    mToolMenu->DisableAll();
    mKasekiRock->Finish();
    mToolMenu->Deactivate();
    gCurrentDisplay->mBlend->Reset();
    gCurrentDisplay->mWin->Reset();
    DinoHelpBase::Disable();
    mTools[mTool]->Unk1C();
    mSerial.AddTail(new MTimer(60));
    DinoSoundPlaySe(0x37, 0);
    mState = data_ov002_02165aec.State02161d48_08;
}

// Quits: fades both screens and the music out.
// What the cleaning reports back.
struct DinoCleaningResult {
    u32 score;      // 0x00: in percent
    u32 bonus;      // 0x04: the time bonus
    u32 frames;     // 0x08: left on the timer
    u32 unk0C;
};

extern "C" const u8 data_0208a230;    // whether the time bonus always counts

// Fills in the result (with the time bonus for a full score or always with data_0208a230), then fades out to the
// report (mode 1) or straight out.
void DinoCleaning::State02162cec() {
    if (mUnkA0 != NULL) {
        ((DinoCleaningResult *)mUnkA0)->frames = mStatus->GetTimer()->GetRemainingFrames();
        ((DinoCleaningResult *)mUnkA0)->bonus = 0;
        if ((((DinoCleaningResult *)mUnkA0)->score == 100 || data_0208a230) && mUnkA4 == 0 && mMode != 0) {
            u32 seconds = mStatus->GetTimer()->GetSeconds();
            if (seconds > mUnk12C) {
                u32 bonus = seconds - mUnk12C;
                ((DinoCleaningResult *)mUnkA0)->bonus = std::min(data_ov002_021659ec.mMaxBonus, bonus);
                ((DinoCleaningResult *)mUnkA0)->score += ((DinoCleaningResult *)mUnkA0)->bonus;
            }
        }
        if (mUnkA8 == 1) {
            ((DinoCleaningResult *)mUnkA0)->score += 25;
        }
    }
    if (mPaper != NULL) {
        mPaper->SetDone();
    }
    if (mMode != 1) {
        MTask *fadeTop;
        MTask *fadeBottom;
        MTask *exit;
        exit = CreateExitTransfer();
        fadeBottom = gLcd->GetBottomDisplay()->mBright->CreateFadeOutToBlack(60, 0);
        fadeTop = gLcd->GetTopDisplay()->mBright->CreateFadeOutToBlack(60, 0);
        MParallelTaskSet *fades = new MParallelTaskSet(TRUE);
        fades->AddTail(fadeTop);
        fades->AddTail(fadeBottom);
        MTaskSet &serial = mSerial;
        serial.AddTail(fades);
        serial.AddTail(exit);
        return;
    }
    MTask *empty;
    MTask *pictureOut;
    MTask *exitPicture;
    MTask *timer;
    MTask *report;
    MTask *stopBgm;
    MParallelTaskSet *fades;
    MTask *fadeTop;
    MTask *fadeBottom;
    MTask *exit;
    MBlend *blend = gLcd->GetBottomDisplay()->mBlend;
    exit = CreateExitTransfer();
    fadeBottom = gLcd->GetBottomDisplay()->mBright->CreateFadeOutToBlack(60, 0);
    fadeTop = gLcd->GetTopDisplay()->mBright->CreateFadeOutToBlack(60, 0);
    fades = new MParallelTaskSet(TRUE);
    fades->AddTail(fadeTop);
    fades->AddTail(fadeBottom);
    stopBgm = DinoSoundCreateStopBgmTask(-1, 60);
    report = new DinoCleaningReport((u32)this);
    timer = new MTimer(30);
    exitPicture = MTaskExitTransfer::Create(mResultPicture);
    pictureOut = blend->CreateCrossFadeOutTask(mResultPicture->GetPlaneMask(), 30, 0);
    empty = MTaskSetMakeEmptyTask::Create(mUnk13C);
    MTask *fadeOut = blend->CreateCrossFadeOutTask(0, 30, 0);
    MTaskSet &serial = mSerial;
    serial.AddTail(fadeOut);
    serial.AddTail(empty);
    serial.AddTail(pictureOut);
    serial.AddTail(exitPicture);
    serial.AddTail(timer);
    serial.AddTail(report);
    serial.AddTail(stopBgm);
    serial.AddTail(fades);
    serial.AddTail(exit);
}

void DinoCleaning::StateQuit() {
    MTask *fadeTop;
    MTask *fadeBottom;
    MTask *exit;
    exit = CreateExitTransfer();
    fadeBottom = gLcd->GetBottomDisplay()->mBright->CreateFadeOutToBlack(120, 0);
    fadeTop = gLcd->GetTopDisplay()->mBright->CreateFadeOutToBlack(120, 0);
    MParallelTaskSet *fades = new MParallelTaskSet(TRUE);
    fades->AddTail(fadeTop);
    fades->AddTail(fadeBottom);
    MTask *stopBgm = DinoSoundCreateStopBgmTask(-1, 120);
    MTaskSet &serial = mSerial;
    serial.AddTail(stopBgm);
    serial.AddTail(fades);
    serial.AddTail(exit);
}

#pragma auto_inline off

// Shows the x-ray image: zooming in from white, then fading out.
void DinoCleaningKasekiRock::StartXray() {
    ((DinoCleaning *)unk7C)->GetUnkE4()->SetUnkAC(11);
    DinoCleaningKasekiRockXrayBlendManager *xray = mXray;
    DinoSoundPlaySe(0x26, 0);
    xray->mTasks->Clear();
    MFx32 to;
    MFx32 from;
    from = MFx32::FromRaw(0x1B33);
    to = MFx32(1);
    MTask *scale = xray->mImage->CreateScaleAnime(from, to, 45, -1);
    MTask *fade = xray->GetDisplay()->mBright->CreateFadeInFromWhite(60, 0);
    MParallelTaskSet *set = new MParallelTaskSet(TRUE);
    set->AddTail(fade);
    set->AddTail(scale);
    xray->mTasks->AddTail(set);
    xray->mImage->Show();
    MBlend *blend = xray->GetDisplay()->mBlend;
    blend->SetMode(1);
    blend->SetAlpha(16, 16);
    blend->SetTarget1(xray->mImage->GetPlaneMask());
    blend->SetTarget2(0x3F);
    xray->mTimer = data_0208a23c;
    xray->mState = data_ov002_021657d4.StateFade_28;
    mView->CreatePalAnimes();
}

void DinoCleaningKasekiRock::StopXray() {
    DinoCleaningKasekiRockXrayBlendManager *xray = mXray;
    xray->mTasks->Clear();
    xray->GetDisplay()->mBlend->Reset();
    xray->GetDisplay()->mBright->Reset();
    xray->mImage->Hide();
    mView->LoadPalettes();
}

extern "C" void func_ov002_0215a4fc(DinoCleaningImageView *view, MPoint pos);

extern "C" void func_ov002_0215fdd4(DinoCleaningKasekiRock *rock, MPoint pos) {
    func_ov002_0215a4fc(rock->mView, pos);
}

// The file's static initialization (see dino_static_init.h).
// NONMATCHING: scheduling, as in dino_cleaning_stop_confirm_menu_item.cpp (the screen rect lies past the 0x80 anchor).
#pragma always_inline on
static struct DinoCleaningStaticInit {
    DinoCleaningStaticInit() {
        DINO_COMMON_LAYOUT_INIT(data_ov002_021657d4, 0x18, 0x94, 0x58, 0x40, 0xc4, 0xa4, 0xac, 0xb4);
        DINO_POOL_WORD(data_ov002_021657d4, 0xc) = 4096;
        DINO_POOL_WORD(data_ov002_021657d4, 0x8) = 4096;
    }
} sDinoCleaningStaticInit;
