// DinoCleaningImageLayer.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_cleaning_image_layer.h"
#include "types.h"

DinoCleaningImageLayer::DinoCleaningImageLayer(u32 unk1C44, std::string name, u8 unk800, u32 sound, u32 unk1C40)
    : mShared(FALSE), mName(name), mUnk14(0), mUnk800(unk800), mUnk1C3C(sound), mUnk1C40(unk1C40), mUnk1C44(unk1C44),
      mUnk1C48(0) {
    mPixels = new u8[0xC200];
    mUnk1C = mPixels + 0x100;
    mPicture = MPictureFile(std::string(name), 0);
}

DinoCleaningImageLayer::~DinoCleaningImageLayer() {
    if (!mShared) {
        delete[] mPixels;
    }
}
