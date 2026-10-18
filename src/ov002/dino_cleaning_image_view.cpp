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

DinoCleaningImageViewScaleAnime::~DinoCleaningImageViewScaleAnime() {
}

#pragma auto_inline off
// Vector instances the game emits in this file (made explicitly until the code that uses them is decompiled).
template class std::__vec_deleter<DinoCleaningStatusEntry, std::allocator<DinoCleaningStatusEntry> >;
template class std::__vec_deleter<u32, std::allocator<u32> >;
