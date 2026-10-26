// DinoCleaningKasekiRock: the rock being cleaned (its layers, hits and drop), with its X-ray blend manager.
// (The game's DinoCleaning file is a separate translation unit: its pools and static initialization are its own.)

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
#include "game/dino_se_motion.h"
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

// Minus half of d (to centre something d bigger than the screen).
static inline MPoint DinoHalfBack(const MPoint &d) {
    return MPoint(-d.x / 2, -d.y / 2);
}

// NONMATCHING: 80%, the game loads offset.y into a saved register before GetSize and reads the size's y before its x
// (as State02160638 does).
void DinoCleaningKasekiRock::SetOffsetAll(MPoint offset) {
    mUnk80->SetPosition(DinoHalfBack(mUnk80->GetSize() - MPoint(256, 192)) + offset);
    mOam->SetOffset(MPoint(-offset.x, -offset.y));
    mUnk88->SetPosition(offset);
    mView->SetPosition(offset);
}

// NONMATCHING: 95%, as SetOffsetAll.
void DinoCleaningKasekiRock::SetOffset(MPoint offset) {
    mUnk80->SetPosition(DinoHalfBack(mUnk80->GetSize() - MPoint(256, 192)) + offset);
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

// Knocks out the top rock's cells left with no top rock around them (where the rock below is through already), then
// refreshes the view.
extern "C" void func_ov002_0215fdec(DinoCleaningKasekiRock *rock) {
    for (int col = data_ov002_02165854.mBounds.left; col < data_ov002_02165854.mBounds.right; col++) {
        for (int row = data_ov002_02165854.mBounds.top; row < data_ov002_02165854.mBounds.bottom; row++) {
            if (rock->mRock90->GetCellLayer(col, row) != 0 && rock->mRock8C->GetCellLayer(col, row) == 0) {
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
static struct DinoCleaningKasekiRockStaticInit {
    DinoCleaningKasekiRockStaticInit() {
        DINO_COMMON_LAYOUT_INIT(data_ov002_021657d4, 0x18, 0x94, 0x58, 0x40, 0xc4, 0xa4, 0xac, 0xb4);
        DINO_POOL_WORD(data_ov002_021657d4, 0xc) = 4096;
        DINO_POOL_WORD(data_ov002_021657d4, 0x8) = 4096;
    }
} sDinoCleaningKasekiRockStaticInit;
