// DinoCleaningImage.
// Only the destructors are decompiled so far.

// Derived destructors have their bases' destructor bodies inlined; auto-inlining the ones defined above does that.
#pragma auto_inline on

#include "game/dino_cleaning_image.h"
#include "types.h"

DinoCleaningImage::~DinoCleaningImage() {
    for (std::vector<DinoCleaningImageLayer *>::iterator it = mLayers.begin(); it != mLayers.end(); ++it) {
        delete *it;
    }
}
