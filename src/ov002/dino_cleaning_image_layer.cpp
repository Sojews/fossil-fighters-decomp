// DinoCleaningImageLayer.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_cleaning_image_layer.h"
#include "types.h"

DinoCleaningImageLayer::DinoCleaningImageLayer(u32 unk1C44, std::string name, u8 unk800, u32 sound, MFx32 unk1C40)
    : mShared(FALSE), mName(name), mUnk14(0), mUnk800(unk800), mUnk1C3C(sound), mUnk1C40(unk1C40), mUnk1C44(unk1C44),
      mUnk1C48(0) {
    mPixels = new u8[0xC200];
    mUnk1C = mPixels + 0x100;
    mPicture = MPictureFile(std::string(name), 0);
}

// (The arrays copy as wholes, element by element.)
template <class T> static inline void DinoCopyArray(T &dst, const T &src) {
    struct Block {
        T a;
    };
    *(Block *)&dst = *(const Block *)&src;
}

DinoCleaningImageLayer::DinoCleaningImageLayer(const DinoCleaningImageLayer &other) {
    mShared = other.mShared;
    mName = other.mName;
    mUnk14 = other.mUnk14;
    mPixels = other.mPixels;
    mUnk1C = other.mUnk1C;
    DinoCopyArray(mHard, other.mHard);
    DinoCopyArray(mCells410, other.mCells410);
    mUnk800 = other.mUnk800;
    DinoCopyArray(mCells, other.mCells);
    DinoCopyArray(mGrid, other.mGrid);
    mPicture = other.mPicture;
    DinoCopyArray(mPalette, other.mPalette);
    mUnk1C28 = other.mUnk1C28;
    mUnk1C2C = other.mUnk1C2C;
    mUnk1C30 = other.mUnk1C30;
    mUnk1C34 = other.mUnk1C34;
    mUnk1C38 = other.mUnk1C38;
    mUnk1C3C = other.mUnk1C3C;
    mUnk1C40 = other.mUnk1C40;
    mUnk1C44 = other.mUnk1C44;
    mUnk1C48 = other.mUnk1C48;
    mShared = TRUE;
}

DinoCleaningImageLayer::~DinoCleaningImageLayer() {
    if (!mShared) {
        delete[] mPixels;
    }
}
