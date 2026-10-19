// DinoCleaningImage: the picture being cleaned and its layers.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_cleaning_image.h"
#include "game/dino_cleaning.h"
#include "game/dino_common_live.h"
#include "game/dino_sound.h"
#include "m/pal.h"
#include "nitro/mi.h"
#include "types.h"

#pragma auto_inline off

void DinoCleaningImageLayer::SetUnk14(u8 value) {
    mUnk14 = value;
}

u32 DinoCleaningImageLayer::GetUnk1C3C() {
    return mUnk1C3C;
}

u8 DinoCleaningImageLayer::GetUnk800() {
    return mUnk800;
}

void DinoCleaningImageLayer::CreatePalAnime(MTaskSet *set, MPalUnit *block, u32 start, u16 color) {
    MPalAnime *anime = new MPalAnime(block, start, 2);
    anime->AddColor(color);
    anime->AddFrame(mPalette);
    anime->SetDuration(60);
    set->AddTail(anime);
}

void DinoCleaningImageLayer::LoadPalette(MPalUnit *block, u32 start) {
    block->Load(mPalette, start, 2);
}

DinoCleaningImage::DinoCleaningImage(DinoCleaning *cleaning)
    : mCleaning(cleaning), mUnk08(FALSE), mPicture(std::string(""), 0) {
}

#pragma auto_inline on

DinoCleaningImage::~DinoCleaningImage() {
    for (std::vector<DinoCleaningImageLayer *>::iterator it = mLayers.begin(); it != mLayers.end(); ++it) {
        delete *it;
    }
}

#pragma auto_inline off

void DinoCleaningImage::AddLayer(DinoCleaningImageLayer *layer) {
    mLayers.push_back(layer);
}

DinoCleaningImageLayer *DinoCleaningImage::GetLayer(int index) {
    return mLayers[index];
}

int DinoCleaningImage::CountPalettes() {
    int count = 0;

    for (std::vector<DinoCleaningImageLayer *>::iterator it = mLayers.begin(); it != mLayers.end(); ++it) {
        if (!(*it)->GetUnk800()) {
            count += 2;
        }
    }
    return count;
}

void DinoCleaningImage::CreatePalAnimes(MTaskSet *set, MPalUnit *block, u32 start, u16 color) {
    for (std::vector<DinoCleaningImageLayer *>::iterator it = mLayers.begin(); it != mLayers.end(); ++it) {
        (*it)->CreatePalAnime(set, block, start, color);
        if (!(*it)->GetUnk800()) {
            start += 2;
        }
    }
}

void DinoCleaningImage::LoadPalettes(MPalUnit *block, u32 start) {
    for (std::vector<DinoCleaningImageLayer *>::iterator it = mLayers.begin(); it != mLayers.end(); ++it) {
        (*it)->LoadPalette(block, start);
        if (!(*it)->GetUnk800()) {
            start += 2;
        }
    }
}

void DinoCleaningImage::PlayLayerSounds() {
    for (std::vector<DinoCleaningImageLayer *>::iterator it = mLayers.begin(); it != mLayers.end(); ++it) {
        if ((*it)->mUnk1C48) {
            (*it)->mUnk1C48 = FALSE;
            u32 se = (*it)->GetUnk1C3C();
            if (se != 0) {
                if ((u32)(se - 0x2A) <= 1) {
                    mCleaning->GetUnkE4()->SetUnkAC(9);
                }
                DinoSoundPlaySe((*it)->GetUnk1C3C(), 0);
            }
        }
    }
}

MFx32 DinoCleaningImage::GetRatio() {
    return MFx32(100) * MFx32(mUnk13FC) / MFx32(mUnk1400);
}

u8 DinoCleaningImage::GetMask(int row, int col) {
    return mMask[row][col];
}

u8 DinoCleaningImage::GetCell(int row, int col) {
    return mLayers[mGrid[row][col]]->mCells[row][col];
}

void DinoCleaningImage::ClearGrid() {
    MI_CpuFill8(mGrid, 0, sizeof(mGrid));
}

#pragma auto_inline off
// A vector member the game emits in this file (made explicitly until the code that copies the vector is decompiled).
template void std::__vec_deleter<u32, std::allocator<u32> >::assign<u32 *>(u32 *first, u32 *last, Metrowerks::int2type<true>);
