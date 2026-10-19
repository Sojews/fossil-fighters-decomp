// DinoCleaningImageView, DinoCleaningNewDust, DinoCleaningLastDust.

#pragma ipa file

#include "types.h"
#include "game/dino_cleaning_image_view.h"
#include "game/dino_cleaning_image.h"
#include "m/smart_oam.h"
#include "m/bg.h"
#include "m/display.h"
#include "m/pal.h"
#include "game/dino_cleaning_new_dust.h"
#include "game/dino_cleaning_last_dust.h"
#include "game/dino_cleaning_image_view_scale_anime.h"
#include "game/dino_cleaning_status.h"
#include "game/dino_cleaning_image_layer.h"
#include "game/dino_cleaning_neighbour.h"

#pragma auto_inline off

DinoCleaningImageView::~DinoCleaningImageView() {
    delete mOamTasks;
    operator delete(mUnkD9F8);
    delete mPalTasks;
    for (std::vector<DinoCleaningImage *>::iterator it = mImages.begin(); it != mImages.end(); it++) {
        delete *it;
    }
    MBg *bg = mDisplay->mBg;
    bg->DeleteIndexBitmap((MIndexBitmap *) mScreen);
    if (!mKeepPal) {
        bg->mPal->Free(mPal);
    }
}

// NONMATCHING (with the file built ipa, as its vector instances need): push_back's tag temporaries swap stack
// slots.
void DinoCleaningImageView::AddImage(DinoCleaningImage *image) {
    image->mView = this;
    mImages.push_back(image);
}

DinoCleaningImage *DinoCleaningImageView::GetImage(int index) {
    return mImages[index];
}

// NONMATCHING: the game keeps start (0) in a register instead of folding it.
void DinoCleaningImageView::CreatePalAnimes() {
    u32 start = 0;

    mPalTasks->Clear();
    mImages[0]->CreatePalAnimes(mPalTasks, mPal, start, 0x8000);
    mImages[1]->CreatePalAnimes(mPalTasks, mPal, start + mImages[0]->CountPalettes(), 0xFFFF);
}

// NONMATCHING: as CreatePalAnimes.
void DinoCleaningImageView::LoadPalettes() {
    u32 start = 0;

    mPalTasks->Clear();
    mImages[0]->LoadPalettes(mPal, start);
    mImages[1]->LoadPalettes(mPal, start + mImages[0]->CountPalettes());
}

void DinoCleaningImageView::Move(int x, int y) {
    SetPosition(MPoint(x, y));
}

void DinoCleaningImageView::SetPosition(MPoint pos) {
    mPos.x = pos.x;
    mPos.y = pos.y;
    mScreen->SetOffset(MPoint(-pos.x, 1 - pos.y));
}

MPoint DinoCleaningImageView::GetPosition() {
    return mPos;
}

void DinoCleaningImageView::SetPriority(int priority) {
    mScreen->mBgCntBits.priority = priority;
}

int DinoCleaningImageView::GetPriority() {
    return mScreen->mBgCntBits.priority;
}

void DinoCleaningImageView::Show() {
    mScreen->Show();
}

void DinoCleaningImageView::Hide() {
    mScreen->Hide();
}

BOOL DinoCleaningImageView::IsHidden() {
    return mScreen->IsHidden();
}

u32 DinoCleaningImageView::GetPlaneMask() {
    return 1 << mScreen->mBg;
}

void DinoCleaningImageView::CreateOam() {
    mOam = new MSmartOam(300, 50, TRUE);
    mOamTasks->AddTail(mOam);
}

void DinoCleaningImageView::DestroyOam() {
    mOamTasks->Clear();
    mOam = NULL;
}

BOOL DinoCleaningImageView::IsUnkD9E0Zero() {
    if (mUnkD9E0 == 0) {
        return TRUE;
    }
    return FALSE;
}

// NONMATCHING: 96%, the game reloads the column in the neighbour loop (one instruction more).
void DinoCleaningImage::SetCellLayer(int row, int col, u32 layer) {
    u32 cur = mGrid[row][col];
    if (layer == cur) {
        return;
    }
    for (; cur > layer; cur--) {
        DinoCleaningImageLayer *l = mLayers[cur];
        if (l->mHard[row][col] == 0 && l->mCells410[row][col] == 0) {
            mUnk13FC--;
            mLayers[cur]->mUnk1C48 = TRUE;
        }
    }
    mGrid[row][col] = layer;
    mView->mDirty[row][col] = TRUE;
    for (u32 i = 0; i < 6; i++) {
        MPoint n = GetNeighbour(MPoint(row, col), i);
        mView->mDirty[n.x][n.y] = TRUE;
    }
}

// The pooled constants the code reads.
extern "C" struct {
    u8 unk00[0x3C];
    MPoint mCellOrigin;     // 0x3C: (2, 2)
} data_ov002_021646e4;

// a - b. (Taking a by value and returning it keeps both points on the stack, as the game's code does.)
static inline MPoint DinoPointSub(MPoint a, const MPoint &b) {
    a.x -= b.x;
    a.y -= b.y;
    return a;
}

// Adds dirt to a cell.
void DinoCleaningImageView::AddDirt(int x, int y) {
    mCells[x][y]++;
    mUnkD9E0++;
    mDirty[x][y] = TRUE;
    MPoint d = DinoPointSub(MPoint(x, y), data_ov002_021646e4.mCellOrigin);
    mDirtSum.x += d.x * 8 + (d.y & 1) * 4;
    mDirtSum.y += d.y * 8;
}

// How much of the image is still dirty, in percent.
MFx32 DinoCleaningImageView::GetDirtRatio() {
    int count = 0;
    for (int col = 0; col < 28; col++) {
        for (int row = 0; row < 36; row++) {
            if (mCells[row][col] != 0) {
                count++;
            }
        }
    }
    return MFx32(100) * MFx32(count) / MFx32(36 * 28);
}

void DinoCleaningImageView::SetScale(MFx32 scale) {
    mScreen->SetCenter(128, 108);
    mScreen->SetScale(scale, scale);
}

MTask *DinoCleaningImageView::CreateScaleAnime(MFx32 from, MFx32 to, int duration, int ease) {
    return new DinoCleaningImageViewScaleAnime(this, from, to, duration, ease);
}

void DinoCleaningImageViewScaleAnime::Apply(int step) {
    MFx32ValueAnime::Apply(step);
    MFx32 scale = mValue;
    mView->SetScale(scale);
}

#pragma auto_inline off

DinoCleaningNewDust::~DinoCleaningNewDust() {
}

DinoCleaningLastDust::~DinoCleaningLastDust() {
}

// NONMATCHING: 97.8%, stack slots only (the game copies the duration once, for the comparison, yet loads it before
// the frame is advanced).
void DinoCleaningLastDust::Idle() {
    MFx32 duration = mDuration;
    MFx32 frame = mFrame;
    mFrame += MFx32(1);
    if (frame >= duration) {
        Exit();
        return;
    }
    MFx32Point pos = mFrom + (mTo - mFrom) * mFrame / mDuration;
    MFx32Point p = pos;
    mMotion->SetPosition(MPoint(MFx32ToInt(p.x), MFx32ToInt(p.y)));
}

DinoCleaningImageViewScaleAnime::~DinoCleaningImageViewScaleAnime() {
}

#pragma auto_inline off
// Vector instances the game emits in this file (made explicitly until the code that uses them is decompiled).
template class std::__vec_deleter<DinoCleaningStatusEntry, std::allocator<DinoCleaningStatusEntry> >;
// (A vector<DinoCleaningStatusEntry>'s operator= copies with this.)
template void std::__vec_deleter<DinoCleaningStatusEntry, std::allocator<DinoCleaningStatusEntry> >::assign<DinoCleaningStatusEntry *>(
    DinoCleaningStatusEntry *first, DinoCleaningStatusEntry *last, Metrowerks::int2type<true>);
template class std::__vec_deleter<u32, std::allocator<u32> >;
